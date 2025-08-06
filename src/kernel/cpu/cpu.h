#pragma once
#include "libc/stddef.h"


void  cli(void);
void  sti(void);
void  hlt(void);
void  outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void  io_wait(void);