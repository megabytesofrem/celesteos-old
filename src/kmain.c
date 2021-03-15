#include <lib/libk.h>
#include <lib/string.h>
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
	for (int i = 0; i < 2; i++)
		klog(KLOG_INFO, DEBUG_MSGS[i]);

	pmm_init(info);
	klog(KLOG_INFO, "[kmain.c] init pmm complete\n");

	for (;;) ;
}