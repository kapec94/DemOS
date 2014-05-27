/*
 * interrupts_isr.S - Interrupts Service Routines.
 *
 */

.macro isr handler
.global __isr_\handler
__isr_\handler:
	pusha
	call	_\handler
	popa
	iret
.endm

isr dummy_handler
isr int0_div_error
isr int3_breakpoint
isr int13_triple_fault

isr int_keyboard
