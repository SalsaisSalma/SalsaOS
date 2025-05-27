#include "libc/stdlib.h"

#define VGA_BUFFER 0xB8000
#define WHITE_ON_BLACK 0x07

void cls(void) /* clears the screen */
{
    uint16_t blank = (WHITE_ON_BLACK << 8) | ' ';
    uint16_t *vga = (uint16_t *)VGA_BUFFER;

    for (size_t i = 0; i < 80 * 25; ++i)
        vga[i] = blank;
}

void putchar(int pos, char c) {
    uint16_t *vga = (uint16_t *)VGA_BUFFER;
    vga[pos] = (WHITE_ON_BLACK << 8) | c;
}

void kernel_main() {

    /* basic smoke testing for malloc and free */
    /* 1. allocate and write */
    cls();
    char *m = malloc(32);
    if (!m) { putchar(0, 'F'); return; }

    const char *src = "Salsa";
    for (size_t i = 0; src[i]; ++i) {
        m[i] = src[i];    /* copy into heap */
        putchar(i, m[i]); /* show it        */
    }

    /* 2. free the block and verify allocator re-uses it */
    free(m);
    // m = NULL;

    char *a = malloc(32);         /* should give back same address */
    if (a == m)
        putchar(10, '+');         /* print a '+' if we got it back */

    /* 3. write through the re-used block */
    a[0] = 'O';
    a[1] = 'K';
    putchar(12, a[0]);
    putchar(13, a[1]);
}
