; Allign start address 
        times ($$-$) &15 nop

return_from_protect_mode:

; BACK TO REAL MODE
	cli
	mov	eax,cr0		; get CR0 into EAX
	and	al, 0xFE	; clear Protected Mode bit
	mov	cr0,eax		; after this we are back in Real Mode!

	jmp 	short flush_ip2
flush_ip2:

; Restore Segment Registers
	mov ax, 0
	mov fs,ax
	mov gs,ax
	mov ax, [fs:backup_SS]
	mov ds,ax
	mov es,ax
	mov ss,ax
	xor eax, eax
	mov ax, [backup_SP]
	mov esp, eax		; RSTACK_SIZE or "ax"

; setup registers
	mov eax,0
	mov ebx,eax
	mov ecx,eax
	mov edx,eax
	mov esi,eax
	mov edi,eax
	mov ebp,eax
; Restore Gate Settings (Disable Gate A20)
	call Stop_A20
; Restore DOS PIC Assignment
        call Stop_8259
; load DOS IDT register parameters
	cli
	lidt [dos_idt]
; Exit to DOS
quit_to_dos:
	sti
	mov ax, 0x4C01
	int 0x21

far_loop:
	BEEP
	jmp far_loop
