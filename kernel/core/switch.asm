[BITS 32]

; This file implements context/switch routines ...

[GLOBAL _SWITCH_KernelTask__FUsUl]
[GLOBAL _SWITCH_KernelTask2__FUsUl]
[GLOBAL _SWITCH_BackToDOS__Fv]
[GLOBAL _SWITCH_Reboot__Fv]


[SECTION .text]

%include "start/start.h"
%include "start/gdt.h"

[EXTERN  _CRT_PrintDWord__FUl]
[EXTERN  _CRT_Print__FPc]
[EXTERN  _CRT_NewLine__Fv]

_SWITCH_KernelTask__FUsUl:
	push eax
	mov  eax, [ ss : esp + 8]
	mov word  [KTS_Sele], ax
	mov  eax, [ ss : esp + 12]
	mov dword [KTS_Addr], eax
	pop eax
	; call dword far [KernelTaskSwitch]
	jmp dword far [KernelTaskSwitch]
	ret


_SWITCH_KernelTask2__FUsUl:
	push eax
	mov  eax, [ ss : esp + 8]
	mov word  [KTS_Sele], ax
	mov  eax, [ ss : esp + 12]
	mov dword [KTS_Addr], eax
	pop eax
	call dword far [KernelTaskSwitch]
	;jmp dword far [KernelTaskSwitch]
	ret


_SWITCH_BackToDOS__Fv:
	call _CRT_NewLine__Fv

	mov eax, Message_GivingUp
	push eax
	call _CRT_Print__FPc

	cli
	mov ax, selector_RAW			; selector_kernel_RAW
	mov fs, ax

	mov eax,  [ fs:0x1B0 ]
	mov dword [KBS_Addr], eax
	mov ax,   [ fs:0x1B4 ]
	mov word  [KBS_Sele], ax

	cli
;TEMP // IS IT NECESSARY TO RE-ENABLE THE KEYBOARD DRIVER !
        xor eax, eax
.wait00:
        in al, 0x64
        and al, 2
        jz .wait99
        inc eax
        cmp eax, 0x100000
        jng .wait00
.wait99
        mov al, 0xAE
        out 0x64, al

        cli
        mov al, 0x20
        out 0x20, al
;/TEMP
	jmp dword far [ KernelBackSwitch ]
	ret


_SWITCH_Reboot__Fv:
	mov eax, REBOOT_MESSAGE
	push eax
	call _CRT_Print__FPc
	lidt [rebootIDT]
	int 0
	hlt			;paranoid


[SECTION .data]

KernelTaskSwitch:
	KTS_Addr:
		dd 0
	KTS_Sele:
		dw 0

Message_GivingUp:
	db 'Kernel is giving up ...', 0, 0, '$$'

KernelBackSwitch:
	KBS_Addr:
		dd 0
	KBS_Sele:
		dw 0

REBOOT_MESSAGE:
	db 'Rebooting.....', '$$', 0,0
rebootIDT:
	dw 0
	dd 0

