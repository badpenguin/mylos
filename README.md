# MyLOS - My Little Operating System

![MyLOS](/doc/mylos.png?raw=true)

## The Story

In 1992 when I started my first year at the university I only knewed MSDOS, Assembler and Pascal.

so, when I first met Unix and "C" there at the University, I falled in love about how operating system works.

I wanted to test how time sharing scheduling via interrupt was working and to do this i did a little trick.

I created a kernel that could be launched on top of MSDOS :)

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

