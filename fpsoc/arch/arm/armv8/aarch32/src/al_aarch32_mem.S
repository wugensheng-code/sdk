/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <al_aarch32_sysreg.h>
#include <asm_macros_common.S>

.globl clear_mem
/* -----------------------------------------------------------------------
 * void zeromem(void *mem, unsigned int length)
 *
 * Initialise a region in normal memory to 0. This functions complies with the
 * AAPCS and can be called from C code.
 *
 * -----------------------------------------------------------------------
 */
func clear_mem
	/*
	 * Readable names for registers
	 *
	 * Registers r0, r1 and r2 are also set by zeromem which
	 * branches into the fallback path directly, so cursor, length and
	 * stop_address should not be retargeted to other registers.
	 */
	cursor       .req r0 /* Start address and then current address */
	length       .req r1 /* Length in bytes of the region to zero out */
	/*
	 * Reusing the r1 register as length is only used at the beginning of
	 * the function.
	 */
	stop_address .req r1  /* Address past the last zeroed byte */
	zeroreg1     .req r2  /* Source register filled with 0 */
	zeroreg2     .req r3  /* Source register filled with 0 */
	tmp	     .req r12 /* Temporary scratch register */

	mov	zeroreg1, #0

	/* stop_address is the address past the last to zero */
	add	stop_address, cursor, length

	/*
	 * Length cannot be used anymore as it shares the same register with
	 * stop_address.
	 */
	.unreq	length

	/*
	 * If the start address is already aligned to 8 bytes, skip this loop.
	 */
	tst	cursor, #(8-1)
	beq	.Lzeromem_8bytes_aligned

	/* Calculate the next address aligned to 8 bytes */
	orr	tmp, cursor, #(8-1)
	adds	tmp, tmp, #1
	/* If it overflows, fallback to byte per byte zeroing */
	beq	.Lzeromem_1byte_aligned
	/* If the next aligned address is after the stop address, fall back */
	cmp	tmp, stop_address
	bhs	.Lzeromem_1byte_aligned

	/* zero byte per byte */
1:
	strb	zeroreg1, [cursor], #1
	cmp	cursor, tmp
	bne	1b

	/* zero 8 bytes at a time */
.Lzeromem_8bytes_aligned:

	/* Calculate the last 8 bytes aligned address. */
	bic	tmp, stop_address, #(8-1)

	cmp	cursor, tmp
	bhs	2f

	mov	zeroreg2, #0
1:
	stmia	cursor!, {zeroreg1, zeroreg2}
	cmp	cursor, tmp
	blo	1b
2:

	/* zero byte per byte */
.Lzeromem_1byte_aligned:
	cmp	cursor, stop_address
	beq	2f
1:
	strb	zeroreg1, [cursor], #1
	cmp	cursor, stop_address
	bne	1b
2:
	bx	lr

	.unreq	cursor
	/*
	 * length is already unreq'ed to reuse the register for another
	 * variable.
	 */
	.unreq	stop_address
	.unreq	zeroreg1
	.unreq	zeroreg2
	.unreq	tmp
endfunc clear_mem


