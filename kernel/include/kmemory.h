// Memory allocation layout
//	00 - 01 MB	Reserved for MS-DOS
//	01 - 02 MB	Reserved for Kernel Code and Data
//	03 - 16 MB	Reserved for DMA
//	16 - 64 MB	User space

#define KMEMORY_1MB	0x00100000	//  1 MB
#define KMEMORY_16MB	0x01000000	// 16 MB
#define KMEMORY_32MB	0x02000000	// 32 MB
//define KMEMORY_BLOCKSIZE	4096	// Minimum size of a memory block


// Define Kernel Parameters
#define KERNEL_CODE_START	KMEMORY_1MB
#define KERNEL_MEMORY_START	(KMEMORY_1MB * 2)
#define KERNEL_MEMORY_LIMIT	(KMEMORY_1MB * 8)
#define MEMORY_MAXIMUM_LIMIT	KMEMORY_16MB


extern 	void KMEMORY_Init(void);
extern  DWORD *KERNEL_CODE_ENDS_HERE;
extern  DWORD KMEMORY_CodeEnd(void);
extern	void KMEMORY_Stats(void);
extern	DWORD KMEMORY_Alloc( DWORD NeededSize, DWORD AlignSize );

