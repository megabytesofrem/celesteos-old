/**
 * vmm.c -- Virtual memory manager for the kernel
 * 
 * What it does:
 * 
 * The VMM allows userland processes to be mapped in potentially conflicting memory
 * addresses without messing up the kernel or each other.
 * 
 * Authors:
 * 		Charlotte (charpointer)
 */

#include <lib/string.h>

#include "pmm.h"
#include "vmm.h"

struct vmm_pagemap create_pagemap() {
	struct vmm_pagemap map = {0};
	
	uintptr_t p4 = (uintptr_t)pmm_alloc(1);
	memset(p4, 0, 0x1000);

	map.pml4 = p4;
	return map;
}

static inline bool entry_present(uintptr_t *table, uintptr_t idx) {
	return ((table[idx]) & 1);
}

/**
 * Translate a physical address to a virtual address by offsetting by
 * the higher half offset.
 */
uintptr_t phys_to_virt(uintptr_t phys) {
	return phys + HIGHERHALF_OFFSET;
}

/**
 * Walk the page tables, and unmap a page at the specified virtual address
 * 
 * @param pagemap: the page map to walk
 * @param virt: the virtual address to unmap
 */
void vmm_unmap_page(struct vmm_pagemap *pagemap, uintptr_t virt) {
	uintptr_t pml4_i = (virt >> 39) & 0x1FF;
	uintptr_t pml3_i = (virt >> 30) & 0x1FF;
	uintptr_t pml2_i = (virt >> 21) & 0x1FF;
	uintptr_t pml1_i = (virt >> 12) & 0x1FF;

	uintptr_t *pml4 = pagemap->pml4;
	
	/* walk the page tables */
	if (!entry_present(pml4, pml4_i))
		return;

	uintptr_t *pml3 = phys_to_virt(pml4[pml4_i & ~(0x1ff)]);
	if (!entry_present(pml3, pml4_i))
		return;

	uintptr_t *pml2 = phys_to_virt(pml3[pml3_i & ~(0x1ff)]);
	if (!entry_present(pml2, pml3_i))
		return;

	uintptr_t *pml1 = phys_to_virt(pml2[pml2_i & ~(0x1ff)]);
	if (!entry_present(pml1, pml2_i))
		return;

	pml1[pml2_i] = 0;
	asm volatile("invlpg %0" : : "m"(virt) : "memory");
}

void vmm_map_page(struct vmm_pagemap *pagemap, uintptr_t virt, uintptr_t phys, enum vmm_page_flags flags) {
	/**
	 * Our goal here is to map PHYS to VIRT in PAGEMAP with the set flags of FLAGS
	 * 
	 * How we do it:
	 * 
	 * The formula to split an address into its paging indices is:
	 * FORMULA: (virt_addr >> ((bits_per_level * (index - 1)) + 4k_gran))
	 * So for the PML 4, ((9 * (4 - 1)) + 12) = 39, and we shift the virtual address by that to get the index
	 * into the page table.
	 * 
	 * NOTES:
	 * - The 12 comes from the 4K granularity used for each page (2^12=4K)
	 * - The 9 is the amount of bits per page level. Each table translates 9 bits from the address
	 * 
	 * ADDITIONAL:
	 * There are 512 entries in a page table (2 ^ 9)
	 * 
	 * If we then shift the virtual address by that formula, we get the index in the low 9 bits
	 * However we only need the low 9 bits so we AND by 0xFF
	 */

	uint64_t phys_mem_map = 0xFFFF800000000000;

	uintptr_t pml4_i = (virt >> 39) & 0x1FF;
	uintptr_t pml3_i = (virt >> 30) & 0x1FF;
	uintptr_t pml2_i = (virt >> 21) & 0x1FF;
	uintptr_t pml1_i = (virt >> 12) & 0x1FF;

	uintptr_t *pml4 = pagemap->pml4;
	uintptr_t *pml3 = NULL;
	uintptr_t *pml2 = NULL;
	uintptr_t *pml1 = NULL;

	/* pml4 */
	if (!entry_present(pml4, pml4_i)) {
		// create a new PML3 and set it in pml4[pml4_index]
		uintptr_t p3 = (uintptr_t)pmm_alloc(1);
		memset(p3, 0, 0x1000);

		// set writable and user accessible flags
		pml4[pml4_i] = p3 | (1 << 0) | PAGE_WRITABLE | PAGE_USER;
		pml3 = p3;
	}
	else {
		// we already created PML3, retrieve it's address from PML4
		pml3 = (pml4[pml4_i] & 0x000FFFFFFFFFF000) /*+ phys_mem_map*/;
	}

	/* pml3 */
	if (!entry_present(pml3, pml3_i)) {
		// create a new PML2 and set it in pml3[pml3_index]
		uintptr_t p2 = (uintptr_t)pmm_alloc(1);
		memset(p2, 0, 0x1000);

		// set writable and user accessible flags
		pml3[pml3_i] = p2 | (1 << 0) | PAGE_WRITABLE | PAGE_USER;
		pml2 = p2;
	}
	else {
		// we already created PML2, retrieve it's address from PML3
		pml2 = (pml3[pml3_i] & 0x000FFFFFFFFFF000) /*+ phys_mem_map*/;
	}

	/* pml2 */
	if (!entry_present(pml2, pml2_i)) {
		// create a new PML2 and set it in pml3[pml3_index]
		uintptr_t p1 = (uintptr_t)pmm_alloc(1);
		memset(p1, 0, 0x1000);

		// set writable and user accessible flags
		pml2[pml2_i] = p1 | (1 << 0) | PAGE_WRITABLE | PAGE_USER;
		pml1 = p1;
	}
	else {
		// we already created PML1, retrieve its address from PML2
		pml1 = (pml2[pml2_i] & 0x000FFFFFFFFFF000) /*+ phys_mem_map*/;
	}

	/* map pml1 to the physical address */
	uintptr_t pml1_entry = 0;
	pml1_entry |= phys;
	pml1_entry |= (1 << 0);
	
	// set PAGE_WRITABLE and PAGE_USER flags if needed
	if ((flags & PAGE_WRITABLE))
		pml1_entry |= (1 << 1);
	if ((flags & PAGE_USER))
		pml1_entry |= (1 << 2);

	pml1[pml1_i] = pml1_entry;

	asm volatile("invlpg %0" : : "m"(virt) : "memory");
}