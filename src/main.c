/*
 * main.c
 *
 *  Created on: 20 kwi 2014
 *      Author: Mateusz
 */

#include <raw_video.h>
#include <vga.h>

void __attribute__((noreturn, cdecl)) kstart(int magic, void* mb_info)
{
	vga_init();

	// TODO: get video mode from mb_info instead of hard coded values.
	rvid_init(DEFAULT_VIDEO_BASE, 80, 25);

	rvid_setattr(RVID_ATTR(COLOR_WHITE, COLOR_BLUE));
	rvid_clrscr();

	rvid_puts("Hello world!\n");
	rvid_puts("This is second line.\n");
	rvid_puts("This is third line.\n");

	rvid_puts("\n\n");

	rvid_puts("That was hell of a gap!");

	while (1);
}
