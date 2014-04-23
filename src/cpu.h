/*
 * cpu.h - various CPU routines
 *
 */

#ifndef CPU_H_
#define CPU_H_

#include <defs.h>

static inline void out(u16 port, u8 val)
{
	asm volatile("out %0, %1"
		: /* no output operands */
		: "a" (val), "d" (port)
		: );
}

static inline u8 in(u16 port)
{
	u8 ret = 0;
	asm volatile("in %1, %0"
		: "=a" (ret)
		: "d" (port)
		:);
	return ret;
}

#endif /* CPU_H_ */
