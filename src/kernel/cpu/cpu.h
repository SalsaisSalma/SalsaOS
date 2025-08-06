#pragma once

#include "libc/stddef.h"

static inline void cli(void);
static inline void sti(void);
static inline void hlt(void);

static inline void  outb(uint16_t port, uint8_t val);
static inline uint8_t inb(uint16_t port);

static inline void io_wait(void);
