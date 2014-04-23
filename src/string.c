/*
 * string.c - Standard string library implementation.
 *
 */

#include <string.h>

void* memset(void* ptr, int value, size_t num)
{
	u8* _ptr = (u8*)ptr;
	while (num > 0) {
		*(_ptr + (--num)) = (u8)value;
	}
	return ptr;
}
