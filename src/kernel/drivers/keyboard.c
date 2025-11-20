#include "keyboard.h"

#include "cpu/cpu.h"
#include "interrupts/pic.h"
#include "libc/stdio.h"

#define KB_DATA 0x60

#define KBD_BUF_SIZE 256

static volatile uint16_t kbd_tail = 0;
static volatile uint16_t kbd_head = 0;
static char kbd_buf[KBD_BUF_SIZE];


// minimal set 1 map just for testing
static const char scancode_to_char[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',  0, '\\',
    'z','x','c','v','b','n','m',',','.','/',  0,      0,  0,  ' '
};

static inline int kbd_enqueue_char(char ch) {
    uint16_t next = (kbd_head + 1) & (KBD_BUF_SIZE - 1);
    if (next == kbd_tail) return 0; /* full */

    kbd_buf[kbd_head] = ch;                         
    kbd_head = next;
    return 1;
}

int kbd_poll_char(char *out) {
    if (kbd_head == kbd_tail) return 0;
    *out = kbd_buf[kbd_tail];
    kbd_tail = (kbd_tail + 1) & (KBD_BUF_SIZE - 1);
    return 1;
}

void keyboard_isr(void) {
    uint8_t sc = inb(KB_DATA);
    
    
    // ignore key releases (>= 0x80)
    //TODO fix this for unmapped keys (shift, Ctrl ecc ecc...)
    if (sc < 0x80) {
        
        char ch = scancode_to_char[sc];
        
        if (ch) kbd_enqueue_char(ch);
        
    }

    // tell the PIC we're done with IRQ1
    PIC_sendEOI(1);
}
