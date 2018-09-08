[BITS 32]


[GLOBAL _KEYBOARD_Handler__Fv]
[GLOBAL _KEYBOARD_SetLeds__FUc]
[GLOBAL _KEYBOARD_Start__Fv]
[GLOBAL _KEYBOARD_Reset__Fv]

[EXTERN _CRT_Print__FPc]
[EXTERN _CRT_NewLine__Fv]
[EXTERN _KEYBOARD_PutRawCode__FUs]

%include "kernel/include/commons.inc"
%include "start/ports.h"

%define KEYBOARD_STATUSPORT		0x64
%define KEYBOARD_COMMANDPORT		0x64
%define KEYBOARD_DATAPORT		0x60
%define KEYBOARD_INPUTBUFFER_FULL	00000010b
%define KEYBOARD_OUTPUTBUFFER_FULL	00000001b

%define KBD_KRAW_UNKNOWN 0xFE
%define KBD_KRAW_PAUSE	 0xFF


[SECTION .text]

%macro MACRO_Keyboard_Wait   0
	xor eax, eax
%%wait00:
	in al, 0x64
	and al, 2
	jz %%wait99
	inc eax
	cmp eax, 0x100000
	jng %%wait00
%%wait99:
%endmacro


%macro MACRO_Keyboard_Enable   0
	MACRO_Keyboard_Wait
        mov al, 0xAE
        out 0x64, al
;NEW, works without too ...
;	MACRO_Keyboard_Wait
;/NEW
%endmacro

%macro MACRO_Keyboard_Disable   0
	MACRO_Keyboard_Wait
        mov al, 0xAD
        out 0x64, al
	MACRO_Keyboard_Wait
%endmacro

; ====================================================================
InputBufferEmpty:
		PUSH EAX
		PUSH ECX
		MOV ECX, 0x2FFFF
.IBE0
		JMP .IBE1
.IBE1
		JMP .IBE2
.IBE2
		IN AL,   KEYBOARD_STATUSPORT
		TEST AL, KEYBOARD_INPUTBUFFER_FULL
		LOOPNZ .IBE0
		POP ECX
		POP EAX
		RET

OutputBufferFull:
		PUSH EAX
		PUSH ECX
		MOV ECX, 0x2FFFF
.OBF0
		JMP .OBF1
.OBF1
		JMP .OBF2
.OBF2
		IN AL,  KEYBOARD_STATUSPORT
		TEST AL,KEYBOARD_OUTPUTBUFFER_FULL
		LOOPZ .OBF0
		POP ECX
		POP EAX
		RET

;__________________________________________________________________________________________________



; ====================================================================

_KEYBOARD_Handler__Fv:
	cli		; overplus ?
	pushfd
	pushad
	
; Disable the keyboard to disallow ints while we are reading this
	MACRO_Keyboard_Disable


; Read the scan code from the keyboard 
.loop_again
	in al, 0x60

	; if scancode begins with E0, E1 then it is an "extended key".
	; So, before to push this raw scan-code, we have to read another code
	; from the keyboard to form a 2 byte scan-code
	cmp al, 0xE0			; if scancode=0xE0 then save_scancode
	je .save_scancode		; for the next time
	cmp al, 0xE1			; if scancode=0xE1 then save_scancode else
	jne .process_scancode		; it is a normal key then process it
.save_scancode
	; save the extended scancode (E0 or E1) and try to get another code from keyb
	mov byte [KBD_ScanCode1], al
	jmp .next_scancode

.process_scancode
	; check if there is a previous scancode saved else check for an extended key
	; i.e. to check the press of the PAUSE keyboard
	mov ah, byte [KBD_ScanCode1]
	cmp ah, 0x00
	jz .simple_scancode

	; Check prev_scancode = E0
	cmp ah, 0xE0
	jz .E0

	; Check prev_scancode = E1
	cmp ah, 0xE1
	jz .E1

	; 0xF0 is our arbitrary temporary flag for the "pause" code sequence
	cmp ah, 0xF0
	jz .F0

	; otherwise it is Unknown
	jmp .unknown_sequence

.simple_scancode
	mov byte [KBD_ScanCode1], 0x00
	mov byte [KBD_ScanCode2], al
	jmp .valid_sequence

.E0
	; The keyboard maintains its own internal caps lock and
	; num lock statuses. In caps lock mode E0 AA precedes make
	; code and E0 2A follows break code. In num lock mode,
	; E0 2A precedes make code and E0 AA follows break code.
	; We do our own book-keeping, so we will just ignore these.
	; 
	; For my keyboard there is no caps lock mode, but there are
	; both Shift-L and Shift-R modes. The former mode generates
	; E0 2A / E0 AA pairs, the latter E0 B6 / E0 36 pairs.

	; if one of this sequence si found it is skipped
	cmp al, 0x2A
	jz .ignore_E0
	cmp al, 0xAA
	jz .ignore_E0
	cmp al, 0x36
	jz .ignore_E0
	cmp al, 0xB6
	jz .ignore_E0
	; at the end the code, if valid, is saved and processed
	mov byte [KBD_ScanCode2], al
	jmp .valid_sequence

