#include "printf/putchar.h"
#include <stdint.h>

volatile uint16_t *vga = (volatile uint16_t *)0xB8000;
uint8_t attr = 0x0F;

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define LAST_COL 79
#define LAST_ROW 24
#define ROW_BYTES 0xA0
#define ROW_CELLS 80

int current_row = 0;
int current_col = 0;

int row_size = 80;
int last_row = 24;

int get_index(int row, int col) {
    return row * row_size + col;
}

void scroll() {
    uint16_t blank = (attr << 8) | ' ';
    for (int row = 1; row <= last_row; row++) {
        for (int col = 0; col < row_size; col++) {
            vga[(row - 1) * row_size + col] = vga[row * row_size + col];
        }
    }
    for (int col = 0; col < row_size; col++) {
        vga[last_row * row_size + col] = blank;
    }
    current_row = last_row;
    current_col = 0;
}

void putchar_(char c) {
    // Write to screen char-by-char, take \n as newline,
    // if final row/column is passed, clear the top row
    // and move all rows up by one to simulate scrolling.
    uint16_t blank = (attr << 8) | ' ';
    if (c == '\n') {
        current_col = 0;
        current_row++;

        if (current_row > last_row) {
            scroll();
        }

        return;
    }
    if (c == '\r') {
        current_col = 0;
        return;
    }
    if (c == '\t') {
        current_col = (current_col + 8) & ~(8 - 1);

        if (current_col > LAST_COL) {
            current_col = 0;
            current_row++;
        }

        if (current_row > last_row) {
            scroll();
        }

        return;
    }
    if (c == '\b') {
        if (current_col > 0) {
            current_col--;
        }

        vga[get_index(current_row, current_col)] =
            (attr << 8) | ' ';

        return;
    }
    vga[get_index(current_row, current_col)] = (attr << 8) | c;
    current_col++;
    if (current_col > LAST_COL) {
        current_col = 0;
        current_row++;
    }
    if (current_row > last_row) {
        scroll();
    }
}