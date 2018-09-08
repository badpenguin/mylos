#define PACKED __attribute__ ((packed))

#define NULL	0
#define FALSE	0

#define BYTE	unsigned char
#define WORD	unsigned short
#define DWORD	unsigned long
#define KRESULT unsigned long

#define selector_kernel_RAW	 24	// Our Flat Selector
#define selector_kernel32_CS	104
#define selector_kernel32_DS	112

#define selector_task0          56
#define selector_stack0		64
#define selector_stack1         73
#define selector_stack2         82

