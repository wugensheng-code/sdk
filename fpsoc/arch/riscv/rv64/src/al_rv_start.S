/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "al_rv_sysregs.h"

#define MAX_INTR    (130)
/* Select instructions of different lengths depending
   on the riscv architecture */
.macro DECLARE_INT_HANDLER  INT_HDL_NAME
#if defined(__riscv_xlen) && (__riscv_xlen == 32)
    .word \INT_HDL_NAME
#else
    .dword \INT_HDL_NAME
#endif
.endm

.section .vtable
.rept MAX_INTR
    DECLARE_INT_HANDLER     default_intexc_handler          /* 1: Reserved */
.endr

    .globl vector_base
    .type vector_base, @object
vector_base:
    DECLARE_INT_HANDLER     default_intexc_handler          /* 0: Reserved, default handler for Flash download mode */
.rept MAX_INTR
    DECLARE_INT_HANDLER     default_intexc_handler          /* 1: Reserved */
.endr

.section .init
.globl _start
.type _start, @function
_start:
    /* Disable global interrupt by set sie bitfield
       in machine status register */
    csrw mie, zero

    /* Save complier option */
    .option push

    /* Set complier ban instruction relaxation */
    .option norelax

    /* Write gp reg value from link script(.ld) */
    la gp, __global_pointer$

    /* Pop save complier option */
    .option pop

    /* Write sp reg value from link script(.ld) */
    la sp, _sp

    /* Set riscv interrupt vector base address  */
    la t0, vector_base
    csrw CSR_MTVT, t0

    /*
     * Configure the entry addresses for interrupt and exception handlers to exc_entry.
     * Due to settings above, Exception and NMI will share common entry.
     * exc_entry function address need align at 64byte
     */
    la t0, exc_entry
    csrw CSR_MTVEC, t0

    call riscv_vendor_init

    call load_code
    call load_data
    call clear_bss

    /* Call global constructors */
    la a0, __libc_fini_array
    call atexit
    /* Call C/C++ constructor start up code */
    call __libc_init_array

    /* do pre-init steps before main */
    call board_init
    call _premain_init
    /* Call Main Function argc = argv = 0 */
    li a0, 0
    li a1, 0

#ifdef RTOS_RTTHREAD
    /* Call entry function when using RT-Thread */
    call entry
#else
    call main
#endif
    /* When main returns, the program is stuck in an infinite loop */
1:  j 1b

.global board_init
.weak board_init
board_init:
    nop
    ret
