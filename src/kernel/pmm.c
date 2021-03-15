#include <driver/vga.h>
#include <lib/string.h>
#include "pmm.h"

uint64_t total_memory;

void pmm_init(multiboot_info_t *info) {
	total_memory = 0;
	multiboot_entry_t *entries = (multiboot_entry_t*)info->mmap_addr;

	for (size_t i = 0; i < info->mmap_length; i++) {
		// loop through all entries in the memory map
		if (entries[i].type == 1)
			total_memory += entries[i].length;
	}

	vga_writes("total memory detected is ");
	vga_writes(utoa(total_memory));
	vga_writes("\n");
}