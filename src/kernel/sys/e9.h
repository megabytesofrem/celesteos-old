/**
 * e9.h -- functions for writing to port 0xE9 (Qemu/Boschs debug port)
 * 
 * Authors:
 * 		Charlotte (charpointer)
 */

#ifndef E9_H
#define E9_H

#include <kernel/sys/ports.h>

static inline void e9_write(const char *s) {
    while (*s != '\0') {
		outb(0xE9, *s++);
	}
}

#endif