#pragma once

#include "libc/stddef.h"


void PIC_sendEOI(uint8_t irq);

/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2);


void PIC_disable(void);

void IRQ_set_mask(uint8_t IRQline);
void IRQ_clear_mask(uint8_t IRQline);


uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);