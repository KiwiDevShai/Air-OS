#include <stdint.h>
#include "io.h"
#include "printf/printf.h"
#include "common.h"
#include "multiboot_parse.h"

__attribute__((noreturn))
void hcf(void) {
    asm volatile ("cli" ::: "memory");

    for (;;) {
        asm volatile ("hlt");
    }

    __builtin_unreachable();
}

void kmain(uintptr_t multiboot_info_ptr) {
	// first disable cursor to save my eyes!
    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
    // Hello from C
    printf_("Long mode entered.\n");
    printf_("Attempting to get memory map...");
    struct multiboot2_info *mbi = (struct multiboot2_info *)multiboot_info_ptr;
    struct multiboot_tag_mmap *mmap = multiboot2_get_mmap(mbi);
    if (mmap == NULL) {
        printf_(" ERROR: NO MMAP!");
        hcf();
        // once we have panic() we'll use that
    }
    printf_(" Success!");
    // DEATH
    hcf();
}