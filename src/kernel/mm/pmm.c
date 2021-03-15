/**
 * pmm.c -- Physical memory manager for the kernel
 * 
 * How we allocate:
 * 
 * 1) The memory map entries are looped through until we find the total amount
 *    of physical memory in the system.
 * 2) We find a location with enough free pages to hold our bitmap
 * 3) We clear out the bitmap by marking all pages as in use
 * 4) We then unset the necessary amount of bits to mark them as free
 * 
 * Authors:
 * 		Charlotte (charpointer)
 * 		Harry (harrego)
 */


#include <driver/vga.h>
#include <lib/libk.h>
#include <lib/string.h>

#include "pmm.h"

static void *bitmap;

/* Total physical memory */
uint64_t total_memory;

void pmm_init(multiboot_info_t *info) {
	total_memory = 0;
	multiboot_entry_t *mmap_entries = (multiboot_entry_t*)info->mmap_addr;

	int nentries = (info->mmap_length / sizeof(multiboot_entry_t));

	for (size_t i = 0; i < nentries; i++) {
		// loop through all mmap_entries in the memory map
		if (mmap_entries[i].type == MULTIBOOT_MEMORY_AVAILABLE)
			total_memory += mmap_entries[i].length;
	}

	// calculate size of the bitmap
	size_t bmap_size = total_memory / PMM_PAGE_SIZE / sizeof(uint8_t);

	// find a location with enough free pages
	for (size_t i = 0; i < nentries; i++) {
		// skip over any mmap_entries that aren't available
		if (mmap_entries[i].type != MULTIBOOT_MEMORY_AVAILABLE)
			continue;

		if (mmap_entries[i].length >= bmap_size) {
			// we have a location with enough free pages to store our bitmap
			bitmap = (void *)(mmap_entries[i].addr); // offset by 0xffff800000000000 later when we have higher half direct mapping
			memset(bitmap, 0xff, bmap_size);

			// shrink the memory map
			mmap_entries[i].length -= bmap_size;
			mmap_entries[i].addr += bmap_size;

			break;
		}
	}

	for (size_t i = 0; i < nentries; i++) {
		// skip over any mmap_entries that aren't available
		if (mmap_entries[i].type != MULTIBOOT_MEMORY_AVAILABLE)
			continue;

		for (uintptr_t j = 0; j < mmap_entries[i].length; j += PMM_PAGE_SIZE) {
			// TODO: unset the bitmap so its free
		}
	}
	kprintf("total mem: %d\n", total_memory);
	kprintf("number of entries: %d\n", nentries);
}