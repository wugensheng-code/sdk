/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

OUTPUT_ARCH( "riscv" )

ENTRY( _start )

__ROM_BASE = 0x70400000;
__ROM_SIZE = 0x100000;

MEMORY
{
  flash (rxai!w) : ORIGIN  = __ROM_BASE, LENGTH = __ROM_SIZE

  itcm  (rxa!w)  : ORIGIN  = 0x60000000, LENGTH = 512K
  ocm   (rxa!w)  : ORIGIN  = 0x61000000, LENGTH = 256K
  npuram  (rxa!w) : ORIGIN = 0x63e00000, LENGTH = 256K
}


SECTIONS
{
  __stack_size = DEFINED(__stack_size) ? __stack_size : 2k;
  .init           :
  {
    KEEP (*(SORT_NONE(.init)))
  } >flash AT>flash

  .ilalign         :
  {
    . = ALIGN(4);
    PROVIDE( _ilm_lma = . );
  } >flash AT>flash

  .ialign         :
  {
    PROVIDE( _ilm = . );
  } >flash AT>flash

  PROVIDE (_text = .);
  .text           :
  {
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
  } >flash AT>flash

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

  } >flash AT>flash


    .eh_frame : {
       *(.eh_frame)
    } >ram AT>ram

    .eh_framehdr : {
       __eh_framehdr_start = .;
       *(.eh_framehdr)
       __eh_framehdr_end = .;
    } >ram AT>ram

    .gcc_except_table : {
       *(.gcc_except_table)

  } >flash AT>flash

  . = ALIGN(4096);
  .cli_section :
  {
    _cli_section_start = .;
    KEEP(*(cli_section))
    _cli_section_end   = .;
  }  >itcm AT>flash

  . = ALIGN(4096);
  .unity_section :
  {
    _unity_test_start = .;
    KEEP(*(unity_section))
    _unity_test_end   = .;
  }  >itcm AT>flash

  .fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  } >itcm AT>flash

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
  } >itcm AT>flash

  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >itcm AT>flash

  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >itcm AT>flash

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
  } >itcm AT>flash

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >itcm AT>flash

  . = ALIGN(4k);

  .lalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = LOADADDR(.data) );
  } >itcm AT>flash

  .dalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data = ADDR(.data));
  } >itcm AT>flash

  .data          :
  {
    *(.vtable)
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
  } >itcm AT>flash

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
  } >itcm AT>flash

  . = ALIGN(8);
  PROVIDE( _end = . );
  PROVIDE( end = . );
  PROVIDE(_heap_start = . );

  .stack ORIGIN(itcm) + LENGTH(itcm) - __stack_size :
  {
    PROVIDE( _heap_end = . );
    . = __stack_size;
    PROVIDE( _sp = . );
  } >itcm AT>flash
}

