// into process.cpp

typedef	DWORD		PMEM_Link;

struct TProcess {
	char        	Name[16];		// Name of the process
	DWORD 		PID;			// PID
	DWORD		PParent;		// ^Parent TSS
	PMEM_Link	Memory;			// Memory Used
	TTSS		*TSS;			// Pointer To TSS
	WORD		Selector;		// Selector of the TSS into the GDT
	WORD		Status;			// 0=Running, 1=Ready, 2=Waiting
	WORD		Priority;		// 0=Scheduled only if is the only ready job; n=scheduled for n*QUANTUM
	DWORD		CpuTime;		// Time used by the process
	DWORD		SKTimes;		// Times process have been scheduled
	DWORD		PreTimes;		// Times process have been pre-empted
	DWORD		CreationTime;		// CPU Time at wich the process have been created
};

extern struct TProcess *PROCESS_List;
extern DWORD  PROCESS_Count;

extern void PROCESS_Init(void);
extern void PROCESS_Stats(void);
extern DWORD PROCESS_Create( const char ProcessName[16], DWORD ProcessLinearAddress, DWORD ProcessStackSize, WORD PrioLevel );

