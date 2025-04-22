#include "libc/stdlib.h"

#define VGA_BUFFER 0xB8000
#define WHITE_ON_BLACK 0x07

void putchar(int pos, char c) {
    char *video = (char*)VGA_BUFFER;
    video[pos * 2] = c;
    video[pos * 2 + 1] = WHITE_ON_BLACK;
}

void kernel_main() {
    char *mem1 = malloc(16);
    char *mem2 = malloc(16);
    
    
    mem1[0] = 'A';
    mem2[0] = 'B';
    
    putchar(0, mem1[0]); // Expect 'A'
    putchar(1, mem2[0]); // Expect 'B'
    
}
