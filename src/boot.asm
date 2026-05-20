; Multiboot2 header

section .multiboot
align 8
magic equ 0xE85250D6
arch equ 0
length equ header_end - header_start

header_start:
dd magic
dd arch
dd length
dd -(magic + arch + length)

; Tags

align 8
info_req:
    dw 1 ; information request tag
    dw 0 ; flags
    dd info_req_end - info_req ; length

    dd 6 ; memory map
    dd 8 ; framebuffer
    dd 15 ; acpi
info_req_end:

align 8
dw 0 ; End tag
dw 0 ; Flag
dd 8 ; 8 bytes

header_end:

; Starting entry point
section .text
global _start
extern kmain
_start:
    cli ; no interruptions for now cause i hate the interruptions!
    cmp eax, 0x36D76289
    jne .hang
    
    ; time for some stack-foolery (c needs stack so this is nice)
    mov esp, stack_top ; one small step for compilers, one giant leap for kernel-kind

    lgdt [gdt_descriptor]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.gdt_loaded

.gdt_loaded:
    push ebx
    call kmain
.hang:
    ; we shouldn't have gotten hear, so we halt and catch fire!
    hlt
    jmp .hang

; GDT
section .rodata
align 8

gdt_start:
    dq 0 ; null descriptor
    dq 0x00CF9A000000FFFF ; kernel code
    dq 0x00CF92000000FFFF ; kernel data
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; Stack
section .bss
align 16
stack_bottom:
    resb 16384 ; 16kb! WOW! so much kb!
stack_top: