%define KERNEL_HIGH_VMA 0xFFFFFFFF80000000

; Multiboot header
MB_ALIGN	equ 1 << 0						; align loaded modules on page boundaries
MB_MEMINFO	equ 1 << 1						; provide memory map info

; set MB_VIDEOMODE equ 1 << 2 later when we want GRUB to setup a framebuffer for us

FLAGS	equ MB_ALIGN | MB_MEMINFO			; multiboot flags
MAGIC	equ 0x1BADB002						; magic number
CHECKSUM equ -(MAGIC + FLAGS)				; checksum, prove we are multiboot

section .multiboot
align 4										; multiboot section is aligned by 4 bytes
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bss
align 16
stack:
	; allocate 16384 bytes, the stack grows downwards on x86
	resb 8192
	.top:

; macro to setup pagetables later
%macro gen_pd_2mb 3
	%assign i %1
	%rep 2
		dq (i | 0x83)
		%assign i i+0x200000
	%endrep
	%rep %3
		dq 0
	%endrep
%endmacro

section .data

section .text
global _start
_start:
	; The bootloader has loaded us into 32-bit protected mode on an x86 machine.
	; Interrupts are disabled. Paging is disabled. The processor state is as
	; defined in the multiboot standard. The kernel has full control of the CPU.

	; move the stack pointer to the top of our newly created stack
	mov edi, ebx
	mov esp, stack.top - KERNEL_HIGH_VMA

	bits 32

	; load page tables
	mov eax, p4_table - KERNEL_HIGH_VMA
	mov cr3, eax	; set control register 3 to the level 4 page table

	; enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; long mode bit
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	; load the new GDT
	lgdt [gdt.ptr_low - KERNEL_HIGH_VMA]

	; load new code selector (long jump)
	jmp (0x8):(start64 - KERNEL_HIGH_VMA)
	bits 64

start64:
	bits 64
	mov ax, 0x10
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	; jump to the higher half kernel
	mov rax, higher_half
	jmp rax

higher_half:
	mov rsp, stack.top
	lgdt [gdt.ptr]

	; call out to kmain
	extern kmain
	call kmain
	cli
.hang: hlt
	jmp .hang
;.end

section .data
;
; Define a static page table in Assembly just for now
; This should map 1 GB total
;
align 4096
p4_table:
	; map lower half of memory
	dq p3_table_low + 0x3 - KERNEL_HIGH_VMA
	times 510 dq 0 ; padding
	
	; map higher half of memory
	dq p3_table_high + 0x3 - KERNEL_HIGH_VMA
p3_table_low:
	; map lower half of memory
	dq p2_table + 0x3 - KERNEL_HIGH_VMA
	times 511 dq 0 ; padding
p3_table_high:
	times 510 dq 0 ; padding

	; map higher half of memory
	dq p2_table + 0x3 - KERNEL_HIGH_VMA
	dq 0
p2_table:
	; identity map 1 GB
	;gen_pd_2mb 0, 10, 502
	gen_pd_2mb 0, 512, 0
;
; Define a static GDT in Assembly
; We need this (i think) so we can get into higher half mode, this will be replaced in C.
;
gdt:
	.null: equ $ - gdt
		dw 0xFFFF				; limit (low)
		dw 0					; base (low)
		db 0					; base (mid)
		db 0					; access
		db 0					; granularity
		db 0					; base (high)
	
	.kernel_code: equ $ - gdt
		dw 0
		dw 0
		db 0
		db 10011010b			; access (exec/read)
		db 10101111b			; granularity
		db 0

	.kernel_data: equ $ - gdt
		dw 0
		dw 0
		db 0
		db 10010010b			; access (read/write)
		db 0
		db 0

	.user_code: equ $ - gdt
		dw 0
		dw 0
		db 0
		db 11111010b			; access (exec/read)
		db 10101111b			; granularity
		db 0

	.user_data: equ $ - gdt
		dw 0
		dw 0
		db 0
		db 10010010b			; access (read/write)
		db 0
		db 0

	.ptr:
		dw $ - gdt - 1			; limit
		dq gdt					; base

	.ptr_low:					; 32 bit GDT pointer
		dw $ - gdt - 1			; limit
		dq gdt - KERNEL_HIGH_VMA; base