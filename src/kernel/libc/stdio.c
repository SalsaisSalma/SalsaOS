#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"

#define VGA_BUFFER 0xB8000
#define WHITE_ON_BLACK 0x07 
#define NUMBER_OF_COLUMNS 80
#define NUMBER_OF_ROWS 25
static size_t pos = 0; 

extern int kbd_poll_char(char *out);

/* clears the screen */
void cls(void) {
    uint16_t blank = (WHITE_ON_BLACK << 8) | ' ';
    uint16_t *vga = (uint16_t *)VGA_BUFFER;

    for (size_t i = 0; i < NUMBER_OF_COLUMNS * NUMBER_OF_ROWS; i++)
        vga[i] = blank;

    pos = 0;
}

void scroll_up(void) {
    uint16_t blank = (WHITE_ON_BLACK << 8) | ' ';
    uint16_t *vga = (uint16_t *)VGA_BUFFER;
    /* move every char from 2nd row up */
    for (int i = 0; i < (NUMBER_OF_COLUMNS * (NUMBER_OF_ROWS - 1)); i++) {
        vga[i] = vga[i + NUMBER_OF_COLUMNS];
    }
    /* clear last row */
    for (int i = (NUMBER_OF_COLUMNS * (NUMBER_OF_ROWS - 1)); i <NUMBER_OF_COLUMNS * NUMBER_OF_ROWS; i++)
        vga[i] = blank;
    /* go up a row */
    pos = pos - NUMBER_OF_COLUMNS;
}

void putchar(char c) {
    uint16_t blank = (WHITE_ON_BLACK << 8) | ' ';
    uint16_t *vga = (uint16_t *)VGA_BUFFER;
    
    switch (c) {
        case '\n':
            pos += NUMBER_OF_COLUMNS - (pos % NUMBER_OF_COLUMNS); /* move to next line */
            break;
        case '\b':
            if (pos > 0) {
                pos -= 1;
                vga[pos] = blank;
            }
            break;
        default:
            vga[pos] = (WHITE_ON_BLACK << 8) | c;
            pos++;
            break;
    }
    /* if pos hit the bottom of the screen scroll up */
    if (pos >= NUMBER_OF_COLUMNS * NUMBER_OF_ROWS) {
        scroll_up();
    }
}

/*
    i know i can just do 
    while (*format) {
        putchar(*format);
        *format++;
    }
    but i think it lakcs readability and it's not that more efficient
*/
int printf(const char *format) {
    
    size_t i = 0;
    while (format[i] != '\0') {
        putchar(format[i]);
        i++;
    }
    
    return 0;
}

/* same as printf but adding a \n at the end */
int puts(const char *format) { 
    
    size_t i = 0;
    while (format[i] != '\0') {
        putchar(format[i]);
        i++;
    }
    putchar('\n'); /* move to next line */
    
    return 0;
}

int getc(void) {
    char c;
    /* non busy waiting */
    while (true) {
        if (kbd_poll_char(&c)) return c;

        __asm__ volatile("hlt");
    }
}

/* small helper to grow capacity geometrically (64,128,256,...) or linear after 1 KiB */
/* since my realloc is what it is i just do this instead of realloc everytime */
static int grow(char **pline, size_t *pcapacity, size_t need) {
    char  *line     = *pline;
    size_t capacity = *pcapacity;

    if (need <= capacity) return 1;

    size_t newcap = capacity ? capacity : 64;
    while (newcap < need)
        newcap = (newcap < 1024) ? (newcap * 2) : (newcap + 1024);

    char *tmp = (char*)realloc(line, newcap);
    if (!tmp) {
        if (line) free(line);
        *pline = NULL;
        *pcapacity = 0;
        return 0;
    }

    *pline = tmp;
    *pcapacity = newcap;
    return 1;
}

char *getline(void) {
    char *line = NULL;
    size_t size_of_line = 0;
    size_t capacity = 0;

    while (true) {
        int ch = getc();    
        //TODO make something more clever than this in the keyboard driver             
        if (ch == '\r') ch = '\n';

        switch (ch) {
            case '\b':
                if (size_of_line > 0) {
                    /* backspace */ 
                    putchar('\b');
                    /* just reduce logical length; do NOT shrink buffer (avoids fragmentation since realloc is bad) */
                    size_of_line--;
                    line[size_of_line] = '\0';
                }
                break;

            case '\n': {
                // ensure room for '\n' + NUL
                if (!grow(&line, &capacity, size_of_line + 2)) return NULL;
                line[size_of_line++] = '\n';
                line[size_of_line]   = '\0';
                putchar('\n');
                return line;
            }

            default:
                /* ensure room for this char + NULL (keep NULL reserved during editing) */
                if (!grow(&line, &capacity, size_of_line + 1 + 1)) return NULL;
                line[size_of_line++] = (char)ch;
                putchar((char)ch);   
                /* keep the buffer always NULL-terminated for safety while editing */
                line[size_of_line] = '\0';
                break;
        }
    }
}
