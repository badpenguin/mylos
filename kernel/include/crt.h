/*
; Color in VGA mode 0x03

; Foreground color masks bit
	fore_black	equ 000b
	fore_blue	equ 001b
	fore_green	equ 010b
	fore_cyan	equ 011b
	fore_red	equ 100b
	fore_magenta	equ 101b
	fore_brown	equ 110b	; "yellow" if bright bit enabled
	fore_gray	equ 111b	; "white" if bright bit enabled

; Background color masks bit
	back_black	equ 0000000b
	back_blue	equ 0010000b
	back_green	equ 0100000b
	back_cyan	equ 0110000b
	back_red	equ 1000000b
	back_magenta	equ 1010000b
	back_brown	equ 1100000b	; yellow if bright bit enabled
	back_gray	equ 1110000b	; white if bright bit enabled

; Special attribure
	fore_bright	equ 00001000b
	back_blink	equ 10000000b

; My Color
	my_color	equ 10011110b	; Yellow blink on blue
	my_color2	equ 00000100b	; Red on black
	my_color3	equ 00001010b	; Light Green on black
*/


#define	CRT_ColorScheme00	0x0A	// Light Green on black (VT100)
#define	CRT_ColorScheme01	0x1F	// Yellow on Blue
#define	CRT_ColorScheme02	0x1E	// White On Blue
#define CRT_ColorScheme03	0x74	// Black On White (Reverse)


// Exported from the CRT Driver
extern void CRT_Init(void);
extern void CRT_Clear(void);
extern void CRT_Print(char *string);
extern void CRT_SetColor(BYTE color);
extern void CRT_GotoXY( WORD X, WORD Y);
extern void CRT_NewLine(void);
extern void CRT_UpdateHardwareCursor(void);


// Exported from the CRT Utility
extern void CRT_PrintWord( WORD w );
extern void CRT_PrintChar( char c );
extern void CRT_PrintDWord( DWORD dw );

