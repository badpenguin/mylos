#include "kernel.h"
#include "crt.h"
#include "idt.h"
#include "kbd.h"
#include "kmemory.h"
#include "tss.h"
#include "process.h"
#include "gdt.h"
#include "console.h"
#include "switch.h"
#include "timer.h"
#include "sched.h"


// Forward declarations
extern void KERNEL32_Start(void);
extern void KERNEL_IDLE(void);


// THis is the entry point of the kernel, for some reason 'ld' need that
// the entry point must be the first function of the file otherwise it
// will not link as fisr asm instruction
void ENTRY32(void)
{
	// This is the kernel SIGNATURE !!!
	__asm__ __volatile__ ("nop;nop;nop;nop");
	KERNEL32_Start();
	// GCC Put a 'ret' instruction instead of a 'ret far', so i needed
	//  to adjust this manually
	__asm__ __volatile__ ("cli;lret");
}

	// Global Vars Goes under this comment, NEVER BEFORE !!!
	// ....
	//


// This is the Kernel Primal Scream
void KERNEL32_Start(void)
{
	// First, initialize the CRT Driver so we can write to the console
	CRT_Init();
        CRT_SetColor( CRT_ColorScheme00 );
	// START.COM write 9 lines of info, so we continue at the 10th line
	CRT_GotoXY( 0, 9 );			
        CRT_Print("CRT Function Available");
        CRT_NewLine();

	// SetUp a New IDT with default IRQ Handlers
  	IDT_Init();
	CRT_Print("IDT Initialized");
        CRT_NewLine();

	// Now we can safely start the keyboard handler
	CRT_Print("Initializing the Keyboard Driver");
        CRT_NewLine();
  	KEYBOARD_Init();
  	KEYBOARD_Reset();
	// Hook (i.e. Redirect) the IRQ 33 to the Keyboard_Handler
        IDT_SetHardwareInterrupt(33,(DWORD)KEYBOARD_Handler);
	KEYBOARD_Start();

	// Start Kernel Memory Manager
	CRT_Print( "Initializing the Kernel's Static Memory Manager" );
        CRT_NewLine();
  	KMEMORY_Init();

	// Setup the new GDT for process
	CRT_Print( "Initializing the GDT" );
	CRT_NewLine();
    	GDT_Init();

	// Start Process Manager
	CRT_Print( "Initializing Process Manager" );
	CRT_NewLine();
	PROCESS_Init();

	// Spawn Processes
	CRT_Print("Starting Kernel tasks: ");

	CRT_Print("IDLE ");	
	PROCESS_Create( "IDLE", (DWORD)KERNEL_IDLE, 256, 0 );

	CRT_Print("CONSOLE ");
	PROCESS_Create( "CONSOLE", (DWORD)CONSOLE_Init, 4096, 10 );

//	CRT_Print("IDLE2 ");
//	PROCESS_Create( "IDLE2", (DWORD)KERNEL_IDLE, 256, 1 );

	CRT_NewLine();

	// It's time to let the dance begin ...
	CRT_Print("Initializing the SCHEDULER");
	CRT_NewLine();
	SCHEDULER_Init();

	CRT_Print("Enabling the SCHEDULER");
	CRT_NewLine();
	IDT_SetHardwareInterrupt(32,(DWORD)TIMER_Scheduler);
	// This unmask the timer IRQ and let the scheduler go ...
	// ... after enabling interrupts ...
	TIMER_Start();

	// Driver Loaded now, we can enable IRQ ...
	// ... say your prayer little one ...
	CRT_Print( "Enabling Interrupt" );
        CRT_NewLine();
	__asm__ __volatile__ ("sti");

        CRT_Print( "Kernel is now running ..." );
	while (1) {
		// At this point the kernel is pre-empted by the scheduler
		// ... we never more returns here
		asm("nop");
	}
	// We will never comes here ... now
	// before the scheduler was ready we goes here and return to ENTRY32
	// then, the 'ret far' give us back to ms-dos
}


// For semplycity i put the code HERE
void KERNEL_IDLE(void) {
	CRT_Print(" HALLO FROM IDLE PROCESS !!! ");
	while (1) {
		// CRT_Print(".");
		asm( "nop;nop;nop;" );
	}
}

