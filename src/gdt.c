/*
 * gdt.c
 *
 */

#include <raw_video.h>
#include <multiboot.h>
#include <configuration.h>
#include "gdt.h"

static void _prepare_gdt(size_t* start,
		struct gdt_entry_t* global_descriptor_table);
static int _find_mem(size_t mem, void* base, size_t length, void** found);

size_t gdt_init(void* mmap_base, size_t mmap_length,
		struct gdt_entry_t* global_descriptor_table)
{
	size_t i, j = 0;
	size_t loaded = 0;

	void* ss_ptr;
	void* cs_ptr;
	void* ds_ptr;

	_prepare_gdt(&i, global_descriptor_table);

	while (j < SEGMENT_PACK_SIZE * 3) {
		if (_find_mem(SS_SIZE, mmap_base, mmap_length, &ss_ptr) &&
			_find_mem(CS_SIZE, mmap_base, mmap_length, &cs_ptr) &&
			_find_mem(DS_SIZE, mmap_base, mmap_length, &ds_ptr))
		{
			gdt_pack_entry(ss_ptr + SS_SIZE, SS_SIZE,
					gdt_access_byte(
						GDT_AB_DATA_WRITABLE,
						GDT_AB_EXPAND_DOWN,
						GDT_AB_DATA_SEGMENT,
						PRIV_KERNEL),
					gdt_flags(
						GDT_FLAGS_NO_GRANURALITY,
						GDT_FLAGS_32PM),
					&global_descriptor_table[i + j]);

			gdt_pack_entry(cs_ptr, CS_SIZE,
					gdt_access_byte(
						GDT_AB_CODE_EXECUTABLE,
						GDT_AB_NONCONFORMING,
						GDT_AB_CODE_SEGMENT,
						PRIV_KERNEL),
					gdt_flags(
						GDT_FLAGS_NO_GRANURALITY,
						GDT_FLAGS_32PM),
					&global_descriptor_table[i + j + 1]);

			gdt_pack_entry(ds_ptr, DS_SIZE,
					gdt_access_byte(
						GDT_AB_DATA_READABLE,
						GDT_AB_EXPAND_UP,
						GDT_AB_DATA_SEGMENT,
						PRIV_KERNEL),
					gdt_flags(
						GDT_FLAGS_NO_GRANURALITY,
						GDT_FLAGS_32PM),
					&global_descriptor_table[i + j + 2]);

			j += 3;
			loaded ++;
		}
		else
		{
			break;
		}
	}
	return loaded;
}

struct table_descriptor_t* table_descriptor(void* offset, u32 size,
		struct table_descriptor_t* out)
{
	if (size == 0) {
		return NULL;
	}
	out->offset = offset;
	out->size = size - 1;

	return out;
}

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

void idt_init(struct idt_entry_t* global_interrupts_table,
		struct table_descriptor_t* idt_descriptor)
{
	size_t i = 0;

	for (i = 0; i < IDT_SIZE; i++) {
		idt_null_entry(&global_interrupts_table[i]);
	}

	table_descriptor(global_interrupts_table,
			IDT_SIZE * sizeof(struct idt_entry_t),
			idt_descriptor);
}

struct idt_entry_t* idt_pack_entry(void* base_ptr, u32 selector, u32 type,
		struct idt_entry_t* out)
{
	u32 base = (u32)base_ptr;

	out->base0 = (u16)(base & 0xFFFF);
	out->selector = (u16)(selector & 0xFFFF);
	out->reserved = 0;
	out->type = (u16)(type & 0xFFFF);
	out->base1 = (u16)((base >> 16) & 0xFFFF);

	return out;
}

struct idt_entry_t* idt_null_entry(struct idt_entry_t* out)
{
	return idt_pack_entry(NULL, 0, 0, out);
}

u32	idt_type(u32 gate_type, u32 privilege)
{
	struct idt_type_t type;

	type.gate_type = (u8)(gate_type & 0xF);
	type.storage = 0;
	type.privl = (u8)(privilege & 0b11);
	type.present = 1;

	return *(u8*)(&type);
}

