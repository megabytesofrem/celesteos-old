#include <lib/libk.h>
#include <lib/string.h>
#include <kernel/sys/isr.h>
#include <kernel/sys/ports.h>
#include <kernel/mm/pmm.h>

#include <multiboot.h>
#include "common.h"

/* vga text mode driver */
#include <driver/vga.h>

#define BOOT_MSG "Booting CelesteOS..\n\n"

const char* DEBUG_MSGS[2] = { "kmain.c: loaded at KERN_HIGH_VMA, 0xffffffff80000000\n", \
							  "kmain.c: initialized vga at 0xb8000\n" };

int kmain(multiboot_info_t *info) {
	vga_init();

	klog(KLOG_INFO, BOOT_MSG);

	// /* debug */
	for (int i = 0; i < 2; i++)
		klog(KLOG_INFO, DEBUG_MSGS[i]);

	/* install ISRs */
	isr_install();

	/* cause a page fault since -1 is unmapped */
	int *a = (int *)-1;
	a = 1;


	// test pmm
	pmm_init(info);
	void *one = pmm_alloc(5);

	// // TODO: add %x formatting to kprintf, klog
	kprintf("one is at address %ud\n", &one);
	// print_bitmap();

	pmm_free(one, 5);
	klog(KLOG_INFO, "kmain.c: init pmm complete\n");

	/* kmain should never return */
	for (;;) ;
}