OUTPUT_ARCH(arm)
ENTRY(startup)
STACK_SIZE = 0x400;
SECTIONS
{
  . = 0x1000000;
  .text :
  {
   *mycrt0.o (.text)
   *(.text)
   *(.fixup)
   *(.gnu.warning)
   *(.text.lock)
   *(.rodata)
   *(.rodata*)
   *(.glue_7)
   *(.glue_7t)
  }

  . = ALIGN(4);

  .data :
  {
   _data = .;
   *(.data)
   _edata = .;
  }

  .bss :
  {
   __bss_start = .;
   *(.bss)
   *(COMMON)
   __bss_end = .;
  }

  .stack :
  {
   . = ALIGN(256);
   . += STACK_SIZE;
  }

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

