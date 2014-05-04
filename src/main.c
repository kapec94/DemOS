/*
 * main.c
 *
 */

#include <stddef.h>
#include <multiboot.h>
#include <cpu.h>
#include <gdt.h>
#include <com.h>
#include <vga.h>
#include <raw_video.h>
#include <interrupts.h>

#define SEGMENT_PACK_SIZE	0x10
#define GDT_SIZE			0x1FF

#define IDT_SIZE			0x100

/* stack segment size - 8kB */
#define SS_SIZE 0x2000
/* code segment size - 32kB */
#define CS_SIZE 0x8000
/* data segment size - 32kB */
#define DS_SIZE 0x8000

static size_t _init_gdt_mmap(void* base, size_t length);
static void _init_gdt_memfields(size_t lower, size_t upper);
static void _init_idt();

static void _test_gdt_api();

struct gdt_entry_t global_descriptor_table[GDT_SIZE];
struct idt_entry_t global_interrupts_table[IDT_SIZE];

struct table_descriptor_t idt_descriptor;

void __attribute__((noreturn, cdecl)) kstart(int magic, struct multiboot_info_t* mb_info)
{
	vga_init();

	/* TODO: get video mode from mb_info instead of hard coded values. */
	rvid_init(DEFAULT_VIDEO_BASE, 80, 25);
	rvid_clrscr();

	if (magic != MB_MAGIC) {
		rvid_printf("Multiboot magic is 0x%x != 0x%x! Halting.\n",
				magic, MB_MAGIC);
		hlt();
		ud2();
	}

	if (GDT_SIZE == 0) {
		rvid_printf("GDT_SIZE == 0. Prepare for crash.\n");
	}
	else if (GDT_SIZE > 0x2000) {
		rvid_printf("GDT_SIZE > 0x2000. Prepare for undefined behavior.\n");
	}

	_test_gdt_api();

	if (get_bit(mb_info->flags, MB_MMAP_INFO_AVAILABLE)) {
		rvid_printf("Setting up GDT using mmap entries...\n");
		_init_gdt_mmap(mb_info->mmap_addr, mb_info->mmap_length);
	}
	else if (get_bit(mb_info->flags, MB_MEM_FIELDS_AVAILABLE)) {
		rvid_printf("Setting up GDT using memfields. " \
				"This probably means not all available RAM will be used!");
		_init_gdt_memfields(mb_info->mem_lower, mb_info->mem_upper);
	}

	rvid_printf("Setting up IDT...\n");
	_init_idt();

	rvid_printf("LGDT, LIDT\n");
	lgdt(global_descriptor_table);
	set_cs(0x08);
	set_ds(0x10);

	lidt(idt_descriptor);
	sti();

	while (1) {
		hlt();
	}
}

void _prepare_gdt(size_t* start)
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

size_t _init_gdt_mmap(void* base, size_t length)
{
	size_t i, j = 0;
	size_t loaded = 0;

	void* ss_ptr;
	void* cs_ptr;
	void* ds_ptr;

	_prepare_gdt(&i);

	while (j < SEGMENT_PACK_SIZE * 3) {
		if (_find_mem(SS_SIZE, base, length, &ss_ptr) &&
			_find_mem(CS_SIZE, base, length, &cs_ptr) &&
			_find_mem(DS_SIZE, base, length, &ds_ptr))
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

void _init_gdt_memfields(size_t lower, size_t upper)
{
	rvid_printf("_init_gdt_memfields not yet implemented.\n");
	hlt();
	ud2();
}

void _init_idt()
{
	size_t i = 0;
	struct int_entry_t* int_entry;

	for (i = 0; i < IDT_SIZE; i++) {
		idt_null_entry(&global_interrupts_table[i]);
	}

	/*
	 * Filling up exception handlers entries (int <= 0x10)
	 * Currently commented out, since we don't probably want
	 * to quiet every exception we don't handle (let it triple-fault)
	 *
	 */
	/*
	for (i = 0; i < EXCEPTION_COUNT; i++) {
		idt_pack_entry(ISR(dummy_handler), 0x08,
				idt_type(IDT_GATE_32BIT_INT, PRIV_KERNEL),
				&global_interrupts_table[i]);
		i++;
	}
	*/

	int_entry = &exception_handlers[0];
	while (int_entry->i != -1) {
		idt_pack_entry(int_entry->base, 0x08,
				idt_type(IDT_GATE_32BIT_INT, PRIV_KERNEL),
				&global_interrupts_table[int_entry->i]);
		int_entry ++;
	}

	table_descriptor(global_interrupts_table,
			IDT_SIZE * sizeof(struct idt_entry_t),
			&idt_descriptor);
}

#define ASSERT(test) if (!(test)) rvid_printf("FAIL at line %d\n", __LINE__)
void _test_gdt_api()
{
	struct gdt_entry_t entry;

	rvid_printf("Starting GDT functions tests...\n");
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
