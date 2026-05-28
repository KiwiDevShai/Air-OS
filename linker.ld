ENTRY(_start)
SECTIONS
{
    KERNEL_VMA = 0xFFFFFFFF80000000;
    . = 1M;
    .boot : { *(.multiboot) *(.boot) *(.boottables) }
    . = . + KERNEL_VMA;
    .text : AT(ADDR(.text) - KERNEL_VMA) { *(.text) }
    .rodata : AT(ADDR(.rodata) - KERNEL_VMA) { *(.rodata) }
    .data : AT(ADDR(.data) - KERNEL_VMA) { *(.data) }
    .bss : AT(ADDR(.bss) - KERNEL_VMA) { *(.bss) }
}