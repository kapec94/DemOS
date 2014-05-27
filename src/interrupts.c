/*
 * interrupts.c
 *
 */

#include <cpu.h>
#include <raw_video.h>
#include "interrupts.h"

struct int_entry_t exception_handlers[] = {
		{ i: 0, base: ISR(int0_div_error) },
		{ i: 3, base: ISR(int3_breakpoint) },
		{ i: 13, base: ISR(int13_triple_fault) },
		{ i: -1, base: NULL }
};

struct int_entry_t interrupt_handlers[] = {
		{ i: 0, base: ISR(int_timer) },
		{ i: 1, base: ISR(int_keyboard) },
		{ i: -1, base: NULL }
};

void dummy_handler()
{

}

void int0_div_error()
{
	rvid_printf("Division error!\n");

	cli();
	hlt();
}

void int3_breakpoint()
{
	rvid_printf("int3 handler!\n");
}

void int13_triple_fault()
{
	rvid_printf("TRIPLE FAULT\n");
	cli();
	hlt();
}

void int_timer()
{
	rvid_printf("TIMER\n");
}

void int_keyboard()
{
	rvid_printf("KEYBOARD\n");
}
