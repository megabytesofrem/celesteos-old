#ifndef SYS_PORTS_H
#define SYS_PORTS_H

#include <common.h>

static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile ("inb %1, %0"
				  : "=a"(ret) 
				  : "Nd"(port));
	return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
	asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

#endif