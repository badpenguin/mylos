[BITS 32]

; This define generic IRQ Handler and Exception Error and other
; 'traps' features

[GLOBAL _INT_Spurius__Fv]
[GLOBAL _KERNEL_Panic__Fv]

[GLOBAL _INT_00__Fv]
[GLOBAL _INT_01__Fv]
[GLOBAL _INT_02__Fv]
[GLOBAL _INT_03__Fv]
[GLOBAL _INT_04__Fv]
[GLOBAL _INT_05__Fv]
[GLOBAL _INT_06__Fv]
[GLOBAL _INT_07__Fv]
[GLOBAL _INT_08__Fv]
[GLOBAL _INT_09__Fv]
[GLOBAL _INT_0A__Fv]
[GLOBAL _INT_0B__Fv]
[GLOBAL _INT_0C__Fv]
[GLOBAL _INT_0D__Fv]
[GLOBAL _INT_0E__Fv]
[GLOBAL _INT_0F__Fv]
[GLOBAL _INT_10__Fv]

[GLOBAL _TRAPS_FOREVER]
[GLOBAL _OS_DUMP__Fv]



[SECTION .text]

[EXTERN  _CRT_PrintDWord__FUl]
[EXTERN  _CRT_Print__FPc]
[EXTERN  _CRT_NewLine__Fv]
[EXTERN  _SWITCH_BackToDOS__Fv]

_INT_Spurius__Fv:
	push eax
	mov eax, MESSAGESPURIUS
	push eax
	call _CRT_Print__FPc
 	pop eax
 	call forever
	iret

_INT_00__Fv:
	push eax
	mov eax, MESSAGE00
	push eax
	call _CRT_Print__FPc
 	pop eax
 	call forever
	iret

_INT_01__Fv:
	push eax
	mov eax, MESSAGE01
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_02__Fv:
	push eax
	mov eax, MESSAGE02
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_03__Fv:
	push eax
	mov eax, MESSAGE03
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_04__Fv:
	push eax
	mov eax, MESSAGE04
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_05__Fv:
	push eax
	mov eax, MESSAGE05
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_06__Fv:
	push eax
	mov eax, MESSAGE06
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_07__Fv:
	push eax
	mov eax, MESSAGE07
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_08__Fv:
	push eax
	mov eax, MESSAGE08
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_09__Fv:
	push eax
	mov eax, MESSAGE09
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_0A__Fv:
	push eax
	mov eax, MESSAGE0A
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_0B__Fv:
	push eax
	mov eax, MESSAGE0B
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_0C__Fv:
	push eax
	mov eax, MESSAGE0C
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_0D__Fv:
	push eax
	mov eax, MESSAGE0D
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_0E__Fv:
	push eax
	mov eax, MESSAGE0E
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_0F__Fv:
	push eax
	mov eax, MESSAGE0F
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret

_INT_10__Fv:
	push eax
	mov eax, MESSAGE10
	push eax
	call _CRT_Print__FPc
	pop eax
	call forever
	pop eax
	iret


forever:
_KERNEL_Panic__Fv:
	mov eax, PANIC_MESSAGE
	push eax
	call _CRT_NewLine__Fv
	call _CRT_Print__FPc
	pop eax
	call _OS_DUMP__Fv
	jmp  _SWITCH_BackToDOS__Fv
.here_here_here
	jmp .here_here_here


%imacro TRACE_DEBUG 1
	mov eax, %1
	push eax
	call _CRT_Print__FPc
	pop eax
	
	pop eax
	call _CRT_PrintDWord__FUl
%endmacro


_OS_DUMP__Fv:
	pushfd
	push esp
	push ebp
	push esi
	push edi
	push edx
	push ecx
	push ebx
	push eax

        ; Save segment registers
        xor eax, eax
        mov ax, ss
        push eax

        mov ax, gs
        push eax

        mov ax, fs
        push eax

        mov ax, es
        push eax

        mov ax, ds
        push eax

        mov ax, cs
        push eax

	; Begin Tracing

	call _CRT_NewLine__Fv
        TRACE_DEBUG trace_CS
        TRACE_DEBUG trace_DS
        TRACE_DEBUG trace_ES
        TRACE_DEBUG trace_FS
        call _CRT_NewLine__Fv

        TRACE_DEBUG trace_GS
        TRACE_DEBUG trace_SS
        call _CRT_NewLine__Fv

        TRACE_DEBUG trace_EAX
        TRACE_DEBUG trace_EBX
        TRACE_DEBUG trace_ECX
        TRACE_DEBUG trace_EDX
        call _CRT_NewLine__Fv

        TRACE_DEBUG trace_EDI
        TRACE_DEBUG trace_ESI
        TRACE_DEBUG trace_EBP
        TRACE_DEBUG trace_ESP
        call _CRT_NewLine__Fv

        TRACE_DEBUG trace_EFLAGS
        call _CRT_NewLine__Fv
        call _CRT_NewLine__Fv

        mov eax, trace_Stack
	push eax
	call _CRT_Print__FPc
	pop eax
        call _CRT_NewLine__Fv

        mov ecx, 32
	mov edx, 8
	mov ax, ss
	mov gs, ax
	mov ebp, esp
