#define		charEx		unsigned int

extern char	readkey(void);		// Return an ASCII Code
extern charEx	readkeyEx(void);	// Return an Extended Key
extern char 	ConvertExtendedKey( charEx ExCode );
extern void	waitAnyKey(void);

// -------------------------------------------------------------------------------------------

// VirtualKey returned from "readkey"
#define VK_ESC  0x001B

// Extended Key value to add to the normal value
#define VKA_SHIFT	0x0100
#define VKA_CONTROL	0x0200
#define VKA_ALT		0x0400


// Extended Virtual Key Returned from "readkeyEx"
#define	VKE_ESC		0x0001
#define VKE_SLASH	0x0108	// VKE_SLASH = SHIFT + KEY '7'
#define VKE_BACKSPACE	0x000E
#define VKE_TAB		0x000F
#define VKE_ENTER	0x001C
#define VKE_SPACE	0x0039
#define VKE_F1  	0x003B
#define VKE_F2  	0x003C
#define VKE_F3  	0x003D
#define VKE_F4  	0x003E
#define VKE_F5  	0x003F
#define VKE_F6  	0x0040
#define VKE_F7  	0x0041
#define VKE_F8  	0x0042
#define VKE_F9  	0x0043
#define VKE_F10 	0x0044
#define VKE_BLOCNUM	0x0045	// This is never pressed togheter with Shift, Ctrl or Alt
#define VKE_SCROLL	0x0046	// This is never pressed togheter with Shift, Ctrl or Alt
#define VKE_HOME	0x0047
#define VKE_UP		0x0048
#define VKE_PGUP	0x0049
#define VKE_LEFT	0x004B
#define VKE_RIGHT	0x004D
#define VKE_END		0x004F
#define VKE_DOWN	0x0050
#define VKE_PGDN	0x0051
#define VKE_INSERT	0x0052
#define VKE_DELETE	0x0053
#define VKE_F11 	0x0057
#define VKE_F12 	0x0058

// Non standard Key Code
#define VKE_SYSREQ	0x0060	// This is never pressed togheter with Shift, Ctrl or Alt
#define VKE_PRINT	0x0061	// This is never pressed togheter with Shift, Ctrl or Alt
#define VKE_CAPS	0x0062	// This is never pressed togheter with Shift, Ctrl or Alt
#define VKE_RSHIFT	0x0063	// This is never pressed togheter with Shift, Ctrl or Alt
#define VKE_LSHIFT	0x0064	// This is never pressed togheter with Shift, Ctrl or Alt
#define VKE_RCTRL 	0x0065	// This is never pressed togheter with Shift, Ctrl or Alt
#define VKE_LCTRL 	0x0066	// This is never pressed togheter with Shift, Ctrl or Alt
#define VKE_RALT  	0x0067	// This is never pressed togheter with Shift, Ctrl or Alt
#define VKE_LALT  	0x0068	// This is never pressed togheter with Shift, Ctrl or Alt

#define VKE_PAUSE	0x00FF  // This is never pressed togheter with Shift, Ctrl or Alt

