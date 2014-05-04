/*
 * gdt.h
 *
 */

#ifndef GDT_H_
#define GDT_H_

#include <stddef.h>

/* VALUES FOR gdt_access_byte FUNCTION */
/* readwrite argument */
#define GDT_AB_CODE_READABLE	1
#define GDT_AB_CODE_EXECUTABLE	0
#define GDT_AB_DATA_WRITABLE	1
#define GDT_AB_DATA_READABLE	0

/* direction argument */
#define GDT_AB_EXPAND_UP		0
#define GDT_AB_EXPAND_DOWN		1
#define GDT_AB_NONCONFORMING	0
#define GDT_AB_CONFORMING		1

/* type argument */
#define GDT_AB_CODE_SEGMENT		1
#define GDT_AB_DATA_SEGMENT		0

/* privilege argument */
#define GDT_AB_PRIV_KERNEL		0
#define GDT_AB_PRIV_DRIVER1		1
#define GDT_AB_PRIV_DRIVER2		2
#define GDT_AB_PRIV_USER		3

/* VALUES FOR gdt_flags FUNCTION */
#define GDT_FLAGS_GRANURALITY	1
#define GDT_FLAGS_NO_GRANURALITY 0
#define GDT_FLAGS_16PM			0
#define GDT_FLAGS_32PM			1

struct gdt_descriptor_t {
	u16		size;
	void*	offset;
} __attribute__((packed));

struct gdt_entry_t {
	u16		limit0;
	u16		base0;
	u8		base1;
	u8		access;
	u8		limit1 : 4;
	u8		flags : 4;
	u8		base2;
} __attribute__((packed));

struct gdt_access_byte_t {
	u8		accessed : 1;
	u8		readwrite : 1;
	u8		direction : 1;
	u8		executable : 1;
	u8		reserved : 1;
	u8		privl : 2;
	u8		present : 1;
} __attribute__((packed));

struct gdt_entry_t* gdt_pack_entry(void* base, u32 limit, u32 access_byte,
		u32 flags, struct gdt_entry_t* out);
struct gdt_entry_t* gdt_null_entry(struct gdt_entry_t* out);

struct gdt_descriptor_t* gdt_descriptor(void* offset, u32 size,
		struct gdt_descriptor_t* out);

u32 gdt_access_byte(u32 readwrite, u32 direction, u32 type,
		u32 privilege);
u32 gdt_flags(u32 granularity, u32 size);

#endif /* GDT_H_ */
