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
