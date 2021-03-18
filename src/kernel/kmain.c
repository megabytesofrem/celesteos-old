#include <lib/libk.h>
#include <lib/string.h>

#include <sys/cpu/isr.h>
#include <sys/ports.h>
#include <mm/pmm.h>
#include <mm/vmm.h>

#include "multiboot.h"
#include "common.h"

/* vga text mode driver */
#include <device/vga.h>

#define BOOT_MSG "Booting CelesteOS!\n\n"

const char* DEBUG_MSGS[2] = { "kmain.c: loaded at KERN_HIGH_VMA, 0xffffffff80000000\n", \
							  "kmain.c: initialized vga at 0xb8000\n" };

int kmain(multiboot_info_t *info) {

	pmm_init(info);
	struct vmm_pagemap map = create_pagemap();

	/* map some memory to play w/ */
	for (size_t i = 0; i < (128 * 1024 * 1024); i += 0x1000) {
		vmm_map_page(&map, i, i, PAGE_WRITABLE);
	}

	for (size_t i = 0; i < (128 * 1024 * 1024); i += 0x1000) {
		vmm_map_page(&map, i + 0xFFFFFFFF80000000, i, PAGE_WRITABLE);
	}

	asm volatile("mov %0, %%cr3" : : "r"(map.pml4) : "memory");

	vga_init();
	klog(KLOG_INFO, "vga driver initialized at 0xb8000..\n");

	klog(KLOG_INFO, "setting up interrupts..\n");
	isr_install();

	klog(KLOG_INFO, BOOT_MSG);

	int *a = (int *)0x1000;
	*a = 1;
	
	for (;;) ;
}