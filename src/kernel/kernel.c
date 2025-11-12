
#include "libc/stdio.h"
#include "cpu/cpu.h"
#include "interrupts/interrupts.h"
#include "interrupts/pic.h"

void kernel_main(void) {
    cls();
    puts("Welcome to SalsaOS");
    
    cli();
    idt_init();                 // load IDT with exceptions + IRQ1 gate
    PIC_remap(0x20, 0x28);      // IRQs -> 0x20..0x2F
    IRQ_set_mask(0x00);         // start masked
    IRQ_clear_mask(1);          // unmask keyboard only
    sti();                      // enable interrupts

    puts("Type on your keyboard...");
    char *command = NULL;
    while (true) {
        command = getline(); 
        // printf(command);
    }
}
