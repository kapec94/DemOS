/*
 * com.c - serial port driver
 *
 */

#include <cpu.h>
#include "com.h"

static inline void _set_reg(u16 port, u8 offset, u8 val);
static inline u8 _get_reg(u16 port, u8 offset);
static void _set_baud_divisor(u16 port, u32 div);

static u32 _base_baud = 115200;

int com_init_port(u16 port, u32 baud)
{
	u16 div = 0;

	if (port != COM_PORT1 && port != COM_PORT2) {
		// Invalid port address.
		return E_NO;
	}

	div = _base_baud / baud;
	if (div * baud != _base_baud) {
		// baud is not a divider of base baud rate.
		return E_VAL;
	}

	_set_baud_divisor(port, div);

	return S_OK;
}

u8 _get_reg(u16 port, u8 offset)
{
	return in(port + offset);
}

void _set_reg(u16 port, u8 offset, u8 val)
{
	out(port + offset, val);
}

void _set_baud_divisor(u16 port, u32 div)
{
	u8 divl = (u8)div;
	u8 divh = (u8)(div >> 8);
	u8 lcr = _get_reg(port, COM_REG_LCR);
	_set_reg(port, COM_REG_LCR, lcr | COM_DLAB_BIT);

	_set_reg(port, COM_REG_BAUDL, divl);
	_set_reg(port, COM_REG_BAUDH, divh);

	_set_reg(port, COM_REG_LCR, lcr);
}


