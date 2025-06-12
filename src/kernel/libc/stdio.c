#include "stdio.h"


#define VGA_BUFFER 0xB8000
#define WHITE_ON_BLACK 0x07 
#define NUMBER_OF_COLUMNS 80

static size_t pos; 


void cls(void) {/* clears the screen */
    uint16_t blank = (WHITE_ON_BLACK << 8) | ' ';
    uint16_t *vga = (uint16_t *)VGA_BUFFER;

    for (size_t i = 0; i < NUMBER_OF_COLUMNS * 25; ++i)
        vga[i] = blank;

    pos = 0;
}

void putchar(int pos, char c) {
    uint16_t *vga = (uint16_t *)VGA_BUFFER;
    vga[pos] = (WHITE_ON_BLACK << 8) | c;
}


int printf(const char *format) {
    
    size_t i = 0;
    while (format[i] != '\0') {
        

        if (format[i] == '\n') {
            pos += NUMBER_OF_COLUMNS - (i % NUMBER_OF_COLUMNS); /* move to next line */
            format = &format[i + 1]; /* start again from next string */
            i = 0;
        } else {
            putchar(pos, format[i]);
            pos++;
            i++;
        }
        
    }
    
    return 0;
}

int puts(const char *format) { /* same as printf but adding a \n at the end */
    size_t i = 0;
    while (format[i] != '\0') {
        
        if (format[i] == '\n') {
            pos += (NUMBER_OF_COLUMNS - i);
        } else {
            putchar(pos, format[i]);
            pos++;
        }
        i++;
    }
    pos += NUMBER_OF_COLUMNS - i;
    return 0;
}