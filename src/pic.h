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

#define PIC_CMD_EOI		(0x20)	/* End Of Interrupt command */
#define PIC_CMD_DISABLE	(0xFF)

int pic_init();
int pic_disable();

int pic_send_eoi(u32 irq);

int pic_remap(u32 offset_master, u32 offset_slave);


#endif /* PIC_H_ */
