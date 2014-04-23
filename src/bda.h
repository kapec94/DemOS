/*
 * bda.h - BIOS Data Area API
 *
 */

#ifndef BDA_H_
#define BDA_H_

#define BDA_ADDRESS		(0x400)

/* There are many more values here, but basically only these two are
 * interesting.
 */
#define BDA_COM_PORTS	(0x00)
#define BDA_VIDEO_IO	(0x63)

int bda_query(u8 addr, size_t len, void* out);

#endif /* BDA_H_ */
