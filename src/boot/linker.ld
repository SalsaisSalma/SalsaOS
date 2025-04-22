ENTRY(_start)

SECTIONS
{
    . = 1M; /* Load at 1MB */

    .boot : {
        KEEP(*(.multiboot_header))
    }

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(COMMON)
        *(.bss)

        /* defining heap symbols */
        . = ALIGN(4096);
        _heap_start = .;
        . = . + (64 * 1024); /* define a 64KB heap */
        _heap_end = .;
    }
}
