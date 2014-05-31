/*
 * raw_video.h
 *
 *  Created on: 22 kwi 2014
 *      Author: Mateusz
 */

#ifndef RAW_VIDEO_H_
#define RAW_VIDEO_H_

#include <stddef.h>
#include <stdarg.h>

#define DEFAULT_VIDEO_BASE	(void*)(0xB8000)

#define RVID_ATTR(fg, bg)	(u8)((fg & 0b1111) | ((bg & 0b1111) << 4))

#define COLOR_BLACK 	0x00
#define COLOR_BLUE  	0x01
#define COLOR_GREEN 	0x02
#define COLOR_CYAN  	0x03
#define COLOR_RED   	0x04
#define COLOR_MAGENTA	0x05
#define COLOR_BROWN		0x06
#define COLOR_LGRAY		0x07
#define COLOR_DGRAY		0x08
#define COLOR_LBLUE		0x09
#define COLOR_LGREEN	0x0A
#define COLOR_LCYAN		0x0B
#define COLOR_LRED		0x0C
#define COLOR_LMAGENTA	0x0D
#define COLOR_YELLOW	0x0E
#define COLOR_WHITE		0x0F

int rvid_init(void* base, int width, int height);

u16 rvid_getattr();
void rvid_setattr(u16 attr);
int rvid_clrscr();

int rvid_getpos(int* x, int* y);
int rvid_setpos(int x, int y);
int rvid_putchar(int c);
int rvid_puts(const char* s);

void rvid_cursor_disable();
void rvid_cursor_enable();

#endif /* RAW_VIDEO_H_ */
