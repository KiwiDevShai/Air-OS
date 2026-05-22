#pragma once

#include <stdint.h>
#include "multiboot2.h"
#include "common.h"

struct multiboot_tag *multiboot2_find_tag(
    struct multiboot2_info *mbi,
    uint32_t type
);

struct multiboot_tag_mmap *multiboot2_get_mmap(
    struct multiboot2_info *mbi
);

struct multiboot_tag_framebuffer *multiboot2_get_framebuffer(
    struct multiboot2_info *mbi
);

struct multiboot_tag *multiboot2_get_acpi(
    struct multiboot2_info *mbi
);