/*
 * multiboot.h
 *
 *  Created on: 20 kwi 2014
 *      Author: Mateusz
 */

#ifndef MULTIBOOT_H_
#define MULTIBOOT_H_

#include <stddef.h>
#include <mbdef.h>

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
	u64		base_addr;
	u64		length;
	u32		type;
} __attribute__((packed));

#endif /* __ASSEMBLER__ */

#endif /* MULTIBOOT_H_ */
