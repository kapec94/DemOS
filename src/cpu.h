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

#define out(port, val) \
	__asm__ volatile("out %0, %1" \
		: /* no output operands */ \
		: "a" ((u8)(val)), "d" ((u16)(port)) \
		: )

/* we out to so-called 'null port' */
#define io_wait() out(0x80, 0x00)

/* It has to be a function */
static inline u8 in(u32 port)
{
	u8 ret = 0;
	__asm__ volatile("in %1, %0"
		: "=a" (ret)
		: "d" ((u16)port)
		:);
	return ret;
}

static inline u32 getticks()
{
	u32 ret;
	__asm__ volatile("rdtsc"
		: "=A" (ret));
	return ret;
}

#define sti() __asm__ volatile("sti")
#define cli() __asm__ volatile("cli")

#define nop() __asm__ volatile("nop")
#define hlt() __asm__ volatile("hlt")
/* Undefined instruction. It is meant to crash the kernel. */
#define ud2() __asm__ volatile("ud2")

#define lgdt(gdt) \
		__asm__ volatile("lgdt %0" \
				: /* no output */ \
				: "m" ((gdt)) \
				: /* no clobber */)

#define lidt(idt) \
		__asm__ volatile("lidt %0" \
				: /* no output */ \
				: "m" ((idt)) \
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
