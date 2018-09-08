Stop_A20:
	; Disable A20 from CPU address BUS.
	; Input:   None
	; Output:  CY=ERROR
	; Register(s) modified:  None
	cli
        push    ax
        mov     ah, disable_bit20        ; gate address bit 20 on
        Call    Gate_A20
        or      al,al                   ; was command accepted?
        jz      A20_Shut                ; go if yes
        stc                             ; set error flag
A20_Shut:
	pop     ax
	ret                             ; exit

;---------------------------------------------------------------
Enable_gate20:
        push    ax
        mov     ah,enable_bit20         ; gate address bit 20 on
        Call    Gate_A20
        or      al,al                   ; command accepted?
        jz      A20_OK                  ; go if yes
        stc                             ; set error flag
A20_OK: pop     ax
        ret                             ; exit

;---------------------------------------------------------------
; Gate_A20:  This routine controls a signal which gates address
;            line 20 (A20).  The gate A20 signal is an output of
;            of the 8042 slave processor (keyboard controller).
;            A20 should be gated on before entering protected
;            mode, to allow addressing of the entire 16M address
;            space of the 80286, or 4G address space of the
;            80386 & 80486.  It should be gated off after
;            entering real mode -- from protected mode.
;---------------------------------------------------------------
; Input:   AH = DD ==> A20 gated off (A20 always 0)
;          AH = DF ==> A20 gated on  (CPU controls A20)
; Output:  AL = 0  ==> Operation successful
;          AL = 2  ==> Operation failed, 8042 can't accept cmd
; Register(s) modified:  AX
;---------------------------------------------------------------
Gate_A20:
        pushf                   ; save interrupt status
        cli                     ; disable ints while using 8042
        Call    Empty_8042      ; insure 8042 input buffer empty
        jnz     A20_Fail        ; ret: 8042 unable to accept cmd
        IO_DELAY 1              ; I/O Delay
        mov     al,0D1h         ; 8042 cmd to write output port
        out     KBC_STAT,al     ; output cmd to 8042
        Call    Empty_8042      ; wait for 8042 to accept cmd
        jnz     A20_Fail        ; ret: 8042 unable to accept cmd
        mov     al,ah           ; 8042 port data
        out     KBC_CTL,al      ; output port data to 8042
        Call    Empty_8042      ; wait for 8042 to port data
        push    cx              ; save it
        mov     cx,14h          ;
@DLY:   IO_DELAY 1		; Wait for KBC to execute the
        loop    @DLY            ;  command.  (about 25uS)
        pop     cx              ; restore it

A20_Fail:
        popf                    ; restore flags
        ret


;---------------------------------------------------------------
; EMPTY_8042:  This routine waits for the 8042 buffer to empty.
;---------------------------------------------------------------
; Input:   None
; Output:  AL = 0, 8042 input buffer empty:     ZF
;          AL = 2, Time out; 8042 buffer full:  NZ
; Register(s) modified:  AX
;---------------------------------------------------------------
Empty_8042:
        push    cx              ; save CX
        xor     cx,cx           ; CX=0:  timeout value
Try_KBC:
        IO_DELAY 1
        in      al,KBC_STAT     ; read 8042 status port
        and     al,inpt_buf_full; input buffer full flag (D1)
        loopnz  Try_KBC         ; loop until input buffer empty
                                ;   or timeout
        pop     cx              ; restore CX
        ret