void _prepare_gdt(size_t* start,
		struct gdt_entry_t* global_descriptor_table)
{
	size_t i = 0;
	for (i = 0; i < GDT_SIZE; i++) {
		gdt_null_entry(&global_descriptor_table[i]);
	}

	/* first entry we'll actually set to contain the GDT descriptor. */
	table_descriptor(global_descriptor_table,
			GDT_SIZE * sizeof(struct gdt_entry_t),
			(struct table_descriptor_t*)&global_descriptor_table[0]);

	/* second and third entries contain 1:1 mapping of address space */
	gdt_pack_entry((void*)0x00000000, 0xFFFFF,
			gdt_access_byte(
				GDT_AB_CODE_EXECUTABLE,
				GDT_AB_NONCONFORMING,
				GDT_AB_CODE_SEGMENT,
				PRIV_KERNEL),
			gdt_flags(
				GDT_FLAGS_GRANURALITY,
				GDT_FLAGS_32PM),
			&global_descriptor_table[1]);
	gdt_pack_entry((void*)0x00000000, 0xFFFFF,
			gdt_access_byte(
				GDT_AB_DATA_WRITABLE,
				GDT_AB_EXPAND_UP,
				GDT_AB_DATA_SEGMENT,
				PRIV_KERNEL),
			gdt_flags(
				GDT_FLAGS_GRANURALITY,
				GDT_FLAGS_32PM),
			&global_descriptor_table[2]);

	/* used entries count may change in the future */
	*start = 3;
}

int _find_mem(size_t mem, void* base, size_t length,
		void** found)
{
	size_t offset = 0;
	struct mmap_entry_t* entry;

	while (offset < length) {
		entry = base + offset;

		if (entry->type != MB_MMAP_MEM_AVAILABLE) {
			offset += entry->size + sizeof(entry->size);
		}
		else if (entry->length < mem) {
			offset += entry->size + sizeof(entry->size);
		}
		else {
			*found = (void*)(u32)(entry->base_addr & 0xFFFFFFFF);

			/* We remove this memory from mmap entries,
			 * so further searches won't get confused.
			 */
			entry->base_addr += (u32)mem;
			entry->length -= (u32)mem;

			return S_OK;
		}
	}
	*found = NULL;
	return E_NO;
}

#define ASSERT(test) if (!(test)) rvid_printf("FAIL at line %d\n", __LINE__)
void _test_gdt_api()
{
	struct gdt_entry_t entry;

	rvid_printf("test_gdt_flags\n");
	ASSERT(gdt_flags(GDT_FLAGS_GRANURALITY, GDT_FLAGS_32PM) == 0b1100);
	ASSERT(gdt_flags(GDT_FLAGS_GRANURALITY, GDT_FLAGS_16PM) == 0b1000);

	rvid_printf("test_gdt_access_byte\n");
	ASSERT(gdt_access_byte(0, 0, 0, 0) == 0b10010000);
	ASSERT(gdt_access_byte(1, 0, 0, 0) == 0b10010010);
	ASSERT(gdt_access_byte(0, 1, 0, 0) == 0b10010100);
	ASSERT(gdt_access_byte(0, 0, 1, 0) == 0b10011000);
	ASSERT(gdt_access_byte(0, 0, 0, 0b10) == 0b11010000);

	rvid_printf("test_gdt_pack_entry\n");
	gdt_pack_entry((void*)0x11223344, 0x56677, 0x88, 0x9, &entry);
	ASSERT(entry.limit0 == 0x6677);
	ASSERT(entry.base0 == 0x3344);
	ASSERT(entry.base1 == 0x22);
	ASSERT(entry.access == 0x88);
	ASSERT(entry.limit1 == 0x5);
	ASSERT(entry.flags == 0x9);
	ASSERT(entry.base2 == 0x11);
	ASSERT(*(u64*)&entry == 0x1195882233446677);

	rvid_printf("Done.\n");
}
