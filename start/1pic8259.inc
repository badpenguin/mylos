;----------------------------------------------------------------------------
; REPROGRAM THE 8259 & RESTORE DEFAULT SETTINGS
;----------------------------------------------------------------------------

Stop_8259:
; First, reprogram the interrupt handler
; PIC-1 it seems mapped to INT 0x08 to 0x0F   and to INT 0x70-0x7F
	cli
	xor ax, ax
	mov fs, ax

; Reprogram Master PIC (0-7)
        Mov  al, 0x11     ; ICW1: Bit 0 - Requires ICW4
        Out  0x20, al      ; Need ICW4 because were running on 8086+ machines
	IO_DELAY 1
        Mov  al, 0x08
        Out  0x21, al
	IO_DELAY 1
        Mov  al, 4	; original - 10000000b	; try with 4
        out  0x21, al
	IO_DELAY 1
        Mov  al, 1
        Out  0x21, al       ; ICW4: 8086 Mode
	IO_DELAY 1

; Reprogram Slave PIC (8-15)
        Mov  al, 0x11      ; ICW1: Bit 0 - Requires ICW4
        Out  0xA0, al      ; Need ICW4 because were running on 8086+ machines
	IO_DELAY 1
        Mov  al, 0x70
        Out  0xA1, al
	IO_DELAY 1
        Mov  al, 2	; 7 - ; try with 2
        out  0xA1, al
	IO_DELAY 1
        Mov  al, 1
	Out  0xA1, al       ; ICW4: 8086 Mode
	IO_DELAY 1

; Second restore the interrupt status of the 8259A
        pushf                   	; save interrupt flag
        cli                     	; we REALLY don't want an int
                                	; to come through while we are
        push    ax              	; reprogramming the PIC masks
        mov     al, [fs:my_8259a_mask]
        out     port_8259a_mask, al     ; restore master PIC mask
        IO_DELAY 1              	; I/O delay
        mov     al, [fs:my_8259a_mask]
        out     port_8259b_mask, al	; restore slave PIC mask
        pop     ax
        popf      	              	; restore interrupt flag
; End 8259 release procedure
end_8259:
	ret
