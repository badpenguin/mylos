;----------------------------------------------------------------------------
; SWITCH TO PROTECT MODE
;----------------------------------------------------------------------------

start_pmode:

; #DEBUG
        DOS_MSG msg_switch

; Prepare for switch.
	cli
        lgdt [gdt_pointer]	; (done before !)
	lidt [pm_idt]		; (done before !)
        mov eax,cr0		; Get the current CR0
        or al,1			; Set the PM mode bit
        mov cr0,eax             ; Switch to pmode.
	jmp short flush_pm
flush_pm:

; Initialize protected mode segment registers
	mov ax, selector_kernel_DS
        mov ds, ax
        mov es, ax
        mov ax, selector_RAW
	mov fs, ax
	mov gs, ax
	mov ss, ax
        mov esp, 0x200000		; This will be enough !

; Insert here the long jump - we surely know that code32_start is at 1MB+code32_start
        jmp dword selector_kernel_CS : code32_start
