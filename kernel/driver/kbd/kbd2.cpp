#define DEBUG_KEYBOARD_DRIVER


#include "kernel.h"
#include "crt.h"
#include "kbd.h"
#include "kbd_api.h"
#include "switch.h"

#define	KBD_Buffer_MaxSize	256
WORD 	KBD_Buffer[KBD_Buffer_MaxSize];
WORD	KBD_Buffer_Head, KBD_Buffer_Tail, KBD_Buffer_Size;
WORD	KBD_StatusW;		// Maintain the status of the main keys
WORD 	KBD_StatusB;		// Maintain the status of char behaviors
WORD	KBD_StatusL;		// Maintain the status of leds behaviors


// Constant to Maintain the status of the key-pressed (WORD)
#define KBD_Status1_Scroll	1
#define KBD_Status1_Caps	2
#define KBD_Status1_NumLock  	4
#define KBD_Status1_LShift	8
#define KBD_Status1_RShift	16
#define KBD_Status1_Shifts	8+16
#define KBD_Status1_LCtrl	32
#define KBD_Status1_RCtrl	64
#define KBD_Status1_Ctrls	32+64
#define KBD_Status1_LAlt	128
#define KBD_Status1_RAlt	256
#define KBD_Status1_Alts	(128+256)

// Raw Mode key code
#define VKS_Normal	0
#define VKS_Shift	1
#define VKS_Ctrl	2
#define VKS_Alt		4


#ifdef DEBUG_KEYBOARD_DRIVER

  void DEBUG_STATUSW(void) {
	CRT_Print("StatusW: ");
	CRT_PrintWord( KBD_StatusW );
  }

  void DEBUG_STATUSB(void) {
	CRT_Print("StatusB: ");
	CRT_PrintWord( (WORD)KBD_StatusB );
  }

#endif 

//-----------------------------------------------------------------

void KEYBOARD_Init(void){
	// Keyboard buffer init
	int  i=0;
	KBD_Buffer_Head = 0;
	KBD_Buffer_Tail = 0;
	KBD_Buffer_Size = 0;
	for  (i=0; i<KBD_Buffer_MaxSize; i++) {
		KBD_Buffer[i]=0;
	}
	// Keyboard engine
	KBD_StatusW  = 0;
	KBD_StatusB  = 0;
	KBD_StatusL  = 0;
}

//-----------------------------------------------------------------

WORD QueryLedsStatus(WORD KStatus2W ) {
	WORD result = 0;
	if ( (KStatus2W & KBD_Status1_Caps) > 0 ) {
		result =  result + 4;
	}
	if ( (KStatus2W & KBD_Status1_NumLock) > 0 ) {
		result =  result + 2;
	}
	if ( (KStatus2W & KBD_Status1_Scroll) > 0 ) {
		result =  result + 1;
	}
	return result;
}


// extern __inline__
WORD QueryKeyboardStatus(WORD KStatus2W ) {
	if ( (KStatus2W & KBD_Status1_Alts) > 0 ) {
		return VKS_Alt;
	}
	else if ( (KStatus2W & KBD_Status1_Ctrls) > 0 ) {
		return VKS_Ctrl;
	}
	else if ( (KStatus2W & KBD_Status1_Shifts) > 0 ) {
		return VKS_Shift;
	}
	else {
		return VKS_Normal;
	}	
}

//-----------------------------------------------------------------

char ConvertExtendedKey( charEx ExCode ) {
  char asciicode= '\0';
  BYTE KStatus	= ExCode >> 8;
  BYTE KCode	= ExCode & 0xFF;

  asciicode = 0;
  switch (KStatus) {
    case VKS_Normal:
	asciicode = KBD_Layout_NORMAL[ KCode ];
	break;
    case VKS_Shift:
	asciicode = KBD_Layout_SHIFT[ KCode ];
	break;
    case VKS_Ctrl:
	asciicode = KBD_Layout_CTRL[ KCode ];
	break;
    case VKS_Alt:
	asciicode = KBD_Layout_ALT[ KCode ];
	break;
  } 
  return ( (char)asciicode );
}

// ************************************************************************

