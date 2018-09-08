; Initialize protected mode segment registers
; (Second Times)
; Stack pointer is placed at 2MB
	mov ax, selector_kernel_DS
	mov ds, ax
	mov es, ax
	mov ax, selector_RAW
	mov fs, ax
	mov gs, ax
;stack
	mov ss, ax
	mov esp, 0x200000

	; Clear NT Bit
	push dword 0x00000000
	popfd

	; Load TSS
	mov ax, selector_task0
	ltr ax

	; Load LDT with null descriptor
	; ?????

	PMODE_PRINT32 8, 20, welcome32_message, selector_kernel_DS, my_color

;--------------------------------------------------------------------
;                               M  A  I  N
;--------------------------------------------------------------------

; perform high copy - move kernel high
	mov ax, selector_kernel_DS
	mov ds, ax
        mov esi, KERNEL32_START
	mov ax, selector_kernel32_DS
	mov es, ax
	mov edi, 0

	mov ecx, 0xFFFF		; 64KB
	rep
	movsb


; check signature
        mov ax, selector_kernel32_CS
	mov fs, ax
        mov eax, [fs:0]
;        TRACE_DEBUG trace_EAX

        mov ax, selector_kernel32_CS
	mov fs, ax
        mov eax, [fs:0]

        cmp eax, 0x90909090
        jne bad_kernel_signature

	mov ax, selector_kernel32_DS
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call dword selector_kernel32_CS : 0
;--------------------------------------------------------------------
;                               E  X  I  T
;--------------------------------------------------------------------
; Return_From_ProtectMode - First jump into code16 segment
	jmp     dword selector_code16 : return_from_protect_mode

;--------------------------------------------------------------------
;                         Failed High Copy
;--------------------------------------------------------------------
bad_kernel_signature:
        PRINTK_NL
        PRINTK bad32_signature, selector_kernel_DS
        int 0x20

