/*
 * vga.c
 *
 */

#include <stddef.h>
#include <cpu.h>
#include "vga.h"

static inline void _get_crt_reg(u16* addr, u16* data)
{
	// http://www.osdever.net/FreeVGA/vga/extreg.htm#3CCR3C2W
	if (in(VGA_REG_MISCR) & VGA_IOAS) {
		*addr = VGA_REG_CRTA_1;
		*data = VGA_REG_CRTD_1;
	} else {
		*addr = VGA_REG_CRTA_0;
		*data = VGA_REG_CRTD_0;
	}
}

// CRT registers - address and data
static u16 _crta = 0;
static u16 _crtd = 0;

int vga_init()
{
	_get_crt_reg(&_crta, &_crtd);
	return S_OK;
}

u8 vga_read_crt(u8 index)
{
	// we save the old index to restore it later
	u8 old = in(_crta);
	// output new index
	out(_crta, index);
	// read data
	u8 data = in(_crtd);
	// ...
	out(_crta, old);
	// profit
	return data;
}

void vga_write_crt(u8 index, u8 val)
{
	// we save the old index to restore it later
	u8 old = in(_crta);
	// output new index
	out(_crta, index);
	// write data
	out(_crtd, val);

	out(_crta, old);
}
