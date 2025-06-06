#include "stdio.h"


#define VGA_BUFFER 0xB8000
#define WHITE_ON_BLACK 0x07 


static size_t pos; 


void cls(void) {/* clears the screen */
    uint16_t blank = (WHITE_ON_BLACK << 8) | ' ';
    uint16_t *vga = (uint16_t *)VGA_BUFFER;

    for (size_t i = 0; i < 80 * 25; ++i)
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
            pos += ((80 - i) % 80); /* move to next line */
        } else {
            putchar(pos, format[i]);
            pos++;
        }
        i++;
    }
    
    return 0;
}

int puts (const char *format) { /* same as printf but adding a \n at the end */
    size_t i = 0;
    while (format[i] != '\0') {
        
        if (format[i] == '\n') {
            pos += (80 - i);
        } else {
            putchar(pos, format[i]);
            pos++;
        }
        i++;
    }
    pos += 80 - i;
    return 0;
}