#include "kernel.h"
#include "crt.h"

char	KBD_Debug_String[]="\0\0\0\0\0\0\0\0\0\0";
char	HEXA[]="0123456789ABCDEF?";


void CRT_PrintChar( char c ) {
	KBD_Debug_String[0] = c;
	KBD_Debug_String[1] = 32;
	KBD_Debug_String[2] = 0;
	CRT_Print( KBD_Debug_String );

}


void CRT_PrintWord( WORD w ) {
	WORD b1 = w>>8;
	WORD b2 = w & 0xFF;
	KBD_Debug_String[0] = HEXA[ b1>>4   ];
	KBD_Debug_String[1] = HEXA[ b1 & 0x0F ];
	KBD_Debug_String[2] = HEXA[ b2>>4   ];
	KBD_Debug_String[3] = HEXA[ b2 & 0x0F ];
	KBD_Debug_String[4] = 32;
	KBD_Debug_String[5] = 0;
	CRT_Print( KBD_Debug_String );
}


void CRT_PrintDWord( DWORD dw ) {
	DWORD w1 = dw>>16;
	DWORD w2 = dw & 0xFFFF;

	WORD a1 = w1>>8;
	WORD a2 = w1 & 0xFF;

	WORD b1 = w2>>8;
	WORD b2 = w2 & 0xFF;

	KBD_Debug_String[0] = HEXA[ a1>>4   ];
	KBD_Debug_String[1] = HEXA[ a1 & 0x0F ];
	KBD_Debug_String[2] = HEXA[ a2>>4   ];
	KBD_Debug_String[3] = HEXA[ a2 & 0x0F ];
	KBD_Debug_String[4] = HEXA[ b1>>4   ];
	KBD_Debug_String[5] = HEXA[ b1 & 0x0F ];
	KBD_Debug_String[6] = HEXA[ b2>>4   ];
	KBD_Debug_String[7] = HEXA[ b2 & 0x0F ];
	KBD_Debug_String[8] = 32;
	KBD_Debug_String[9] = 0;
	CRT_Print( KBD_Debug_String );
}

