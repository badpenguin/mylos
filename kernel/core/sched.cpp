//; Ladies and gentlemen ... the scheduler !!!

#include "kernel.h"
#include "tss.h"
#include "process.h"
#include "crt.h"
#include "kbd_api.h"

// QUANTUM is the minimu time a process can run
#define		QUANTUM		10

DWORD		SK_CpuTime;
DWORD		SK_IRQCounter;			// IRQ NOT Served by the TIMER DRIVER due to semaphore RED
DWORD		SK_SchedTime;			// Time the process have been scheduled
DWORD		SK_SwitchTime;			// Time remain before the process have to leave
WORD		SK_IdleTask;
TProcess	* SK_ActiveTask;		// Active Task

extern WORD	SK_InTask;			// Selector of the previos task
extern WORD	SK_OutTask;			// Selector of the scheduled task
DWORD		SK_IDLETime;			// Time the kernel goes into the IDLE section




void SCHEDULER_Init( void ) {
	// Initialize the Scheduler
	SK_CpuTime = 0;
	SK_IRQCounter = 0;
	SK_SchedTime = 0;	
	SK_SwitchTime = 0;	
	SK_ActiveTask = NULL;
	SK_InTask  = 0;		
	SK_OutTask = 0;
	SK_IDLETime = 0;
	SK_IdleTask = 0;
}




void SCHEDULER_Do( void ) {
	long LastProcess     = -1;
	long SelectedProcess = -1;
	WORD LastPriority    = 0;
	WORD i = 0;

	CRT_Print("’");
//	CRT_Print("  Out:");
//	CRT_PrintWord( SK_InTask );

	// Update Outgoing task

	SK_OutTask = SK_InTask;
	if ( SK_InTask > 0 ) {

		if ( SK_ActiveTask != NULL ) {
	  		SK_ActiveTask->Status   = 1;	// From RUNNING to READY
			SK_ActiveTask->PreTimes = SK_ActiveTask->PreTimes + 1;
			SK_ActiveTask->CpuTime  = SK_ActiveTask->CpuTime  + SK_CpuTime;
			SK_CpuTime = 0;
		}

		// Find The Last Process by the Selector
		i=0;
		while (i<PROCESS_Count) {
			if ( PROCESS_List[i].Selector == SK_InTask ) {
				LastProcess=i;
				i = PROCESS_Count;
			}
			i++;
		}

	}

	// Search Until the End of the List
        // Process with priority 0 are scheduled only if there are
        // no other READY project

	for ( i=LastProcess+1; i<PROCESS_Count; i++ ) {
		if (PROCESS_List[i].Status == 1) {
			LastPriority = PROCESS_List[i].Priority;
        		SelectedProcess = i;
                	if (LastPriority>0) {
				i = PROCESS_Count;		// exit;
			}
		}	
	}

        if ( LastPriority==0 ) {
		for ( i=0; i<LastProcess; i++ ) {
			if (PROCESS_List[i].Status == 1) {
				LastPriority = PROCESS_List[i].Priority;
				// Avoid to override a previous 0 priority scheduled task
				if (LastPriority>0) {
	        			SelectedProcess = i;
					i = LastProcess;	// exit;
				}
			}	
		}
        }

	// Put the Selected process in a running status
	if (SelectedProcess>=0) {
		SK_ActiveTask = (PROCESS_List+SelectedProcess);
		SK_ActiveTask->SKTimes = SK_ActiveTask->SKTimes  + 1;
		SK_ActiveTask->Status  = 0;
		SK_InTask    = SK_ActiveTask->Selector;
		if ( SK_InTask == 0 ) {
			CRT_Print("Aiiee, scheduled selector problem !");
		}
		SK_SwitchTime = (SK_ActiveTask->Priority +1) * QUANTUM;
	} else {
		CRT_Print("*** NO ACTIVE TASKS ***");
		SK_IdleTask = SK_OutTask;
		SK_ActiveTask = NULL;
		SK_InTask = 0;
	}

//	CRT_Print("  In:");
//	CRT_PrintWord( SK_InTask );
//	CRT_NewLine();
}

