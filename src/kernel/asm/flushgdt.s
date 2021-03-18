%define KERNEL_HIGH_VMA 0xFFFFFFFF80000000
bits 64

; make this global so we can use it in C
global gdt_flush

gdt_flush:
	lgdt [rdi]			; load GDT
	
	push rbp			; save the base pointer for later
	mov rbp, rsp	
	push qword 0x10		; push data segment
	push rbp			; push stack pointer
	pushfq				; push flags
	push qword 0x08		; push the code segment
	push qword .flush	; push the return address
	iretq
.flush:
	pop rbp
	mov rax, 0x10		; load all other segments with the data segment

	mov ds, rax
	mov es, rax
	mov fs, rax
	mov gs, rax
	mov ss, rax
	ret