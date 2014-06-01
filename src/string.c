/*
 * string.c - Standard string library implementation.
 *
 */

#include <string.h>

size_t strlen(const char* s)
{
	size_t i = 0;
	while (s[i++] != 0);
	return i;
}

char* strcpy(char* dest, const char* source)
{
	while ((*dest++ = *source++)) ;
	return dest;
}

int strcmp(const char* str1, const char* str2)
{
	do {
		if (*str1 < *str2) {
			return 1;
		}
		if (*str1 > *str2) {
			return -1;
		}
	} while (*(str1++) && *(str2++));

	return 0;
}

static char* _strtok_init(char* str, const char* delim);
static int _char_in(int c, const char* delim);

static char* _current = NULL;
char* strtok(char* str, const char* delimiters)
{
	if (str) {
		_current = _strtok_init(str, delimiters);
		return _current;
	}
	if (!_current || !*_current) return NULL;

	char* begin = _current;
	for (; *_current != '\0'; _current++) {
		if (_char_in(*_current, delimiters)) {
			*_current++ = '\0';
			break;
		}
	}

	return begin;
}

static char* _strtok_init(char* str, const char* delim)
{
	while (*str != '\0' && _char_in(*str, delim)) {
		str++;
	}
	return str;
}

static int _char_in(int c, const char* delim)
{
	for (; *delim != '\0'; delim++) {
		if (c == *delim)
			return 1;
	}
	return 0;
}

void* memset(void* ptr, int value, size_t num)
{
	__asm__("repz stosb"
			: /* no output */
			: "a" (value), "D" (ptr), "c" (num)
			: /* no clobber */);
	return ptr;
}

void* memcpy(void* dest, const void* src, size_t num)
{
	__asm__("repz movsb"
			: /* no output */
			: "S" (src), "D" (dest), "c" (num)
			: /* no clobber */);
	return dest;
}
