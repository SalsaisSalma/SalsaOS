#include "cpu/cpu.h"
#include "interrupts/pic.h"
#include "libc/stdio.h"

#define KB_DATA 0x60


static const char scancode_to_char[128];

void keyboard_isr(void);