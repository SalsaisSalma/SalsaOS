#include "interrupts.h"

// source osdev.org


__attribute__((aligned(0x10))) 
static idt_entry_t idt[IDT_MAX_DESCRIPTORS]; // Create an array of IDT entries; aligned for performance

static idtr_t idtr;




__attribute__((noreturn))
void exception_handler() {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}


void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs      = GDT_OFFSET_KERNEL_CODE;
    descriptor->ist            = 0;
    descriptor->attributes     = flags;
    descriptor->isr_mid        = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high       = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved       = 0;
}


static bool vectors[IDT_MAX_DESCRIPTORS];

extern void* isr_stub_table[];
extern void  irq1_stub(void); 

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

     // IRQ1 (keyboard) goes to vector 0x20+1 = 33 after PIC remap
    idt_set_descriptor(32 + 1, irq1_stub, 0x8E);


    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}