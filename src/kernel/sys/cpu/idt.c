/**
 * idt.c -- Functions for setting up the Interrupt Descriptor Table (IDT)
 * 
 * Authors:
 * 		Charlotte (charpointer)
 * 		Harry (harrego)
 */
#include "idt.h"

struct idt_entry idt[256];

void idt_setgate(int entry, uint64_t handler, int typeattr) {
	/* Create the entry in the IDT */
	idt[entry].offset_low = (uint16_t)handler;	/* low bits */
	idt[entry].selector = 0x8;					/* KERNEL_CODE_SELECTOR */
	idt[entry].ist = 0x0;						/* zero for now */
	idt[entry].type_attr = typeattr;
	idt[entry].offset_mid = (handler >> 16);
	idt[entry].offset_high = (handler >> 32);
	idt[entry].zero = 0x0;
}

void idt_load() {
	/* Fill the IDT descriptor */
	struct idt_pointer ptr;
	ptr.addr = (uint64_t)idt;
	ptr.size = sizeof(idt) - 1;

	/* Load the IDT */
	asm volatile("lidtq %0" : : "m"(ptr));

	/* Store the interrupt flag */
	asm volatile("sti");
}