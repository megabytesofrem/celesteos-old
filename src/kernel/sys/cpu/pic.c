/**
 * pic.c -- Control the 8259 PIC (Programmable Interrupt Controller)
 * Authors:
 * 		Charlotte (charpointer)
 */

#include "pic.h"

#include "../ports.h"

/**
 * Reinitialize the PIC controllers, giving them specified offsets rather than
 * the default offsets 0x8 and 0x70.
 * 
 * @param offset1: the first offset
 * @param offset2: the second offset
 */
void pic_remap(int offset1, int offset2) {
	unsigned char a1, a2;

	/*  save masks */
	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);

	/* initialize both PICs */
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();

	/* master and slave PIC offset */
	outb(PIC1_DATA, offset1);
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();

	/* tell master PIC there is a slave PIC */
	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2); /* tell slave PIC its cascade identity (0000 0010) */
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC1_DATA, ICW4_8086);
	io_wait();

	/* restore saved masks */
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

/**
 * Send an EOI to one of two PIC controllers depending on the IRQ.
 * If the IRC is greater than 0x8, an EOI will be sent to the slave PIC controller
 */
void pic_sendeoi(unsigned char irq) {
	/* 0x20 is PIC EOI code */
	if (irq >= 8)
		outb(PIC2_COMMAND, 0x20);

	outb(PIC1_COMMAND, 0x20);
}

void pic_maskirq(unsigned char irq) {
	uint16_t port;
	uint8_t value;

	if (irq < 8) {
		port = PIC1_DATA;
	}
	else {
		port = PIC2_DATA;
		irq -= 8;
	}

	/* shift to mask bits */
	value = inb(port) | (1 << irq);
	outb(port, value);
}

void pic_clearmask(unsigned char irq) {
	uint16_t port;
	uint8_t value;

	if (irq < 8) {
		port = PIC1_DATA;
	}
	else {
		port = PIC2_DATA;
		irq -= 8;
	}

	/* unmask bits */
	value = inb(port) & ~(1 << irq);
	outb(port, value);
}

/* TODO: add ISR/IRR */