#if 0
	.macro el3_arch_init_common
	/* ---------------------------------------------------------------------
	 * SCTLR has already been initialised - read current value before
	 * modifying.
	 *
	 * SCTLR.I: Enable the instruction cache.
	 *
	 * SCTLR.A: Enable Alignment fault checking. All instructions that load
	 *  or store one or more registers have an alignment check that the
	 *  address being accessed is aligned to the size of the data element(s)
	 *  being accessed.
	 * ---------------------------------------------------------------------
	 */
	ldr	r1, =(SCTLR_I_BIT | SCTLR_A_BIT)
	ldcopr	r0, SCTLR
	orr	r0, r0, r1
	stcopr	r0, SCTLR
	isb

	/* ---------------------------------------------------------------------
	 * Initialise SCR, setting all fields rather than relying on the hw.
	 *
	 * SCR.SIF: Enabled so that Secure state instruction fetches from
	 *  Non-secure memory are not permitted.
	 * ---------------------------------------------------------------------
	 * ldr	r0, =(SCR_RESET_VAL | SCR_SIF_BIT)
	 * stcopr	r0, SCR
	 * ---------------------------------------------------------------------
	 */

	/* -----------------------------------------------------
	 * Enable the Asynchronous data abort now that the
	 * exception vectors have been setup.
	 * -----------------------------------------------------
	 * cpsie   a
	 * isb
	 * -----------------------------------------------------
	 */

	/* ---------------------------------------------------------------------
	 * Initialise NSACR, setting all the fields, except for the
	 * IMPLEMENTATION DEFINED field, rather than relying on the hw. Some
	 * fields are architecturally UNKNOWN on reset.
	 *
	 * NSACR_ENABLE_FP_ACCESS: Represents NSACR.cp11 and NSACR.cp10. The
	 *  cp11 field is ignored, but is set to same value as cp10. The cp10
	 *  field is set to allow access to Advanced SIMD and floating point
	 *  features from both Security states.
	 * ---------------------------------------------------------------------
	 */

	/*
	 *---------------------------------------------------------------------
	 * Need to set NSACR in boot
	 *---------------------------------------------------------------------
	 * ldcopr	r0, NSACR
	 * and	r0, r0, #NSACR_IMP_DEF_MASK
	 * orr	r0, r0, #(NSACR_RESET_VAL | NSACR_ENABLE_FP_ACCESS)
	 * stcopr	r0, NSACR
	 * isb
	*/

	/* ---------------------------------------------------------------------
	 * Initialise CPACR, setting all fields rather than relying on hw. Some
	 * fields are architecturally UNKNOWN on reset.
	 *
	 * CPACR.TRCDIS: Trap control for PL0 and PL1 System register accesses
	 *  to trace registers. Set to zero to allow access.
	 *
	 * CPACR_ENABLE_FP_ACCESS: Represents CPACR.cp11 and CPACR.cp10. The
	 *  cp11 field is ignored, but is set to same value as cp10. The cp10
	 *  field is set to allow full access from PL0 and PL1 to floating-point
	 *  and Advanced SIMD features.
	 * ---------------------------------------------------------------------
	 * ldr	r0, =((CPACR_RESET_VAL | CPACR_ENABLE_FP_ACCESS) & ~(TRCDIS_BIT))
	 * stcopr	r0, CPACR
	 * isb
	 * ---------------------------------------------------------------------
	 */

	/* ---------------------------------------------------------------------
	 * Initialise FPEXC, setting all fields rather than relying on hw. Some
	 * fields are architecturally UNKNOWN on reset and are set to zero
	 * except for field(s) listed below.
	 *
	 * FPEXC.EN: Enable access to Advanced SIMD and floating point features
	 *  from all exception levels.
         *
         * __SOFTFP__: Predefined macro exposed by soft-float toolchain.
         *  ARMv7 and Cortex-A32(ARMv8/aarch32) has both soft-float and
         *  hard-float variants of toolchain, avoid compiling below code with
         *  soft-float toolchain as "vmsr" instruction will not be recognized.
	 * ---------------------------------------------------------------------
	 * #if ((ARM_ARCH_MAJOR > 7) || defined(ARMV7_SUPPORTS_VFP)) && !(__SOFTFP__)
	 * 	ldr	r0, =(FPEXC_RESET_VAL | FPEXC_EN_BIT)
	 * 	vmsr	FPEXC, r0
	 * 	isb
	 * ---------------------------------------------------------------------
	 * #endif
	 */

#if (ARM_ARCH_MAJOR > 7)
	/* ---------------------------------------------------------------------
	 * Initialise SDCR, setting all the fields rather than relying on hw.
	 *
	 * SDCR.SPD: Disable AArch32 privileged debug. Debug exceptions from
	 *  Secure EL1 are disabled.
	 *
	 * SDCR.SCCD: Set to one so that cycle counting by PMCCNTR is prohibited
	 *  in Secure state. This bit is RES0 in versions of the architecture
	 *  earlier than ARMv8.5, setting it to 1 doesn't have any effect on
	 *  them.
	 * ---------------------------------------------------------------------
	 */
	ldr	r0, =(SDCR_RESET_VAL | SDCR_SPD(SDCR_SPD_DISABLE) | SDCR_SCCD_BIT)
	stcopr	r0, SDCR

	/* ---------------------------------------------------------------------
	 * Initialise PMCR, setting all fields rather than relying
	 * on hw. Some fields are architecturally UNKNOWN on reset.
	 *
	 * PMCR.LP: Set to one so that event counter overflow, that
	 *  is recorded in PMOVSCLR[0-30], occurs on the increment
	 *  that changes PMEVCNTR<n>[63] from 1 to 0, when ARMv8.5-PMU
	 *  is implemented. This bit is RES0 in versions of the architecture
	 *  earlier than ARMv8.5, setting it to 1 doesn't have any effect
	 *  on them.
	 *  This bit is Reserved, UNK/SBZP in ARMv7.
	 *
	 * PMCR.LC: Set to one so that cycle counter overflow, that
	 *  is recorded in PMOVSCLR[31], occurs on the increment
	 *  that changes PMCCNTR[63] from 1 to 0.
	 *  This bit is Reserved, UNK/SBZP in ARMv7.
	 *
	 * PMCR.DP: Set to one to prohibit cycle counting whilst in Secure mode.
	 * ---------------------------------------------------------------------
	 */
	ldr	r0, =(PMCR_RESET_VAL | PMCR_DP_BIT | PMCR_LC_BIT | \
		      PMCR_LP_BIT)
#else
	ldr	r0, =(PMCR_RESET_VAL | PMCR_DP_BIT)
#endif
	stcopr	r0, PMCR

	/*
	 * If Data Independent Timing (DIT) functionality is implemented,
	 * always enable DIT in EL3
	 * -----------------------------------------------------------------------------
	 *	ldcopr	r0, ID_PFR0
	 *	and	r0, r0, #(ID_PFR0_DIT_MASK << ID_PFR0_DIT_SHIFT)
	 *	cmp	r0, #ID_PFR0_DIT_SUPPORTED
	 *	bne	1f
	 *	mrs	r0, cpsr
	 *	orr	r0, r0, #CPSR_DIT_BIT
	 *	msr	cpsr_cxsf, r0
	 * 1:
	 * -----------------------------------------------------------------------------
	 */

	.endm

#endif