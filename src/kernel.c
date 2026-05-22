#include <stdint.h>
#include "io.h"
#include "printf/printf.h"
#include "multiboot2.h"

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
    printf_("Hello from C!\nTest: 5 + 5 = %i\n", 5 + 5);
    // DEATH
    hcf();
}