#include <multiboot.h>
#include <configuration.h>

.data
	.set mb_flags, (MULTIBOOT_INCLUDE_MEMORY_INFO | MULTIBOOT_INCLUDE_VIDEO_INFO |	MULTIBOOT_LOAD_INFO_AVAILABLE)

.section .header
	// Multiboot basic header
	.long	MULTIBOOT_MAGIC
	.long	mb_flags
	.long	MULTIBOOT_CHECKSUM(mb_flags)

	// Kernel load addresses
	.long	KERNEL_LOAD_ADDRESS
	.long	KERNEL_LOAD_ADDRESS
	.long	_erdata
	.long	_ebss
	.long	_entry

	// Graphics info
	.long	1	// We choose text mode
	.long 	0	// We have no preference over
	.long	0	// 	width nor height.
	.long	0	// The same goes for color depth.

.section .text
.global _entry
_entry:
	movl	$STACK_BASE,	%esp
	movl	%esp,	%ebp
	pushl	%ebx
	pushl	%eax
_loop:
	jmp 	_kstart
	hlt
