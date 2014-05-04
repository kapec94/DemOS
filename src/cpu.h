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

#define hlt() __asm__ volatile("hlt")
/* Undefined instruction. It is meant to crash the kernel. */
#define ud2() __asm__ volatile("ud2")

#define lgdt(gdt) \
		__asm__ volatile("lgdt %0" \
				: /* no output */ \
				: "m" ((gdt)) \
				: /* no clobber */)
#define set_cs(cs) \
		__asm__ volatile( \
				"ljmp %0, $.fake_label\n\t" \
				".fake_label:" \
				: /* no output */ \
				: "n" (cs) \
				: /* no clobber */)

#define set_ds(ds) \
		__asm__ volatile( \
				"mov %0, %%eax\n\t" \
				"mov %%ax, %%ds\n\t" \
				"mov %%ax, %%es\n\t" \
				"mov %%ax, %%fs\n\t" \
				"mov %%ax, %%gs\n\t" \
				"mov %%ax, %%ss" \
				: /* no output */ \
				: "n" (ds) \
				: "eax")

#endif /* CPU_H_ */
