// Credits: This code was originnally learned from OS2000
// OS2000 was my main inspiration to start this project
// i like its approach but thinked to do more things 

#include "kernel.h"
#include "kmemory.h"

// These are defined into traps.asm
extern void INT_Spurius(void);
extern void INT_00(void);
extern void INT_01(void);
extern void INT_02(void);
extern void INT_03(void);
extern void INT_04(void);
extern void INT_05(void);
extern void INT_06(void);
extern void INT_07(void);
extern void INT_08(void);
extern void INT_09(void);
extern void INT_0A(void);
extern void INT_0B(void);
extern void INT_0C(void);
extern void INT_0D(void);
extern void INT_0E(void);
extern void INT_0F(void);
extern void INT_10(void);
extern void INT_11(void);
extern void INT_12(void);
extern void INT_13(void);
extern void INT_14(void);
extern void INT_15(void);
extern void INT_16(void);
extern void INT_17(void);
extern void INT_18(void);
extern void INT_19(void);
extern void INT_1A(void);
extern void INT_1B(void);
extern void INT_1C(void);
extern void INT_1D(void);
extern void INT_1E(void);
extern void INT_1F(void);

//These externs aren't really, they're just here for the compiler.
extern DWORD default_handler;
extern DWORD default_lo_int;
extern DWORD default_hi_int;
extern DWORD irq0;
extern DWORD irq1;
extern DWORD irq2;
extern DWORD irq3;
extern DWORD irq4;
extern DWORD irq5;
extern DWORD irq6;
extern DWORD irq7;


struct idt_gate {
        WORD offset1;
        WORD selector;
        BYTE DWORD_count;
        BYTE type;
        WORD offset2;
} idt[256];


struct
{
        WORD size;
        DWORD offset;
} idt_desc;

  

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// Set interrupt
//

