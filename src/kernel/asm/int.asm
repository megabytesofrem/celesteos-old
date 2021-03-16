global isr0			; Divide by Zero Exception
global isr1			; Debug Exception
global isr2			; Non Maskable Interrupt
global isr3			; Breakpoint
global isr4			; Overflow
global isr5			; Bound Range Exceeded
global isr6			; Invalid Opcode
global isr7			; No Coprocessor
global isr8			; Double Fault
global isr9			; Coprocessor Segment Overrun
global isr10		; Invalid TSS
global isr11		; Segment Not Present
global isr12		; Stack Fault
global isr13		; General Protection Fault
global isr14		; Page Fault
global isr15		; Reserved
global isr16		; Coprocessor Fault
global isr17		; Alignment Check
global isr18		; Machine Check
global isr19		; Reserved
global isr20		; Reserved
global isr21		; Reserved
global isr22		; Reserved
global isr23		; Reserved
global isr24		; Reserved
global isr25		; Reserved
global isr26		; Reserved
global isr27		; Reserved
global isr28		; Reserved
global isr29		; Reserved
global isr30		; Reserved
global isr31		; Reserved

; IRQs
global irq0			; PIC Timer Interrupt
global irq1			; Keyboard Interrupt (PS/2)
global irq2			; Cascade (used internally by the two PICs, never raised)
global irq3			; COM2 (if enabled)
global irq4			; COM1 (if enabled)
global irq5			; LPT2 (if enabled)
global irq6			; Floppy Disk
global irq7			; LPT1 / Unreliable "spurious" interrupt (usually)
global irq8			; CMON real-time clock (if enabled)
global irq9			; Free for peripherals / legacy SCSI / NIC
global irq10		; Free for peripherals / SCSI / NIC
global irq11		; Free for peripherals / SCSI / NIC
global irq12		; PS/2 mouse
global irq13		; FPU / Coprocessor / Inter-processor
global irq14		; Primary ATA hard disk
global irq15		; Secondary ATA Hard Disk

extern irq_handler
extern isr_handler

%macro pushaq 0
	push rax
	push rcx
	push rdx
	push rbx
	push rbp
	push rsi
	push rdi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro ; pushaq

%macro popaq 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rsi
	pop rbp
	pop rbx
	pop rdx
	pop rcx
	pop rax
%endmacro ; popaq

; ISRs

isr_common:
	pushaq			; push all registers
	cld				; clear direction flag
	xor rax, rax	; zero out rax
	mov rdi, rsp	
	call isr_handler
	popaq

	add rsp, 16
	iretq			; return from interrupt

irq_common:
	pushaq			; push all registers
	cld				; clear direction flag
	xor rax, rax	; zero out rax
	mov rdi, rsp	
	call irq_handler
	popaq

	add rsp, 16
	iretq			; return from interrupt

isr0:
	; 0: Divide By Zero
	push byte 0
	push byte 0
	jmp isr_common

isr1:
	; 1: Debug Exception
	push byte 0
	push byte 1
	jmp isr_common

isr2:
	; 2: Non Maskable Interrupt
	push byte 0
	push byte 2
	jmp isr_common

isr3:
	; 3: Breakpoint
	push byte 0
	push byte 3
	jmp isr_common

isr4:
	; 4: INTO Exception
	push byte 0
	push byte 4
	jmp isr_common

isr5:
	; 5: Out of Bounds
	push byte 0
	push byte 5
	jmp isr_common

isr6:
	; 6: Invalid Opcode
	push byte 0
	push byte 6
	jmp isr_common

isr7:
	; 7: Coprocessor Not Available
	push byte 0
	push byte 7
	jmp isr_common

isr8:
	; 8: Double Fault
	push byte 0
	push byte 8
	jmp isr_common

isr9:
	; 9: Coprocessor Segment Overrun
	push byte 0
	push byte 9
	jmp isr_common

isr10:
	; 10: Bad TSS
	push byte 0
	push byte 10
	jmp isr_common

isr11:
	; 11: Segment Not Present
	push byte 0
	push byte 11
	jmp isr_common

isr12:
	; 12: Stack Fault
	push byte 0
	push byte 12
	jmp isr_common

isr13:
	; 13: General Protection Fault
	push byte 0
	push byte 13
	jmp isr_common

isr14:
	; 14: Page Fault
	push byte 0
	push byte 14
	jmp isr_common

isr15:
	; 15: Reserved
	push byte 0
	push byte 15
	jmp isr_common

isr16:
	; 16: Floating Point Exception
	push byte 0
	push byte 16
	jmp isr_common

isr17:
	; 17: Alignment Check Exception
	push byte 0
	push byte 17
	jmp isr_common

isr18:
	; 18: Machine Check Exception
	push byte 0
	push byte 18
	jmp isr_common

isr19:
	; 19: Reserved
	push byte 0
	push byte 19
	jmp isr_common

isr20:
	; 20: Reserved
	push byte 0
	push byte 20
	jmp isr_common

isr21:
	; 21: Reserved
	push byte 0
	push byte 21
	jmp isr_common

isr22:
	; 22: Reserved
	push byte 0
	push byte 22
	jmp isr_common

isr23:
	; 23: Reserved
	push byte 0
	push byte 23
	jmp isr_common

isr24:
	; 24: Reserved
	push byte 0
	push byte 24
	jmp isr_common

isr25:
	; 25: Reserved
	push byte 0
	push byte 25
	jmp isr_common

isr26:
	; 26: Reserved
	push byte 0
	push byte 26
	jmp isr_common

isr27:
	; 27: Reserved
	push byte 0
	push byte 27
	jmp isr_common

isr28:
	; 28: Reserved
	push byte 0
	push byte 28
	jmp isr_common

isr29:
	; 29: Reserved
	push byte 0
	push byte 29
	jmp isr_common

isr30:
	; 30: Reserved
	push byte 0
	push byte 30
	jmp isr_common

isr31:
	; 31: Reserved
	push byte 0
	push byte 31
	jmp isr_common

; IRQs
irq0:
	push byte 0
	push byte 32
	jmp irq_common

irq1:
	push byte 1
	push byte 33
	jmp irq_common

irq2:
	push byte 2
	push byte 34
	jmp irq_common

irq3:
	push byte 3
	push byte 35
	jmp irq_common

irq4:
	push byte 4
	push byte 36
	jmp irq_common

irq5:
	push byte 5
	push byte 37
	jmp irq_common

irq6:
	push byte 6
	push byte 38
	jmp irq_common

irq7:
	push byte 7
	push byte 39
	jmp irq_common

irq8:
	push byte 8
	push byte 40
	jmp irq_common

irq9:
	push byte 9
	push byte 41
	jmp irq_common

irq10:
	push byte 10
	push byte 42
	jmp irq_common

irq11:
	push byte 11
	push byte 43
	jmp irq_common

irq12:
	push byte 12
	push byte 44
	jmp irq_common

irq13:
	push byte 13
	push byte 45
	jmp irq_common

irq14:
	push byte 14
	push byte 46
	jmp irq_common

irq15:
	push byte 15
	push byte 47
	jmp irq_common