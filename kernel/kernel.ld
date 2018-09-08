/*
	This file instruct ld.exe (our linker)
	how to assemble our object files.

	Remember: kernel.o must be the FIRST and KMEM_ENDS the last !!!
*/

SECTIONS {
	output : { 
		kernel.o
/* Kernel.o must be always the first */
		driver/crt/crt.o
		driver/crt/crt2.o
		core/idt.o
		driver/kbd/kbd2.o
		driver/kbd/kbd.o
		driver/kbd/kbd3.o
		ddk/kbd_api.o
		driver/console/console.o
		core/process.o
		core/gdt.o
		core/traps.o
		core/switch.o
		core/timer.o
		core/sched.o
/* Leave memory manager at the end of the Kernel */
		core/kmemory.o
		core/kmem_end.o
	}
}