void IDT_SetHardwareInterrupt (int intno, DWORD pointer) {
        idt[intno].selector=104;
        idt[intno].DWORD_count=0;
        idt[intno].type=0x0ee;
        idt[intno].offset1=pointer;
        idt[intno].offset2=pointer>>16;
        return;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// Output a BYTE to a port.
//

void outportb(DWORD port, DWORD data)
{
        asm ("movl %0,%%edx\n\t"
                "movl %1,%%eax\n\t"
                "outb %%al,%%dx\n\t": /* out */ : /* In */ "g" (port),
                        "g" (data) );
        return;
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// Default interrupt handlers.
// This function is never called, it just contains the handlers.
//

void default_handlers (void)
{
        asm (
			"_default_lo_int:\n\t"
                "cli \n\t"
"push %eax \n\t"
"mov $0x20,%al \n\t"
"outb %al,$0x20 \n\t"
"pop %eax \n\t"
"iret \n\t"

"_default_hi_int:\n\t"
"cli \n\t"
"push %eax \n\t"
"mov $0x20,%al \n\t"
"outb %al,$0xa0 \n\t"
"pop %eax \n\t"
"iret \n\t"

"_irq0: \n\t"
"cli\n\t"
"push %ds\n\t"
"push %eax\n\t"
"push %ebx\n\t"
"mov $24,%eax\n\t"
"mov %ax,%ds \n\t"
"mov $0xb8000+144,%ebx\n\t"
"movb (%ebx),%ax\n\t"
"inc %ax\n\t"
"and $0x7,%ax\n\t"
"or $0xF0,%ax\n\t"
"movb %ax,(%ebx)\n\t"
"pop %ebx\n\t"
"pop %eax\n\t"
"pop %ds\n\t"
"jmp _default_lo_int\n\t"

"_irq1: \n\t"
"cli \n\t"
"push %ds\n\t"
"push %eax\n\t"
"push %ebx\n\t"
"mov $24,%eax\n\t"
"mov %ax,%ds\n\t"
"mov $0xb8000+146,%ebx\n\t"
"incb (%ebx)\n\t"
"inb $0x60,%al\n\t"
"pop %ebx\n\t"
"pop %eax\n\t"
"pop %ds\n\t"
"jmp _default_lo_int\n\t"

"_irq2: \n\t"
"cli \n\t"
"push %ds\n\t"
"push %eax\n\t"
"push %ebx\n\t"
"mov $24,%eax\n\t"
"mov %ax,%ds\n\t"
"mov $0xb8000+148,%ebx\n\t"
"incb (%ebx)\n\t"
"pop %ebx\n\t"
"pop %eax\n\t"
"pop %ds\n\t"
"jmp _default_lo_int\n\t"

"_irq3: \n\t"
"cli \n\t"
"push %ds\n\t"
"push %eax\n\t"
"push %ebx\n\t"
"mov $24,%eax\n\t"
"mov %ax,%ds\n\t"
"mov $0xb8000+150,%ebx\n\t"
"incb (%ebx)\n\t"
"pop %ebx\n\t"
"pop %eax\n\t"
"pop %ds\n\t"
"jmp _default_lo_int\n\t"

"_irq4: \n\t"
"cli \n\t"
"push %ds\n\t"
"push %eax\n\t"
"push %ebx\n\t"
"mov $24,%eax\n\t"
"mov %ax,%ds\n\t"
"mov $0xb8000+152,%ebx\n\t"
"incb (%ebx)\n\t"
"pop %ebx\n\t"
"pop %eax\n\t"
"pop %ds\n\t"
"jmp _default_lo_int\n\t"

"_irq5: \n\t"
"cli \n\t"
"push %ds\n\t"
"push %eax\n\t"
"push %ebx\n\t"
"mov $24,%eax\n\t"
"mov %ax,%ds\n\t"
"mov $0xb8000+154,%ebx\n\t"
"incb (%ebx)\n\t"
"pop %ebx\n\t"
"pop %eax\n\t"
"pop %ds\n\t"
"jmp _default_lo_int\n\t"

"_irq6: \n\t"
"cli \n\t"
"push %ds\n\t"
"push %eax\n\t"
"push %ebx\n\t"
"mov $24,%eax\n\t"
"mov %ax,%ds\n\t"
"mov $0xb8000+156,%ebx\n\t"
"incb (%ebx)\n\t"
"pop %ebx\n\t"
"pop %eax\n\t"
"pop %ds\n\t"
"jmp _default_lo_int\n\t"

"_irq7: \n\t"
"cli \n\t"
"push %ds\n\t"
"push %eax\n\t"
"push %ebx\n\t"
"mov $24,%eax\n\t"
"mov %ax,%ds\n\t"
"mov $0xb8000+158,%ebx\n\t"
"incb (%ebx)\n\t"
"pop %ebx\n\t"
"pop %eax\n\t"
"pop %ds\n\t"
"jmp _default_lo_int\n\t"

                );

        asm (
		"_default_handler:\n\t"
		"iret\n\t"
		);

        return; //Just for compiler's sake.
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// Entry point.
//

void IDT_Init(void) {
        int i;
        //Fill IDT with default entries.
        for (i=0;i<256;i++)
        {
                IDT_SetHardwareInterrupt(i,(DWORD)INT_Spurius);
        }
        for (i=32;i<40;i++)
                IDT_SetHardwareInterrupt(i,(DWORD)&default_lo_int);
        for (i=40;i<48;i++)
                IDT_SetHardwareInterrupt(i,(DWORD)&default_hi_int);
	
	// Set the Hardware IRQ
        IDT_SetHardwareInterrupt(32,(DWORD)&irq0);
        IDT_SetHardwareInterrupt(33,(DWORD)&irq1);
        IDT_SetHardwareInterrupt(34,(DWORD)&irq2);
        IDT_SetHardwareInterrupt(35,(DWORD)&irq3);
        IDT_SetHardwareInterrupt(36,(DWORD)&irq4);
        IDT_SetHardwareInterrupt(37,(DWORD)&irq5);
        IDT_SetHardwareInterrupt(38,(DWORD)&irq6);
        IDT_SetHardwareInterrupt(39,(DWORD)&irq7);

	// Set Exception Handler
        IDT_SetHardwareInterrupt(0x00,(DWORD)INT_00);
        IDT_SetHardwareInterrupt(0x01,(DWORD)INT_01);
        IDT_SetHardwareInterrupt(0x02,(DWORD)INT_02);
        IDT_SetHardwareInterrupt(0x03,(DWORD)INT_03);
        IDT_SetHardwareInterrupt(0x04,(DWORD)INT_04);
        IDT_SetHardwareInterrupt(0x05,(DWORD)INT_05);
        IDT_SetHardwareInterrupt(0x06,(DWORD)INT_06);
        IDT_SetHardwareInterrupt(0x07,(DWORD)INT_07);
        IDT_SetHardwareInterrupt(0x08,(DWORD)INT_08);
        IDT_SetHardwareInterrupt(0x09,(DWORD)INT_09);
        IDT_SetHardwareInterrupt(0x0A,(DWORD)INT_0A);
        IDT_SetHardwareInterrupt(0x0B,(DWORD)INT_0B);
        IDT_SetHardwareInterrupt(0x0C,(DWORD)INT_0C);
        IDT_SetHardwareInterrupt(0x0D,(DWORD)INT_0D);
        IDT_SetHardwareInterrupt(0x0E,(DWORD)INT_0E);
        IDT_SetHardwareInterrupt(0x0F,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x10,(DWORD)INT_10);
        IDT_SetHardwareInterrupt(0x11,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x12,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x13,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x14,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x15,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x16,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x17,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x18,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x19,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x1A,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x1B,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x1C,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x1D,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x1E,(DWORD)INT_0F);
        IDT_SetHardwareInterrupt(0x1F,(DWORD)INT_0F);

        idt_desc.size=(256*8)-1;
        idt_desc.offset=((DWORD)idt) + KERNEL_CODE_START;	// 0x100000;
        //Set new IDT 
        asm __volatile__ ("lidt _idt_desc");
}

