/*
 * pic.c
 *
 */

#include <cpu.h>
#include "pic.h"

int pic_remap(u32 offset_master, u32 offset_slave)
{
	/* Save current interrupt masks */
	u8 mask1 = in(PIC1_DATA);
	u8 mask2 = in(PIC2_DATA);

	out(PIC1_CMD, PIC_CMD_INIT | PIC_ICW1_ICW4_AVAILABLE);
	out(PIC2_CMD, PIC_CMD_INIT | PIC_ICW1_ICW4_AVAILABLE);

	out(PIC1_DATA, offset_master);
	out(PIC2_DATA, offset_slave);

	out(PIC1_DATA, 0b0100);		/* We tell master that there is a slave
								 * at 0b0100 */
	out(PIC2_DATA, 0b0010);		/* Tell slave its cascade identity.
								 * XXX: whatever it means... */

	out(PIC1_DATA, PIC_ICW4_8086_MODE);
	out(PIC2_DATA, PIC_ICW4_8086_MODE);

	/* Restore interrupt masks */
	out(PIC1_DATA, mask1);
	out(PIC2_DATA, mask2);

	return S_OK;
}

int pic_disable()
{
	out(PIC1_CMD, PIC_CMD_DISABLE);
	out(PIC2_CMD, PIC_CMD_DISABLE);

	return S_OK;
}

int pic_eoi(u32 irq)
{
	if (irq >= 8) {
		out(PIC2_CMD, PIC_CMD_EOI);
	}

	/* The EOI command must be sent to master PIC even if irq >= 8 */
	out(PIC1_CMD, PIC_CMD_EOI);

	return S_OK;
}

int pic_mask_irq(u32 irq)
{
	u16 port;
	u8 mask;

	if (irq >= 8) {
		irq -= 8;
		port = PIC2_DATA;
	} else {
		port = PIC1_DATA;
	}

	mask = in(port);
	set_bit(mask, irq);
	out(port, mask);

	return S_OK;
}

int pic_unmask_irq(u32 irq)
{
	u16 port;
	u8 mask;

	if (irq >= 8) {
		irq -= 8;
		port = PIC2_DATA;
	} else {
		port = PIC1_DATA;
	}

	mask = in(port);
	clear_bit(mask, irq);
	out(port, mask);

	return S_OK;
}

int pic_mask_irqs()
{
	int i;
	for (i = 0; i < 0x10; i++) {
		pic_mask_irq(i);
	}
	return S_OK;
}
