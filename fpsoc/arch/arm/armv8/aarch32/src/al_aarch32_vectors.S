/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <al_aarch32_sysreg.h>
#include <asm_macros_common.S>

    .globl    undefined_handler         /* Undef */
    .globl    svc_handler               /* Syscall */
    .globl    prefetch_abort_handler    /* Prefetch abort */
    .globl    data_abort_handler        /* Data abort */
    .globl    panic_handler             /* Reserved */
    .globl    irq_handler               /* IRQ */
    .globl    fiq_handler               /* FIQ */


func irq_handler                        /* IRQ vector handler */
    stmdb   sp!,{r0-r3,r12,lr}          /* state save from compiled code*/
    vpush   {d0-d7}
    vpush   {d16-d31}

    vmrs    r1, FPSCR
    push    {r1}
    vmrs    r1, FPEXC
    push    {r1}

    bl      do_irq_handle               /* IRQ vector */

    pop     {r1}
    vmsr    FPEXC, r1
    pop     {r1}
    vmsr    FPSCR, r1
    vpop    {d16-d31}
    vpop    {d0-d7}

    ldmia   sp!,{r0-r3,r12,lr}          /* state restore from compiled code */
    subs    pc, lr, #4                  /* adjust return */
endfunc irq_handler

func fiq_handler                        /* FIQ vector handler */
    stmdb   sp!,{r0-r3,r12,lr}          /* state save from compiled code */
    vpush   {d0-d7}
    vpush   {d16-d31}
    vmrs    r1, FPSCR
    push    {r1}
    vmrs    r1, FPEXC
    push    {r1}

    bl      do_fiq_handle               /* FIQ vector */

    pop     {r1}
    vmsr    FPEXC, r1
    pop     {r1}
    vmsr    FPSCR, r1
    vpop    {d16-d31}
    vpop    {d0-d7}
    ldmia   sp!,{r0-r3,r12,lr}          /* state restore from compiled code */
    subs    pc, lr, #4                  /* adjust return */
endfunc fiq_handler


func undefined_handler                  /* Undefined handler */
    b .
endfunc undefined_handler


func svc_handler                        /* SWI handler */
    b .
endfunc svc_handler


func data_abort_handler                 /* Data Abort handler */
    b .
endfunc data_abort_handler


func prefetch_abort_handler             /* Prefetch Abort handler */
    b .
endfunc prefetch_abort_handler


func panic_handler                      /* Prefetch Abort handler */
    b .
endfunc panic_handler