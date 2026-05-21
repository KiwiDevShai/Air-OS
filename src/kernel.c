#include <stdint.h>
#include "io.h"
#include "vendor/multiboot2.h"



__attribute__((noreturn))
void hcf(void) {
    asm volatile ("cli" ::: "memory");

    for (;;) {
        asm volatile ("hlt");
    }

    __builtin_unreachable();
}

void kmain(uintptr_t multiboot_info_pointer) {
	// first disable cursor to save my eyes!
    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
    // Hello from C
    for (char *s = "Hello from C", *v = (char*)0xB8000; *s; *v++ = *s++, *v++ = 7);
    // DEATH
    hcf();
}