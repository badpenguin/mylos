
;---------------------------------------------------
; Set up IDT & PMode IRQ Handler
;----------------------------------------------------
start_idt:
	DOS_MSG msg_setidt
	DOS_DOT

	mov word [IRQ00], Handle_IRQ_Generic
	mov word [IRQ01], Handle_IRQ_Generic
	mov word [IRQ02], Handle_IRQ_Generic
        mov word [IRQ03], Handle_IRQ_Generic
        mov word [IRQ04], Handle_IRQ_Generic
        mov word [IRQ05], Handle_IRQ_Generic
        mov word [IRQ06], Handle_IRQ_Generic
        mov word [IRQ07], Handle_IRQ_Generic
        mov word [IRQ08], Handle_IRQ_Generic
        mov word [IRQ09], Handle_IRQ_Generic
        mov word [IRQ0A], Handle_IRQ_Generic
        mov word [IRQ0B], Handle_IRQ_Generic
        mov word [IRQ0C], Handle_IRQ_Generic
        mov word [IRQ0D], Handle_IRQ_Generic
        mov word [IRQ0E], Handle_IRQ_Generic
        mov word [IRQ0F], Handle_IRQ_Generic
        mov word [IRQ10], Handle_IRQ_Generic
        mov word [IRQ11], Handle_IRQ_Generic
        mov word [IRQ12], Handle_IRQ_Generic
        mov word [IRQ13], Handle_IRQ_Generic
        mov word [IRQ14], Handle_IRQ_Generic
        mov word [IRQ15], Handle_IRQ_Generic
        mov word [IRQ16], Handle_IRQ_Generic
        mov word [IRQ17], Handle_IRQ_Generic
        mov word [IRQ18], Handle_IRQ_Generic
        mov word [IRQ19], Handle_IRQ_Generic
        mov word [IRQ1A], Handle_IRQ_Generic
        mov word [IRQ1B], Handle_IRQ_Generic
        mov word [IRQ1C], Handle_IRQ_Generic
        mov word [IRQ1D], Handle_IRQ_Generic
        mov word [IRQ1E], Handle_IRQ_Generic
        mov word [IRQ1F], Handle_IRQ_Generic
        mov word [IRQ20], Handle_IRQ_Generic
        mov word [IRQ21], Handle_IRQ_Generic

; Calc idt_base
	push cs
        xor eax,eax
        pop ax
        mov edx, IRQ00
        shl eax,4
        add eax,edx
        mov [IDT_base], eax

	DOS_DOT

end_idt:
	ret

;-------------------------------------------------------------------------
