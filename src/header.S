#include <mbdef.h>
#include <configuration.h>

.data
	.set mb_flags, (1 << MBH_INCLUDE_MEMORY_INFO)

.section .header
	// Multiboot basic header
	.long	MBH_MAGIC
	.long	mb_flags
	.long	MBH_CHECKSUM(mb_flags)

.section .text
.global _entry
_entry:
	movl	$STACK_BASE,	%esp
	movl	%esp,	%ebp

	pushl	%ebx
	pushl	%eax

	call 	_kstart
	hlt
