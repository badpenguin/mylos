//; This handle a STATIC LINEAR MEMORY MANAGER, for now ...

#include "kernel.h"
#include "crt.h"
#include "kmemory.h"


DWORD KMEMORY_CodeEnd(void) {
	return ((DWORD)(&KERNEL_CODE_ENDS_HERE)); // + KERNEL_CODE_START;
}


DWORD	KMEM_Start;
DWORD	KMEM_Current;


void KMEMORY_Init(void) {
	// Compute End of the Kernel code and data
	KMEM_Start   = KMEMORY_CodeEnd() + 32;
	// Begin the kernel memory at the next page
	KMEM_Start   = KMEM_Start+ ( 4096 - ( KMEM_Start % 4096 ) );
	KMEM_Current = 0;
}


void KMEMORY_Stats(void) {
	CRT_Print("Kernel Statical Memory (KMEM) Stats"); CRT_NewLine();
	CRT_Print("Kernel Code Ends at ....:"); CRT_PrintDWord( KMEMORY_CodeEnd() ); CRT_NewLine();
	CRT_Print("Kernel Memory begin at .:"); CRT_PrintDWord( KMEM_Start ); CRT_NewLine();
	CRT_Print("Kernel Memory ends at ..:"); CRT_PrintDWord( KERNEL_MEMORY_LIMIT ); CRT_NewLine();
	CRT_Print("Kernel Memory Allocated :"); CRT_PrintDWord( KMEM_Current ); CRT_NewLine();
	CRT_Print("Kernel Memory Free .....:"); CRT_PrintDWord( KERNEL_MEMORY_LIMIT-KMEM_Start-KMEM_Current ); CRT_NewLine();
}


DWORD KMEMORY_Alloc( DWORD NeededSize, DWORD AlignSize ) {
	DWORD CurrentEnd = KMEM_Start + KMEM_Current;
	DWORD AlignedSize = 0;
	if ( ( CurrentEnd + AlignSize ) < KERNEL_MEMORY_LIMIT ) {
		AlignedSize  = NeededSize+ ( AlignSize - ( KMEM_Start % AlignSize ) );
		KMEM_Current = KMEM_Current + NeededSize + AlignedSize;
		return ( CurrentEnd + AlignedSize );
	} else {
		return 0;
	}
}

