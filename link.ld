INCLUDE "configuration.ld"

ENTRY(_entry)

SECTIONS
{
	. = KERNEL_LOAD_ADDRESS;
	.header : { *(.header) }
	.text : { *(.text) }
	.data : { *(.data) }
	.rdata : { *(.rdata) }
	_erdata = .;
	.bss : { *(.bss) }
	_ebss = .;
}