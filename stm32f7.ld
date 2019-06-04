MEMORY
{
	FLASH (rx) : ORIGIN = 0x00200000, LENGTH = 1M
	SRAM (rwx) : ORIGIN = 0x20010000, LENGTH = 256K
}

SECTIONS
{
	.mytext :
	{
		KEEP(*(.vector_table))
		*(.text)
		*(.rodata)
		_mydata_lma_start = .; /* LMA is equivalent to VMA in this section */
	} > FLASH

	.mydata : AT(_mydata_lma_start) /* tell linker whrere (LMA) this section should be put at */
	{
		_mydata_vma_start = .;
		*(.data)
		_mydata_vma_end = .;
	} > SRAM

	.mybss :
	{
		_mybss_vma_start = .;
		*(.bss)
		*(COMMON)
		_mybss_vma_end = .;
	} > SRAM
	
	_msp_init = ORIGIN(SRAM) + LENGTH(SRAM);
}
