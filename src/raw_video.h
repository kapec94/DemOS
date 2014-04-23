/*
 * raw_video.h
 *
 *  Created on: 22 kwi 2014
 *      Author: Mateusz
 */

#ifndef RAW_VIDEO_H_
#define RAW_VIDEO_H_

#include <defs.h>

#define DEFAULT_VIDEO_BASE	(void*)(0xB8000)

int rvid_init(void* base, int width, int height);
int rvid_clrscr();
int rvid_getpos(int* x, int* y);
int rvid_setpos(int x, int y);
int rvid_putcharattr(u8 c, u8 attr);
int rvid_putchar(u8 c);
int rvid_puts(const char* s);

#endif /* RAW_VIDEO_H_ */
