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

#include <device/vga.h>
#include <lib/libk.h>
#include <lib/string.h>

#include <kernel/sys/e9.h>
#include "pmm.h"

static void *bitmap;
static size_t last_page = 0;
static size_t bitmap_size = 0;

/* Total physical memory */
uint64_t total_memory;

void pmm_init(multiboot_info_t *info) {
	total_memory = 0;

	/* get total entries in the memory map */
	struct multiboot_entry *mmap_entries = (struct multiboot_entry*)info->mmap_addr;

	int nentries = (info->mmap_length / sizeof(struct multiboot_entry));

	// 1. calculate size of the bitmap
	for (size_t i = 0; i < nentries; i++) {
		if (mmap_entries[i].type == MULTIBOOT_MEMORY_AVAILABLE)
			total_memory += mmap_entries[i].length;
	}

	// calculate size of the bitmap
	size_t factor = PMM_PAGE_SIZE;
	bitmap_size = (total_memory + factor - 1) / factor / 8;

	// find a location with enough free pages
	for (size_t i = 0; i < nentries; i++) {
		// skip over any mmap_entries that aren't available
		if (mmap_entries[i].type != MULTIBOOT_MEMORY_AVAILABLE)
			continue;

		if (mmap_entries[i].length >= bitmap_size) {
			// we have a location with enough free pages to store our bitmap
			bitmap = (void *)(mmap_entries[i].addr); // offset by 0xffff800000000000 later when we have higher half direct mapping
			memset(bitmap, 0xff, bitmap_size);

			// shrink the memory map
			mmap_entries[i].length = (mmap_entries[i].length - bitmap_size) & ~(PMM_PAGE_SIZE - 1);
			mmap_entries[i].addr = (mmap_entries[i].addr + bitmap_size + PMM_PAGE_SIZE - 1) & ~(PMM_PAGE_SIZE - 1);

			break;
		}
	}

	for (size_t i = 0; i < nentries; i++) {
		// skip over any mmap_entries that aren't available
		if (mmap_entries[i].type != MULTIBOOT_MEMORY_AVAILABLE)
			continue;

		// set all "blocks" in the bitmap to free
		for (uintptr_t j = 0; j < mmap_entries[i].length & ~0xFFFUL; j += PMM_PAGE_SIZE) {
			bitmap_unset((uint8_t*)bitmap, (mmap_entries[i].addr + j) / PMM_PAGE_SIZE);
		}
	}
	klog(KLOG_INFO, "total mem: %d\n", total_memory);
	klog(KLOG_INFO, "number of entries: %d\n", nentries);
}

static void* inner_alloc(size_t count, size_t limit) {
	/**
	 * The limit is the highest page the memory manager is allowed to allocate.
	 * While we have a page lower than the limit, make sure the next page isn't allocated.
	 * 
	 * If it isn't, loop until the count is reached to skip over blocks we don't care about.
	 * This also makes sure we don't encounter an already allocated page in the middle of our block.
	 */

	size_t p = 0;

	for (size_t i = 0; i < limit; i++) {
		if (!bitmap_test((uint8_t*)bitmap, i)) {
			// If the internal counter is the count of pages, set the page as used
			if (++p == count) {
				size_t page = i - count + 1;

				// Set the page as used
				for (size_t j = page; j <= i; j++) {
					bitmap_set((uint8_t*)bitmap, j);
				}

				// return the address of the page
				void *addr = (void *)(page * PMM_PAGE_SIZE);
				return addr;
			}
		}
		else {
			p = 0;
		}
	}

	// If we get here, we couldn't allocate a page so just return NULL
	return NULL;
}

void* pmm_alloc(size_t count) {
	/**
	 * To allocate a block we must:
	 * 1) Calculate the number of blocks required (by rounding up)
	 * 2) Iterate over the bitmap, looking for a series (of atleast the requested amount of) contiguous free blocks
	 * 3) Mark the blocks we have allocated as used
	 */
	klog(KLOG_INFO, "allocated %d pages\n", count);

	void *addr = inner_alloc(count, total_memory / PMM_PAGE_SIZE);
	printk("addr returned: %ul\n", addr);

	return addr;
}

void pmm_free(void *ptr, size_t count) {
	size_t page = (size_t)ptr / PMM_PAGE_SIZE;

	klog(KLOG_INFO, "freeing %d pages\n", count);

	// Loop through the bitmap and unset the bits for our page
	for (size_t i = page; i < page + count; i++) {
		klog(KLOG_INFO, "freed page %d/%d\n", i, count);
		bitmap_unset((uint8_t*)bitmap, i);
	}
}