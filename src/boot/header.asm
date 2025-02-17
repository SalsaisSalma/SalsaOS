SECTION .multiboot
align 8
multiboot_start:
    dd 0xE85250D6
    dd 0
    dd multiboot_end - multiboot_start
    dd -(0xE85250D6 + 0 + (multiboot_end - multiboot_start))

    ; End tag
    dd 0
    dd 8
multiboot_end:
