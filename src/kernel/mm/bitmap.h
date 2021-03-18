#ifndef _BITMAP_H
#define _BITMAP_H

#include <common.h>

/* Bit manipulation functions */
static inline void bitmap_set(uint8_t *bitmap, size_t idx) {
	bitmap[idx / 8] |= 1 << (idx % 8);
}

static inline void bitmap_unset(uint8_t *bitmap, size_t idx) {
	bitmap[idx / 8] &= ~(1 << (idx % 8));
}

static inline void bitmap_flip(uint8_t *bitmap, size_t idx) {
	bitmap[idx / 8] ^= 1 << (idx % 8);
}

static inline int bitmap_test(uint8_t *bitmap, size_t idx) {
	return !!(bitmap[idx / 8] & (1 << (idx % 8)));
}

#endif