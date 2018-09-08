[BITS 32]

; This MUST BE the LAST file linked by 'ld'
; in this way, knowing the address of 'KERNEL_CODE_ENDS_HERE' 
; we know the kernel's SIZE !

[GLOBAL _KERNEL_CODE_ENDS_HERE]


[SECTION .data]

_KERNEL_CODE_ENDS_HERE:
	dd 0, 0
