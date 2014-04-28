/*
 * main.c
 *
 *  Created on: 20 kwi 2014
 *      Author: Mateusz
 */

#include <raw_video.h>
#include <com.h>
#include <vga.h>
#include <gdb_impl.h>

void __attribute__((noreturn, cdecl)) kstart(int magic, void* mb_info)
{
	vga_init();

	// TODO: get video mode from mb_info instead of hard coded values.
	rvid_init(DEFAULT_VIDEO_BASE, 80, 25);
	rvid_setattr(RVID_ATTR(COLOR_WHITE, COLOR_BLUE));
	rvid_clrscr();

	gdb_stub_init();

	while (1) {
		hlt();
	}
}
