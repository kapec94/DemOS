/*
 * interrupts.h - Interrupt procedures declarations
 *
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <stddef.h>

#define ISR(name) _isr_ ## name
#define DECL_HANDLER(name) \
	void name(); \
	void ISR(name)()
#define EXCEPTION_COUNT	0x10

struct int_entry_t {
	int i;
	void* base;
};

typedef void(*int_proc)(void);

DECL_HANDLER(dummy_handler);
DECL_HANDLER(int0_div_error);
DECL_HANDLER(int3_breakpoint);
DECL_HANDLER(int13_triple_fault);

DECL_HANDLER(int_timer);
DECL_HANDLER(int_keyboard);

extern struct int_entry_t exception_handlers[];
extern struct int_entry_t interrupt_handlers[];

#endif /* INTERRUPTS_H_ */
