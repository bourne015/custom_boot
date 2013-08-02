OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
	. = 0x55000000;

	. = ALIGN(4);
	.text : {
		start.o (.text)
		clock.o (.text)
		ddr.o (.text)
		nand.o (.text)
		*(.text)
	}

	. = ALIGN(4);
	.rodata : {
		*(.rodata*)
	}

	. = ALIGN(4);
	.data : {
		*(.data)
	}

	. = ALIGN(4);
	bss_start = .;
	.bss : {
		*(.bss)
		*(.common)
	}
	bss_end = .;
}
