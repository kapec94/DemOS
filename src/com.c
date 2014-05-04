/*
 * com.c - serial port driver
 *
 */

#include <cpu.h>
#include <bda.h>

#include "com.h"

static inline void _set_reg(u16 port, u8 offset, u8 val);
static inline u8 _get_reg(u16 port, u8 offset);

static u32 _base_baud = 115200;

int com_init_port(u16 port, u32 baud, u8 data_len, u8 stop_bit, u8 parity)
{
	u16 div = 0, lcr = 0;

	div = _base_baud / baud;
	if (div * baud != _base_baud) {
		/* baud is not a divider of base baud rate. */
		return E_NO;
	}

	lcr = _get_reg(port, COM_REG_LCR);

	/* Setting baud divisor */
	_set_reg(port, COM_REG_LCR, lcr | COM_LCR_DLAB_BIT);

	_set_reg(port, COM_REG_BAUDL, (u8)div);
	_set_reg(port, COM_REG_BAUDH, (u8)(div >> 8));

	/* we'll clear DLAB bit in a while. */
	/* setting character data length (little hack-ish method) */
	lcr = ((lcr >> 2) << 2) | (data_len & 0b11);

	/* setting stop bits */
	if (stop_bit)
		set_bit(lcr, COM_LCR_STOP_BIT);
	else
		clear_bit(lcr, COM_LCR_STOP_BIT);

	/* setting parity bits */
	parity = parity & 0b111;
	clear_bit(lcr, COM_LCR_PAR1_BIT);
	clear_bit(lcr, COM_LCR_PAR2_BIT);
	clear_bit(lcr, COM_LCR_PAR3_BIT);

	if (parity != COM_PARITY_NONE) {
		set_bit(lcr, COM_LCR_PAR1_BIT);
	}

	switch(parity) {
	case COM_PARITY_ODD:
		break;
	case COM_PARITY_EVEN:
		set_bit(lcr, COM_LCR_PAR2_BIT);
		break;
	case COM_PARITY_MARK:
		set_bit(lcr, COM_LCR_PAR3_BIT);
		break;
	case COM_PARITY_SPACE:
		set_bit(lcr, COM_LCR_PAR2_BIT);
		set_bit(lcr, COM_LCR_PAR3_BIT);
		break;
	}

	_set_reg(port, COM_REG_LCR, lcr);
	return S_OK;
}

void com_get_ports(u16* out, size_t* len)
{
	u16 ports[4];
	u32 i = 0, j = 0;

	bda_query(BDA_COM_PORTS, sizeof(ports), &ports);
	while (i < 4) {
		if (ports[i] != 0) {
			out[j++] = ports[i];
		}
		i++;
	}
	*len = j;
}

int com_data_available(u16 port)
{
	return get_bit(_get_reg(port, COM_REG_LSR), COM_LSR_RBF_BIT);
}

int com_transmitter_empty(u16 port)
{
	return get_bit(_get_reg(port, COM_REG_LSR), COM_LSR_TEMT_BIT);
}

u8 com_data_read(u16 port)
{
	while (!com_data_available(port));
	return _get_reg(port, COM_REG_DATA);
}

void com_data_write(u16 port, u8 val)
{
	while (!com_transmitter_empty(port));
	_set_reg(port, COM_REG_DATA, val);
}

u8 _get_reg(u16 port, u8 offset)
{
	return in(port + offset);
}

void _set_reg(u16 port, u8 offset, u8 val)
{
	out(port + offset, val);
}
