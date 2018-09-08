;----------------------------------------------------------------------------
; CONSTANT
;----------------------------------------------------------------------------

; PIC - Programmabile Interrupt Controller, i.e. 8259 chip
	port_8259a_icw  	equ 0x20	;I/O for master PIC
	port_8259a_mask		equ 0x21	;I/O for master PIC mask
	port_8259b_icw  	equ 0xA0	;I/O for slave PIC
	port_8259b_mask		equ 0xA1	;I/O for slace PIC mask

; Keyboard controller, i.e. 8042 chip
	KBC_CTL 	equ	060h	; 8042 control port
	KBC_STAT	equ	064h	; 8042 status port
;	Cmos_index	equ	070h	; CMOS address port
;	Cmos_data	equ	071h	; CMOS data port
;	Shut_down	equ	00fh	; CMOS index for shutdwn
	inpt_buf_full	equ	2	; Input buffer full
;	Shutdown_CMD	equ	0feh	; Shutdown CMD for KBC
	enable_bit20	equ	0dfh	; enable A20 command
	disable_bit20	equ	0ddh	; disable A20 command

; Speaker Control
	port_PIT_Control	equ 0x43
	port_PIT_Channel2	equ 0x42
	port_Speaker		equ 0x61
