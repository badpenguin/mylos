#include	"kernel.h"
#include	"kbd.h"
#include	"kbd_api.h"


char    readkey(void) {
	WORD w = KBD_ExtractKeyCode();
	if ( w == 0) {
   		return ((char)0);
	} else {
		return ConvertExtendedKey( w );
	}
}


charEx readkeyEx(void) {
	return ((charEx)KBD_ExtractKeyCode());
}


void	waitAnyKey(void) {
	while ( KBD_ExtractKeyCode() == 0 ) {
		asm("nop;nop;nop;nop");
	}
}

