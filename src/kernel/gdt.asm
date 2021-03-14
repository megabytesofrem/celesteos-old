%define KERNEL_HIGH_VMA 0xFFFFFFFF80000000

section .data
; define a static GDT in assembly
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