/*
 * cpu.h - various CPU routines
 *
 */

#ifndef CPU_H_
#define CPU_H_

#include <stddef.h>

#define get_bit(m, n) ((m & (1 << n)) >> n)
#define set_bit(m, n) (m |= (1 << n))
#define clear_bit(m, n) (m &= ~(1 << n))
#define toggle_bit(m, n) (m ^= (1 << n))

static inline void out(u16 port, u8 val)
{
	__asm__ volatile("out %0, %1"
		: /* no output operands */
		: "a" (val), "d" (port)
		: );
}

static inline u8 in(u16 port)
{
	u8 ret = 0;
	__asm__ volatile("in %1, %0"
		: "=a" (ret)
		: "d" (port)
		:);
	return ret;
}

static inline void hlt()
{
	__asm__ volatile("hlt");
}

void cpu_setup_gdt();

#endif /* CPU_H_ */