.ignore_E0
	; skip the E0 sequence and read the next scan code, if any
	mov byte [KBD_ScanCode1], 0
	jmp .next_scancode


.E1
	; prev_scancode = E1
	cmp al, 0x1D			; try to see if a pause key was pressed
	je .pause_sequence
	cmp al, 0x9D
	je .pause_sequence
	jmp .unknown_sequence		; otherwise it is an unknown sequence
.pause_sequence
	; store temporary scancode for "pause" key sequence and read another code 
	; of the sequence
	mov byte [KBD_ScanCode1], 0xF0
	jmp .next_scancode


.F0
	; temporary sequence for the "pause" keys
	cmp al, 0x45			; again, we wait for E1
	je  .valid_pause_sequence
	cmp al, 0xE1			; again, we wait for 9D
	je  .next_scancode
	cmp al, 0x9D			; agasin, we wait for C5
	je  .next_scancode
	cmp al, 0xC5
	je  .ignore_pause_release
	jmp .invalid_pause_sequence
.ignore_pause_release
	mov byte [KBD_ScanCode1], 0x00
	mov byte [KBD_ScanCode2], 0x00
	jmp .next_scancode
.valid_pause_sequence
	mov byte [KBD_ScanCode1], KBD_KRAW_PAUSE	; our arbitrary raw code for the
	mov byte [KBD_ScanCode2], KBD_KRAW_PAUSE	; pause key
	jmp .valid_sequence

.invalid_pause_sequence
.unknown_sequence
	mov byte [KBD_ScanCode1], KBD_KRAW_UNKNOWN
	mov byte [KBD_ScanCode2], KBD_KRAW_UNKNOWN

; Output a debug message here
	mov eax, KBD_Msg01
	push eax
	call _CRT_Print__FPc
	pop  eax
	;jmp .next_scancode
	jmp .come_out

;==============================

; here we try to see if there are other code to be read from the keyboard
; else exit from the loob
.next_scancode
	in al, 0x64
	and al, 1
	jz .come_out
	jmp .loop_again

.come_out
	MACRO_Keyboard_Enable

	; EOI - End of Interrupt
	cli
	mov al, 0x20
	out 0x20, al

	popad
	popfd
	iret

;==============================

.valid_sequence
	mov ah, byte [KBD_ScanCode1]
	mov al, byte [KBD_ScanCode2]
	push ax
	call _KEYBOARD_PutRawCode__FUs
	pop ax
	mov byte [KBD_ScanCode1], 0
	mov byte [KBD_ScanCode2], 0
	jmp .next_scancode

;====================================

_KEYBOARD_Start__Fv:
	mov dx,port_8259a_mask	;dx = pic1 mask port
	in al,dx		;Get current PIC mask
	and al,0xfd		;Clear bit 1 (keyboard), bit 0 = timer
	pushad			;bit of a delay I guess
	popad
	out dx,al		;Set new PIC mask
	ret

_KEYBOARD_Reset__Fv:
	mov dx, 0x61
	in al,dx

	pushad			; delay
	popad

	mov bl, al		; save al
	and al,0x80
	out dx,al

	pushad			; delay
	popad

	mov al, bl		; save al
	out dx,al

	ret

;=============================================================
;
;Set keyboard LEDS from PMX
; b0	Scrollock
; b1	Numlock
; b2	Capslock
; b3-7	0
; A '1' bit turns the indicator ON. 

_KEYBOARD_SetLeds__FUc:
	pushfd
        pushad

        mov ebp, esp
        mov bl, byte [ss: ebp + _PUSH_ALL_AND_FLAG + 4 ]

;	/// TODO: Mask IRQ Here !!!

;/// OLD
; Wait for buffer empty
	MACRO_Keyboard_Wait

	; set/reset status indicators
	mov	al,   0xED	; command for turning on/off the LEDs
	out  	0x60, byte al	; send command to keyboard dataport

;	/// NEW
;	call OutputBufferFull

	MACRO_Keyboard_Wait

;	call OutputBufferFull
;	IN	al, 0x60

;.wait_l2
;	IN	al, 0x64	; read status register
;	TEST	al, 0x02	; check whether OUTPUT buffer is full
;	JNZ	.wait_l2	; input buffer full thus wait

;	call InputBufferEmpty

	mov	al, bl		; put in al the current
	and	al, 00000111b	; mask all but the 3 low order bits
        OUT     0x60, byte al   ; switch on LED 

; /// NEW
	call OutputBufferFull
	in al, 0x60

; /// TODO: UNMASK IRQ HERE
	popad
	popfd
	ret


[SECTION .data]

; ------------------------------------------- ScanCode's buffer

; Contain the temporary scancode readed from the keyboard

KBD_ScanCode1:		; to access extended key byte
	db 0
KBD_ScanCode2:		; to access key code as byte
	db 0

KBD_Msg01:
	db '[KBD-01] Unknown or illegal raw scancode sequence', 0, 0, '$'

