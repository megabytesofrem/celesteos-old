#include "common.h"
#include <lib/string.h>

/* vga text mode driver */
#include <driver/vga.h>

#define BOOT_MSG "Booting CelesteOS..\n\n"

const char* DEBUG_MSGS[2] = { "[kmain.c] loaded at KERN_HIGH_VMA, 0xffffffff80000000\n", \
							  "[kmain.c] initialized vga at 0xb8000\n" };

int kmain() {
	vga_init();

	vga_setcolor(vga_entry_color(VGA_COLOR_LBLUE, VGA_COLOR_BLACK));
	vga_writes(BOOT_MSG);

	vga_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	/* debug */
	for (int i = 0; i < 2; i++)
		vga_writes(DEBUG_MSGS[i]);

	for (;;) ;
}