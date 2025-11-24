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
    IRQ_clear_mask(1);          // unmask keyboard only
    sti();                      // enable interrupts
    
    /* smoke test */
    char data[512];
    ata_read_sector(0, (uint8_t*)data);
    printf(data);
    while (true) {
        char *msg = getline();
        ata_write_sector(0, (uint8_t*)msg);

    }
    
}
