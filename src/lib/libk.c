/**
 * libk.c -- Provides useful kernel level utility functions
 * 
 * Authors:
 * 		Charlotte (charpointer)
 * 		Harry (harrego)
 */

#include <driver/vga.h>
#include <lib/string.h>

#include "libk.h"

#ifdef LOG_TO_E9
#include <kernel/sys/ports.h> /* for outb */
#endif

void kputs(char *s) {
	size_t len = strlen(s);
	s[len] = '\0';

	vga_write(s, len);
}

void kprintf(const char *fmt, ...) {
	int i = 0;

	va_list args;
	va_start(args, fmt);

	while (fmt[i]) {
		if (fmt[i] == '%' && fmt[i+1] == 'c') {
			char c = va_arg(args, int);
			vga_putchar(c);
			i++;
		}
		else if (fmt[i] == '%' && fmt[i+1] == 'd') {
			int d = va_arg(args, int);
			char* s = itoa(d, 10);

			while (*s != '\0') {
				vga_putchar(*s++);
			}
			i++;
		}
		else if (fmt[i] == '%' && fmt[i+1] == 'u') {
			i++;
			if (fmt[i+1] == 'd') {
				uint64_t ud = va_arg(args, uint64_t);
				char *s = utoa(ud);

				while (*s != '\0') {
					vga_putchar(*s++);
				}
				i++;
			}
			else if (fmt[i+1] == 'l') {
				uint64_t ul = va_arg(args, uint64_t);
				char *s = utoa(ul);

				while (*s != '\0') {
					vga_putchar(*s++);
				}
				i++;
			}
		}
		else if (fmt[i] == '%' && fmt[i+1] == 's') {
			char *s = va_arg(args, char*);
			for (size_t j = 0; j < strlen(s); j++)
				vga_putchar(s[j]);
			i++;
		}
		else if (fmt[i] == '%' && fmt[i+1] == '%') {
			vga_putchar('%');
		}
		else {
			vga_putchar(fmt[i]);
		}
		i++;
	}

	va_end(args);
}

void klog(klog_level level, const char *s) {

	// TODO: replace vga_write calls with kprintf
	switch (level) {
		case KLOG_INFO:
			vga_setcolor(vga_entry_color(VGA_COLOR_LGREY, VGA_COLOR_BLACK));
			break;
		case KLOG_WARN:
			vga_setcolor(vga_entry_color(VGA_COLOR_LBROWN, VGA_COLOR_BLACK));
			break;
		case KLOG_ERROR:
			vga_setcolor(vga_entry_color(VGA_COLOR_LRED, VGA_COLOR_BLACK));
			break;
		default: break;
	}

	kputs(s);

#ifdef LOG_TO_E9
	while (*s != '\0') {
		outb(0xE9, *s++);
	}
#endif
}