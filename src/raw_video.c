/*
 * raw_video.c
 *
 *  Created on: 22 kwi 2014
 *      Author: Mateusz
 */

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <vga.h>

#include "raw_video.h"

#define ASSERT_INIT()		if (video_ptr == NULL) return E_NO

static volatile u16* video_ptr = NULL;
static u16* video_base = NULL;

static u32 video_cols = 0;
static u32 video_rows = 0;

static u16 video_attr = RVID_ATTR(COLOR_LGRAY, COLOR_BLACK);

static volatile int _cursor_enabled = 1;
void _cursor_update();
void _cursor_setpos(int x, int y);

int rvid_init(void* base, int width, int height)
{
	if (base == NULL) {
		base = DEFAULT_VIDEO_BASE;
	}

	video_ptr = video_base = (u16*)base;
	video_cols = width;
	video_rows = height;

	return S_OK;
}

int rvid_clrscr()
{
	ASSERT_INIT();
	__asm__ volatile("repz stosw"
			: /* no output */
			: "c" (video_cols * video_rows),
			  "D" (video_base),
			  "a" ((u16)(video_attr << 8 | ' '))
			:);

	return S_OK;
}

u16 rvid_getattr()
{
	return video_attr;
}

void rvid_setattr(u16 attr)
{
	video_attr = attr;
}

int rvid_getpos(int* x, int* y)
{
	ASSERT_INIT();
	u32 offset = video_ptr - video_base;

	if (x) {
		*x = offset % video_cols;
	}
	if (y) {
		*y = offset / video_cols;
	}

	return S_OK;
}

int rvid_setpos(int x, int y)
{
	ASSERT_INIT();
	u32 offset = y * video_cols + x;
	video_ptr = video_base + offset;

	_cursor_update();
	return S_OK;
}

int rvid_putchar(int c)
{
	ASSERT_INIT();

	int y;

	switch (c) {
	case '\r':
		rvid_getpos(NULL, &y);
		rvid_setpos(0, y);
		break;
	case '\n':
		rvid_getpos(NULL, &y);
		rvid_setpos(0, y + 1);
		break;
	case '\0':
		break;
	default:
		*video_ptr++ = video_attr << 8 | (u8)c;
		_cursor_update();
		break;
	}

	return c;
}

int rvid_puts(const char* s)
{
	ASSERT_INIT();
	int with_cursor = _cursor_enabled;

	if (with_cursor) {
		rvid_cursor_disable();
	}
	while (rvid_putchar(*s++) != 0) ;
	if (with_cursor) {
		rvid_cursor_enable();
	}

	return strlen(s);
}

void rvid_cursor_disable()
{
	u8 csr = vga_read_crt(VGA_CRT_CSR);
	set_bit(csr, VGA_CSR_CD);
	vga_write_crt(VGA_CRT_CSR, csr);
	_cursor_enabled = 0;
}

void rvid_cursor_enable()
{
	u8 csr = vga_read_crt(VGA_CRT_CSR);
	clear_bit(csr, VGA_CSR_CD);
	vga_write_crt(VGA_CRT_CSR, csr);
	_cursor_enabled = 1;
}

void _cursor_update()
{
	int x, y;
	rvid_getpos(&x, &y);

	_cursor_setpos(x, y);
}

void _cursor_setpos(int x, int y)
{
	int pos = y * video_cols + x;

	vga_write_crt(VGA_CRT_LOCL, pos & 0xFF);
	vga_write_crt(VGA_CRT_LOCH, (pos >> 8) & 0xFF);
}
