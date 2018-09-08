/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
/* Copyright (c) 1995 DJ Delorie.  Permission granted to use for any
   purpose, provided this copyright remains attached and unmodified.

   THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ		Far Pointer Simulation Functions			บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

The farpoke* and farpeek* take selectors.
The farnspoke* and farnspeek* don't (note the `ns' for `no selector').

Warning: These routines all use the %fs register for their accesses. 
GCC normally uses only %ds and %es, and libc functions (movedata,
dosmemget, dosmemput) use %gs.  Still, you should be careful about
assumptions concerning whether or not the value you put in %fs will be
preserved across calls to other functions.  If you guess wrong, your
program will crash.  Better safe than sorry. 

*/

#ifndef __dj_include_sys_farptr_h_
#define __dj_include_sys_farptr_h_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __dj_ENFORCE_ANSI_FREESTANDING

#ifndef __STRICT_ANSI__

#ifndef _POSIX_SOURCE

void _farpokeb(unsigned short, unsigned long, unsigned char);
void _farpokew(unsigned short, unsigned long, unsigned short);
void _farpokel(unsigned short, unsigned long, unsigned long);
unsigned char _farpeekb(unsigned short, unsigned long);
unsigned short _farpeekw(unsigned short, unsigned long);
unsigned long _farpeekl(unsigned short, unsigned long);
void _farsetsel(unsigned short);
unsigned short _fargetsel(void);
void _farnspokeb(unsigned long, unsigned char);
void _farnspokew(unsigned long, unsigned short);
void _farnspokel(unsigned long, unsigned long);
unsigned char _farnspeekb(unsigned long);
unsigned short _farnspeekw(unsigned long);
unsigned long _farnspeekl(unsigned long);

extern __inline__ void
_farpokeb(unsigned short selector,
	 unsigned long offset,
	 unsigned char value)
{
  __asm__ __volatile__ ("movw %w0,%%fs\n"
      "	.byte 0x64 \n"
      "	movb %b1,(%k2)"
      :
      : "rm" (selector), "qi" (value), "r" (offset));
}

extern __inline__ void
_farpokew(unsigned short selector,
	 unsigned long offset,
	 unsigned short value)
{
  __asm__ __volatile__ ("movw %w0,%%fs \n"
      "	.byte 0x64 \n"
      "	movw %w1,(%k2)"
      :
      : "rm" (selector), "ri" (value), "r" (offset));
}

extern __inline__ void
_farpokel(unsigned short selector,
	 unsigned long offset,
	 unsigned long value)
{
  __asm__ __volatile__ ("movw %w0,%%fs \n"
      "	.byte 0x64 \n"
      "	movl %k1,(%k2)"
      :
      : "rm" (selector), "ri" (value), "r" (offset));
}

extern __inline__ unsigned char
_farpeekb(unsigned short selector,
	 unsigned long offset)
{
  unsigned char result;
  __asm__ __volatile__ ("movw %w1,%%fs \n"
      "	.byte 0x64 \n"
      "	movb (%k2),%b0"
      : "=q" (result)
      : "rm" (selector), "r" (offset));
  return result;
}

extern __inline__ unsigned short
_farpeekw(unsigned short selector,
	 unsigned long offset)
{
  unsigned short result;
  __asm__ __volatile__ ("movw %w1, %%fs \n"
      "	.byte 0x64 \n"
      "	movw (%k2),%w0 \n"
      : "=r" (result)
      : "rm" (selector), "r" (offset));
  return result;
}

extern __inline__ unsigned long
_farpeekl(unsigned short selector,
	 unsigned long offset)
{
  unsigned long result;
  __asm__ __volatile__ ("movw %w1,%%fs\n"
      "	.byte 0x64\n"
      "	movl (%k2),%k0"
      : "=r" (result)
      : "rm" (selector), "r" (offset));
  return result;
}

extern __inline__ void
_farsetsel(unsigned short selector)
{
  __asm__ __volatile__ ("movw %w0,%%fs"
      :
      : "rm" (selector));
}

extern __inline__ unsigned short
_fargetsel(void)
{
  unsigned short selector;
  __asm__ __volatile__ ("movw %%fs,%w0 \n"
      : "=r" (selector)
      : );
  return selector;
}

extern __inline__ void
_farnspokeb(unsigned long offset,
	 unsigned char value)
{
  __asm__ __volatile__ (".byte 0x64\n"
      "	movb %b0,(%k1)"
      :
      : "qi" (value), "r" (offset));
}

extern __inline__ void
_farnspokew(unsigned long offset,
	 unsigned short value)
{
  __asm__ __volatile__ (".byte 0x64\n"
      "	movw %w0,(%k1)"
      :
      : "ri" (value), "r" (offset));
}

extern __inline__ void
_farnspokel(unsigned long offset,
	 unsigned long value)
{
  __asm__ __volatile__ (".byte 0x64\n"
      "	movl %k0,(%k1)"
      :
      : "ri" (value), "r" (offset));
}

extern __inline__ unsigned char
_farnspeekb(unsigned long offset)
{
  unsigned char result;
  __asm__ __volatile__ (".byte 0x64\n"
      "	movb (%k1),%b0"
      : "=q" (result)
      : "r" (offset));
  return result;
}

extern __inline__ unsigned short
_farnspeekw(unsigned long offset)
{
  unsigned short result;
  __asm__ __volatile__ (".byte 0x64\n"
      "	movw (%k1),%w0"
      : "=r" (result)
      : "r" (offset));
  return result;
}

extern __inline__ unsigned long
_farnspeekl(unsigned long offset)
{
  unsigned long result;
  __asm__ __volatile__ (".byte 0x64\n"
      "	movl (%k1),%k0"
      : "=r" (result)
      : "r" (offset));
  return result;
}

#endif /* !_POSIX_SOURCE */
#endif /* !__STRICT_ANSI__ */
#endif /* !__dj_ENFORCE_ANSI_FREESTANDING */

#ifndef __dj_ENFORCE_FUNCTION_CALLS
#endif /* !__dj_ENFORCE_FUNCTION_CALLS */

#ifdef __cplusplus
}
#endif

#endif /* !__dj_include_sys_farptr_h_ */

