#--------------------------------------------------------
#			M Y L O S
#--------------------------------------------------------

CC=gcc
ASM=nasm

#KFLAG=-Wall -Wstrict-prototypes -O2 -fno-strength-reduce -fpack-struct
KFLAG=-Wall -Wstrict-prototypes -Wunused -O2 -fomit-frame-pointer -fno-strength-reduce -fpack-struct	\
	-Wno-write-strings -m32 

CFLAGS=-I kernel/include/ -I kernel/ddk/
	
KOBJ=kernel/driver/crt/crt.o kernel/driver/crt/crt2.o \
	kernel/driver/console/console.o	\
	kernel/driver/kbd/kbd.o kernel/driver/kbd/kbd2.o kernel/driver/kbd/kbd3.o \
	kernel/ddk/kbd_api.o	\
	kernel/core/gdt.o	kernel/core/idt.o \
	kernel/core/kmemory.o	kernel/core/kmem_end.o	\
	kernel/core/process.o	kernel/core/sched.o	kernel/core/switch.o \
	kernel/core/timer.o	kernel/core/traps.o \
	kernel/kernel.o

all: kernel start test
	@echo Everythins is o.k. now !  

rebuild: clean_all all

debug: kernel
	@echo Disassembling Kernel.com
	cd kernel
	ndisasm.exe -u -b 32 -a -i kernel.com > kernel.asm
#	@rem ndisasm.exe -u -b 32 -a -i \mylos\test.com > \mylos\test.asm
	cd ..

clean_all: clean_kernel clean_start
	@echo CLEAN ALL DONE  

clean: clean_kernel
	@echo CLEAN DONE  

clean_kernel:
	@echo Cleaning Kernels drivers
	rm -f kernel/driver/console/*.o
	rm -f kernel/driver/crt/*.o
	rm -f kernel/driver/kbd/*.o
	rm -f kernel/core/*.o
	rm -f kernel/ddk/*.o
	cd kernel && rm -f *.o *.com *.map *.asm
	@echo Cleaning Kernel files 
	@echo Kernel cleared

clean_start:
	@echo Cleaning Start files
	rm -f start/*.o
	@echo Start cleared

#--------------------------------------------------------
#			K E R N E L
#--------------------------------------------------------

kernel : kernel/kernel.com
	@echo Kernel was successfully builded

kernel/kernel.com: $(KOBJ) kernel/kernel.ld
	@echo Linking kernel.com
	#ORIGINAL MSDOS:
	#cd kernel ; ld --script=kernel.ld -e _ENTRY32__Fv -m elf_i386 --format binary -oformat binary -o kernel.com -s -S -Map kernel.map -Ttext 0 --warn-common
	cd kernel ; ld  -m i386linux --script=kernel.ld -e _ENTRY32__Fv --oformat=binary -o kernel.com -s -S -Map kernel.map -Ttext 0 --warn-common


#--------------------------------------------------------
#			S T A R T  
#--------------------------------------------------------

start: start/start.com
	echo Start is OK !

S_0INC= start/0gatea20.inc	start/0idt.inc		start/0pic8259.inc	\
	start/0pmode.inc	start/0video.inc	start/0gdt.inc		\
	start/highcopy.inc	start/video32.inc

S_1INC= start/1back.inc		start/1gatea20.inc	start/1pic8259.inc	\
	start/irq16.inc
S_DAT = start/debug32.dat	start/breeze.dat	start/gdt.dat		\
	start/highcopy.dat	start/idt.dat		start/debug16.dat	\
	start/tss32.dat
S_MAC = start/debug16.mac	start/debug32.mac	start/gdt.mac		\
	start/ports.mac		start/speaker.mac
S_HEAD= start/video32.h		start/gdt.h		start/idt.h		\
	start/ports.h		start/start.h

start/start.com: start/start.asm start/start32.inc $(S_0INC) $(S_1INC) $(S_DAT) $(S_DAT) $(S_MAC) $(S_HEAD)
	echo Compiling START loader
	rm start/start.com
	$(ASM) -f bin start.asm -o start.com -s -w+orphan-labels -w+macro-params -dSMALL
	cd ..

#--------------------------------------------------------
#			T E S T    
#--------------------------------------------------------

test: test.com

test.com: kernel/kernel.com start/start.com
	echo Creating Test.com
	copy /b start/start.com + kernel/kernel.com test.com

# ----------------------------------------------------------- KERNEL +  TEST

test_kernel: kernel test
	echo TEST KERNEL READY 

# ---------------------------------------------- KERNEL RULES

.c.o :
	echo  1. Compiling  $< into object $@
	$(CC) -c $< -o $@ $(KFLAG)

.cpp.o :
	echo  2. Compiling+ $< into object $@
	$(CC) -c $< $(KFLAG)

.asm.o :
	echo  3. Assembling $< into object $@
	$(ASM) $< -f coff -o $@

%.o: %.cpp
	@echo  4. Compiling+ $< into object $@
	$(CC) $(CFLAGS) -c $< -o $@ $(KFLAG)

%.o: %.asm
	@echo  5. Assembling $< into object $@
	$(ASM) $< -f coff -o $@

# ---------------------------------------------- KERNEL DEPENDENCIES

include makefile.dep

#
kernel/core/kmem_end.o :  kernel/core/kmem_end.asm
kernel/core/switch.o :  kernel/core/switch.asm
kernel/core/timer.o :  kernel/core/timer.asm
kernel/core/traps.o :  kernel/core/traps.asm
kernel/driver/crt/crt.o :  kernel/driver/crt/crt.asm
kernel/driver/kbd/kbd3.o :  kernel/driver/kbd/kbd3.asm
kernel/driver/kbd/kbd.o :  kernel/driver/kbd/kbd.asm

#

dep: 
	echo "# KERNEL DEPENDENCIES" > makefile.dep
	$(CC) $(CFLAGS) -M $(wildcard kernel/driver/console/*.cpp) >> makefile.dep
	$(CC) $(CFLAGS) -M $(wildcard kernel/driver/crt/*.cpp) >> makefile.dep
	$(CC) $(CFLAGS) -M $(wildcard kernel/driver/kbd/*.cpp) >> makefile.dep
	$(CC) $(CFLAGS) -M $(wildcard kernel/core/*.cpp) >> makefile.dep
	$(CC) $(CFLAGS) -M $(wildcard kernel/ddk/*.cpp) >> makefile.dep
	$(CC) $(CFLAGS) -M $(wildcard kernel/*.cpp)       >> makefile.dep

