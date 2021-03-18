/**
 * libk.c -- Provides useful kernel level utility functions
 * 
 * Authors:
 * 		Charlotte (charpointer)
 * 		Harry (harrego)
 */

#include <device/vga.h>
#include <lib/string.h>

#include "libk.h"

#ifdef LOG_TO_E9
#include <kernel/sys/ports.h> /* for outb */
#endif


/**
 * internal utility functions for printing to the kernel log
 */

void __kprintc(char c) {
	vga_putchar(c);
	#ifdef LOG_TO_E9
		outb(0xE9, c);
	#endif
}

void __kprints(char *s) {
	while (*s != 0) {
		__kprintc(*s);
		*s++;
	}
}

void __kprintd(int d) {
	char* s = itoa(d, 10);

	while (*s != 0) {
		vga_putchar(*s);
		#ifdef LOG_TO_E9
			outb(0xE9, *s);
		#endif
		*s++;
	}
}

void __kprint_unsigned(uint64_t ud) {
	char *s = utoa(ud);

	while (*s != 0) {
		vga_putchar(*s);
		#ifdef LOG_TO_E9
			outb(0xE9, *s);
		#endif
		*s++;
	}
}

void kputs(char *s) {
	size_t len = strlen(s);
	s[len] = '\0';

	vga_write(s, len);
}

void printk(const char *fmt, ...) {
	int i = 0;

	va_list args;
	va_start(args, fmt);


	while (fmt[i]) {
		if (fmt[i] == '%' && fmt[i+1] == 'c') {
			char c = va_arg(args, int);
			__kprintc(c);
			i++;
		}
		else if (fmt[i] == '%' && fmt[i+1] == 'd') {
			int d = va_arg(args, int);
			__kprintd(d);
			i++;
		}
		else if (fmt[i] == '%' && fmt[i+1] == 'u') {
			i++;
			if (fmt[i+1] == 'd') {
				uint64_t ud = va_arg(args, uint64_t);
				__kprint_unsigned(ud);
				i++;
			}
			else if (fmt[i+1] == 'l') {
				uint64_t ul = va_arg(args, uint64_t);
				__kprint_unsigned(ul);
				i++;
			}
		}
		else if (fmt[i] == '%' && fmt[i+1] == 's') {
			char *s = va_arg(args, char*);
			__kprints(s);
			i++;
		}
		else if (fmt[i] == '%' && fmt[i+1] == '%') {
			vga_putchar('%');
			#ifdef LOG_TO_E9
				outb(0xE9, '%');
			#endif
		}
		else {
			__kprintc(fmt[i]);
		}
		i++;
	}

	va_end(args);
}

void klog(enum klog_level level, const char *fmt, ...) {
	int i = 0;

	/* switch on the level */
	switch (level) {
		case KLOG_INFO:
			vga_setcolor(vga_entry_color(VGA_COLOR_GREY, VGA_COLOR_BLACK));
			vga_write("[INFO] ", strlen("[INFO] "));
			break;
		case KLOG_WARN:
			vga_setcolor(vga_entry_color(VGA_COLOR_BROWN, VGA_COLOR_BLACK));
			vga_write("[WARN] ", strlen("[WARN] "));
			break;
		case KLOG_ERROR:
			vga_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
			vga_write("[ERROR] ", strlen("[ERROR] "));
			break;
		case KLOG_NONE:
			/* No prefix */
			vga_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
			break;
	}

	va_list args;
	va_start(args, fmt);

	vga_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));

	while (fmt[i]) {
		if (fmt[i] == '%' && fmt[i+1] == 'c') {
			char c = va_arg(args, int);
			__kprintc(c);
			i++;
		}
		else if (fmt[i] == '%' && fmt[i+1] == 'd') {
			int d = va_arg(args, int);
			__kprintd(d);
			i++;
		}
		else if (fmt[i] == '%' && fmt[i+1] == 'u') {
			i++;
			if (fmt[i+1] == 'd') {
				uint64_t ud = va_arg(args, uint64_t);
				__kprint_unsigned(ud);
				i++;
			}
			else if (fmt[i+1] == 'l') {
				uint64_t ul = va_arg(args, uint64_t);
				__kprint_unsigned(ul);
				i++;
			}
		}
		else if (fmt[i] == '%' && fmt[i+1] == 's') {
			char *s = va_arg(args, char*);
			__kprints(s);
			i++;
		}
		else if (fmt[i] == '%' && fmt[i+1] == '%') {
			vga_putchar('%');
			#ifdef LOG_TO_E9
				outb(0xE9, '%');
			#endif
		}
		else {
			__kprintc(fmt[i]);
		}
		i++;
	}

	va_end(args);
}