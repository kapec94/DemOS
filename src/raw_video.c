/*
 * raw_video.c
 *
 *  Created on: 22 kwi 2014
 *      Author: Mateusz
 */

#include <defs.h>
#include <string.h>

#include "raw_video.h"

#define ASSERT_INIT()		if (video_ptr == NULL) return E_NO

u16* video_ptr = NULL;
u16* video_base = NULL;

u32 video_cols = 0;
u32 video_rows = 0;

int rvid_init(void* base, int width, int height)
{
	if (base == NULL) {
		base = DEFAULT_VIDEO_BASE;
	}

	video_ptr = video_base = (u16*)base;
	video_cols = width;
	video_rows = height;

	return E_OK;
}

int rvid_getpos(int* x, int* y)
{
	ASSERT_INIT();
	u32 offset = (u32)video_ptr - (u32)video_base;

	if (x) {
		*x = offset % video_cols;
	}
	if (y) {
		*y = offset / video_rows;
	}

	return E_OK;
}

int rvid_setpos(int x, int y)
{
	ASSERT_INIT();
	video_ptr = video_base + y * video_cols + x;

	return E_OK;
}

int rvid_movepos(int x, int y)
{
	ASSERT_INIT();
	int offset = (u32)video_ptr - (u32)video_base;
	offset += y * video_cols + x;

	if (offset < 0) return E_NO;

	video_ptr = video_base + offset;
	return E_OK;
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
		rvid_putchar('\r');
		rvid_movepos(0, 1);
		break;
	default:
		*video_ptr++ = (0x07 << 8) | c; // Light-gray on black
		break;
	}

	return c;
}

int rvid_puts(const char* s)
{
	ASSERT_INIT();
	while (rvid_putchar(*s++) != 0) ;

	return E_OK;
}

int rvid_clrscr()
{
	ASSERT_INIT();
	memset(video_base, 0, video_cols * video_rows);

	return E_OK;
}
