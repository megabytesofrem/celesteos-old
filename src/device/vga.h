#ifndef VGA_DRIVER_H
#define VGA_DRIVER_H
#include <common.h>

#define VGA_TEXTMODE_WIDTH  80
#define VGA_TEXTMODE_HEIGHT 25

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_GREY = 8,

	/* light colors */
	VGA_COLOR_LGREY = 7,
	VGA_COLOR_LBLUE = 9,
	VGA_COLOR_LGREEN = 10,
	VGA_COLOR_LCYAN = 11,
	VGA_COLOR_LRED = 12,
	VGA_COLOR_LMAGENTA = 13,
	VGA_COLOR_LBROWN = 14,
	VGA_COLOR_WHITE = 15

};

static inline uint16_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

void vga_init();
void vga_setcolor(uint8_t color);
void vga_putat(char c, uint8_t color, size_t x, size_t y);
void vga_putchar(char c);
void vga_write(const char *s, size_t len);
void vga_writes(const char *s);
void vga_clear();

#endif /* vga driver */