/*
 * conio.h
 *
 */

#ifndef CONIO_H_
#define CONIO_H_

#include <stdarg.h>

int kbhit();
int ungetch(int c);
int getch();
int getche();
int putch(int c);
char* cgets(char* buffer);
int cprintf(const char* format, ...);
int cputs(const char* str);
int cscanf(const char* format, ...);

int vcprintf(const char* format, va_list va);
int vcscanf(const char* format, va_list va);

#endif /* CONIO_H_ */
