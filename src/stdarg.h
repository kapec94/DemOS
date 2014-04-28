/*
 * stdarg.h - variable arguments.
 * Wrapper around gcc builtins.
 *
 */

#ifndef STDARG_H_
#define STDARG_H_

/* Define __gnuc_va_list.  */

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif

typedef __gnuc_va_list va_list;

/* Define the standard macros for the user,
   if this invocation was from the user program.  */
#ifdef STDARG_H_

#define va_start(v,l)	__builtin_va_start(v,l)
#define va_end(v)		__builtin_va_end(v)
#define va_arg(v,l)		__builtin_va_arg(v,l)
#if !defined(__STRICT_ANSI__) || __STDC_VERSION__ + 0 >= 199900L
#define va_copy(d,s)	__builtin_va_copy(d,s)
#endif
#define __va_copy(d,s)	__builtin_va_copy(d,s)

#endif

#endif /* STDARG_H_ */
