#include "libc/stdio.h"
#include "cpu/cpu.h"
#include "interrupts/interrupts.h"
#include "interrupts/pic.h"
#include "drivers/ata.h"
void kernel_main(void) {
    cls();
    puts("Welcome to SalsaOS");
    
    cli();
    idt_init();                 // load IDT with exceptions + IRQ1 gate
    PIC_remap(0x20, 0x28);      // IRQs -> 0x20..0x2F
    IRQ_set_mask(0x00);         // start masked
    IRQ_clear_mask(1);          // unmask keyboard only
    sti();                      // enable interrupts

    char write_data[512] = "damndamn";
    ata_write_sector(0, (uint8_t*)write_data);
    puts("should be working");
    char read_data[512];
    ata_read_sector(0, (uint8_t*)read_data);
    puts(read_data);
}
