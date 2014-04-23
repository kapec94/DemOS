/*
 * string.h - Standard string library
 *
 */

#ifndef STRING_H_
#define STRING_H_

#include <defs.h>

void* memset(void* ptr, int value, size_t num);
void* memcpy(void* dest, const void* src, size_t num);

#endif /* STRING_H_ */
