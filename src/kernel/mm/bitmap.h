#ifndef _BITMAP_H
#define _BITMAP_H

#include <common.h>

/* Bit manipulation functions */
#define bitmap_set(bitmap, b)	( (bitmap) |=  (1UL<<(b)) )
#define bitmap_unset(bitmap, b)	( (bitmap) &= ~(1UL<<(b)) )
#define bitmap_flip(bitmap, b)	( (bitmap) ^=  (1UL<<(b)) )
#define bitmap_test(bitmap, b)	(!! (bitmap) & (1UL<<(b)) )

#endif