/*
 * raw_video.c
 *
 *  Created on: 22 kwi 2014
 *      Author: Mateusz
 */

#include <defs.h>
#include <string.h>
#include <vga.h>

#include "raw_video.h"

#define ASSERT_INIT()		if (video_ptr == NULL) return E_NO

static void _init_cursor();

static u16* video_ptr = NULL;
static u16* video_base = NULL;

static u32 video_cols = 0;
static u32 video_rows = 0;

static u8 video_attr = RVID_ATTR(COLOR_LGRAY, COLOR_BLACK);

int rvid_init(void* base, int width, int height)
{
	if (base == NULL) {
		base = DEFAULT_VIDEO_BASE;
	}

	_init_cursor();

	video_ptr = video_base = (u16*)base;
	video_cols = width;
	video_rows = height;

	return S_OK;
}

int rvid_clrscr()
{
	ASSERT_INIT();
	asm volatile("repz stosw"
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

	return S_OK;
}

int rvid_putchar(u8 c)
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
		*video_ptr++ = video_attr << 8 | c;
		break;
	}

	return c;
}

int rvid_puts(const char* s)
{
	ASSERT_INIT();
	while (rvid_putchar(*s++) != 0) ;

	return S_OK;
}

void _init_cursor()
{
	// We disable cursor
	u8 csr = vga_read_crt(VGA_CRT_CSR);
	vga_write_crt(VGA_CRT_CSR, csr | VGA_CSR_CD);
}
