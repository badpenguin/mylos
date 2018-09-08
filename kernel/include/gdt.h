// into gdt.cpp

struct TGDT_Descriptor {
        WORD SegLimit1;
        WORD Base1500;
        BYTE Base2316;
        BYTE SegType;
	BYTE SegLimit2;
        BYTE Base3124;
};

extern void GDT_Init(void);
extern void GDT_Stats(void);

extern struct TGDT_Descriptor *gdt;
extern DWORD GDT_FindFreeDescriptor(void);

