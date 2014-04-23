/*
 * multiboot.h
 *
 *  Created on: 20 kwi 2014
 *      Author: Mateusz
 */

#ifndef MULTIBOOT_H_
#define MULTIBOOT_H_

// Multiboot header magic.
#define MULTIBOOT_MAGIC	0x1BADB002

// Multiboot header flags
/* All boot modules loaded along with the operating system must be aligned on
 * page (4KB) boundaries */
#define MULTIBOOT_ALIGN_BOOT_MODULES	(1 << 0)
/* Information on available memory via at least the ‘mem_*’ fields of the
 * Multiboot information structure must be included. If the boot loader is
 * capable of passing a memory map (the ‘mmap_*’ fields) and one exists,
 * then it may be included as well. */
#define MULTIBOOT_INCLUDE_MEMORY_INFO	(1 << 1)
/* Information about the video mode table (see Boot information format) must
 * be available to the kernel. */
#define MULTIBOOT_INCLUDE_VIDEO_INFO	(1 << 2)
/* Fields at offsets 12-28 in the Multiboot header are valid, and the boot
 * loader should use them instead of the fields in the actual executable
 * header to calculate where to load the OS image. This information does not
 * need to be provided if the kernel image is in elf format, but it must be
 * provided if the images is in a.out format or in some other format. */
#define MULTIBOOT_LOAD_INFO_AVAILABLE	(1 << 16)

/* Calculates Multiboot magic checksum */
#define MULTIBOOT_CHECKSUM(flags) (-(MULTIBOOT_MAGIC + flags))

#ifndef __ASSEMBLER__

/* Multiboot structure contains various information about the boot environment.
 * Some of them available only when explicitly asked for, other available when
 * possible. */
// TODO: end it.
struct multiboot_info_t {
	u32		flags;

	u32		mem_lower;
	u32		mem_upper;

	u32		boot_device;

	const char*	cmdline;

	u32		mods_count;
	void*	mods_addr;

	u8		syms[16];
};

#endif // __ASSEMBLER__

#endif /* MULTIBOOT_H_ */
