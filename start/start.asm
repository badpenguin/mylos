; START.ASM

; This program provide the following services:
;   * Starting from MS-DOS (with non Himem.sys or Emm386.exe loaded)
;   * Run in full protect mode
;   # Return to MS-DOS          

; To compile you need:
;       NASM - The Netwide Assembler 
;	with nasm 0.95 you can compile from plain DOS otherwise 
;	you will get out of memory messages


;------------------------------------------------------------------------
; This is a .com file template, with only one section, it starts at 0x100
; of the segment and use default 16 bit program.

[ORG 0x0100]

[BITS 16]

	jmp ENTRY16

;----------------------------------------------------------------------------
; Constants
;----------------------------------------------------------------------------
	%include "start.h"
	%include "gdt.h"
	%include "idt.h"
	%include "ports.h"
;       %include "video32.h"

;----------------------------------------------------------------------------
; Data & Variables
;----------------------------------------------------------------------------
	%include "debug16.dat"          ; Contain debug message
	%include "breeze.dat"           ; Contain a new type of font
	%include "gdt.dat"
	%include "idt.dat"
;        %include "highcopy.dat"
	%include "tss32.dat"

;----------------------------------------------------------------------------
; Macros 
;----------------------------------------------------------------------------
	%include "debug16.mac"
	%include "gdt.mac"
	%include "ports.mac"
        %include "speaker.mac"
        %include "debug32.mac"

;----------------------------------------------------------------------------
;------------------------     E N T R Y   P O I N T     ---------------------
;----------------------------------------------------------------------------

ENTRY16:

; Safety - disable ints
	cli

; Set up segment registers
; set FS=GS=0x0000 segment to access the first 64KB
	xor eax, eax
	mov fs, ax
	mov gs, ax
; align CS=DS=ES=SS. If you run from DOS they are already aligned 
; but with Bill around nobody can be sure ...
	push cs
	pop ax
	mov ds, ax
	mov es, ax
; set up the stack
	mov ss, ax
	mov sp, stack_base

; Set text-mode 3, just used to clear the screen
	mov     ax,3
	int     10h

; #DEBUG - Display the start message
	DOS_MSG msg_logo
	DOS_MSG msg_pstart
	DOS_DOT

; Backup stack pointer and other things, that we will need later
; to return back to DOS, into first 64 KB
	mov     word [fs : backup_SS], ss
	mov     word [fs : backup_SP], sp
	DOS_DOT

; Save the Return_From_Pmode to be retrieved from kernel.cpp and other stuff
	mov dword [FS:backup_ROff], return_from_protect_mode
	mov word  [FS:backup_RSel], selector_code16
	DOS_DOT

;----------------------------------------------------------------------------
; Startup Sequence
;----------------------------------------------------------------------------
	call start_video        ; into "0video.inc"
	call start_gdt          ; into "0gdt.inc"
	call start_idt          ; into "0idt.inc"
	; call high_copy
	call start_a20          ; into "0gatea20.inc"
        call save_8259          ; into "0pic8259.inc"   
        call start_8259         ; into "0pic8259.inc"
	jmp start_pmode

;-------------------------------------------------------------------------
; 16 BIT - PROCEDURES
;-------------------------------------------------------------------------
	%include "0video.inc"
	%include "0gdt.inc"
	%include "0idt.inc"
	%include "0gateA20.inc"
        %include "0pic8259.inc"
;       %include "debug.inc"
	%include "0pmode.inc"
;	%include "highcopy.inc"

	%include "1gateA20.inc"
        %include "1pic8259.inc"
	%include "irq16.inc"
	%include "1back.inc"

;-------------------------------------------------------------------------
; STACK     
;-------------------------------------------------------------------------

; Padding, for speed.
	times ($$-$) &15 nop

; Set up 4KB of stack
stack:                  
	times 4096 db 0
stack_base:
	dw 0,0

; Padding, for speed.
	times ($$-$) &15 nop


;------------------------------------ Stack for PM
stack_level0:
	times stack0_limit db 0
stack_base0:
	dw 0,0
	times ($$-$) &15 nop

stack_level1:
	times stack1_limit db 0
stack_base1:
	dw 0,0
	times ($$-$) &15 nop

stack_level2:
	times stack2_limit db 0
stack_base2:
	dw 0,0
	times ($$-$) &15 nop


;-------------------------------------------------------------------------
;-------------------------------------------------------------------------
;                 32 BIT PROTECT MODE SIDE OF THE PROGRAM
;-------------------------------------------------------------------------
;-------------------------------------------------------------------------

; Padding, for speed.
	times ($$-$) &15 nop

[BITS 32]

; CONSTANTS
	%include "video32.h"

; MACROS


;--------------------------------------------------------------------
code32_start:
	; This is our main code
	%include "start32.inc"
        %include "video32.inc"
	%include "debug32.dat"

; ***  procedure HTOA  ***
; convert hexadecimal code to ASCII code
; input : 
; si - address to put ASCII code, 
; cx - hexadecimal code size
; eax - hexadecimal code

; Padding, for speed.
start_signature:
        db '<KERNEL32_START>'
	times ($$-$) &15 db 0

KERNEL32_START:
