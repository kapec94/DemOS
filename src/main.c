/*
 * main.c
 *
 */

#include <raw_video.h>
#include <multiboot.h>
#include <cpu.h>
#include <com.h>
#include <vga.h>
#include <gdb_impl.h>

void __attribute__((noreturn, cdecl)) kstart(int magic, struct multiboot_info_t* mb_info)
{
	vga_init();

	/* TODO: get video mode from mb_info instead of hard coded values. */
	rvid_init(DEFAULT_VIDEO_BASE, 80, 25);
	rvid_setattr(RVID_ATTR(COLOR_WHITE, COLOR_BLUE));
	rvid_clrscr();

	if (magic != MB_MAGIC) {
		rvid_printf("Multiboot magic is 0x%x != 0x%x! Halting.\n",
				magic, MB_MAGIC);
		hlt();
	}

	rvid_printf("mb_info available at %x\n", mb_info);
	rvid_printf("flags = %x\n", mb_info->flags);
	if (get_bit(mb_info->flags, MB_MEM_FIELDS_AVAILABLE)) {
		rvid_printf("mem fields available.\n");
		rvid_printf("mem_lower = %d\nmem_upper = %d\n", mb_info->mem_lower, mb_info->mem_upper);
	}
	if (get_bit(mb_info->flags, MB_MMAP_INFO_AVAILABLE)) {
		rvid_printf("mmap fields available.\n");
	}
	if (get_bit(mb_info->flags, MB_VBE_INFO_AVAILABLE)) {
		rvid_printf("VBE info available.\n");
		rvid_printf("vbe_mode = %x\n", mb_info->vbe_mode);
	}

	while (1) {
		hlt();
	}
}
