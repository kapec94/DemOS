/*
 * cpu.c - various CPU routines
 *
 *  Created on: 22 kwi 2014
 *      Author: Mateusz
 */


#include <defs.h>

void out(u16 port, u8 val)
{
	__asm__("out %%al, %%dx"
		: /* no output operands */
		: "d"(port), "a" (val)
		: "%edx", "%eax");
}

u8 in(u16 port)
{
	u8 ret = 0;
	__asm__("in %1, %0"
		: "=a"(ret)
		: "d"(port)
		: "%edx", "%eax");
	return ret;
}
