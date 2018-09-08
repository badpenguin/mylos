#include "kernel.h"
#include "crt.h"
#include "kmemory.h"
#include "tss.h"
#include "process.h"
#include "gdt.h"
#include "switch.h"
#include "kbd_api.h"

#define		MIN_AWAITING_YA		0x00C00000

extern WORD IDLESelector;
extern void OS_DUMP(void);

void CONSOLE_Init(void) {
	charEx	LastKey;
	DWORD	WaitCount;

	CRT_Print("Starting Console ...");
	CRT_NewLine();

	// Await a litte bit
	CRT_Print( "Awaiting for your orders, captain Kirk." );	CRT_NewLine();
	LastKey = 0;
	WaitCount = 0;
//	while ( ( LastKey == 0 ) && ( WaitCount < MIN_AWAITING_YA ) ) {
//		LastKey = readkeyEx();
//		WaitCount++;
//	}

	// Display Console screen
        CRT_SetColor( CRT_ColorScheme02 );
	CRT_Clear();

	CRT_GotoXY(0,0);
        CRT_SetColor( CRT_ColorScheme03 );
	CRT_Print("MYLOS - System Console                                                         ");

	CRT_GotoXY(0,2);
        CRT_SetColor( CRT_ColorScheme02 );
	CRT_Print( "[F1]  Display stats about Kernel Statical Memory Manager");	CRT_NewLine();
	CRT_Print( "[F2]  Display stats about Kernel Process Manager");	CRT_NewLine();
	CRT_Print( "[F3]  Display stats about Kernel Structure (GDT)");	CRT_NewLine();
	CRT_Print( "[F4]  OS DUMP"); CRT_NewLine();
	CRT_Print( "[F5]  "); CRT_NewLine();
	CRT_Print( "[F6]  "); CRT_NewLine();
	CRT_Print( "[F7]  "); CRT_NewLine();
	CRT_NewLine();
	CRT_Print( "[ESC] Shut Down the system, and return to DOS"); CRT_NewLine();
	CRT_NewLine();
	CRT_Print( "בגדהוא ");	CRT_NewLine();

	// Loop until timeout or Esc Key is pressed  ...
	LastKey = 0;
	WaitCount = 0;
	while ( LastKey != 1 ) {
		LastKey = readkeyEx();
		if ( LastKey == VKE_F1 ) {
			CRT_NewLine();
			KMEMORY_Stats();
		} else
		if ( LastKey == VKE_F2 ) {
			CRT_NewLine();
			PROCESS_Stats();
		} else
		if ( LastKey == VKE_F3 ) {
			CRT_NewLine();
			GDT_Stats();
		} else
		if ( LastKey == VKE_F4 ) {
			OS_DUMP();
		} else
		if ( LastKey == VKE_F5 ) {
		} else
		if ( LastKey == VKE_F6) {
		} else
		if ( LastKey == VKE_F7) {
		} else
		if ( LastKey != 0 ) {
			CRT_NewLine();
			CRT_Print( "ReadedKey: " );
			CRT_PrintWord( (WORD)LastKey );
			CRT_PrintChar( ConvertExtendedKey(LastKey) );			
		}
		WaitCount++;
	}
	
	// Test
	CRT_NewLine();
	CRT_NewLine();
	CRT_Print("Emptying Keyboard buffer : ");
	while ( LastKey != 0 ) {
		CRT_PrintWord( (WORD)LastKey );
		CRT_PrintWord( 
			(WORD)( ConvertExtendedKey(LastKey) )
		);
		LastKey = readkeyEx();
	}
	CRT_NewLine();
	CRT_NewLine();
// *** Console END ***
	CRT_Print( "Disabling Scheduler" );
	CRT_NewLine();
	asm("cli");
	CRT_Print( "Switching back to DOS" );
	CRT_NewLine();
	SWITCH_BackToDOS();
}

