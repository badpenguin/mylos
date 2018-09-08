; Message for IRQ 0 ------------------------------------------------------


; Handle procedure for IRQ 0 ---------------------------------------------

Handle_IRQ_Generic:
	BEEP
        PMODE_PRINT32 23, 1, irq_generic_message, selector_kernel_DS, my_color2
        jmp     dword selector_code16 : return_from_protect_mode
	BEEP
	iretd


Handle_IRQ20:
        PRINTK_COLOR my_color3
        PRINTK_GOTOXY 0, 23
        PRINTK irq20_message, selector_kernel_DS
        jmp     dword selector_code16 : return_from_protect_mode
	iretd

;------------------------------------------------------------------------

irq_generic_message:
	db 32,32,2,32,1fh
	db 'You caught a Generic IRQ !'
	db 0, 0, '$$'

irq20_message:
        db 'System Call 0x20 - Return Directly to DOS !!', 0, 0, '$$'
