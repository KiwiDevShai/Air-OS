#pragma once
#include <stdint.h>

#define ALIGN_UP(x, a) \
    (((uintptr_t)(x) + ((uintptr_t)(a) - 1)) & ~((uintptr_t)(a) - 1))

#define ALIGN_DOWN(x, a) \
    ((uintptr_t)(x) & ~((uintptr_t)(a) - 1))

struct multiboot2_info {
    uint32_t total_size;
    uint32_t reserved;
};