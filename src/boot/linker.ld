ENTRY(_start)

SECTIONS
{
    . = 1M; /* Start at 1MB (GRUB loads kernels here) */

    .boot : {
        KEEP(*(.multiboot_header)) /* Place Multiboot2 header first */
    }

    .text : {
        *(.text) /* Kernel code */
    }

    .rodata : {
        *(.rodata) /* Read-only data (like strings) */
    }

    .data : {
        *(.data) /* Global variables */
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }
}