.trace_stack
	mov eax, [gs:ebp+edx]
	push edx
	push ebp
	push ecx
	push eax
	call _CRT_PrintDWord__FUl
	pop eax
	pop ecx
	pop ebp
	pop edx

	add edx, 4
	dec ecx
	mov eax, ecx
	cmp eax, 0
	jg .trace_stack

;	mov eax, [OS_DUMP_RETURN]
;	push eax

	ret




[SECTION .data]

OS_DUMP_RETURN:
	dd 0x00000000

PANIC_MESSAGE:
	db '!!! Kernel Panic !!!', 0, 0, '$$'

MESSAGESPURIUS:
	db 'You Caught a Spurius IRQ - Check the source code !', 0, 0, '$$'
MESSAGE00:
	db 'Caught Exception 0x00 - DIVISION BY ZERO', 0, 0, '$$'
MESSAGE01:
	db 'Caught Exception 0x01 - DEBUG EXCEPTION ', 0, 0, '$$'
MESSAGE02:
	db 'Caught Exception 0x02 - NMI INTERRUPT ', 0, 0, '$$'
MESSAGE03: 
	db 'Caught Exception 0x03 - BREAK POINT ', 0, 0, '$$'
MESSAGE04:
	db 'Caught Exception 0x04 - INTO Detected Overflow ', 0, 0, '$$'
MESSAGE05:
	db 'Caught Exception 0x05 - BOUND Range Exceeded ', 0, 0, '$$'
MESSAGE06:
	db 'Caught Exception 0x06 - INVALID OPCODE ', 0, 0, '$$'
MESSAGE07:
	db 'Caught Exception 0x07 - COPROCESSOR NOT AVAIABLE ', 0, 0, '$$'
MESSAGE08:
	db 'Caught Exception 0x08 - DOUBLE EXCEPTION ', 0, 0, '$$'
MESSAGE09:
	db 'Caught Exception 0x09 - COPROCESSOR SEGMENT OVERRUN ', 0, 0, '$$'
MESSAGE0A:
	db 'Caught Exception 0x0A - INVALID Task State Segment ', 0, 0, '$$'
MESSAGE0B:
	db 'Caught Exception 0x0B - SEGMENT NOT PRESENT ', 0, 0, '$$'
MESSAGE0C:
	db 'Caught Exception 0x0C - STAK FAULT ', 0, 0, '$$'
MESSAGE0D:
	db 'Caught Exception 0x0D - GENERAL PROTECTION ERROR ', 0, 0, '$$'
MESSAGE0E:
	db 'Caught Exception 0x0E - PAGE FAULT ', 0, 0, '$$'
MESSAGE0F:
	db 'Caught Exception 0x0F - INTEL RESERVED ', 0, 0, '$$'
MESSAGE10:
	db 'Caught Exception 0x10 - COPROCESSOR ERROR ', 0, 0, '$$'


trace_CS        db      '   CS: ', 0
trace_DS        db      '   DS: ', 0
trace_ES        db      '   ES: ', 0
trace_SS        db      '   SS: ', 0
trace_FS        db      '   FS: ', 0
trace_GS        db      '   GS: ', 0
trace_EAX       db      '  eAX: ', 0
trace_EBX       db      '  eBX: ', 0
trace_ECX       db      '  eCX: ', 0
trace_EDX       db      '  eDX: ', 0
trace_EDI       db      '  eDI: ', 0
trace_ESI       db      '  eSI: ', 0
trace_EBP       db      '  eBP: ', 0
trace_ESP       db      '  eSP: ', 0

trace_Stack     db      '  Stack Trace: ', 0
trace_EFLAGS    db      '  EFL: ', 0

