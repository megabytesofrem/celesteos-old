#include <common.h>
#include "vga.h"
#include <lib/string.h>

size_t vga_row;
size_t vga_col;
uint8_t vga_color;
uint16_t *vga_buffer;

void vga_init() {
	vga_row = 0;
	vga_col = 0;
	vga_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

	/* 0xB8000 is the address where video memory is stored */
	vga_buffer = (uint16_t*) 0xB8000;

	vga_clear();
}

void vga_setcolor(uint8_t color) {
	vga_color = color;
}

void vga_putchar(char c) {
	if (c == '\n') {
		// increment the row if we need

		vga_col = 0;
		vga_row++;
		return;
	}

	// reset the column and row if we overflow
	if (++vga_col == VGA_TEXTMODE_WIDTH) {
		vga_col = 0;
		if (++vga_row == VGA_TEXTMODE_HEIGHT)
			vga_row = 0;
	}

	vga_putat(c, vga_color, vga_col, vga_row);
}

void vga_putat(char c, uint8_t color, size_t x, size_t y) {
	vga_buffer[y * VGA_TEXTMODE_WIDTH + x] = (uint16_t)c | (uint16_t)color << 8;
}

void vga_write(const char *s, size_t len) {
	for (size_t i = 0; i < len; i++)
		vga_putchar(s[i]);
}

/* vga_write but is smart enough to correctly figure out the string length */
void vga_writes(const char *s) {
	vga_write(s, strlen(s));
}

void vga_clear() {
	memset(0xB8000, 0, 80*25*2);
}