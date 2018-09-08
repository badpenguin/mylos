start_gdt:

; #DEBUG
        DOS_MSG msg_setgdt

; Fill in our Gdt
	SET_GDT_ENTRY gdt_kernel_CS, cs, 0
	DOS_DOT
	SET_GDT_ENTRY gdt_kernel_DS, cs, 0
	DOS_DOT
	SET_GDT_ENTRY gdt_kernel_RAW, 0, 0
	DOS_DOT
	SET_GDT_ENTRY gdt_code16, cs, 0
	DOS_DOT
	SET_GDT_ENTRY gdt_data16, cs, 0
	DOS_DOT
	SET_GDT_ENTRY gdt_kernel_XS, 0, 0
	DOS_DOT
        ;------------------------------------
	SET_GDT_ENTRY gdt_kernel_Task0, cs, MyTSS
	DOS_DOT
	SET_GDT_ENTRY gdt_kernel_stack0, cs, stack_base0
	DOS_DOT
	SET_GDT_ENTRY gdt_kernel_stack1, cs, stack_base1
	DOS_DOT
	SET_GDT_ENTRY gdt_kernel_stack2, cs, stack_base2
	DOS_DOT
;-------------------------------------------------------
	; Gdt_Terminal already setted
	SET_GDT_ENTRY gdt_irq32,	cs,  IRQ00
	DOS_DOT
;---------------------------------------------------------
;        SET_GDT_ENTRY gdt_kernel32_CS, cs, KERNEL32_START
;        DOS_DOT
;        SET_GDT_ENTRY gdt_kernel32_DS, cs, KERNEL32_START
;        DOS_DOT


; Calc gdt's address.
	push cs
        xor eax, eax
        pop ax
        mov edx, gdt_null
        shl eax, 4
        add eax, edx
        mov [gdt_base], eax
	DOS_DOT

	ret

;----------------------------------------------------------------------
; PROCEDURES
;----------------------------------------------------------------------

Calc_PM_Address:
	; Calculate 32-bit protected mode address.
	; Used for building descriptor tables.
	; Input:   ES:SI = Real mode address
	; Output:  DX:AX = 32-bit linear address
	; Register(s) modified:  AX, DX
        mov     ax,es           ; point to control block
        xor     dh,dh           ; clear upper register
        mov     dl,ah           ; build high byte of 32-bit addr
        shr     dl,4            ; use only high nibble from (AX)
        shl     ax,4            ; strip high nibble from segment
        add     ax,si           ; add GDT offset for low word
        adc     dx,0            ; adj high byte if CY from low
        ret                     ; back to calling program