void Translate_RawCode( BYTE KCode ) {
	unsigned short	asciicode;
	KBD_StatusB    = QueryKeyboardStatus( KBD_StatusW );

	switch (KBD_StatusB) {
		case VKS_Normal:
			asciicode = KBD_Layout_NORMAL[ (BYTE)(KCode) ];

			CRT_Print("Normal Code = ");
			CRT_PrintWord( asciicode );
			CRT_Print(" Char = ");
			CRT_PrintChar( (char)asciicode );
			// CRT_NewLine();

			break;

		case VKS_Shift:
			asciicode = KBD_Layout_SHIFT[ KCode ];

			CRT_Print("SHIFT Code = ");
			CRT_PrintWord( asciicode );
			CRT_Print(" Char = ");
			CRT_PrintChar( (char)asciicode );
			// CRT_NewLine();

			break;

		case VKS_Ctrl:
			asciicode = KBD_Layout_CTRL[ KCode ];

			CRT_Print("CTRL  Code = ");
			CRT_PrintWord( asciicode );
			CRT_Print(" Char = ");
			CRT_PrintChar( (char)asciicode );
			// CRT_NewLine();

			break;

		case VKS_Alt:
			asciicode = KBD_Layout_ALT[ KCode ];

			CRT_Print(" ALT  Code = ");
			CRT_PrintWord( asciicode );
			CRT_Print(" Char = ");
			CRT_PrintChar( (char)asciicode );
			// CRT_NewLine();

			break;
	} 
}

//-----------------------------------------------------------------


KRESULT KBD_IsEmpty(void) {
	if ( KBD_Buffer_Tail == KBD_Buffer_Head ) {
		return 1;
	}
	return 0;
}


KRESULT KBD_IsFull(void) {
	if ( KBD_Buffer_Head == ( (KBD_Buffer_Tail+1) % KBD_Buffer_MaxSize) ) {
		return 1;
	}		
	return 0;
}


// ***CERTIFIED***
KRESULT KBD_InsertKeyCode( WORD ExtendedKeyCode ) {
	if ( KBD_IsFull() ) {
		CRT_Print("[kbd2.c] Keyboard buffer IS FULL !");
		return 0;
	} else {
		KBD_Buffer_Tail = (KBD_Buffer_Tail+1) % KBD_Buffer_MaxSize;
		KBD_Buffer[ KBD_Buffer_Tail ] = ExtendedKeyCode;
		return 1;
	}	
}

// Called from outside 
// must be atomic
WORD KBD_ExtractKeyCode(void) {
	WORD wResult;
	asm("cli");
	if ( KBD_IsEmpty() ) {
		wResult = 0;
	} else {
		KBD_Buffer_Head = (KBD_Buffer_Head+1) % KBD_Buffer_MaxSize;
		wResult = KBD_Buffer[ KBD_Buffer_Head ];
	}
	asm("sti");
	return wResult;
}

//-----------------------------------------------------------------

