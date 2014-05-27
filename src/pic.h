/*
 * pic.h
 *
 */

#ifndef PIC_H_
#define PIC_H_

#include <stddef.h>

#define PIC1		(0x20)
#define PIC2		(0xA0)

#define PIC1_CMD	(PIC1)
#define PIC1_DATA	(PIC1 + 1)
#define PIC2_CMD	(PIC2)
#define PIC2_DATA	(PIC2 + 1)

#define PIC_CMD_INIT	(0x10)
#define PIC_CMD_EOI		(0x20)	/* End Of Interrupt command */
#define PIC_CMD_DISABLE	(0xFF)

/* FIXME: This needs more documentation.
 * Future me, sorry for these confusing '#define's. I don't fully
 * understand them either.
 */
#define PIC_ICW1_ICW4_AVAILABLE	(0x01)
#define PIC_ICW1_SINGLE_MODE	(0x02)
#define PIC_ICW1_INTERVAL4		(0x04)
#define PIC_ICW1_LEVEL			(0x08)

#define PIC_ICW4_8086_MODE		(0x01)
#define PIC_ICW4_AUTOEOI		(0x02)
#define PIC_ICW4_BUF_SLAVE		(0x08)
#define PIC_ICW4_BUF_MASTER		(0x0C)
#define PIC_ICW4_SFNM			(0x10)

int pic_disable();

int pic_mask_irqs();
int pic_mask_irq(u32 irq);
int pic_unmask_irq(u32 irq);

int pic_eoi(u32 irq);
int pic_remap(u32 offset_master, u32 offset_slave);

#endif /* PIC_H_ */
