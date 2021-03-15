#ifndef MM_PMM_H
#define MM_PMM_H

#include <multiboot.h>
#include <common.h>

#define PMM_PAGE_SIZE 4096			/* 4KiB pages are default */

#define BITMAP_BIT_FREE 0x0			/* a free bit is marked with 0x00 */
#define BITMAP_BIT_USED 0x1			/* a bit that is in use is marked with 0x1 */

typedef struct __attribute__((packed)) {
	unsigned int size;
	// unsigned int base_addr_low, base_addr_high;
	uint64_t addr;
	uint64_t length;
	unsigned int type;
} multiboot_entry_t;

void pmm_init(multiboot_info_t *info);
#endif