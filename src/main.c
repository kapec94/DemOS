/*
 * main.c
 *
 */

#include <stddef.h>

#include <configuration.h>
#include <multiboot.h>
#include <cpu.h>

#include <vga.h>
#include <raw_video.h>
#include <conio.h>

#include <pic.h>
#include <gdt.h>
#include <interrupts.h>

#include <ps2.h>
#include <keyboard.h>

struct gdt_entry_t global_descriptor_table[GDT_SIZE];
struct idt_entry_t global_interrupts_table[IDT_SIZE];

struct table_descriptor_t idt_descriptor;

void __attribute__((noreturn, cdecl)) kstart(int magic, struct multiboot_info_t* mb_info)
{
	u32 result = 0, port = 0;

	vga_init();

	/* TODO: get video mode from mb_info instead of hard coded values. */
	rvid_init(DEFAULT_VIDEO_BASE, 80, 25);
	rvid_clrscr();

	cprintf("%x\n", 0xDEADBEEF);

	if (magic != MB_MAGIC) {
		cprintf("Multiboot magic is 0x%x != 0x%x! Halting.\n",
				magic, MB_MAGIC);
		hlt();
		ud2();
	}

	cprintf("Initializing PIC...\n");
	pic_remap(IRQ_OFFSET, IRQ_OFFSET + 8);
	pic_mask_irqs();

	if (GDT_SIZE == 0) {
		cprintf("GDT_SIZE == 0. Prepare for crash.\n");
	}
	else if (GDT_SIZE > 0x2000) {
		cprintf("GDT_SIZE > 0x2000. Prepare for undefined behavior.\n");
	}

	if (get_bit(mb_info->flags, MB_MMAP_INFO_AVAILABLE) == 0) {
		cprintf("mmap entries unavailable!\n");
		hlt();
		ud2();
	}

	cprintf("Setting up GDT...\n");
	gdt_init(mb_info->mmap_addr, mb_info->mmap_length, global_descriptor_table);

	cprintf("Setting up IDT...\n");
	idt_init(global_interrupts_table, &idt_descriptor);
	interrupts_init(global_interrupts_table);

	cprintf("LGDT, LIDT\n");
	lgdt(global_descriptor_table);
	set_cs(0x08);
	set_ds(0x10);

	lidt(idt_descriptor);

	cprintf("Initializing PS/2 controller...\n");
	result = ps2_init();
	if (result != S_OK) {
		cprintf("Could not initialize PS/2! Error: %x\n", result);
		hlt();
		ud2();
	}

	port = PS2_PORT1;
	if (!kbd_iskbd(port)) {
		port = PS2_PORT2;
		if (!kbd_iskbd(port)) {
			cprintf("No keyboard connected!\n");
			hlt();
			ud2();
		} else {
			cprintf("Found keyboard at port 2\n");
		}
	} else {
		cprintf("Found keyboard at port 1\n");
	}

	kbd_init(port);

	sti();

	cprintf("> ");
	while (1) {
		getche();
	}

	while (1) {
		hlt();
	}
}
