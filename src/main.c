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
		printf("Multiboot magic is 0x%x != 0x%x! Halting.\n",
				magic, MB_MAGIC);
		hlt();
	}

	printf("flags = %x\n\n", mb_info->flags);
	if (get_bit(mb_info->flags, MB_MEM_FIELDS_AVAILABLE)) {
		printf("mem fields available.\n");
		printf("mem_lower = %d\nmem_upper = %d\n", mb_info->mem_lower, mb_info->mem_upper);
	}
	if (get_bit(mb_info->flags, MB_MMAP_INFO_AVAILABLE)) {
		printf("mmap fields available.\n");

	}

	while (1) {
		hlt();
	}
}
