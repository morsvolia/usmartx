OUTPUT_ARCH(arm)
ENTRY(startup)
STACK_SIZE = 0x400;
/* Memory definiton lpc229x */
MEMORY
{
	/* Flash minus boot block for code storage */
	flash (rx) : ORIGIN = 0x00000000, LENGTH = 0x0003E000 
	ram (rw)   : ORIGIN = 0x40000000, LENGTH = 0x00004000
	eram (rw)  : ORIGIN = 0x80000000, LENGTH = 0x04000000
}

SECTIONS
{
  .text :
  {
   *crt0.o (.text)
   *(.text)
   *(.fixup)
   *(.gnu.warning)
   *(.text.lock)
   *(.rodata)
   *(.rodata*)
   *(.glue_7)
   *(.glue_7t)
  } > flash

  . = ALIGN(4);

  .data :
  {
   _data = .;
   *(.data)
   _edata = .;
  } > ram

  .bss :
  {
   __bss_start = .;
   *(.bss)
   *(COMMON)
   __bss_end = .;
  } > ram

  .stack :
  {
   . = ALIGN(256);
   . += STACK_SIZE;
  } > eram

	/* Stabs debugging sections.	*/
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
	.debug_abbrev 0 : { *(.debug_abbrev) }
	.debug_info 0 : { *(.debug_info) }
	.debug_line 0 : { *(.debug_line) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	.debug_aranges 0 : { *(.debug_aranges) }

}

