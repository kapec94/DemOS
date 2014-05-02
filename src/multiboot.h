/*
 * multiboot.h
 *
 *  Created on: 20 kwi 2014
 *      Author: Mateusz
 */

#ifndef MULTIBOOT_H_
#define MULTIBOOT_H_

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

#ifndef __ASSEMBLER__

/* Multiboot structure contains various information about the boot environment.
 * Some of them available only when explicitly asked for, other available when
 * possible.
 * http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#boot-information-format */
struct multiboot_info_t {
	u32		flags;
	u32		mem_lower;
	u32		mem_upper;
	u32		boot_device;
	const char*	cmdline;
	u32		mods_count;
	struct mod_info_t* mods_addr;
	u8		syms[16];
	u32		mmap_length;
	struct mmap_entry_t* mmap_addr;
	u32		drives_length;
	void*	drives_addr;
	void*	config_table;
	const char* boot_loader_name;
	void*	apm_table;

	void*	vbe_control_info;
	void*	vbe_mode_info;
	u16		vbe_mode;
	u16		vbe_interface_seg;
	u16		vbe_interface_off;
	u16		vbe_interface_len;
} __attribute__((packed));

struct boot_device_t {
	u8		disk;
	u8		part1;
	u8		part2;
	u8		part3;
};

struct mod_info_t {
	void*	start;
	void*	end;
	const char* string;
	u32		reserved;
};

struct syms_aout_t {
	u32		tabsize;
	u32		strsize;
	void*	addr;
	u32		reserved;
};

struct syms_elf_t {
	u32		num;
	u32		size;
	void*	addr;
	void*	shndx;
};

struct mmap_entry_t {
	u32		size;
	void*	base_addr;
	u32		length;
	u32		type;
};

#endif /* __ASSEMBLER__ */

#endif /* MULTIBOOT_H_ */
