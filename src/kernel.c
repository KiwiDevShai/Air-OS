#include <stdint.h>
#include "io.h"
#include "vendor/multiboot2.h"

void tmp_vga_write(char c, int x, int y) {
    // this is temporary so we dont need to optimize it
    if (x < 0 || x >= 80 || y < 0 || y >= 25) return;
    volatile uint8_t *vga = (volatile uint8_t *)0xB8000;
    int offset = (y * 80 + x) * 2;
    vga[offset] = c;
    vga[offset + 1] = 0x0F; 
}
void print(char* str, int x, int y) {
    // also temporary i just didnt add tmp
    int cx = x;
    int cy = y;
    int i = 0;
    while (str[i])
    {
        if (str[i] == '\n') {
            cy++;
            cx = 0;
            i++;
            continue;
        }
        
        tmp_vga_write(str[i], cx, cy);
        cx++;
        i++;
        
        if (cx >= 80) {
            cy++;
            cx = 0;
        }
    }
}
void hcf(void) {
    asm volatile (
        "cli\n\t"
        "1:\n\t"
        "hlt\n\t"
        "jmp 1b"
    );
}
void kmain(int32_t multiboot_info_pointer) {
	// first disable cursor to save my eyes!
    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
    // if it booted we will see "Booted!"
    print("Booted!", 0, 0);
    // DEATH
    hcf();
}