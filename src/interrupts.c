/*
 * interrupts.c
 *
 */

#include <cpu.h>
#include <raw_video.h>
#include <gdt.h>
#include <pic.h>
#include <configuration.h>
#include "interrupts.h"

struct int_entry_t exception_handlers[] = {
		{ i: 0, base: ISR(int0_div_error) },
		{ i: 3, base: ISR(int3_breakpoint) },
		{ i: 13, base: ISR(int13_triple_fault) },
		{ i: -1, base: NULL }
};

struct int_entry_t interrupt_handlers[] = {
		{ i: 1, base: ISR(int_keyboard) },
		{ i: -1, base: NULL }
};

static struct idt_entry_t* _idt;

void interrupts_setup(size_t i, void* base)
{
	idt_pack_entry(base, 0x08,
			idt_type(IDT_GATE_32BIT_INT, PRIV_KERNEL),
			&_idt[i]);
}

void interrupts_init(struct idt_entry_t* global_interrupts_table)
{
	struct int_entry_t* int_entry;
	_idt = global_interrupts_table;

	int_entry = &exception_handlers[0];
	while (int_entry->i != -1) {
		interrupts_setup(int_entry->i, int_entry->base);
		int_entry ++;
	}

	int_entry = &interrupt_handlers[0];
	while (int_entry->i != -1) {
		interrupts_setup(int_entry->i + IRQ_OFFSET, int_entry->base);
		pic_unmask_irq(int_entry->i);
		int_entry ++;
	}
}

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

void int_keyboard()
{
	rvid_printf("KEYBOARD\n");
}
