#include "cpu/cpu.h"


void cli(void)  { __asm__ volatile ("cli"); }
void sti(void)  { __asm__ volatile ("sti"); }
void hlt(void)  { __asm__ volatile ("hlt"); }

void outb(uint16_t port, uint8_t val)
{ __asm__ volatile ("outb %0, %1" :: "a"(val), "Nd"(port)); }

uint8_t inb(uint16_t port)
{ uint8_t r; __asm__ volatile ("inb %1, %0" : "=a"(r) : "Nd"(port)); return r; }

void io_wait(void) { outb(0x80, 0); }