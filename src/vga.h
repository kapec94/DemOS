/*
 * vga.h - VGA API
 *
 */

#ifndef VGA_H_
#define VGA_H_

#include <defs.h>
#include <cpu.h>

#define VGA_REG_MISCR	(0x03CC)
#define VGA_REG_MISCW	(0x03C2)

// CRT address register when I/OAS bit of VGA_REG_MISC register is 0
#define VGA_REG_CRTA_0	(0x03B4)
#define VGA_REG_CRTD_0	(0x03B5)
// ...1
#define VGA_REG_CRTA_1	(0x03D4)
#define VGA_REG_CRTD_1	(0x03D5)

// CRT index registers
/* cursor start register */
#define	VGA_CRT_CSR		(0x0A)
/* cursor disable bit */
#define VGA_CSR_CD		(1 << 5)

/* This bit selects the CRT controller addresses. When set to 0, this bit
 * sets the CRT controller addresses to 0x03Bx and the address for the Input
 * Status Register 1 to 0x03BA for compatibility withthe monochrome adapter.
 * When set to 1, this bit sets CRT controller addresses to 0x03Dx and the
 * Input Status Register 1 address to 0x03DA for compatibility with the
 * color/graphics adapter. The Write addresses to the Feature Control register
 * are affected in the same manner. */
#define VGA_IOAS		(1 << 0)

int vga_init();
u8 vga_read_crt(u8 index);
void vga_write_crt(u8 index, u8 val);

#endif /* VGA_H_ */
