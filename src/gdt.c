/*
 * gdt.c
 *
 */

#include "gdt.h"

struct gdt_entry_t* gdt_pack_entry(void* base_ptr, u32 limit, u32 access_byte,
		u32 flags, struct gdt_entry_t* out)
{
	u32 base = (u32)base_ptr;

	out->limit0	= (u16)(limit & 0xFFFF);
	out->base0	= (u16)(base & 0xFFFF);
	out->base1	= (u8)((base >> 16) & 0xFF);
	out->access	= (u8)(access_byte & 0xFF);
	out->limit1	= (u8)((limit >> 16) & 0xF);
	out->flags	= (u8)(flags & 0xF);
	out->base2	= (u8)((base >> 24) & 0xFF);

	return out;
}

struct gdt_entry_t* gdt_null_entry(struct gdt_entry_t* out)
{
	return gdt_pack_entry(0, 0, 0, 0, out);
}

struct gdt_descriptor_t* gdt_descriptor(void* offset, u32 size,
		struct gdt_descriptor_t* out)
{
	if (size == 0) {
		return NULL;
	}
	out->offset = offset;
	out->size = size - 1;

	return out;
}

u32 gdt_access_byte(u32 readwrite, u32 direction, u32 execuable,
		u32 privilege)
{
	struct gdt_access_byte_t access;

	access.accessed = 0;
	access.readwrite = (u8)(readwrite > 0);
	access.direction = (u8)(direction > 0);
	access.executable = (u8)(execuable > 0);
	access.reserved = 1;
	access.privl = (u8)(privilege & 0b11);
	access.present = 1;

	return *(u8*)&access;
}

u32 gdt_flags(u32 granularity, u32 size)
{
	granularity = (u32)(granularity > 0);
	size = (u32)(size > 0);
	return (granularity << 3) | (size << 2);
}
