//; Handle Process

#include "kernel.h"
#include "crt.h"
#include "kmemory.h"
#include "tss.h"
#include "gdt.h"
#include "process.h"
#include "switch.h"
#include "sched.h"

#define		PROCESS_ListSize	4096
DWORD 		PROCESS_MaxCount;
DWORD		PROCESS_Count;
struct TProcess	*PROCESS_List;

void PROCESS_Init(void) {
	// Count how many TProcess can be allocated into one pages
	PROCESS_Count	 = 0;
	PROCESS_MaxCount = (DWORD)( PROCESS_ListSize / sizeof(TProcess) );
	// Get That Physical Pages
	PROCESS_List = (TProcess*)KMEMORY_Alloc( (DWORD)PROCESS_ListSize, 16 );
	// Initialize the list
	if ( PROCESS_List == 0 ) {
		CRT_Print("[PROCESS] Cannot allocate memory for the PROCESS_List memory !");
		SWITCH_BackToDOS();
	}
}


void PROCESS_Stats(void) {
	WORD i;
	// Count how many TProcess can be allocated into one pages
	CRT_NewLine();
	CRT_Print("PROCESS Stats");
	CRT_NewLine();

	CRT_Print( "Process list size is byte ....: ");
	CRT_PrintDWord( (DWORD)PROCESS_ListSize );
	CRT_NewLine();

	CRT_Print( "Process list room for entry ..: " );
	CRT_PrintDWord( PROCESS_MaxCount );
	CRT_NewLine();

	CRT_Print( "Process list begin address is : ");
	CRT_PrintDWord( (DWORD)(PROCESS_List) );
	CRT_NewLine();

	CRT_Print( "Process present into system ..: ");
	CRT_PrintDWord( PROCESS_Count );
	CRT_NewLine();

	CRT_Print( "Times Kernel has become IDLE .: ");
	CRT_PrintDWord( SK_IDLETime );
	CRT_NewLine();

	CRT_Print( "INT 1 not served by Skeduler .: ");
	CRT_PrintDWord( SK_IRQCounter );
	CRT_NewLine();

	CRT_NewLine();
	CRT_Print( " -N-  Name          PID  Memory Status Levl SKedTime PreEmptT CPU_Time" );
	for ( i=0; i<PROCESS_Count; i++) {
		CRT_NewLine();

		CRT_PrintWord( i );
		CRT_Print( PROCESS_List[i].Name + '\0' );
		CRT_PrintWord( PROCESS_List[i].PID );
		CRT_PrintDWord( PROCESS_List[i].Memory );
		CRT_PrintWord( PROCESS_List[i].Status );
		CRT_PrintWord( PROCESS_List[i].Priority );
		CRT_PrintDWord( PROCESS_List[i].SKTimes );
		CRT_PrintDWord( PROCESS_List[i].PreTimes );
		CRT_PrintDWord( PROCESS_List[i].CpuTime );
	}
}


// Pass the Linear address of a procedure and return the Process_List index
DWORD PROCESS_Create( const char ProcessName[16], DWORD ProcessLinearAddress, DWORD ProcessStackSize, WORD PrioLevel ) {

	// Add process to the process list
	TProcess *CurrentProcess = PROCESS_List + PROCESS_Count ;	// Pointer to the first free process
	TTSS	 *CurrentTSS;						// Pointer to the process TSS
	WORD	 CurrentTSSSelector;					// Index in GDT of the Task State Segment Selector
	DWORD	 CurrentTSSLinearPosition;				// Linear address of the process TSS
	DWORD	 ProcessStackStart;					// Linear address of the stack allocated for the TSS


	WORD i = 0;
	while ( ProcessName[i] != NULL ) {
		CurrentProcess->Name[i] = ProcessName[i];
		i++;
	}
	while ( i < 15 ) {
		CurrentProcess->Name[i] = ' ';
		i++;
	}
	CurrentProcess->Name[15] = 0;

	CurrentProcess->PID    = PROCESS_Count;
	CurrentProcess->Memory = NULL;
	CurrentProcess->Status = 1;		// Ready
	CurrentProcess->Priority = PrioLevel;

	// Temporary solution
	CurrentProcess->PParent  =NULL;
	CurrentProcess->CpuTime  =0;
	CurrentProcess->SKTimes  =0;
	CurrentProcess->PreTimes =0;
	CurrentProcess->CreationTime =0;

	PROCESS_Count++;

	// Allocate TSS for the process
	CurrentProcess->TSS  = (TTSS*)KMEMORY_Alloc( 512,512); 		// sieof(TProcess), 16 );
	ProcessStackSize    += 4;					// Start a DWORD before, only for paranoid
	ProcessStackStart    = KMEMORY_Alloc( ProcessStackSize, 16);
	CurrentTSS	     = CurrentProcess->TSS;
	// CRT_Print("Current TSS allocated at: "); CRT_PrintDWord( (DWORD)CurrentTSS ); CRT_NewLine();

	// Fill the TSS Data
	CurrentTSS->BackLink = 0x0000;
	CurrentTSS->esp0  = 1024;
  	CurrentTSS->ss0   = selector_stack0;
	CurrentTSS->esp1  = 1024;
  	CurrentTSS->ss1   = selector_stack1;
	CurrentTSS->esp2  = 1024;
  	CurrentTSS->ss2   = selector_stack2;
	CurrentTSS->cr3   = 0;
	CurrentTSS->eip   = ProcessLinearAddress;
	CurrentTSS->eflags= 0x202;
	CurrentTSS->esp   = ProcessStackStart + ProcessStackSize-4;
	CurrentTSS->ss    = selector_kernel_RAW;
	CurrentTSS->es    = selector_kernel32_DS;
	CurrentTSS->cs    = selector_kernel32_CS;
  	CurrentTSS->ss    = selector_kernel_RAW;
	CurrentTSS->ds    = selector_kernel32_DS;
	CurrentTSS->fs    = selector_kernel_RAW;
	CurrentTSS->gs    = selector_kernel_RAW;
  	CurrentTSS->ldt   = 0;
	CurrentTSS->trap  = 0x00;
        CurrentTSS->iomap = 0x00FF;

	// Find the index into the GDT for a free descriptor
	CurrentTSSSelector   = GDT_FindFreeDescriptor();

	// Allocate that free descriptor with the TSS Descriptor
	CurrentProcess->Selector = CurrentTSSSelector << 3;
	CurrentTSSLinearPosition = KERNEL_CODE_START+ (DWORD)CurrentTSS;
	gdt[ CurrentTSSSelector ].SegLimit1 = 512; // sizeof(TTSS);
	gdt[ CurrentTSSSelector ].Base1500  = CurrentTSSLinearPosition & 0xFFFF;
  	gdt[ CurrentTSSSelector ].Base2316  = (CurrentTSSLinearPosition >> 16) & 0xFF;
  	gdt[ CurrentTSSSelector ].SegType   = 0x89;
  	gdt[ CurrentTSSSelector ].SegLimit2 = 0x0;
  	gdt[ CurrentTSSSelector ].Base3124  = (CurrentTSSLinearPosition >> 24) & 0xFF;
	return ( CurrentProcess->PID );
};

