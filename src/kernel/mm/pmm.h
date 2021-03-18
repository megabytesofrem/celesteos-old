#ifndef _MM_PMM_H
#define _MM_PMM_H

#include <multiboot.h>
#include <common.h>

#include "bitmap.h"

#define PMM_PAGE_SIZE 4096			/* 4KiB pages are default */

struct multiboot_entry {
	unsigned int size;
	// unsigned int base_addr_low, base_addr_high;
	uint64_t addr;
	uint64_t length;
	unsigned int type;
} __attribute__((packed)) ;

void pmm_init(multiboot_info_t *info);
void* pmm_alloc(size_t count);
void pmm_free(void *ptr, size_t count);

#endif