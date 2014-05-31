/*
 * string.h - Standard string library
 *
 */

#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

size_t strlen(const char* s);
char* strcpy(char* dest, const char* src);
int strcmp(const char* str1, const char* str2);

void* memset(void* ptr, int value, size_t num);
void* memcpy(void* dest, const void* src, size_t num);

#endif /* STRING_H_ */
