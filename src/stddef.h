/*
 * stddef.h
 *
 */

#ifndef STDDEF_H_
#define STDDEF_H_

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long 	u64;

typedef u32					size_t;

typedef u16					wint_t;

#define NULL	((void*)0)

#ifdef __STRICT_ANSI__
#define inline __inline__
#endif /* __STRICT_ANSI__ */

#define S_OK	0
#define E_NO	1
#define E_VAL	2

#endif /* STDDEF_H_ */
