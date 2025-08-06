#include "libc/stdio.h"
#include "cpu/cpu.h"          /* cli/sti + port-I/O */
#include "interrupts/interrupts.h"   /* idt_init            */
#include "interrupts/pic.h"          /* PIC_remap etc.      */

void kernel_main(void)
{
    cls();
    puts("let's gooo");
    cli();                        /* 1. stop stray IRQs            */
    PIC_remap(0x20, 0x28);        /* 2. move IRQs to 32-47         */
    idt_init();                   /* 3. load IDT (sti inside)      */
    IRQ_clear_mask(0);            /* 4. unmask PIT so we can test  */

    puts("Interrupts are live!");
    __asm__ volatile ("int3");    /* software test - see console   */
}
