/*
 * mbdef.h
 *
 *  Created on: 31 maj 2014
 *      Author: Stasiu
 */

#ifndef MBDEF_H_
#define MBDEF_H_

#define MBH_MAGIC	0x1BADB002

/* MULTIBOOT HEADER FLAGS */
/* All boot modules loaded along with the operating system must be aligned on
 * page (4KB) boundaries */
#define MBH_ALIGN_BOOT_MODULES	(0)
/* Information on available memory via at least the ‘mem_*’ fields of the
 * Multiboot information structure must be included. If the boot loader is
 * capable of passing a memory map (the ‘mmap_*’ fields) and one exists,
 * then it may be included as well. */
#define MBH_INCLUDE_MEMORY_INFO (1)
/* Information about the video mode table (see Boot information format) must
 * be available to the kernel. */
#define MBH_INCLUDE_VIDEO_INFO	(2)
/* Fields at offsets 12-28 in the Multiboot header are valid, and the boot
 * loader should use them instead of the fields in the actual executable
 * header to calculate where to load the OS image. This information does not
 * need to be provided if the kernel image is in elf format, but it must be
 * provided if the images is in a.out format or in some other format. */
#define MBH_LOAD_INFO_AVAILABLE	(16)

/* Calculates Multiboot magic checksum */
#define MBH_CHECKSUM(flags) (-(MBH_MAGIC + flags))

#define MB_MAGIC	(0x2BADB002)

/* BOOT INFO FLAGS */
/* ‘mem_*’ fields are valid. ‘mem_lower’ and ‘mem_upper’ indicate the amount of
 * lower and upper memory, respectively, in kilobytes. Lower memory starts at
 * address 0, and upper memory starts at address 1 megabyte. */
#define MB_MEM_FIELDS_AVAILABLE	0
/* ‘boot_device’ field is valid, and indicates which bios disk device the boot
 * loader loaded the OS image from */
#define MB_DISK_INFO_AVAILABLE	1
/* ‘cmdline’ field is valid, and contains the physical address of the command
 * line to be passed to the kernel. */
#define MB_CMDLINE_AVAILABLE	2
#define MB_MODS_AVAILABLE		3
#define MB_SYMS_AOUT_AVAILABLE	4
#define MB_SYMS_ELF_AVAILABLE	5
#define MB_MMAP_INFO_AVAILABLE	6
#define MB_DRIVES_AVAILABLE		7
#define MB_VBE_INFO_AVAILABLE	11

/* mmap entry type field value of 1 indicates available ram, and all other
 * values currently indicated a reserved area */
#define MB_MMAP_MEM_AVAILABLE		1

#endif /* MBDEF_H_ */
