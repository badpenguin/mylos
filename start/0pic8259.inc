;----------------------------------------------------------------------------
; REPROGRAM THE 8259 & ENABLE THE A20 LINE
;----------------------------------------------------------------------------

save_8259:

; #DEBUG
        DOS_MSG msg_8259save
; Get the status of the 8259 controller
	cli
        push    ax
        in      al, port_8259a_mask
        mov     byte [fs:my_8259a_mask], al
        IO_DELAY 1
        in      al, port_8259b_mask
        mov     byte [fs:my_8259b_mask], al
        pop     ax

        DOS_DOT
	ret


;-------------------------------------------------------------------------

start_8259:	

; #DEBUG
        DOS_MSG msg_8259start
        DOS_DOT

; We're gonna remap the PIC so IRQ0 is say 32. That means Low IRQs are 32-39
; We're Also mapping the slave Pic to 40 So thats 40-47
; Right at the end of the Interrupt Table out of the way of usefull
; BIOS/CPU interrupts

	cli
; Reprogram Master PIC (0-7)
        Mov  al, 10001b   ; ICW1: Bit 0 - Requires ICW4
        Out  20h, al      ; Need ICW4 because were running on 8086+ machines
        Mov  al, 32	  ; Start at IRQ 32
        Out  21h, al
        Mov  al, 10000000b ; ICW3 ori = 100b
        out  21h, al
        Mov  al, 1	;Finish 8259 initialization
        Out  21h, al       ; ICW4: 8086 Mode

; Reprogram Slave PIC (8-15)
        Mov  al, 10001b    ; ICW1: Bit 0 - Requires ICW4
        Out  0A0h, al      ; Need ICW4 because were running on 8086+ machines
        Mov  al, 40	   ; Start at IRQ 40
        Out  0A1h, al
        Mov  al, 7	; ICW3 ori = 10b
        out  0A1h, al
        Mov  al, 1	;Finish 8259 initialization
        Out  0A1h, al       ; ICW4: 8086 Mode

        DOS_DOT

; NEW 
	mov al,11111011b  	;Mask all but cascade
	out port_8259a_mask, al ;MASK - MASTER (0= Ints ON)
	push eax
	pop eax
	mov al,11111111b	;Mask all
	out port_8259b_mask, al ;MASK - SLAVE

;Clear PIC
	mov al,0x20		;clear the PIC
	out port_8259a_icw, al
	out port_8259b_icw, al

; /NEW

	mov byte [fs:my_was8259set], 0x01
        DOS_DOT

	ret


; When the PICUs are initialized, all the hardware interrupts are MASKED.
; Each driver that uses a hardware interrupt(s) is responsible
; for unmasking that particular IRQ.
;
;F0	IRQ 0	8254 Timer
;F1	IRQ 1 	Keyboard (8042)
;F2	IRQ 2	Cascade from PIC2 (handled internally)
;F3	IRQ 3 	COMM 2 Serial port
;F4	IRQ 4	COMM 1 Serial port
;F5	IRQ 5	Line Printer 2
;F6	IRQ 6	Floppy disk controller
;F7	IRQ 7	Line Printer 1
;
;F8	IRQ 8	CMOS Clock
;F9	IRQ 9	?
;FA	IRQ 10	?
;FB	IRQ 11	?
;FC	IRQ 12	?
;FD	IRQ 13	Math coprocessor
;FE	IRQ 14	Hard disk controller
;FF	IRQ 15	?

