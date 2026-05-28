#include <stdint.h>
#include "io.h"
#include "panic.h"
#include "printf/printf.h"
#include "common.h"
#include "multiboot_parse.h"

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
        PANIC("Failed getting memory map!");
    }
    printf_(" Success!\n");
    printf_("kmain() pointer test: %p", kmain);
    // DEATH
    hcf();
}