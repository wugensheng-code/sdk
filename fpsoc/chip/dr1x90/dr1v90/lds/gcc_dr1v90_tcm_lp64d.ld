/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

OUTPUT_ARCH( "riscv" )

ENTRY( _start )

/* tcm 256K, 0x60000000~0x6003FFFF; tcm 256K, 0x60040000~0x6007FFFF */
MEMORY
{
  tcm (rxa!w) : ORIGIN = 0x60000000, LENGTH = 512K    /*for 9000 dv simulation*/
}


SECTIONS
{
  __stack_size = DEFINED(__stack_size) ? __stack_size : 2k;

  .init           :
  {
    *(.vtable)
    KEEP (*(SORT_NONE(.init)))
  } >tcm AT>tcm

  .ilalign         :
  {
    . = ALIGN(4);
    PROVIDE( _ilm_lma = . );
  } >tcm AT>tcm

  .ialign         :
  {
    PROVIDE( _ilm = . );
  } >tcm AT>tcm

  PROVIDE (_text = .);
  .text           :
  {
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
    PROVIDE (_text_end = .);
    ASSERT (_text_end < 0x6003FFFF, "Error: The text segment should not exceed the tcm address range");
  } >tcm AT>tcm

  .rodata : ALIGN(4)
  {
    . = ALIGN(4);
    *(.rdata)
    *(.rodata .rodata.*)
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
    /* section information for utest */
    __rt_utest_tc_tab_start = .;
    KEEP(*(UtestTcTab))
    __rt_utest_tc_tab_end = .;
    *(.gnu.linkonce.r.*)
    /* section information for usbh class */
    . = ALIGN(4);
    __usbh_class_info_start__ = .;
    KEEP(*(.usbh_class_info))
    __usbh_class_info_end__ = .;

  } >tcm AT>tcm

  . = ALIGN(4096);
  .cli_section :
  {
    _cli_section_start = .;
    KEEP(*(cli_section))
    _cli_section_end   = .;
  }  >tcm AT>tcm

  . = ALIGN(4096);
  .unity_section :
  {
    _unity_test_start = .;
    KEEP(*(unity_section))
    _unity_test_end   = .;
  }  >tcm AT>tcm

  .fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  } >tcm AT>tcm

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
  } >tcm AT>tcm

  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >tcm AT>tcm

  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >tcm AT>tcm

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

    PROVIDE(__ctors_start__ = .);
    /* new GCC version uses .init_array */
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE(__ctors_end__ = .);
  } >tcm AT>tcm

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >tcm AT>tcm

  .eh_frame : {
     *(.eh_frame)
  } >tcm AT>tcm

  .eh_framehdr : {
     __eh_framehdr_start = .;
     *(.eh_framehdr)
     __eh_framehdr_end = .;
  } >tcm AT>tcm

  .gcc_except_table : {
     *(.gcc_except_table)
  } >tcm AT>tcm

  .lalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = . );
  } >tcm AT>tcm

  .dalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data = . );
  } >tcm AT>tcm

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
  } >tcm AT>tcm

  . = ALIGN(4);
  PROVIDE( _edata = . );
  PROVIDE( edata = . );

  PROVIDE( _fbss = . );
  PROVIDE( __bss_start = . );
  .bss            :
  {
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
  } >tcm AT>tcm

  . = ALIGN(8);
  PROVIDE( _end = . );
  PROVIDE( end = . );
  PROVIDE(_heap_start = . );

  .stack ORIGIN(tcm) + LENGTH(tcm) - __stack_size :
  {
    PROVIDE( _heap_end = . );
    . = __stack_size;
    PROVIDE( _sp = . );
  } >tcm AT>tcm
}

