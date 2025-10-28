#pragma once

static inline int kbd_enqueue_char(char ch);
int kbd_poll_char(char *out);

void keyboard_isr(void);
