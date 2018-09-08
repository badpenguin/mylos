# MyLOS - My Little Operating System

![MyLOS](/doc/mylos.png?raw=true)

## The Story

In 1992 when I started my first year at the university I only knewed MSDOS, Assembler and Pascal.

so, when I first met Unix and "C" there at the University, I falled in love about how operating system works.

There were no internet available for me at that times: I was only to able to navigate via "gopher" at the University; and I purchased those books:
- [The C Programming Language: ANSI C Version](https://amzn.to/2M888uh) by Dennis M. Ritchie
- [Developing Your Own 32-Bit Operating System/Book and Cd-Rom](https://amzn.to/2wSOT2d) by RIchard A. Burgess

I wanted to test how time sharing scheduling via interrupt was working and to do this i did a little trick.

I created a kernel that could be launched on top of MSDOS written in assembler (NASM) and CPP (DJCPP) :)

Indeed MSDOS was running without HIGHMEM support, it only lives in the first 640KB of RAM.

While my kernel (kernel.o) could be merged with (start.o) into a DOS executable (test.com)
that takes control over MSDOS (switching to the so called "Protect Mode") and uses only the upper memory and its able to quit and
return politely to MSDOS.

So i learned a few stuffs from "OS2000" about x86 architecture and hardare: keyboard driver and maps, CRT fonts and
other video stuffs, interrupts, etc.

The kernel contains no shell, it execute 3 process:
- "IDLE" has priority 0, the lower, and its never schedule if there are other processes running;
- "CONSOLE" its a program that handle key and video, its used like a shell but you only have predefined keys that print debug infoes;
- "IDLE2" its a simple program that print a message but then does nothing but it gets scheduled togheter with "CONSOLE";

If there is no process to schedule the OS will "halt" the CPU until an interrupt will occour (however the timer is temporary disabled).

Every process is gived a minum "QUANTUM" of time it can run before it gets switched.


Keys managed by "CONSOLE":
- F2 print the process list with how many time the get schedule (SK_TIME) and how much CPU they used (CPU_TIME)

When the timer interrupt ticks it toggle a clock icon onto the display.

## Compiling

This used to work with MSDOS (run without HIMEM.SYS and EMM386.exe):
- djgpp v. 2 (gcc.exe)
- nasm.exe version 0.95 (or nasmw.exe from Win95)
- maker.exe or GNU make

The current Makefile has been ported from MSDOS and i was able to compile under Linux 64bit with gcc
but the linker (ld) does not seems to be happy. I'm trying to figure it out how to make this work again.

Meanwhile i've published this in the hope it can be usefull for some student.

# The original docs

## What is Mylos

Mylos is my try to make a little and working kernel using some advance feature that Windows 95 does not have (i.e. Full 32 bit Protect Mode). 

This is not indeed to be an OS more powerful than Linux or more nice than W95, this is only a try to learn many things:
- Protect Mode Programming
- NASM a native 32 bit assembler
- DJGPP, the port of GCC for DOS, a strong 32 bit "C" compiler
- some Hardware programming stuff
- MAKE and makefiles

I had a developed a little program REBUILD.EXE that increase a counter and display it on the screen. This is very usefull if used into a MAKEFILE to display the current "Rebuild Version" of your project.

## Basic Concepts

I will explain now the basic topics that i have adopted in the current developing status of mylos.
Since testing a kernel is a VERY VERY hard things (i.e. milions of reboots), the kernel have to start from DOS and return to it :-)

In order to satisfy the "point one", we simply start from plain dos (no himem.sys and no emm386.exe) then a loader will do all the initialization and then copy the kernel high (at 1MB), so the kernel is executed. After all, the control is returned to the loader, that can return safetly to DOS.

The loader is called START.COM, it is written entirely in assembler, with NASM, to be compact. Only when the entire OS will be ready than i could think of load from floppy or HD.

The kernel is kernel.com (until it will be under 64KB, after of that i will need a more complex loader), it is written entirely in C/C++ with DJGPP (the porting of gcc for DOS), and some drivers in NASM (so i can test Assembler/C++ programming interface).

The kernel will be kernel.bin when it will grow larger than 64KB

## Start.com

What does it now ?
- perform all the initialization: A20 Line, PIC, GDT, IDT and more
- jump in full protect mode
- copy the kernel at 1MB

The kernel is currently attached at the end of start.com to form kernel.com (until it will be less than 64kb)
in case of error provide a trace debug of the stack and of the registers
jump to the kernel and wait that it returns

## Kernel.com
What does it now ?
- kernel init some structures than load and/or initialize his device driver
- a CRT driver is avaiable to handle the characters screen
- a KEYBOARD handler/driver is avaiable, into the SYS-REQUEST key is now hardcoded a call to the function that return to DOS, very usefull when your task crash. The keyboard driver display now every rawcode you press, this is only for debugging
- two task are created from the kernel initialization: CONSOLE and IDLE
- The console display a menu', if you press a function key, you execute a function according to that menų. It can be used to display stats information about the allocated memory, the process created, the GDT, the IDT and more ...
- The IDLE does nothing, for now.
- The Kernel can now be used to write Text based application (but this is not my goal). At this point of the project you have full support for the Video and Keyboard API and task switching.
