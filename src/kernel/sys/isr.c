/**
 * isr.c -- Functions for setting up and handling Interrupt Service Requests (ISRs)
 * 
 * Authors:
 * 		Charlotte (charpointer)
 * 		Harry (harrego)
 */

#include <lib/libk.h>
#include <lib/string.h>
#include <kernel/sys/e9.h>
#include <driver/vga.h>

#include "isr.h"
#include "idt.h"
#include "ports.h"

const char* exceptions[32] = {
	"Division by Zero",				/*0*/
	"Debug",						/*1*/
	"Non Maskable Interrupt",		/*2*/
	"Breakpoint",					/*3*/
	"Into Detected Overflow",		/*4*/
	"Out of Bounds",				/*5*/
	"Invalid Opcode",				/*6*/
	"No Coprocessor",				/*7*/

	"Double Fault",					/*8*/
	"Coprocessor Segment Overrun",	/*9*/
	"Bad TSS",						/*10*/
	"Segment Not Present",			/*11*/
	"Stack Fault",					/*12*/
	"General Protection Fault",		/*13*/
	"Page Fault",					/*14*/
	"Unknown Interrupt",			/*15*/

	"Coprocessor Fault"				/*16*/
	"Alignment Check",				/*17*/
	"Machine Check",				/*18*/
	"Unknown Instruction",			/*19*/
	"Assert Error",					/*20*/

	"Reserved",						/*21*/
	"Reserved",						/*22*/
	"Reserved",						/*23*/
	"Reserved",						/*24*/
	"Reserved",						/*25*/
	"Reserved",						/*26*/
	"Reserved",						/*27*/
	"Reserved",						/*28*/
	"Reserved",						/*29*/
	"Reserved",						/*30*/
	"Reserved",						/*31*/
};

/* Defined in assembly */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// extern void irq0();
// extern void irq1();
// extern void irq2();
// extern void irq3();
// extern void irq4();
// extern void irq5();
// extern void irq6();
// extern void irq7();
// extern void irq8();
// extern void irq9();
// extern void irq10();
// extern void irq11();
// extern void irq12();
// extern void irq13();
// extern void irq14();
// extern void irq15();


void isr_install() {
	/* Install the ISRs */
	idt_setgate(0,  isr0,  0x8f);
	idt_setgate(1,  isr1,  0x8f);
	idt_setgate(2,  isr2,  0x8f);
	idt_setgate(3,  isr3,  0x8f);
	idt_setgate(4,  isr4,  0x8f);
	idt_setgate(5,  isr5,  0x8f);
	idt_setgate(6,  isr6,  0x8f);
	idt_setgate(7,  isr7,  0x8f);
	idt_setgate(8,  isr8,  0x8f);
	idt_setgate(9,  isr9,  0x8f);
	idt_setgate(10, isr10, 0x8f);
	idt_setgate(11, isr11, 0x8f);
	idt_setgate(12, isr12, 0x8f);
	idt_setgate(13, isr13, 0x8f);
	idt_setgate(14, isr14, 0x8f);
	idt_setgate(15, isr15, 0x8f);
	idt_setgate(16, isr16, 0x8f);
	idt_setgate(17, isr17, 0x8f);
	idt_setgate(18, isr18, 0x8f);
	idt_setgate(19, isr19, 0x8f);
	idt_setgate(20, isr20, 0x8f);
	idt_setgate(21, isr21, 0x8f);
	idt_setgate(22, isr22, 0x8f);
	idt_setgate(23, isr23, 0x8f);
	idt_setgate(24, isr24, 0x8f);
	idt_setgate(25, isr25, 0x8f);
	idt_setgate(26, isr26, 0x8f);
	idt_setgate(27, isr27, 0x8f);
	idt_setgate(28, isr28, 0x8f);
	idt_setgate(29, isr29, 0x8f);
	idt_setgate(30, isr30, 0x8f);
	idt_setgate(31, isr31, 0x8f);

	idt_load();
}

void isr_handler(registers_t *regs) {
	char *msg = exceptions[regs->int_no];

	vga_setcolor(vga_entry_color(VGA_COLOR_LRED, VGA_COLOR_BLACK));
	kprintf("[isr.c] exception %s (%d), err code: %d\n", msg, regs->int_no, regs->err_code);
	kprintf("[isr.c] RIP=%d\n", regs->rip);

	/* write to port e9 */
#ifdef LOG_TO_E9
	e9_write("exception ");
	e9_write(msg);
	e9_write(", err code: ");
	e9_write(itoa(regs->err_code, 10));
	e9_write(", RIP=");
	e9_write(itoa(regs->rip, 10));
	e9_write("\n");
#endif

	vga_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
}