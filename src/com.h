/*
 * com.h
 *
 */

#ifndef COM_H_
#define COM_H_

#include <defs.h>

#define COM_PORT1	(u16)(0x3F8)
#define COM_PORT2	(u16)(0x2F8)

/* Data register. Reading this registers read from the Receive buffer.
 * Writing to this register writes to the Transmit buffer. */
#define COM_REG_DATA	(0)
/* Interrupt Enable Register. */
#define	COM_REG_IER 	(1)
/* With DLAB set to 1, this is the least significant byte of the divisor value
 * for setting the baud rate. */
#define COM_REG_BAUDL	(0)
/* With DLAB set to 1, this is the most significant byte of the divisor
 * value. */
#define COM_REG_BAUDH	(1)
/* Interrupt Identification and FIFO control registers. */
#define COM_REG_IIR 	(2)
/* Line Control Register. The most significant bit of this register is the
 * DLAB. */
#define COM_REG_LCR 	(3)
/* Modem Control Register. */
#define COM_REG_MCR 	(4)
/* Line Status Register. */
#define COM_REG_LSR 	(5)
/* Modem Status Register. */
#define COM_REG_MSR 	(6)
/* Scratch Register. */
#define COM_REG_SR  	(7)

/* When this bit is set, offsets 0 and 1 are mapped to the low and high bytes
 * of the Divisor register for setting the baud rate of the port. When this
 * bit is clear, offsets 0 and 1 are mapped to their normal registers. */
#define COM_DLAB_BIT	(1 << 7)

int com_init_port(u16 port, u32 baud);

#endif /* COM_H_ */
