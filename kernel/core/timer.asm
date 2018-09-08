[BITS 32]

; This is the IRQ 0 - INT 31 - The TIMER Handler
; This check if the semaphore is red otherwise decrease the 
; current process's CpuTime. When this value decrease to 0
; it invoke the skeduler. If skeduler does not skedule any process
; (i.e. all the process are in wait state or blocked ) the processor
; is halted until a new interrupt

[GLOBAL _TIMER_Scheduler__Fv]
[GLOBAL _SK_InTask]
[GLOBAL _SK_OutTask]
[GLOBAL _TIMER_Start__Fv]

[SECTION .text]

%include "start/gdt.h"
%include "start/ports.h"

[EXTERN _SK_CpuTime]
[EXTERN _SK_IRQCounter]
[EXTERN _SK_SwitchTime]
[EXTERN _SK_SchedTime]
[EXTERN _SK_IDLETime]
[EXTERN _SCHEDULER_Do__Fv]
[EXTERN _CRT_Print__FPc]
[EXTERN _CRT_PrintDWord__FUl]
[EXTERN _CRT_PrintWord__FUs]
[EXTERN  _CRT_NewLine__Fv]
[EXTERN _SK_IdleTask]

_TIMER_Scheduler__Fv:
	cli
	pushad

; Check the Timer Semaphore 
; I don't know if is necessary because ints are automagically disabled
; if somebody know if i can remove it let me know
        mov bl,0xff
        xchg byte [_SK_Semaphore], bl   ; Set lock
        cmp bl,0xff                     ; Already locked?
        jne .cont1                      ; yes, someone already switching tasks
	add dword [_SK_IRQCounter], 1	; Count IRQ not served
	add dword [_SK_CpuTime],    1	; However count the time as spent by the current process
	jmp .exit1
; Increase counter
.cont1
	add dword [_SK_CpuTime],    1

; Display a rotating Clock (I love that things !)
; Inspired from OS2000 
	mov  ax, selector_RAW 		; (24)
	mov  fs, ax
	mov ah, 0x80	;0x78
	mov al, byte [clock_counter]
	inc al
	mov byte [clock_counter], al
	and al, 0x07
	or  al, 0xF0
	mov  word [fs: 0x000B8000 + 144 ], ax

;clear the PIC
	mov al,   0x20
	out 0x20, al

; Decrease the amout of time the current process can use CPU
	sub dword [_SK_SwitchTime], 1
        cmp dword [_SK_SwitchTime], 0
        jg .exit2

; The current process CPU Time has expired, go to see if there is somebody else
.retrySwitch
	call _SCHEDULER_Do__Fv

; If Scheduled process is 0 (NOONE) go IDLE !
	mov ax, word [_SK_InTask]
	cmp ax, 0
	je  .idleTime

; Check if the task was the same, if yes DON'T JUMP
; otherwise we get GPE (0x0D) because of task reentrancy issue
	mov ax, word [_SK_InTask]
	cmp ax, word [_SK_OutTask]
	je  .exit3

; Check if the task is the same before we have gone IDLE, if yes DON'T JUMP
; otherwise we get GPE (0x0D) because of task reentrancy issue
	mov ax, word [_SK_InTask]
	cmp ax, word [_SK_IdleTask]
	je .exit4

; Release the semaphore and do that Task Switch
	mov byte [_SK_Semaphore], 0
	jmp dword far [_SK_InTask-4]
	mov dword [_SK_SchedTime], 0		; UNUSED ???
.exit3
	add dword [_SK_SchedTime], 1
.exit2
	mov byte [_SK_Semaphore], 0
.exit1
	popad
	iretd
; Clean the IDLETask value/flag
.exit4
	mov word [_SK_IdleTask], 0
	jmp .exit3

;-----------------------------------

; Stop the Processor until an INT <> TIMER occurs
.idleTime
	pushad
	pushfd

	mov eax, IDLEMESSAGE
	push eax
	call _CRT_Print__FPc
        call _CRT_NewLine__Fv
	pop eax

	call _TIMER_Stop__Fv	; Mask Timer IRQ
	popfd
	popad


;Stop the machine until an interrupt occurs
	pushad
	pushfd
	sti
	hlt
	popfd
	popad

	pushad
	pushfd
	mov eax, RESUME_IDLEMESSAGE
	push eax
	call _CRT_Print__FPc
        call _CRT_NewLine__Fv
	pop eax

	add dword [ _SK_IDLETime ], 1

	call _TIMER_Start__Fv
	popfd
	popad
	jmp .retrySwitch

;====================================
; UnMask the TIMER IRQ
_TIMER_Start__Fv:
	mov dx,port_8259a_mask	;dx = pic1 mask port
	in al,dx		;Get current PIC mask
	and al,0xfe		;Clear bit 0 (timer)
	pushad			;bit of a delay I guess
	popad
	out dx,al		;Set new PIC mask
	ret

;====================================
; Mask the TIMER IRQ
_TIMER_Stop__Fv:
	mov dx,port_8259a_mask	;dx = pic1 mask port
	in al,dx		;Get current PIC mask
	or al,0x01		;Set bit 0 (timer)
	pushad			;bit of a delay I guess
	popad
	out dx,al		;Set new PIC mask
	ret


[SECTION .data]

InTask_start:
	dd 0x00000000
_SK_InTask:
	dw 0x0000

OutTask_start:
	dd 0x00000000
_SK_OutTask:
	dw 0x0000

; Rotating clock counter
clock_counter:
	db 0

IDLEMESSAGE:
 	db 'Kernel is going IDLE.', 0, 0, '$$'

RESUME_IDLEMESSAGE:
 	db 'Kernel is RESUMING from IDLE.', 0, 0, '$$'

_SK_Semaphore:
	db 0

