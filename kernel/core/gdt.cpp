// This handle the GDT
// When kernel start a new GDT is builded copying the GDT created by start.com
// this because old GDT reside in DOS memory are and i wont it to be up 1 MB

#include "kernel.h"
#include "crt.h"
#include "kmemory.h"
#include "farptr.h"
#include "tss.h"
#include "gdt.h"
#include "traps.h"

#define GDT_RESERVED	20		// Reserve first 20 entries for the Kernel
#define GDT_MAX_LEN	8192
#define GDT_SIZE	(sizeof(TGDT_Descriptor) * GDT_MAX_LEN)

struct TGDT_Descriptor *gdt;

// define SELECTOR_GDT(X) = (X<<3) + RPL
// define SELECTOR_LDT(X) = (X<<3) + RPL + 4;

struct TGDT_IDT_Pointer{
	WORD limit;
	DWORD base;
} GDT_old_gdt, GDT_new_gdt, GDT_old_idt;


static __inline__
void CPU_StoreGDT(TGDT_IDT_Pointer *old_gdt)
{
	asm volatile ("sgdt %0" : "=m"(*old_gdt));
}

static __inline__
void CPU_LoadGDT(TGDT_IDT_Pointer *new_gdt)
{
	asm volatile ("lgdt %0" : /* no output */ : "m"(*new_gdt));
}


void GDT_Init(void) {
	BYTE  b;	// buffer
	DWORD a;	// Address old gdt
	BYTE *d;	// Address new gdt
	WORD  i;	// Index

	// Allocate memory for the new BIG "gdt" and clean it
	gdt = (TGDT_Descriptor*)KMEMORY_Alloc( GDT_SIZE, 4096);	
	for ( i=0; i<GDT_MAX_LEN ; i++)	{
		gdt[i].SegLimit1 = 0;
        	gdt[i].Base1500  = 0;
        	gdt[i].Base2316  = 0;
        	gdt[i].SegType   = 0;
		gdt[i].SegLimit2 = 0;
        	gdt[i].Base3124  = 0;
	}

	// Get Previous GDT then copy, byte per byte, the old on the newer
	CPU_StoreGDT( &GDT_old_gdt );

	a = GDT_old_gdt.base;
	d = (BYTE*)gdt;
	for ( i=0; i < (GDT_old_gdt.limit+1) ; i++ ) {
		b = _farpeekb( selector_kernel_RAW, a+i );
		_farpokeb( selector_kernel32_DS, (DWORD)d+i, b );
	}

	// Install our new GDT
	GDT_new_gdt.limit = (WORD)GDT_SIZE - 1;
	GDT_new_gdt.base = (DWORD)KERNEL_CODE_START + (DWORD)gdt;  // Linear Memory
	CPU_LoadGDT( &GDT_new_gdt );
}



void GDT_Stats(void) {
	CRT_NewLine();
	CRT_Print( "GDT Stats "); CRT_NewLine();
	CRT_Print( "GDT is made of elements nr. ...............: "); CRT_PrintWord( GDT_MAX_LEN ); CRT_NewLine();
	CRT_Print( "GDT need to allocate bytes ................: "); CRT_PrintDWord( GDT_SIZE ); CRT_NewLine();
	CRT_Print( "GDT was allocated at addr. ................: "); CRT_PrintDWord( (DWORD)gdt ); CRT_NewLine();

	// Getting Previous GDT
	CRT_Print( "Previous GDT is allocate at (limit,base) ..: ");
	CRT_PrintWord( GDT_old_gdt.limit );
	CRT_PrintDWord( GDT_old_gdt.base );
	CRT_NewLine();

	CRT_Print( "The newer GDT is allocated at linear addr. : "); 
	CRT_PrintWord(  GDT_new_gdt.limit ); 
	CRT_PrintDWord( GDT_new_gdt.base  ); 
	CRT_NewLine();
}

// =====================================================================

void Process_FillTSSDesc( WORD TssSelector, DWORD TssAddress ) {
	WORD i = TssSelector / 8;

	gdt[i].SegLimit1 = sizeof(TTSS);		// TSS Size
       	gdt[i].Base1500  = (TssAddress & 0xFFFF);
       	gdt[i].Base2316  = (TssAddress >> 16);
       	gdt[i].SegType   = 0xE9;
	gdt[i].SegLimit2 = 0x00;
       	gdt[i].Base3124  = (TssAddress >> 24);
}


// This have to be changed and adapted to process
void Process_FillKernelTSS(TTSS* ptss, DWORD kernelstack)
{
	ptss->eip = (DWORD)(void*)&KERNEL_Panic;
	ptss->esp = kernelstack;
	ptss->cs = selector_kernel32_CS;
	ptss->ds = selector_kernel32_DS;
	ptss->es = selector_kernel32_DS;
	ptss->fs = selector_kernel_RAW;
	ptss->gs = selector_kernel_RAW;
	ptss->ss = selector_kernel_RAW;
	ptss->ss0 = selector_kernel_RAW;
	ptss->esp0 = kernelstack;
	ptss->trap = 0x00;
	ptss->iomap = 0x00FF;
	ptss->eflags = 0x00000202;
	// ptss->cr3 = (DWORD)(void*)pd;
}


DWORD GDT_FindFreeDescriptor(void) {
	WORD i = GDT_RESERVED;
	while ( gdt[i].SegLimit1 != 0 ) { 
	  i++; 
	}
	if (i < GDT_MAX_LEN) {
		return i;
	} else {
		return 0;
	}
}

