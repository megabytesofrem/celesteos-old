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

const char* DEBUG_MSGS[2] = { "[kmain.c] loaded at KERN_HIGH_VMA, 0xffffffff80000000\n", \
							  "[kmain.c] initialized vga at 0xb8000\n" };

int kmain(multiboot_info_t *info) {
	vga_init();

	klog(KLOG_INFO, BOOT_MSG);

	/* debug */
	// for (int i = 0; i < 2; i++)
	// 	klog(KLOG_INFO, DEBUG_MSGS[i]);

	klog(KLOG_WARN, "we are going to cause a PF :D\n");
	/* install ISRs */
	isr_install();

	/* cause a page fault since -1 is unmapped */
	// int *a = (int *)-1;
	// *a = 1;

	pmm_init(info);
	klog(KLOG_INFO, "[kmain.c] init pmm complete\n");

	for (;;) ;
}