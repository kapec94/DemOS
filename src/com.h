/*
 * com.h
 *
 */

#ifndef COM_H_
#define COM_H_

#include <stddef.h>

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

#define COM_LCR_STOP_BIT	2
#define COM_LCR_PAR1_BIT	3
#define COM_LCR_PAR2_BIT	4
#define COM_LCR_PAR3_BIT	5
/* When this bit is set, offsets 0 and 1 are mapped to the low and high bytes
 * of the Divisor register for setting the baud rate of the port. When this
 * bit is clear, offsets 0 and 1 are mapped to their normal registers. */
#define COM_LCR_DLAB_BIT	7

/* receive buffer full (data available) */
#define COM_LSR_RBF_BIT		0
/* transmitter empty (new transmission can be started) */
#define COM_LSR_TEMT_BIT	6

/* This defines, how many bits of data will be sent in one character. Set this
 * value by writing to the two least significant bits of the LCR */
#define COM_DATA_LEN_5	(0b00)
#define COM_DATA_LEN_6	(0b01)
#define COM_DATA_LEN_7	(0b10)
#define COM_DATA_LEN_8	(0b11)

/* This defines how many bits will be used as an end to the character.
 * bit 2 of LCR.
 */
#define COM_STOP_ONE	(0b00)
#define COM_STOP_MORE	(0b01) /* 1.5 for DATA_LEN == 5, 2 otherwise */

/* Parity bits. */
#define COM_PARITY_NONE	(0b000)		/* no parity bit */
#define COM_PARITY_ODD	(0b001)		/* makes character sum odd */
#define COM_PARITY_EVEN	(0b011)		/* makes character sum even */
#define COM_PARITY_MARK	(0b101)		/* always 1 */
#define COM_PARITY_SPACE (0b111)	/* always 0 */

void com_get_ports(u16* out, size_t* len);
int com_init_port(u16 port, u32 baud, u8 data_len, u8 stop_bit, u8 parity);

inline int com_data_available(u16 port);
inline int com_transmitter_empty(u16 port);

inline u8 com_data_read(u16 port);
inline void com_data_write(u16 port, u8 data);

#endif /* COM_H_ */
