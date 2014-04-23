/*
 * string.c - Standard string library implementation.
 *
 */

#include <string.h>

void* memset(void* ptr, int value, size_t num)
{
	asm("repz stosb"
			: /* no output */
			: "a" (value), "D" (ptr), "c" (num)
			: /* no clobber */);
	return ptr;
}

void* memcpy(void* dest, const void* src, size_t num)
{
	asm("repz movsb"
			: /* no output */
			: "S" (src), "D" (dest), "c" (num)
			: /* no clobber */);
	return dest;
}
