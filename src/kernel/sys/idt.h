#ifndef IDT_H
#define IDT_H

#include <common.h>

typedef struct {
	uint16_t offset_low;	/* offset bits 0..15 */
	uint16_t selector;		/* code segment selector in GDT/IDT */
	uint8_t  ist;			/* bits 0..2 hold Interrupt Stack Table offset, rest are zero */
	uint8_t  type_attr;		/* type and attribute */
	uint16_t offset_mid;	/* bits 16..31 */
	uint32_t offset_high;	/* bits 32..63 */
	uint32_t zero;			/* reserved */
} idt_descriptor_t;

typedef struct __attribute__((packed)) {
	uint16_t size;
	uint64_t addr;
} idt_ptr_t;

void idt_setgate(int entry, uint64_t handler, int typeattr);
void idt_load();

#endif