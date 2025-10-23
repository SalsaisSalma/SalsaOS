#include "keyboard.h"

#include "cpu/cpu.h"
#include "interrupts/pic.h"
#include "libc/stdio.h"

#define KB_DATA 0x60

#define KDB_BUF_SIZE 256

static char kdb_buf[KDB_BUF_SIZE];

// minimal set 1 map just for testing
static const char scancode_to_char[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',  0, '\\',
    'z','x','c','v','b','n','m',',','.','/',  0,      0,  0,  ' '
};
static unsigned int len_of_text = 0; // length of the text written until user presses \n
void keyboard_isr(void) {
    uint8_t sc = inb(KB_DATA);

    // ignore key releases (>= 0x80)
    //TODO fix this for unmapped keys (shift, Ctrl ecc ecc...)
    if (sc < 0x80) {
        
        char ch = scancode_to_char[sc];
        
        if (ch) {

            switch (ch) {
                /* if backspace delete last written char */ 
                case '\b':
                    if (len_of_text > 0) {  
                        len_of_text--;
                        putchar(ch);
                    }
                    break;
                case '\n':
                    /*command sent*/
                    len_of_text = 0;
                    putchar(ch);
                    break;
                default:
                    len_of_text++;
                    putchar(ch);
                    break;
            
            }
     
        }
        
    }

    // tell the PIC we're done with IRQ1
    PIC_sendEOI(1);
}
