#include "multiboot_parse.h"
#include <stddef.h>

struct multiboot_tag *multiboot2_find_tag(
    struct multiboot2_info *mbi,
    uint32_t type
) {
    struct multiboot_tag *tag =
        (struct multiboot_tag *)((uint8_t *)mbi + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END) {

        if (tag->type == type)
            return tag;

        tag = (struct multiboot_tag *)
            ((uint8_t *)tag + ALIGN_UP(tag->size, 8));
    }

    return NULL;
}

struct multiboot_tag_mmap *multiboot2_get_mmap(
    struct multiboot2_info *mbi
) {
    return (struct multiboot_tag_mmap *)
        multiboot2_find_tag(mbi, MULTIBOOT_TAG_TYPE_MMAP);
}

struct multiboot_tag_framebuffer *multiboot2_get_framebuffer(
    struct multiboot2_info *mbi
) {
    return (struct multiboot_tag_framebuffer *)
        multiboot2_find_tag(mbi, MULTIBOOT_TAG_TYPE_FRAMEBUFFER);
}

struct multiboot_tag *multiboot2_get_acpi(
    struct multiboot2_info *mbi
) {
    struct multiboot_tag *tag;

    tag = multiboot2_find_tag(
        mbi,
        MULTIBOOT_TAG_TYPE_ACPI_NEW
    );

    if (tag)
        return tag;

    return multiboot2_find_tag(
        mbi,
        MULTIBOOT_TAG_TYPE_ACPI_OLD
    );
}