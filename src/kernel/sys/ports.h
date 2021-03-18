#ifndef PORTS_H
#define PORTS_H

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

static inline void io_wait(void) {
	/* Port 0x80 is used for 'checkpoints' during POST. */
	/* The Linux kernel seems to think it is free for use :-/ */
	asm volatile ( "outb %%al, $0x80" : : "a"(0) );
	
	/* %%al instead of %0 makes no difference.  TODO: does the register need to be zeroed? */
}

#endif