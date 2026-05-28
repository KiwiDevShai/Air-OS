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

; Code
[BITS 32]
section .boot
global _start
extern kmain


; cpuid/longmode
EFLAGS_ID equ 1 << 21
CPUID_EXTENSIONS equ 0x80000000
CPUID_EXT_FEATURES equ 0x80000001

check_cpuid:
    pushfd
    pop eax
    mov ecx, eax
    xor eax, EFLAGS_ID
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    xor eax, ecx
    test eax, EFLAGS_ID
    jz .not_supported
    ret
.not_supported:
    mov word [0xB8000], 0x0F43 ; No CPUID (C)
.hang:
    hlt
    jmp .hang

check_long_mode:
    mov eax, CPUID_EXTENSIONS
    cpuid

    cmp eax, CPUID_EXT_FEATURES
    jb .not_supported

    mov eax, CPUID_EXT_FEATURES
    cpuid

    test edx, 1 << 29
    jz .not_supported
    ret
.not_supported:
    mov word [0xB8000], 0x0F4C ; No Longmode (L)
.hang:
    hlt
    jmp .hang

; set up paging
PRESENT equ 1
WRITABLE equ 2
HUGE equ 128
PAE equ 1 << 5
LME equ 1 << 8
EFER_MSR equ 0xC0000080
PG equ 1 << 31

paging_setup:
    ; set pml4
    mov eax, pdpt_table
    or eax, PRESENT | WRITABLE
    mov [pml4_table], eax
    mov dword [pml4_table + 4], 0
    mov [pml4_table + 4088], eax
    mov dword [pml4_table + 4092], 0

    ; set pdpt
    mov eax, pd_table
    or eax, PRESENT | WRITABLE
    mov [pdpt_table], eax
    mov dword [pdpt_table + 4], 0
    mov [pdpt_table + 4080], eax
    mov dword [pdpt_table + 4084], 0

    ; fill pd table
    mov ecx, 0 ; loop counter
    mov eax, 0 ; physical address
.loop:
    mov edx, eax
    or edx, PRESENT | WRITABLE | HUGE ; MASSIVE!!!
    mov dword [pd_table + ecx * 8], edx
    mov dword [pd_table + ecx * 8 + 4], 0
    add eax, 0x200000
    inc ecx
    cmp ecx, 512
    jne .loop
    ret

enable_pae:
    mov eax, cr4
    or eax, PAE
    mov cr4, eax
    ret

enable_lme:
    mov ecx, EFER_MSR
    rdmsr
    or eax, LME
    wrmsr
    ret

enable_paging:
    mov eax, cr0
    or eax, PG
    mov cr0, eax
    ret
    
; start
_start:
    cli
    cmp eax, 0x36D76289
    jne .hang
    
    ; save multiboot info ptr
    mov [multiboot_info_ptr], ebx

    ; stack setup
    mov esp, stack_top

    ; check cpuid
    call check_cpuid
    
    ; check long mode
    call check_long_mode

    ; paging (the lord gives me his toughest battles)
    call paging_setup
    mov eax, pml4_table
    mov cr3, eax
    call enable_pae
    call enable_lme

    lgdt [gdt_descriptor]
    call enable_paging
    jmp 0x08:trampoline
.hang:
    hlt
    jmp .hang
[BITS 64]
trampoline:
    mov rax, long_mode_start
    jmp rax
section .text
long_mode_start:
    ; we have liftoff
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov rsp, stack_top
    mov rdi, [multiboot_info_ptr]
    call kmain
.hang:
    hlt
    jmp .hang

; GDT
section .boot
align 8

gdt_start:
    dq 0 ; null descriptor
    dq 0x00209A0000000000 ; kernel code
    dq 0x0000920000000000 ; kernel data
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dq gdt_start

; Bootables
section .boottables nobits

; Stack
align 16
stack_bottom:
    resb 16384 ; 16kb
stack_top:

; Paging stuff
align 4096
pml4_table:
    resb 4096

align 4096
pdpt_table:
    resb 4096

align 4096
pd_table:
    resb 4096

; Multiboot info ptr
align 8
multiboot_info_ptr:
    resq 1