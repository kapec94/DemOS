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

static void _init_cursor();
static size_t _puti(int i, int radix, int sign);
static size_t _putu(unsigned u, int radix);

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

	return strlen(s);
}

int rvid_vprintf(const char* f, va_list va)
{
	int chars = 0;
	while (*f != 0) {
		if (*f == '%') {
			switch (*++f) {
			case 's':
				chars += rvid_puts(va_arg(va, const char*));
				break;
			case 'i':
			case 'd':
				chars += _puti(va_arg(va, int), 10, 0);
				break;
			case 'o':
				chars += _putu(va_arg(va, unsigned), 8);
				break;
			case 'x':
				chars += _putu(va_arg(va, unsigned), 16);
				break;
			}
			++f;
		}
		else {
			rvid_putchar(*f++);
			chars++;
		}
	}
	return chars;
}

int rvid_printf(const char* f, ...)
{
	int chars = 0;
	va_list va;
	va_start(va, f);

	chars = rvid_vprintf(f, va);

	va_end(va);
	return chars;
}

void _init_cursor()
{
	/* We disable cursor */
	u8 csr = vga_read_crt(VGA_CRT_CSR);
	vga_write_crt(VGA_CRT_CSR, csr | VGA_CSR_CD);
}

char* digits = "0123456789ABCDEF";
size_t _puti(int i, int radix, int sign)
{
	if (radix > 16 || radix < 2) return 0;

	size_t chars = 0;

	if (sign && i >= 0) {
		rvid_putchar('+');
		chars++;
	}
	if (i < 0) {
		i = -i;
		rvid_putchar('-');
		chars++;
	}

	return chars + _putu((unsigned)i, radix);
}

size_t _putu(unsigned u, int radix)
{
	if (radix > 16 || radix < 2) return 0;

	size_t chars = 0;
	unsigned j = 1, n = 0;

	if (u == 0) {
		rvid_putchar('0');
		chars++;
	} else {
		/* We find out, how big is this number */
		while (u / j > radix) {
			j *= radix;
		}

		while (u != 0) {
			n = u / j;
			u = u % j;
			j /= radix;

			rvid_putchar(digits[n]);
			chars ++;
		}
	}

	return chars;
}
