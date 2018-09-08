struct TTSS {
	DWORD	BackLink;			// Task selector of interrupted task
	DWORD	esp0, ss0;			// Stack for level 0
	DWORD	esp1, ss1;			// Stack for level 1
	DWORD	esp2, ss2;			// Stack for level 2
	DWORD	cr3;
	DWORD	eip;
	DWORD	eflags;
	DWORD	eax, ecx, edx, ebx, esp, ebp;
	DWORD	esi, edi;
	DWORD	es, cs, ss, ds, fs, gs;
	DWORD	ldt;				// LDT Selector, always 0 (i.e. 100)
	WORD 	trap;				// Trap bit ( 1 = DEBUG; 0 = NODEBUG )
	WORD	iomap;				// DW 1111111111111111b	; NULL I/O Permission Bit Map
};

