; Hardware Enable Gate A20

start_a20:


; #DEBUG
        DOS_MSG msg_a20start
	DOS_DOT

enablea20:

; ==================================== NEW
gateA20:
        call .empty
        jnz noA20_short
        out 0xed,ax     ; 1us delay
        mov al,0xd1
        out 0x64,al
        call .empty
        jnz noA20_short
        mov al,0xdf
        out 0x60,al
        call .empty

        mov cx,0x14    ; 25us delay
.ga1    out 0xed,ax
        loop .ga1

	jmp finished_a20

.empty  xor cx,cx
.l1     out 0xed,ax    ; 1us delay
        in al,0x64
        and al,2
        loopnz .l1
        ret

noA20_short:
	jmp noA20

; ==================================== OLD
        pushf
        push fs
        push gs
        cli
        xor ax,ax                       ; set A20 test segments 0 and 0ffffh
        mov fs,ax
        dec ax
        mov gs,ax
	call enablea20test              ; is A20 already enabled?
        jz near __enablea20done

	in al,92h                       ; PS/2 A20 enable
        or al,2
        jmp short $+2
        jmp short $+2
        jmp short $+2
        out 92h,al
        call enablea20test              ; is A20 enabled?
        jz short __enablea20done        ; if yes, done
        call enablea20kbwait            ; AT A20 enable
        jnz short __enablea20f0

        mov al,0d1h
        out 64h,al
        call enablea20kbwait
        jnz short __enablea20f0
        mov al,0dfh
        out 60h,al
        call enablea20kbwait

__enablea20f0:                          ; wait for A20 to enable
        mov cx,800h                     ; do 800h tries
__enablea20l0:
        call enablea20test              ; is A20 enabled?
        jz __enablea20done              ; if yes, done
        in al,40h                       ; get current tick counter
        jmp short $+2
        jmp short $+2
        jmp short $+2
        in al,40h
        mov ah,al
__enablea20l1:                          ; wait a single tick
        in al,40h
        jmp short $+2
        jmp short $+2
        jmp short $+2
        in al,40h
        cmp al,ah
        je __enablea20l1

        loop __enablea20l0              ; loop for another try
        mov ax,0x12
        int 0x10
	cli

; #DEBUG
noA20:
        DOS_MSG msg_noa20line
	jmp quit_to_dos

__enablea20done:
        pop gs
        pop fs
        popf
        jmp finished_a20

enablea20kbwait:                        ; wait for safe to write to 8042
        xor cx,cx
__enablea20kbwaitl0:
        jmp short $+2
        jmp short $+2
        jmp short $+2
        in al,64h                       ; read 8042 status
        test al,2                       ; buffer full?
        loopnz __enablea20kbwaitl0      ; if yes, loop
        ret

enablea20test:                          ; test for enabled A20
        mov al,[fs:0]                   ; get byte from 0:0
        mov ah,al                       ; preserve old byte
        not al                          ; modify byte
        xchg al,[gs:10h]                ; put modified byte to 0ffffh:10h
        cmp ah,[fs:0]                   ; set zero if byte at 0:0 not modified
        mov [gs:10h],al                 ; put back old byte at 0ffffh:10h
        ret                             ; return, zero if A20 enabled

;-----------------------------------------------------------------------------

finished_a20:

; #DEBUG
	DOS_DOT
        DOS_MSG msg_a20ok

	mov byte [my_wasA20set], 0x01

	ret
