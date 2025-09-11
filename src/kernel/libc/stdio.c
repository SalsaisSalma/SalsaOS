#include "stdio.h"


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
    uint16_t *vga = (uint16_t *)VGA_BUFFER;
    if (c == '\n') {
        pos += NUMBER_OF_COLUMNS - (pos % NUMBER_OF_COLUMNS); /* move to next line */
    } else {
        vga[pos] = (WHITE_ON_BLACK << 8) | c;
        pos++;  
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