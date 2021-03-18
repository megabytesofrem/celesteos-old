#ifndef _MM_VMM_H
#define _MM_VMM_H

#include <common.h>

#define HIGHERHALF_OFFSET 0xFFFFFFFF80000000

struct vmm_pagemap {
	uintptr_t *pml4;
};

enum vmm_page_flags {
	PAGE_WRITABLE = (1 << 1),
	PAGE_USER = (1 << 2),
	PAGE_EXEC = (1 << 3)
};

struct vmm_pagemap create_pagemap();

uintptr_t phys_to_virt(uintptr_t phys);
void vmm_map_page(struct vmm_pagemap *pagemap, uintptr_t virt, uintptr_t phys, enum vmm_page_flags flags);
void vmm_unmap_page(struct vmm_pagemap *pagemap, uintptr_t virt);

#endif