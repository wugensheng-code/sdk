/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#include "al_aarch64_macro.h"

    .text

    /* Variables and functions. */
    .extern ullMaxAPIPriorityMask
    .extern pxCurrentTCB
    .extern vTaskSwitchContext
    .extern vApplicationIRQHandler
    .extern ullPortInterruptNesting
    .extern ullPortTaskHasFPUContext
    .extern ullCriticalNesting
    .extern ullPortYieldRequired
    .extern _freertos_vector_table

    .global freertos_vector_base
    .global FreeRTOS_IRQ_Handler
    .global FreeRTOS_SWI_Handler
    .global vPortRestoreTaskContext

#ifdef SMP
    .extern pxCurrentTCBs

#endif


.macro portSAVE_CONTEXT

    /* Switch to use the EL0 stack pointer. */
    MSR     SPSEL, #0

    /* Save the entire context. */
    STP     X0, X1, [SP, #-0x10]!
    STP     X2, X3, [SP, #-0x10]!
    STP     X4, X5, [SP, #-0x10]!
    STP     X6, X7, [SP, #-0x10]!
    STP     X8, X9, [SP, #-0x10]!
    STP     X10, X11, [SP, #-0x10]!
    STP     X12, X13, [SP, #-0x10]!
    STP     X14, X15, [SP, #-0x10]!
    STP     X16, X17, [SP, #-0x10]!
    STP     X18, X19, [SP, #-0x10]!
    STP     X20, X21, [SP, #-0x10]!
    STP     X22, X23, [SP, #-0x10]!
    STP     X24, X25, [SP, #-0x10]!
    STP     X26, X27, [SP, #-0x10]!
    STP     X28, X29, [SP, #-0x10]!
    STP     X30, XZR, [SP, #-0x10]!

    /* Save the SPSR. */
#if defined( GUEST )
    MRS     X3, SPSR_EL1
    MRS     X2, ELR_EL1
#else
    MRS     X3, SPSR_EL3
    /* Save the ELR. */
    MRS     X2, ELR_EL3
#endif

    STP     X2, X3, [SP, #-0x10]!

    /* Save the critical section nesting depth. */
    LDR     X0, ullCriticalNestingConst
    LDR     X3, [X0]

    /* Save the FPU context indicator. */
    LDR     X0, ullPortTaskHasFPUContextConst
    LDR     X2, [X0]

    /* Save the FPU context, if any (32 128-bit registers). */
    CMP     X2, #0
    B.EQ    1f
    STP     Q0, Q1, [SP,#-0x20]!
    STP     Q2, Q3, [SP,#-0x20]!
    STP     Q4, Q5, [SP,#-0x20]!
    STP     Q6, Q7, [SP,#-0x20]!
    STP     Q8, Q9, [SP,#-0x20]!
    STP     Q10, Q11, [SP,#-0x20]!
    STP     Q12, Q13, [SP,#-0x20]!
    STP     Q14, Q15, [SP,#-0x20]!
    STP     Q16, Q17, [SP,#-0x20]!
    STP     Q18, Q19, [SP,#-0x20]!
    STP     Q20, Q21, [SP,#-0x20]!
    STP     Q22, Q23, [SP,#-0x20]!
    STP     Q24, Q25, [SP,#-0x20]!
    STP     Q26, Q27, [SP,#-0x20]!
    STP     Q28, Q29, [SP,#-0x20]!
    STP     Q30, Q31, [SP,#-0x20]!

1:
    /* Store the critical nesting count and FPU context indicator. */
    STP     X2, X3, [SP, #-0x10]!

    LDR     X0, pxCurrentTCBConst

    /* Get coreId AND choose the corresponding index for core in pxCurrentTCB */
    MRS     X1, MPIDR_EL1
    AND     X1, X1, #0xff
    LSL     X1, X1, #3 /* X1 = X1 * 8 */
    ADD     X0, X0, X1

    LDR     X1, [X0]
    MOV     X0, SP   /* Move SP into X0 for saving. */
    STR     X0, [X1]

    /* Switch to use the ELx stack pointer. */
    MSR     SPSEL, #1

    .endm

; /**********************************************************************/

.macro portRESTORE_CONTEXT

    /* Switch to use the EL0 stack pointer. */
    MSR     SPSEL, #0

    /* Set the SP to point to the stack of the task being restored. */
    LDR     X0, pxCurrentTCBConst

    /* Get coreId AND choose the corresponding index for core in pxCurrentTCB */
    MRS     X1, MPIDR_EL1
    AND     X1, X1, #0xff
    LSL     X1, X1, #3 /* X1 = X1 * 8 */
    ADD     X0, X0, X1

    LDR     X1, [X0]
    LDR     X0, [X1]
    MOV     SP, X0

    LDP     X2, X3, [SP], #0x10  /* Critical nesting and FPU context. */

    /* Set the PMR register to be correct for the current critical nesting
    depth. */
    LDR     X0, ullCriticalNestingConst /* X0 holds the address of ullCriticalNesting. */
    MOV     X1, #255                    /* X1 holds the unmask value. */
    CMP     X3, #0
    B.EQ    1f
    LDR     X6, ullMaxAPIPriorityMaskConst
    LDR     X1, [X6]                    /* X1 holds the mask value. */
1:
    MSR     s3_0_c4_c6_0, X1            /* Write the mask value to ICCPMR. s3_0_c4_c6_0 is ICC_PMR_EL1. */
    DSB     SY                          /* _RB_Barriers probably not required here. */
    ISB     SY
    STR     X3, [X0]                    /* Restore the task's critical nesting count. */

    /* Restore the FPU context indicator. */
    LDR     X0, ullPortTaskHasFPUContextConst
    STR     X2, [X0]

    /* Restore the FPU context, if any. */
    CMP     X2, #0
    B.EQ    1f
    LDP     Q30, Q31, [SP], #0x20
    LDP     Q28, Q29, [SP], #0x20
    LDP     Q26, Q27, [SP], #0x20
    LDP     Q24, Q25, [SP], #0x20
    LDP     Q22, Q23, [SP], #0x20
    LDP     Q20, Q21, [SP], #0x20
    LDP     Q18, Q19, [SP], #0x20
    LDP     Q16, Q17, [SP], #0x20
    LDP     Q14, Q15, [SP], #0x20
    LDP     Q12, Q13, [SP], #0x20
    LDP     Q10, Q11, [SP], #0x20
    LDP     Q8, Q9, [SP], #0x20
    LDP     Q6, Q7, [SP], #0x20
    LDP     Q4, Q5, [SP], #0x20
    LDP     Q2, Q3, [SP], #0x20
    LDP     Q0, Q1, [SP], #0x20
1:
    LDP     X2, X3, [SP], #0x10  /* SPSR and ELR. */

#if defined( GUEST )
    /* Restore the SPSR. */
    MSR     SPSR_EL1, X3
    /* Restore the ELR. */
    MSR     ELR_EL1, X2
#else
    /* Restore the SPSR. */
    MSR     SPSR_EL3, X3 /*_RB_ Assumes started in EL3. */
    /* Restore the ELR. */
    MSR     ELR_EL3, X2
#endif

    LDP     X30, XZR, [SP], #0x10
    LDP     X28, X29, [SP], #0x10
    LDP     X26, X27, [SP], #0x10
    LDP     X24, X25, [SP], #0x10
    LDP     X22, X23, [SP], #0x10
    LDP     X20, X21, [SP], #0x10
    LDP     X18, X19, [SP], #0x10
    LDP     X16, X17, [SP], #0x10
    LDP     X14, X15, [SP], #0x10
    LDP     X12, X13, [SP], #0x10
    LDP     X10, X11, [SP], #0x10
    LDP     X8, X9, [SP], #0x10
    LDP     X6, X7, [SP], #0x10
    LDP     X4, X5, [SP], #0x10
    LDP     X2, X3, [SP], #0x10
    LDP     X0, X1, [SP], #0x10

    /* Switch to use the ELx stack pointer.  _RB_ Might not be required. */
    MSR     SPSEL, #1

    ERET

    .endm


/******************************************************************************
 * FreeRTOS_SWI_Handler handler is used to perform a context switch.
 *****************************************************************************/
.align 8
.type FreeRTOS_SWI_Handler, %function
FreeRTOS_SWI_Handler:
    /* Save the context of the current task and select a new task to run. */
    portSAVE_CONTEXT
#if defined( GUEST )
    MRS     X0, ESR_EL1
#else
    MRS     X0, ESR_EL3
#endif

    LSR     X1, X0, #26

#if defined( GUEST )
    CMP     X1, #0x15   /* 0x15 = SVC instruction. */
#else
    CMP     X1, #0x17   /* 0x17 = SMC instruction. */
#endif
    B.NE    FreeRTOS_Abort
    MRS     x0, MPIDR_EL1       /* Get CoreID */
    AND     x0, x0, #0xff
    BL      vTaskSwitchContext

    portRESTORE_CONTEXT

FreeRTOS_Abort:
    /* Full ESR is in X0, exception class code is in X1. */
    B       .

/******************************************************************************
 * vPortRestoreTaskContext is used to start the scheduler.
 *****************************************************************************/
.align 8
.type vPortRestoreTaskContext, %function
vPortRestoreTaskContext:
.set freertos_vector_base,  _freertos_vector_table

    /* Install the FreeRTOS interrupt handlers. */
    LDR     X1, =freertos_vector_base
#if defined( GUEST )
    MSR     VBAR_EL1, X1
#else
    MSR     VBAR_EL3, X1
#endif
    DSB     SY
    ISB     SY

    /* Start the first task. */
    portRESTORE_CONTEXT


/******************************************************************************
 * FreeRTOS_IRQ_Handler handles IRQ entry and exit.

 * This handler is supposed to be used only for IRQs and never for FIQs. Per ARM
 * GIC documentation [1], Group 0 interrupts are always signaled as FIQs. Since
 * this handler is only for IRQs, We can safely assume Group 1 while accessing
 * Interrupt Acknowledge and End Of Interrupt registers and therefore, use
 * ICC_IAR1_EL1 and ICC_EOIR1_EL1.
 *
 * [1] https://developer.arm.com/documentation/198123/0300/Arm-CoreLink-GIC-fundamentals
 *****************************************************************************/
.align 8
.type FreeRTOS_IRQ_Handler, %function
FreeRTOS_IRQ_Handler:
    /* Save volatile registers. */
    STP     X0, X1, [SP, #-0x10]!
    STP     X2, X3, [SP, #-0x10]!
    STP     X4, X5, [SP, #-0x10]!
    STP     X6, X7, [SP, #-0x10]!
    STP     X8, X9, [SP, #-0x10]!
    STP     X10, X11, [SP, #-0x10]!
    STP     X12, X13, [SP, #-0x10]!
    STP     X14, X15, [SP, #-0x10]!
    STP     X16, X17, [SP, #-0x10]!
    STP     X18, X19, [SP, #-0x10]!
    STP     X29, X30, [SP, #-0x10]!

    /* Save the SPSR and ELR. */
#if defined( GUEST )
    MRS     X3, SPSR_EL1
    MRS     X2, ELR_EL1
#else
    MRS     X3, SPSR_EL3
    MRS     X2, ELR_EL3
#endif
    STP     X2, X3, [SP, #-0x10]!

    /* Increment the interrupt nesting counter. */
    LDR     X5, ullPortInterruptNestingConst

    /* Get coreId AND choose the corresponding index for core in ullPortInterruptNesting */
    MRS     X1, MPIDR_EL1
    AND     X1, X1, #0xff
    LSL     X1, X1, #3  /* X1 = X1 * 8 */
    ADD     X5, X5, X1

    LDR     X1, [X5]    /* Old nesting count in X1. */
    ADD     X6, X1, #1
    STR     X6, [X5]    /* Address of nesting count variable in X5. */

    /* Maintain the interrupt nesting information across the function call. */
    STP     X1, X5, [SP, #-0x10]!

    /* Call the C handler. */
    BL vApplicationIRQHandler

    /* Restore the critical nesting count. */
    LDP     X1, X5, [SP], #0x10
    STR     X1, [X5]

    /* Has interrupt nesting unwound? */
    CMP     X1, #0
    B.NE    Exit_IRQ_No_Context_Switch

    /* Is a context switch required? */
    LDR     X0, ullPortYieldRequiredConst

    /* Choose the corresponding index for core in ullPortYieldRequired */
    MRS     X1, MPIDR_EL1
    AND     X1, X1, #0xff
    LSL     X1, X1, #3  /* X1 = X1 * 8 */
    ADD     X0, X0, X1 

    LDR     X1, [X0]
    CMP     X1, #0
    B.EQ    Exit_IRQ_No_Context_Switch

    /* Reset ullPortYieldRequired to 0. */
    MOV     X2, #0
    STR     X2, [X0]

    /* Restore volatile registers. */
    LDP     X4, X5, [SP], #0x10  /* SPSR and ELR. */
#if defined( GUEST )
    MSR     SPSR_EL1, X5
    MSR     ELR_EL1, X4
#else
    MSR     SPSR_EL3, X5 /*_RB_ Assumes started in EL3. */
    MSR     ELR_EL3, X4
#endif
    DSB     SY
    ISB     SY

    LDP     X29, X30, [SP], #0x10
    LDP     X18, X19, [SP], #0x10
    LDP     X16, X17, [SP], #0x10
    LDP     X14, X15, [SP], #0x10
    LDP     X12, X13, [SP], #0x10
    LDP     X10, X11, [SP], #0x10
    LDP     X8, X9, [SP], #0x10
    LDP     X6, X7, [SP], #0x10
    LDP     X4, X5, [SP], #0x10
    LDP     X2, X3, [SP], #0x10
    LDP     X0, X1, [SP], #0x10

    /* Save the context of the current task and select a new task to run. */
    portSAVE_CONTEXT
    MRS     X0, MPIDR_EL1       /* Get CoreID */
    AND     X0, X0, #0xff
    BL vTaskSwitchContext
    portRESTORE_CONTEXT

Exit_IRQ_No_Context_Switch:
    /* Restore volatile registers. */
    LDP     X4, X5, [SP], #0x10  /* SPSR and ELR. */
#if defined( GUEST )
    MSR     SPSR_EL1, X5
    MSR     ELR_EL1, X4
#else
    MSR     SPSR_EL3, X5 /*_RB_ Assumes started in EL3. */
    MSR     ELR_EL3, X4
#endif
    DSB     SY
    ISB     SY

    LDP     X29, X30, [SP], #0x10
    LDP     X18, X19, [SP], #0x10
    LDP     X16, X17, [SP], #0x10
    LDP     X14, X15, [SP], #0x10
    LDP     X12, X13, [SP], #0x10
    LDP     X10, X11, [SP], #0x10
    LDP     X8, X9, [SP], #0x10
    LDP     X6, X7, [SP], #0x10
    LDP     X4, X5, [SP], #0x10
    LDP     X2, X3, [SP], #0x10
    LDP     X0, X1, [SP], #0x10

    ERET


/******************************************************************************
 * Vector table to use when FreeRTOS is running.
 *****************************************************************************/
.set	FREERTOS_VECTORS, (vectors+0x1000)

.org(FREERTOS_VECTORS)
.align	11
_freertos_vector_table:
	/* Current EL Synchronous exception with sp0 */
	.align	7
	b	FreeRTOS_SWI_Handler

	/* Current EL irq exception with sp0 */
	.align	7
	b	FreeRTOS_IRQ_Handler

	/* Current EL fiq exception with sp0 */
	.align	7
	b	FreeRTOS_IRQ_Handler

	/* Current EL Error with sp0 */
	.align	7
	/* store lr(x30) before entry exception */
	stp	x29, x30, [sp, #-16]!
	bl	_exception_entry
	bl	do_bad_error
	b	exception_exit

	/* Current EL (SP_ELx) Synchronous Handler */
	.align	7
	/* store lr(x30) before entry exception */
	b	FreeRTOS_SWI_Handler

	/* Current EL (SP_ELx) irq Handler */
	.align	7
	/* store lr(x30) before entry exception */
	b	FreeRTOS_IRQ_Handler

	 /* Current EL (SP_ELx) FIQ Handler */
	.align	7
	/* store lr(x30) before entry exception */
	b	FreeRTOS_IRQ_Handler

	/* Current EL (SP_ELx) Error Handler */
	.align	7
	/* store lr(x30) before entry exception */
	stp	x29, x30, [sp, #-16]!
	bl	_exception_entry
	bl	do_error
	b	exception_exit

	/* sync, irq,fiq,error for 64bit and 32bit with sp_lower */
	/* total: 128 x 8 = 1024 */
	/* not used */
	.align 10

/* save x0~x30 to sp */
/* return: x0=sp, x1=esr_eln,x2=elr_eln,(n=1,2,3)*/
_exception_entry:
	stp	x27, x28, [sp, #-16]!
	stp	x25, x26, [sp, #-16]!
	stp	x23, x24, [sp, #-16]!
	stp	x21, x22, [sp, #-16]!
	stp	x19, x20, [sp, #-16]!
	stp	x17, x18, [sp, #-16]!
	stp	x15, x16, [sp, #-16]!
	stp	x13, x14, [sp, #-16]!
	stp	x11, x12, [sp, #-16]!
	stp	x9, x10,  [sp, #-16]!
	stp	x7, x8,   [sp, #-16]!
	stp	x5, x6,   [sp, #-16]!
	stp	x3, x4,   [sp, #-16]!
	stp	x1, x2,   [sp, #-16]!
	b	_save_el_regs			/* jump to the second part */

/*
 * Save exception specific context: ESR and ELR,spsr for all exception levels.
 * This is the second part of the shared routine called into from all entries.
 */
_save_el_regs:
	switch_el x11, 3f, 2f, 1f
3:	mrs	x1, esr_el3
	mrs	x2, elr_el3
	mrs x3, spsr_el3
	b	0f
2:	mrs	x1, esr_el2
	mrs	x2, elr_el2
	mrs x3, spsr_el2
	b	0f
1:	mrs	x1, esr_el1
	mrs	x2, elr_el1
	mrs x3, spsr_el1
0:
	stp	x2, x0, [sp, #-16]!
	stp	x3, x1, [sp, #-16]!
	mov	x0, sp
	ret


/*
 * Restore the exception return address, for all exception levels.
 * This is the first part of the shared routine called into from all entries.
 */
exception_exit:
	ldp	x3, x1, [sp],#16
	ldp	x2, x0, [sp],#16
	switch_el x11, 3f, 2f, 1f
3:	msr	elr_el3, x2
	b	_restore_regs
2:	msr	elr_el2, x2
	b	_restore_regs
1:	msr	elr_el1, x2
	b	_restore_regs		/* jump to the second part */

/*
 * Restore the general purpose registers from the exception stack, then return.
 * This is the second part of the shared routine called into from all entries.
 */
_restore_regs:
	ldp	x1, x2,   [sp],#16
	ldp	x3, x4,   [sp],#16
	ldp	x5, x6,   [sp],#16
	ldp	x7, x8,   [sp],#16
	ldp	x9, x10,  [sp],#16
	ldp	x11, x12, [sp],#16
	ldp	x13, x14, [sp],#16
	ldp	x15, x16, [sp],#16
	ldp	x17, x18, [sp],#16
	ldp	x19, x20, [sp],#16
	ldp	x21, x22, [sp],#16
	ldp	x23, x24, [sp],#16
	ldp	x25, x26, [sp],#16
	ldp	x27, x28, [sp],#16
	ldp	x29, x30, [sp],#16
	eret

/*
 *  int32_t lSpinTrylock(uintptr_t ulGateWord);
 */
    .global lSpinTrylock
    .type lSpinTrylock  , %function
lSpinTrylock:
    LDXR    W1, [X0]
    CMP     W1, #1      /* is locked already? */
    BEQ     1f          /* if so, leave with fail */

    MOV     W2, #1      /* locked = 1 */
    STXR    W1, W2, [X0]/* if so attempt to grab it */
    CMP     W1, #0      /* did we get it? zero is yes */

    /* if not, loop while in contention */
    BNE     lSpinTrylock
1:
    MOV     W0, W1
    DMB     SY
    RET

/*
 *  void vSpinUnlock(uintptr_t ulGateWord);
 */
    .global vSpinUnlock
    .type vSpinUnlock  , %function
vSpinUnlock:
    DMB     SY

    MOV     W1, #0
    STR     W1, [X0]

    DSB     SY
    SEV         /* let everyone know */
    RET



.align 8
ullCriticalNestingConst: .dword ullCriticalNesting
ullPortTaskHasFPUContextConst: .dword ullPortTaskHasFPUContext

ullMaxAPIPriorityMaskConst: .dword ullMaxAPIPriorityMask
ullPortInterruptNestingConst: .dword ullPortInterruptNesting
ullPortYieldRequiredConst: .dword ullPortYieldRequired

#ifdef SMP

pxCurrentTCBConst: .dword pxCurrentTCBs

#else

pxCurrentTCBConst: .dword pxCurrentTCB

#endif

.end
