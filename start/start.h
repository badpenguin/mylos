; Standard Delay --------------------------------------------------------
	STANDARD_DELAY	equ 0x100
	STANDARD_PAUSE	equ 0x300

; Absolute Backup Address -----------------------------------------------

; The Programmers PC Sourcebooks tells us that
; IRQ 68h - 6Fh are "not used"  ( 1A0h - 1BFh )
; IRQ 78h - 7Fh are "not used"  ( 1E0h - 1FFh )
; IRQ F1h - FFh are "not used"  ( 3C4h - 3FFh )

; I decide to use the address between IRQ 80 & E0
; 'cause they don't seems to be used by MS-DOS
; All the absolute address are referred to segment 0x0000
	backup_SS	equ	0x1A0
	backup_SP	equ	0x1A2

; 8259 Original Information
	my_8259a_mask	equ	0x1A4	; PIC-1 master original mask
	my_8259b_mask	equ	0x1A5	; PIC-2 slave original mask
	my_was8259set	equ	0x1A6	; Flag to know if 8259 mask was changed
; A20 Line information
	my_wasA20set    equ	0x1A7

; Return from PMODE pointer
	backup_ROff	equ	0x1B0
	backup_RSel	equ	0x1B4