void KEYBOARD_PutRawCode(WORD rawcode) {

	// O.K., separate this ... i don't want to pass 2 parameters
	BYTE hi_rawcode= rawcode >> 8;
	BYTE lo_rawcode= rawcode & 0xFF;

#ifdef DEBUG_KEYBOARD_DRIVER
	// First things to do: debug it !
	CRT_NewLine();
	CRT_Print("Rawcode: ");
	CRT_PrintWord( rawcode );
#endif 

	// When the keyboard handler call us and pass the raw code,
	// it can be one of the followings :
	// 00 <code>, E0 <code>, E1 <code>, FF FF (pause)


	// Check the type of key that was pressed
	if (hi_rawcode == 0) {
		// if the low code is major then 80 then it is a key that have been released
		// so you can safely ignore it
		if (lo_rawcode < 0x80 ) {
			switch (lo_rawcode) {
				case 0x1D: 
					// Send LCTRL down event
					KBD_StatusW = KBD_StatusW | KBD_Status1_LCtrl;
					KBD_InsertKeyCode( VKE_LCTRL  );
					break;
				case 0x2A: 
					// Send LSHIFT down event
					KBD_StatusW = KBD_StatusW | KBD_Status1_LShift;
					KBD_InsertKeyCode( VKE_LSHIFT );
					break;
				case 0x36: 
					// Right Shift
					KBD_StatusW = KBD_StatusW | KBD_Status1_RShift;
					KBD_InsertKeyCode( VKE_RSHIFT );
					break;
				case 0x38: 
					// Send LALT down event
					KBD_StatusW = KBD_StatusW | KBD_Status1_LAlt;
					KBD_InsertKeyCode( VKE_LALT );
					break;
				case 0x3A: 
					// Send CAPS Signal
					KBD_InsertKeyCode( VKE_CAPS );

					// if toggled must untoggle it and viceversa
					if ( (KBD_StatusW & KBD_Status1_Caps ) > 0) {
						KBD_StatusW = KBD_StatusW & ( ~ KBD_Status1_Caps );
						KBD_StatusL = QueryLedsStatus( KBD_StatusW );
						KEYBOARD_SetLeds( KBD_StatusL );
					} else {
						KBD_StatusW = KBD_StatusW | KBD_Status1_Caps;
						KBD_StatusL = QueryLedsStatus( KBD_StatusW );
						KEYBOARD_SetLeds( KBD_StatusL );
					}

					break;
				case 0x45: 
					// Send Bloc Num Signal
					KBD_InsertKeyCode( VKE_BLOCNUM );

					// if toggled must untoggle it and viceversa
					if ( (KBD_StatusW & KBD_Status1_NumLock) > 0) {
						KBD_StatusW = KBD_StatusW & ( ~ KBD_Status1_NumLock );
						KBD_StatusL = QueryLedsStatus( KBD_StatusW );
						KEYBOARD_SetLeds( KBD_StatusL );
					} else {
						KBD_StatusW = KBD_StatusW | KBD_Status1_NumLock;
						KBD_StatusL = QueryLedsStatus( KBD_StatusW );
						KEYBOARD_SetLeds( KBD_StatusL );
					}
					break;

				case 0x46: 
					// Send Scroll Signal
					KBD_InsertKeyCode( VKE_SCROLL );

					// if toggled must untoggle it and viceversa
					if ( (KBD_StatusW & KBD_Status1_Scroll) > 0) {
						KBD_StatusW = KBD_StatusW & ( ~ KBD_Status1_Scroll );
						KBD_StatusL = QueryLedsStatus( KBD_StatusW );
						KEYBOARD_SetLeds( KBD_StatusL );
					} else {
						KBD_StatusW = KBD_StatusW | KBD_Status1_Scroll;
						KBD_StatusL = QueryLedsStatus( KBD_StatusW );
						KEYBOARD_SetLeds( KBD_StatusL );
					}
				default:   
					// A Normal Characters
					KBD_StatusB    = QueryKeyboardStatus( KBD_StatusW );
 					KBD_InsertKeyCode( (KBD_StatusB<<8) + lo_rawcode );
			}
		} else {
			// Here comes code major or equal then 0x80
			switch (lo_rawcode) {
				case 0x9D: 
					// Release Left CTRL
					KBD_StatusW = KBD_StatusW & (~ KBD_Status1_LCtrl);
					break;
				case 0xAA: 
					// Release Left SHIFT
					KBD_StatusW = KBD_StatusW & (~ KBD_Status1_LShift);
					break;
				case 0xB6: 
					// Release Right SHIFT
					KBD_StatusW = KBD_StatusW & (~ KBD_Status1_RShift);
					break;
				case 0xB8: 
					// Release Left ALT
					KBD_StatusW = KBD_StatusW & (~ KBD_Status1_LAlt);
					break;
				case 0xBA: 
					// Ignore the Release of the CAPS Lock
					break;
				case 0xC5: 
					// Ignore the release of NUM BLOC
					break;
				default:
					// This char are "released key"
					// So we can silently ignore them
					break;
			}
		}
	} 
	else if (hi_rawcode == 0xE0 ) {
		switch (lo_rawcode) {
			case 0x1D: 
				// Send RCTRL down event
				KBD_StatusW = KBD_StatusW | KBD_Status1_RCtrl;
				KBD_InsertKeyCode( VKE_RCTRL );
				break;
			case 0x9D: 
				// Release Right CTRL
				KBD_StatusW = KBD_StatusW & (~ KBD_Status1_RCtrl);
				break;
			case 0x38: 
				// Send RALT down event
				KBD_StatusW = KBD_StatusW | KBD_Status1_RAlt;
				KBD_InsertKeyCode( VKE_RALT );
				break;
			case 0xB8: 
				// Release Right ALT
				KBD_StatusW = KBD_StatusW & (~ KBD_Status1_RAlt);
				break;
			case 0x1C: 
				// Enter on numeric PAD
				KBD_InsertKeyCode( VKE_ENTER );
				break; 
			case 0x37:
				// STAMP or CTRL+STAMP=SYSTEM REQUEST
				if ( (KBD_StatusW && KBD_Status1_Ctrls) > 0) {
					KBD_InsertKeyCode( VKE_PRINT );
				} else {
					#ifdef DEBUG_KEYBOARD_DRIVER
						SWITCH_BackToDOS();
					#endif
					KBD_InsertKeyCode( VKE_SYSREQ );
				}
				break;
			case 0x35:
				// Slash on Numeric Pad
				KBD_InsertKeyCode( VKE_SLASH );
				break;
			case 0x47:
				// HOME
				KBD_InsertKeyCode( VKE_HOME );
				break;
			case 0x48:
				// Up Arrow
				KBD_InsertKeyCode( VKE_UP );
				break;
			case 0x49:
				// Pag. UP
				KBD_InsertKeyCode( VKE_PGUP );
				break;
			case 0x4B:
				// Left Arrow
				KBD_InsertKeyCode( VKE_LEFT );
				break;
			case 0x4D:
				// Right Arrow
				KBD_InsertKeyCode( VKE_RIGHT );
				break;
			case 0x4F:
				// END
				KBD_InsertKeyCode( VKE_END );
				break;
			case 0x50:
				// Down Arrow
				KBD_InsertKeyCode( VKE_DOWN );
				break;
			case 0x51:
				// Pag. DOWN
				KBD_InsertKeyCode( VKE_PGDN );
				break;
			case 0x52:
				// INSERT
				KBD_InsertKeyCode( VKE_INSERT );
				break;
			case 0x53:

	// <NEW>
	if ( 
		((KBD_StatusW & ( KBD_Status1_LCtrl | KBD_Status1_RCtrl )) > 0) &&
		((KBD_StatusW & ( KBD_Status1_LAlt  | KBD_Status1_RAlt  )) > 0) 
	) {
		SWITCH_Reboot();
	}
	// </NEW>

				// Normal DELETE
				KBD_InsertKeyCode( VKE_DELETE );
				break;
			default:   
				// CRT_Print("E0 UNKNOWN ");
				break;
		}
	} 
	else if (hi_rawcode == 0xE1 ) {
		CRT_Print("E1 UNKNOWN ");
	} 
	else if (hi_rawcode == 0xFF ) {
		// "PAUSE PRESSED" Put event in queque here
		KBD_InsertKeyCode( VKE_PAUSE );
	} 
	else {
		CRT_Print( "[KEYBOARD_PutRawCode] Unknown sequence in hi_rawcode" );
		CRT_NewLine();
	}
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

/*
(Table P045)
Values for keyboard commands (data also goes to PORT 0060h):
Value   Count   Description
 F0h    double  get/set scan code set
                00h get current set
                01h scancode set 1 (PCs and PS/2 mod 30, except Type 2 ctrlr)
                02h scancode set 2 (ATs, PS/2, default)
                03h scancode set 3

 F3h    double  set typematic rate/delay
                format of the second byte:
                bit7=0 : reserved
                bit6-5 : typemativ delay
                         00b=250ms     10b= 750ms
                         01b=500ms     11b=1000ms
                bit4-0 : typematic rate (see #P050)

 F4h    sngl    enable keyboard

 F5h    sngl    disable keyboard. set default parameters (no keyboard scanning)

 F6h    sngl    set default parameters


(Table P050)
Values for keyboard typematic rate:
 00000b=30.0   10000b=7.5
 00001b=26.7   10001b=6.7
 00010b=24.0   10010b=6.0
 00011b=21.8   10011b=5.5
 00100b=20.0   10100b=5.0
 00101b=18.5   10101b=4.6
 00110b=17.1   10110b=4.3
 00111b=16.0   10111b=4.0
 01000b=15.0   11000b=3.7
 01001b=13.3   11001b=3.3
 01010b=12.0   11010b=3.0
 01011b=10.9   11011b=2.7
 01100b=10.0   11100b=2.5
 01101b= 9.2   11101b=2.3
 01110b= 8.5   11110b=2.1
 01111b= 8.0   11111b=2.0
SeeAlso: #P045

*/
