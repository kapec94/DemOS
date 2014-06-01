/*
 * conio.c
 *
 */

#include <stdarg.h>
#include <stddef.h>

#include <raw_video.h>
#include <keyboard.h>

#include "conio.h"

#define BUFFER_SIZE	1024

static int _vsnprintf(char* out, int n, const char* format, va_list va);
static int _isfmt(char c);
static int _sputc(char* out, int n, int c);
static int _sputs(char* out, int n, const char* in);
static int _sputi(char* out, int n, int i, int radix, int sign);
static int _sputu(char* out, int n, unsigned u, int radix);

static char _buffer[BUFFER_SIZE];
int vcprintf(const char* format, va_list va)
{
	int chars = _vsnprintf(_buffer, BUFFER_SIZE, format, va);
	int c = 0;

	if (chars < BUFFER_SIZE) {
		_buffer[chars] = '\0';
		rvid_puts(_buffer);
	} else {
		c = _buffer[BUFFER_SIZE - 1];
		_buffer[BUFFER_SIZE - 1] = '\0';
		rvid_puts(_buffer);
		rvid_putchar(c);
	}

	return chars + 1;
}

int cprintf(const char* format, ...)
{
	int chars = 0;
	va_list va;

	va_start(va, format);
	chars = vcprintf(format, va);
	va_end(va);

	return chars;
}

int putch(int c)
{
	rvid_putchar(c);
	return c;
}

int cputs(const char* str)
{
	rvid_puts(str);
	return 0;
}

static volatile int _chbuf;
int getch()
{
	int c = 0;
	if (_chbuf != '\0') {
		c = _chbuf;
		_chbuf = '\0';
	} else {
		c = kbd_getch();
	}
	return c;
}

int getche()
{
	int c = getch();
	putch(c);
	return c;
}

int ungetch(int c)
{
	_chbuf = c;
	return c;
}

int _vsnprintf(char* out, int n, const char* f, va_list va)
{
	int chars = 0;
	int print_format;

	while (*f != 0 && n > 0) {
		print_format = 0;

		if (*f == '%') {
			print_format = 1;
			while (!_isfmt(*++f)) {
			}
		}

		size_t printed = 0;
		if (print_format) {
			switch (*f) {
			case 's':
				printed = _sputs(out, n, va_arg(va, const char*));
				break;
			case 'c':
				printed = _sputc(out, n, va_arg(va, int));
				break;
			case 'd':
				printed = _sputi(out, n, va_arg(va, int), 10, 0);
				break;
			case 'o':
				printed = _sputu(out, n, va_arg(va, unsigned), 8);
				break;
			case 'x':
				printed = _sputu(out, n, va_arg(va, unsigned), 16);
				break;
			}
			n -= printed;

			chars += printed;
			out += printed;

			++f;
			print_format = 0;
		}
		else {
			printed = _sputc(out, n, *f++);
			out += printed;
			chars += printed;
			n -= printed;
		}
	}
	return chars;

}

int _isfmt(char c)
{
	switch (c) {
	case 's':
	case 'd':
	case 'x':
	case 'o':
	case 'c':
		return 1;
	default:
		return 0;
	}
}

int _sputc(char* out, int n, int c)
{
	if (c == '\0' || n <= 0) return 0;
	else {
		*out = (u8)c;
		return 1;
	}
}

int _sputs(char* out, int n, const char* in)
{
	if (n <= 0) return 0;

	int chars = 0;
	while (*in != '\0' && n > 1) {
		*out++ = *in++;
		n--;
		chars++;
	};

	out[chars] = '\0';
	return chars;
}

char* digits = "0123456789ABCDEF";
int _sputi(char* out, int n, int i, int radix, int sign)
{
	if (radix > 16 || radix < 2 || n <= 0) return 0;
	size_t chars = 0;

	if (sign && i >= 0) {
		chars += _sputc(out, n, '+');
	}
	if (i < 0) {
		i = -i;
		chars += _sputc(out, n, '-');
	}

	return chars + _sputu(out + chars, n - chars, (unsigned)i, radix);
}

int _sputu(char* out, int n, unsigned u, int radix)
{
	if (radix > 16 || radix < 2 || n <= 0) return 0;

	size_t chars = 0;
	unsigned j = 1, m = 0;

	if (u == 0) {
		_sputc(out, n, '0');
		chars ++;
		out ++;
	} else {
		/* We find out, how big is this number */
		while (u / j >= radix) {
			j *= radix;
		}

		while (j != 0) {
			m = u / j;
			u = u % j;
			j /= radix;

			_sputc(out, n, digits[m]);
			chars ++;
			out ++;
		}
	}

	return chars;
}
