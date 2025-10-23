#include "stdio.h"
#include "stddef.h"

#define VGA_BUFFER 0xB8000
#define WHITE_ON_BLACK 0x07 
#define NUMBER_OF_COLUMNS 80

static size_t pos = 0; 


void cls(void) {/* clears the screen */
    uint16_t blank = (WHITE_ON_BLACK << 8) | ' ';
    uint16_t *vga = (uint16_t *)VGA_BUFFER;

    for (size_t i = 0; i < NUMBER_OF_COLUMNS * 25; ++i)
        vga[i] = blank;

    pos = 0;
}

void putchar(char c) {
    uint16_t blank = (WHITE_ON_BLACK << 8) | ' ';
    uint16_t *vga = (uint16_t *)VGA_BUFFER;
    
    switch (c) {
        case '\n':
            pos += NUMBER_OF_COLUMNS - (pos % NUMBER_OF_COLUMNS); /* move to next line */
            break;
        case '\b':
            pos -= 1;
            vga[pos] = blank;
            break;
        default:
            vga[pos] = (WHITE_ON_BLACK << 8) | c;
            pos++;
            break;
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
// TODO implement check to see if pos hits bottom of the screen
int printf(const char *format) {
    
    size_t i = 0;
    while (format[i] != '\0') {
        putchar(format[i]);
        i++;
    }
    
    return 0;
}

// TODO implement check to see if pos hits bottom of the screen
int puts(const char *format) { /* same as printf but adding a \n at the end */
    
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

    return c;
}

//TODO implement allocation for the string
char *getline(void) {
    char ch;
    char *line;
    int len_of_line;
    while (true) {
        ch = getc();
        switch (ch) {
                /* if backspace delete last written char */ 
            case '\b':
                if (len_of_line > 0) {  
                    len_of_line--;
                    // putchar(ch);
                }
                break;
            case '\n':
                /*command sent*/
                len_of_line = 0;
                // putchar(ch);
                return line;
            default:
                len_of_line++;
                // putchar(ch);
                break;
            
        }
    }
    

    return NULL;
}
