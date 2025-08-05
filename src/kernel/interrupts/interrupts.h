#include "libc/stddef.h"

#define INTERRUPT __attribute__ ((interrupt))

struct interrupt_frame
{
    uword_t ip;
    uword_t cs;
    uword_t flags;
    uword_t sp;
    uword_t ss;
};

void INTERRUPT interrupt_handler(struct interrupt_frame *frame);
