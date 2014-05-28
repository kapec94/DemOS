/*
 * main.c
 *
 */

#include <configuration.h>
#include <stddef.h>
#include <multiboot.h>
#include <cpu.h>
#include <gdt.h>
#include <com.h>
#include <vga.h>
#include <pic.h>
#include <raw_video.h>
#include <interrupts.h>
#include <ps2.h>

struct gdt_entry_t global_descriptor_table[GDT_SIZE];
struct idt_entry_t global_interrupts_table[IDT_SIZE];

struct table_descriptor_t idt_descriptor;

void __attribute__((noreturn, cdecl)) kstart(int magic, struct multiboot_info_t* mb_info)
{
	u32 result = 0;

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

	rvid_printf("Initializing PIC...\n");
	pic_remap(IRQ_OFFSET, IRQ_OFFSET + 8);
	pic_mask_irqs();

	if (GDT_SIZE == 0) {
		rvid_printf("GDT_SIZE == 0. Prepare for crash.\n");
	}
	else if (GDT_SIZE > 0x2000) {
		rvid_printf("GDT_SIZE > 0x2000. Prepare for undefined behavior.\n");
	}

	if (get_bit(mb_info->flags, MB_MMAP_INFO_AVAILABLE) == 0) {
		rvid_printf("mmap entries unavailable!\n");
		hlt();
		ud2();
	}

	rvid_printf("Setting up GDT...\n");
	gdt_init(mb_info->mmap_addr, mb_info->mmap_length, global_descriptor_table);

	rvid_printf("Setting up IDT...\n");
	idt_init(global_interrupts_table, &idt_descriptor);
	interrupts_init(global_interrupts_table);

	rvid_printf("LGDT, LIDT\n");
	lgdt(global_descriptor_table);
	set_cs(0x08);
	set_ds(0x10);

	lidt(idt_descriptor);

	rvid_printf("Initializing PS/2 controller...\n");
	result = ps2_init();
	if (result != S_OK) {
		rvid_printf("Could not initialize PS/2! Error: %x\n", result);
		hlt();
		ud2();
	}

	rvid_printf("Detecting PS/2 device 1... ");
	if (ps2_detect(PS2_PORT1, &result) == S_OK) {
		switch (result) {
		case PS2_DEVTYPE_KBD:
		case PS2_DEVTYPE_KBD_ANCIENT:
		case PS2_DEVTYPE_KBD_TRANS:
			rvid_printf("Keyboard!\n");
			break;
		case PS2_DEVTYPE_MOUSE:
		case PS2_DEVTYPE_MOUSE_SCROLL:
		case PS2_DEVTYPE_MOUSE_5BUT:
			rvid_printf("Mouse!\n");
		}
	}
	rvid_printf("Detecting PS/2 device 2... ");
	if (ps2_detect(PS2_PORT2, &result) == S_OK) {
		switch (result) {
		case PS2_DEVTYPE_KBD:
		case PS2_DEVTYPE_KBD_ANCIENT:
		case PS2_DEVTYPE_KBD_TRANS:
			rvid_printf("Keyboard!\n");
			break;
		case PS2_DEVTYPE_MOUSE:
		case PS2_DEVTYPE_MOUSE_SCROLL:
		case PS2_DEVTYPE_MOUSE_5BUT:
			rvid_printf("Mouse!\n");
		}
	}

	sti();

	rvid_printf("Done.\n");
	while (1) {
		hlt();
	}
}
