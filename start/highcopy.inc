high_copy:
        DOS_MSG msg_int15copy
	DOS_DOT

	xor eax, eax
	mov eax, cs
	shl eax, 4
	add eax, KERNEL32_START
	mov byte [copy_s1], al
	shr eax, 8
	mov byte [copy_s2], al
	mov byte [copy_s3], ah

        cli
        mov     cx, $8000               ; Move 64KB
        push    cs
        pop     es
        mov     ax, copy_gdt            ; ES:SI point to copy_gdt
        mov     si, ax
        mov     ax, 0x8700		; Call the copy function
        int     0x15
	cli
        jc      highcopy_panic          ; this, if INT15 fails

        DOS_MSG msg_highcopyends
	DOS_DOT

;        mov     [high_copy_debug], al
;        mov     bp, high_copy_debug
;        call print_hex
	DOS_DOT

;        mov     ax, 0xFFFF
;        mov     [high_copy_debug], ax
;        mov     bp, high_copy_debug
;        call print_hex
	DOS_DOT

        ret

highcopy_panic:
        DOS_MSG msg_noint15
        jmp quit_to_dos

;----------------------------------------------------------------
