[BITS 32]

%include "start/gdt.h"
%include "start/video32.h"
%include "kernel/include/commons.inc"

CRT_ScreenWidth         equ 80
CRT_ScreenHeight        equ 25
CRT_ScreenSize          equ CRT_ScreenWidth * CRT_ScreenHeight * 2

CRT_Port1		equ 0x3D4
CRT_Port2		equ 0x3D5
CRT_CursorLow		equ 0x0F
CRT_CursorHigh		equ 0x0E


[GLOBAL  _CRT_Init__Fv]
[GLOBAL  _CRT_Clear__Fv]
[GLOBAL  _CRT_Print__FPc]
[GLOBAL  _CRT_SetColor__FUc]
[GLOBAL  _CRT_NewLine__Fv]
[GLOBAL  _CRT_UpdateHardwareCursor__Fv]
[GLOBAL  _CRT_GotoXY__FUsUs]

[EXTERN  _CRT_PrintDWord__FUl]


[SECTION .text]


; void CRT_INIT(void)
_CRT_Init__Fv:
	mov word [CRT_CursorX], 0
	mov word [CRT_CursorY], 0
	mov byte [CRT_Color], 0x1E
	call _CRT_UpdateHardwareCursor__Fv
	ret


;void CRT_CLEAR(void)
_CRT_Clear__Fv:
; Change: es, eax, ecx
	pushad
; Set up selector ES=Raw
	mov ax, selector_RAW
	mov es, ax

	mov ecx, CRT_ScreenSize
	mov al, ' '
	mov ah, byte [CRT_Color]
.p2_loop:
	mov word [es:ecx+0xb8000], ax
	dec cx
	dec cx
	jge .p2_loop
; Restore selector
	popad
	ret


; void CRT_PRINT( char *string );
_CRT_Print__FPc:
; DS = Kernel Data Selector
; ES = RAW memory selector to access 0xb0000
; Get PChar from the stack and save it
	pushad
;
	mov ebp, esp
	mov eax, dword [ss:ebp+_PUSH_ALL+4]
	mov esi,eax

	; DS:ESI is our source string
; Set up selector DS=kernel date ES=Raw
	mov ax, selector_RAW
	mov es, ax
; Calc current cursor offset
	xor eax, eax
	mov ax, word [CRT_CursorY]
	imul ax, 80*2
	xor ebx, ebx
	mov bx, word [CRT_CursorX]
	shl bx, 1 
	add eax, ebx
	add eax, 0xB8000
	mov edi, eax
	; ES:EDI = destination point now in Video Memory
; DS:ESI -> ES:EDI
	xor ecx, ecx    ; this is the char counter
.p1_loop:
	mov al, byte [ds:esi]
	mov ah, byte [CRT_Color]
	mov word [es:edi], ax
	inc esi         ; inc pointer
	inc edi         ; inc 2 byte destination
	inc edi
	inc cx          ; Inc counter
	cmp byte al, 0  ; last char ?
	jg .p1_loop     ; JNE
; Advance the cursor position
	dec cx
	add word [CRT_CursorX], cx
	; if X=>80 then X=X-80 Y=Y+1
	cmp word [CRT_CursorX], CRT_ScreenWidth-1
	jl .end_CRT_Print
	sub word [CRT_CursorX], CRT_ScreenWidth-1
	add word [CRT_CursorY], 1
.end_CRT_Print
	call CRT_ScrollCheck
	call _CRT_UpdateHardwareCursor__Fv
; Restore selector
	popad
	ret


; void CRT_SetColor( unsigned char color );
_CRT_SetColor__FUc:
	push eax
	mov al, [ss: esp + 4 + 4 ]
	mov byte [CRT_Color], al
	pop eax
	ret


_CRT_NewLine__Fv:
	mov word [CRT_CursorX], 0
	add word [CRT_CursorY], 1
	call CRT_ScrollCheck
	call _CRT_UpdateHardwareCursor__Fv
	ret


CRT_ScrollCheck:
	push ax
	mov ax, word [CRT_CursorY]
	cmp ax, CRT_ScreenHeight-1
	jl .dont_need_to_scroll
	call CRT_Scroll
.dont_need_to_scroll
	pop ax
	ret


CRT_Scroll:
	pushad

	; Decrement Cursor Pointer
	sub word [CRT_CursorY], 1
;	mov word [CRT_CursorY], CRT_ScreenHeight-1
;	mov word [CRT_CursorX], 0

	; Move everythings one line up DS:SI ES:DI
	cld	; move forward
	mov esi, 0xB8000 + ( CRT_ScreenWidth * 2 )
	mov edi, 0xB8000
	mov ecx, ( CRT_ScreenWidth * (CRT_ScreenHeight-1) )
	; selector
	mov ax, selector_RAW
	mov ds, ax
	mov es, ax
	; copy
	rep movsw

	; Clean the last line
;OLD
;	mov ax, selector_kernel32_DS
;	mov ds, ax
;	mov ah, [CRT_Color]
;	mov al, 32	; i.e. space ' '
;	mov ebx, 0xB8000 + ( CRT_ScreenWidth * (CRT_ScreenHeight-1) )
;	mov cx, CRT_ScreenWidth * 2
;NEW (WORKS)
	mov ax, selector_kernel32_DS
	mov ds, ax
	mov ah, [CRT_Color]
	mov al, 32	; i.e. space ' '
	mov ebx, 0xB8000 + ( CRT_ScreenWidth * (CRT_ScreenHeight-1) )
	mov ecx, CRT_ScreenWidth * 2
	rep stosw
;	jmp .scroll_exit
;/NEW

; CLEAN= if NEW WORKS !
;.clean_scroll_again
;	mov word [es:ebx], ax
;	inc ebx
;	inc ebx
;	dec cx
;	cmp cx, 0
;	jg  .clean_scroll_again
; /CLEAN

.scroll_exit
	popad
	ret



_CRT_UpdateHardwareCursor__Fv:
	pusha
        ; Compute the offset in bx
	mov ax, [CRT_CursorY]
	imul ax, 80
	add ax, [CRT_CursorX]
	mov bx, ax

	; move the hardware cursor
	mov al, byte CRT_CursorLow
	mov dx, CRT_Port1
	out dx, byte al

	mov dx, CRT_Port2
	mov al, bl
	out dx, al

	mov dx, CRT_Port1
	mov al, byte CRT_CursorHigh
	out dx, al

	mov dx, CRT_Port2
	mov al, bh
	out dx, al

	popa
	ret


_CRT_GotoXY__FUsUs:
	push eax
	mov eax, dword [ ss : esp + 4 + 4 ]
	mov word [CRT_CursorX], ax
	mov eax, dword [ ss : esp + 4 + 8 ]
	mov word [CRT_CursorY], ax
	pop eax
	ret


[SECTION .data]

CRT_CursorX     dw 0
CRT_CursorY     dw 0
CRT_Color       db 0

