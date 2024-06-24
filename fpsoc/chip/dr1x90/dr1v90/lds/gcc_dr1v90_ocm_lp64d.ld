/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

OUTPUT_ARCH( "riscv" )

ENTRY( _start )

MEMORY
{
  ilm (rxa!w) : ORIGIN = 0x61000000, LENGTH = 252K
}

SECTIONS
{
  __stack_size = DEFINED(__stack_size) ? __stack_size : 2K;

  .init           :
  {
    *(.vtable)
    KEEP (*(SORT_NONE(.init)))
  } >ilm AT>ilm

  .ilalign         :
  {
    . = ALIGN(4);
    PROVIDE( _ilm_lma = . );
  } >ilm AT>ilm

  .ialign         :
  {
    PROVIDE( _ilm = . );
  } >ilm AT>ilm

  PROVIDE (_text = .);
  .text           :
  {
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
  } >ilm AT>ilm

  .rodata : ALIGN(4)
  {
    . = ALIGN(4);
    *(.rdata)
    *(.rodata .rodata.*)
    . = ALIGN(4);
    /* section information for initial. */
    . = ALIGN(4);
    __rt_init_start = .;
    KEEP(*(SORT(.rti_fn*)))
    __rt_init_end = .;
    /* section information for finsh shell */
    . = ALIGN(4);
    __fsymtab_start = .;
    KEEP(*(FSymTab))
    __fsymtab_end = .;
    . = ALIGN(4);
    __vsymtab_start = .;
    KEEP(*(VSymTab))
    __vsymtab_end = .;
    . = ALIGN(4);
    __rt_utest_tc_tab_start = .;
    KEEP(*(UtestTcTab))
    __rt_utest_tc_tab_end = .;
    *(.gnu.linkonce.r.*)
    /* section information for usbh class */
    . = ALIGN(4);
    __usbh_class_info_start__ = .;
    KEEP(*(.usbh_class_info))
    __usbh_class_info_end__ = .;

  } >ilm AT>ilm

  . = ALIGN(4096);
  .cli_section :
  {
    _cli_section_start = .;
    KEEP(*(cli_section))
    _cli_section_end   = .;
  }  > ilm AT>ilm

  . = ALIGN(4096);
  .unity_section :
  {
    _unity_test_start = .;
    KEEP(*(unity_section))
    _unity_test_end   = .;
  }  > ilm AT>ilm

  .fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  } >ilm AT>ilm

  . = ALIGN(4);

  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  PROVIDE( _eilm = . );


  .preinit_array  :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >ilm AT>ilm

  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >ilm AT>ilm

  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >ilm AT>ilm

  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >ilm AT>ilm

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >ilm AT>ilm


  .lalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = . );
  } >ilm AT>ilm

  .dalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data = . );
  } >ilm AT>ilm

  .data          :
  {
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata .sdata.* .sdata*)
    *(.gnu.linkonce.s.*)
    . = ALIGN(8);
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)
    _no_cache_section_start = (. & 4K);
    _no_cache_section_end = (. & 4K);
  } >ilm AT>ilm

  . = ALIGN(4);
  PROVIDE( _edata = . );
  PROVIDE( edata = . );

  PROVIDE( _fbss = . );
  PROVIDE( __bss_start = . );
  .bss            :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } >ilm AT>ilm

  . = ALIGN(8);
  PROVIDE( _end = . );
  PROVIDE( end = . );

  .read_buffer (NOLOAD) :  {
	. = ALIGN(32);
	*(.read_buffer)
  } >ilm AT>ilm

  PROVIDE(_heap_start = . );

  .stack ORIGIN(ilm) + LENGTH(ilm) - __stack_size :
  {
    PROVIDE( _heap_end = . );
    . = __stack_size;
    PROVIDE( _sp = . );
  } >ilm AT>ilm
}
