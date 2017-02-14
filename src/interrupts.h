/*
 * interrupts.h - Interrupt procedures declarations
 *
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <stddef.h>

#define ISR(name) __isr_ ## name
#define DECL_HANDLER(name) \
	void name(); \
	void ISR(name)()
#define EXCEPTION_COUNT	0x10

struct int_entry_t {
	int i;
	void* base;
};

typedef void(*int_proc)(void);

void interrupts_init(struct idt_entry_t* global_interrupts_table);
void interrupts_setup(size_t i, void* base);

DECL_HANDLER(dummy_handler);
DECL_HANDLER(int0_div_error);
DECL_HANDLER(int3_breakpoint);
DECL_HANDLER(int13_triple_fault);

DECL_HANDLER(int_keyboard);

extern struct int_entry_t exception_handlers[];
extern struct int_entry_t interrupt_handlers[];

#endif /* INTERRUPTS_H_ */
