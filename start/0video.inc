;----------------------------------------------------------------------------
; VIDEO INITIALIZATION
;----------------------------------------------------------------------------

start_video:

; #DEBUG
	DOS_MSG msg_vstart
	DOS_DOT

; Set the new font
	push cs			; es:=cs
	pop es
        mov ax,0x1100
        mov bp, font_breeze
        xor dx, dx
        xor bl, bl
        mov cx, 256
        mov bh, 16
        int 0x10

	DOS_DOT

; Set big cursor
        mov ax, 0x0100
        mov ch, 0
        mov cl, 0x0f
        int 0x10

	DOS_DOT

;----------------------------------------------------------------------------
; Detect if we are running at least on a 386
;----------------------------------------------------------------------------
; The best way to test for a 80386 is to test the processor's flag register.
; Since the 80386, flag bits 12-14 are used for the I/O Privilege Level (IOPL)
; and the Nested Task (NT) flag, so the only thing to do is to test if these 
; bits are modifyable (the 8086, 8088, 80186 don't use these flags and set 
; them implicitely to zero, the 80286 uses them, but they can only be modified 
; in Protected Mode. DOS can't run in Protected Mode, so if the program runs 
; on a 80286, it is in Real Mode, and there the flags can't be modified).

check_processor:
	pushf				; save flags
	xor	ah,ah			; clear high byte
	push	ax			; push AX onto the stack
	popf				; pop this value into the flag register
	pushf				; push flags onto the stack
	pop	ax			; ...and get flags into AX
	and	ah,0f0h			; try to set the high nibble
	cmp	ah,0f0h			; the high nibble is never 0f0h on a
	je	no386			; 80386!
	mov	ah,70h			; now try to set NT and IOPL
	push	ax
	popf
	pushf
	pop	ax
	and	ah,70h			; if they couldn't be modified, there
	jz	no386			; is no 80386 installed
	popf				; restore the flags
	jmp	ok386			; ...and return
no386:					; if there isn't a 80386, put a msg and exit
; #DEBUG
	DOS_MSG msg_no386
	jmp quit_to_dos
ok386:					; Continue the job

	DOS_DOT

;----------------------------------------------------------------------------
; Detect if we are running Windows 95 or any other V86 mode
;----------------------------------------------------------------------------

; Detect pmode existence.
	mov eax, cr0
        and al,1
        jz no_v86mode

; This is another way to detect it, maybe safer on 286
; smsw ax			
; test al,1
; jz no_v86mode

; Display warning message
	DOS_MSG msg_v86mode
	jmp quit_to_dos

no_v86mode:		
	; Continue the program
	DOS_DOT
	ret
