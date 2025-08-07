#include "libc/stdio.h"
#include "cpu/cpu.h"          /* cli/sti + port-I/O */
#include "interrupts/interrupts.h"   /* idt_init            */
#include "interrupts/pic.h"          /* PIC_remap etc.      */

void kernel_main(void) {
    cls();
    puts("Welcome to SalsaOS");
}
