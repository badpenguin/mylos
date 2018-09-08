
// Keyboard internal handling
extern void KEYBOARD_Handler(void);
extern void KEYBOARD_Init(void);
extern void KEYBOARD_Start(void);
extern void KEYBOARD_Reset(void);

extern void KEYBOARD_SetLeds(BYTE CurrentLedsStatus);
extern void KEYBOARD_PutRawCode(WORD rawcode);


// Keyboard layout and code_page
extern BYTE KBD_Layout_ALT[0x80];
extern BYTE KBD_Layout_CTRL[0x80];
extern BYTE KBD_Layout_SHIFT[0x80];
extern BYTE KBD_Layout_NORMAL[0x80];


// Keyboard FIFO Handling
extern WORD KBD_ExtractKeyCode(void);

