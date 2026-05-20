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

dw 0 ; End tag
dw 0 ; No flags
dd 8 ; 8 bytes

header_end:

; Starting entry point
section .text
global _start
_start:
    cli ; no interruptions for now cause i hate the interruptions!
    
    ; time for some stack-foolery
    mov esp, stack_top ; one small step for compilers, one giant leap for kernel-kind

    ; ok now future me will like this! (c setup)
    push ebx
    push eax

    ; ok now finally lets print A!
    mov word [0xB8000], 0x0F41 ; A
.hang:
    hlt
    jmp .hang
; Stack
section .bss
align 16
stack_bottom:
    resb 16384 ; 16kb! WOW! so much kb!
stack_top: