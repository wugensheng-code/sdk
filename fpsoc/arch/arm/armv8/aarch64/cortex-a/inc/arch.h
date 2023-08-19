/*
 * Copyright (c) 2013-2022, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ARCH_HELPERS_H
#define ARCH_HELPERS_H

#include <cdefs.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "type.h"
#include "sysreg.h"
#include "al_core.h"
#include "barriers.h"

/**********************************************************************
 * Macros which create inline functions to read or write CPU system
 * registers
 *********************************************************************/

#define _DEFINE_SYSREG_READ_FUNC(_name, _reg_name)		\
static inline u_register_t read_ ## _name(void)			\
{								\
	u_register_t v;						\
	__asm__ volatile ("mrs %0, " #_reg_name : "=r" (v));	\
	return v;						\
}

#define _DEFINE_SYSREG_READ_FUNC_NV(_name, _reg_name)		\
static inline u_register_t read_ ## _name(void)			\
{								\
	u_register_t v;						\
	__asm__ ("mrs %0, " #_reg_name : "=r" (v));		\
	return v;						\
}

#define _DEFINE_SYSREG_WRITE_FUNC(_name, _reg_name)			\
static inline void write_ ## _name(u_register_t v)			\
{									\
	__asm__ volatile ("msr " #_reg_name ", %0" : : "r" (v));	\
}

#define SYSREG_WRITE_CONST(reg_name, v)				\
	__asm__ volatile ("msr " #reg_name ", %0" : : "i" (v))

/* Define read function for system register */
#define DEFINE_SYSREG_READ_FUNC(_name) 			\
	_DEFINE_SYSREG_READ_FUNC(_name, _name)

/* Define read & write function for system register */
#define DEFINE_SYSREG_RW_FUNCS(_name)			\
	_DEFINE_SYSREG_READ_FUNC(_name, _name)		\
	_DEFINE_SYSREG_WRITE_FUNC(_name, _name)

/* Define read & write function for renamed system register */
#define DEFINE_RENAME_SYSREG_RW_FUNCS(_name, _reg_name)	\
	_DEFINE_SYSREG_READ_FUNC(_name, _reg_name)	\
	_DEFINE_SYSREG_WRITE_FUNC(_name, _reg_name)

/* Define read function for renamed system register */
#define DEFINE_RENAME_SYSREG_READ_FUNC(_name, _reg_name)	\
	_DEFINE_SYSREG_READ_FUNC(_name, _reg_name)

/* Define write function for renamed system register */
#define DEFINE_RENAME_SYSREG_WRITE_FUNC(_name, _reg_name)	\
	_DEFINE_SYSREG_WRITE_FUNC(_name, _reg_name)

/* Define read function for ID register (w/o volatile qualifier) */
#define DEFINE_IDREG_READ_FUNC(_name)			\
	_DEFINE_SYSREG_READ_FUNC_NV(_name, _name)

/* Define read function for renamed ID register (w/o volatile qualifier) */
#define DEFINE_RENAME_IDREG_READ_FUNC(_name, _reg_name)	\
	_DEFINE_SYSREG_READ_FUNC_NV(_name, _reg_name)

/**********************************************************************
 * Macros to create inline functions for system instructions
 *********************************************************************/

/* Define function for simple system instruction */
#define DEFINE_SYSOP_FUNC(_op)				\
static inline void _op(void)				\
{							\
	__asm__ (#_op);					\
}

/* Define function for system instruction with register parameter */
#define DEFINE_SYSOP_PARAM_FUNC(_op)			\
static inline void _op(uint64_t v)			\
{							\
	 __asm__ (#_op "  %0" : : "r" (v));		\
}

/* Define function for system instruction with type specifier */
#define DEFINE_SYSOP_TYPE_FUNC(_op, _type)		\
static inline void _op ## _type(void)			\
{							\
	__asm__ (#_op " " #_type : : : "memory");			\
}

/* Define function for system instruction with register parameter */
#define DEFINE_SYSOP_TYPE_PARAM_FUNC(_op, _type)	\
static inline void _op ## _type(uint64_t v)		\
{							\
	 __asm__ (#_op " " #_type ", %0" : : "r" (v));	\
}

/*******************************************************************************
 * TLB maintenance accessor prototypes
 ******************************************************************************/

#if ERRATA_A53_819472 || ERRATA_A53_824069 || ERRATA_A53_827319
/*
 * Define function for DC instruction with register parameter that enables
 * the workaround for errata 819472, 824069 and 827319 of Cortex-A53.
 */
#define DEFINE_DCOP_ERRATA_A53_TYPE_PARAM_FUNC(_name, _type)	\
static inline void dc ## _name(uint64_t v)			\
{								\
	__asm__("dc " #_type ", %0" : : "r" (v));		\
}
#endif /* ERRATA_A53_819472 || ERRATA_A53_824069 || ERRATA_A53_827319 */

DEFINE_SYSOP_TYPE_FUNC(tlbi, alle1)
DEFINE_SYSOP_TYPE_FUNC(tlbi, alle1is)
DEFINE_SYSOP_TYPE_FUNC(tlbi, alle2)
DEFINE_SYSOP_TYPE_FUNC(tlbi, alle2is)
DEFINE_SYSOP_TYPE_FUNC(tlbi, alle3)
DEFINE_SYSOP_TYPE_FUNC(tlbi, alle3is)
DEFINE_SYSOP_TYPE_FUNC(tlbi, vmalle1)

DEFINE_SYSOP_TYPE_PARAM_FUNC(tlbi, vaae1is)
DEFINE_SYSOP_TYPE_PARAM_FUNC(tlbi, vaale1is)
DEFINE_SYSOP_TYPE_PARAM_FUNC(tlbi, vae2is)
DEFINE_SYSOP_TYPE_PARAM_FUNC(tlbi, vale2is)
DEFINE_SYSOP_TYPE_PARAM_FUNC(tlbi, vae3is)
DEFINE_SYSOP_TYPE_PARAM_FUNC(tlbi, vale3is)

/*******************************************************************************
 * Cache maintenance accessor prototypes
 ******************************************************************************/
DEFINE_SYSOP_TYPE_PARAM_FUNC(dc, isw)
DEFINE_SYSOP_TYPE_PARAM_FUNC(dc, cisw)
#if ERRATA_A53_827319
DEFINE_DCOP_ERRATA_A53_TYPE_PARAM_FUNC(csw, cisw)
#else
DEFINE_SYSOP_TYPE_PARAM_FUNC(dc, csw)
#endif
#if ERRATA_A53_819472 || ERRATA_A53_824069 || ERRATA_A53_827319
DEFINE_DCOP_ERRATA_A53_TYPE_PARAM_FUNC(cvac, civac)
#else
DEFINE_SYSOP_TYPE_PARAM_FUNC(dc, cvac)
#endif
DEFINE_SYSOP_TYPE_PARAM_FUNC(dc, ivac)
DEFINE_SYSOP_TYPE_PARAM_FUNC(dc, civac)
#if ERRATA_A53_819472 || ERRATA_A53_824069 || ERRATA_A53_827319
DEFINE_DCOP_ERRATA_A53_TYPE_PARAM_FUNC(cvau, civac)
#else
DEFINE_SYSOP_TYPE_PARAM_FUNC(dc, cvau)
#endif
DEFINE_SYSOP_TYPE_PARAM_FUNC(dc, zva)

/*******************************************************************************
 * Address translation accessor prototypes
 ******************************************************************************/
DEFINE_SYSOP_TYPE_PARAM_FUNC(at, s12e1r)
DEFINE_SYSOP_TYPE_PARAM_FUNC(at, s12e1w)
DEFINE_SYSOP_TYPE_PARAM_FUNC(at, s12e0r)
DEFINE_SYSOP_TYPE_PARAM_FUNC(at, s12e0w)
DEFINE_SYSOP_TYPE_PARAM_FUNC(at, s1e1r)
DEFINE_SYSOP_TYPE_PARAM_FUNC(at, s1e2r)
DEFINE_SYSOP_TYPE_PARAM_FUNC(at, s1e3r)

/*******************************************************************************
 * Strip Pointer Authentication Code
 ******************************************************************************/
DEFINE_SYSOP_PARAM_FUNC(xpaci)

void dcsw_op_louis(u_register_t op_type);
void dcsw_op_all(u_register_t op_type);

void disable_mmu_el1(void);
void disable_mmu_el3(void);
void disable_mpu_el2(void);
void disable_mmu_icache_el1(void);
void disable_mmu_icache_el3(void);
void disable_mpu_icache_el2(void);

/*******************************************************************************
 * Misc. accessor prototypes
 ******************************************************************************/

#define write_daifclr(val) SYSREG_WRITE_CONST(daifclr, val)
#define write_daifset(val) SYSREG_WRITE_CONST(daifset, val)

DEFINE_SYSREG_RW_FUNCS(par_el1)
DEFINE_IDREG_READ_FUNC(id_pfr1_el1)
DEFINE_IDREG_READ_FUNC(id_aa64isar0_el1)
DEFINE_IDREG_READ_FUNC(id_aa64isar1_el1)
DEFINE_RENAME_IDREG_READ_FUNC(id_aa64isar2_el1, ID_AA64ISAR2_EL1)
DEFINE_IDREG_READ_FUNC(id_aa64pfr0_el1)
DEFINE_IDREG_READ_FUNC(id_aa64pfr1_el1)
DEFINE_IDREG_READ_FUNC(id_aa64dfr0_el1)
DEFINE_IDREG_READ_FUNC(id_afr0_el1)
DEFINE_SYSREG_READ_FUNC(CurrentEl)
DEFINE_SYSREG_READ_FUNC(ctr_el0)
DEFINE_SYSREG_RW_FUNCS(daif)
DEFINE_SYSREG_RW_FUNCS(spsr_el1)
DEFINE_SYSREG_RW_FUNCS(spsr_el2)
DEFINE_SYSREG_RW_FUNCS(spsr_el3)
DEFINE_SYSREG_RW_FUNCS(elr_el1)
DEFINE_SYSREG_RW_FUNCS(elr_el2)
DEFINE_SYSREG_RW_FUNCS(elr_el3)
DEFINE_SYSREG_RW_FUNCS(mdccsr_el0)
DEFINE_SYSREG_RW_FUNCS(dbgdtrrx_el0)
DEFINE_SYSREG_RW_FUNCS(dbgdtrtx_el0)
DEFINE_SYSREG_RW_FUNCS(sp_el1)
DEFINE_SYSREG_RW_FUNCS(sp_el2)

static inline void enable_irq(void)
{
	/*
	 * The compiler memory barrier will prevent the compiler from
	 * scheduling non-volatile memory access after the write to the
	 * register.
	 *
	 * This could happen if some initialization code issues non-volatile
	 * accesses to an area used by an interrupt handler, in the assumption
	 * that it is safe as the interrupts are disabled at the time it does
	 * that (according to program order). However, non-volatile accesses
	 * are not necessarily in program order relatively with volatile inline
	 * assembly statements (and volatile accesses).
	 */
	__COMPILER_BARRIER();
	write_daifclr(DAIF_IRQ_BIT);
	isb();
}

static inline void enable_fiq(void)
{
	__COMPILER_BARRIER();
	write_daifclr(DAIF_FIQ_BIT);
	isb();
}

static inline void enable_all_intr(void)
{
	__COMPILER_BARRIER();
	write_daifclr(DAIF_IRQ_BIT | DAIF_FIQ_BIT);
	isb();
}

static inline void enable_serror(void)
{
	__COMPILER_BARRIER();
	write_daifclr(DAIF_ABT_BIT);
	isb();
}

static inline void enable_debug_exceptions(void)
{
	__COMPILER_BARRIER();
	write_daifclr(DAIF_DBG_BIT);
	isb();
}

static inline void disable_irq(void)
{
	__COMPILER_BARRIER();
	write_daifset(DAIF_IRQ_BIT);
	isb();
}

static inline void disable_fiq(void)
{
	__COMPILER_BARRIER();
	write_daifset(DAIF_FIQ_BIT);
	isb();
}

static inline void disable_all_intr(void)
{
	__COMPILER_BARRIER();
	write_daifset(DAIF_IRQ_BIT | DAIF_FIQ_BIT);
	isb();
}

static inline void disable_serror(void)
{
	__COMPILER_BARRIER();
	write_daifset(DAIF_ABT_BIT);
	isb();
}

static inline void disable_debug_exceptions(void)
{
	__COMPILER_BARRIER();
	write_daifset(DAIF_DBG_BIT);
	isb();
}

void __dead2 smc(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3,
		 uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7);

/*******************************************************************************
 * System register accessor prototypes
 ******************************************************************************/
DEFINE_IDREG_READ_FUNC(midr_el1)
DEFINE_SYSREG_READ_FUNC(mpidr_el1)
DEFINE_IDREG_READ_FUNC(id_aa64mmfr0_el1)
DEFINE_IDREG_READ_FUNC(id_aa64mmfr1_el1)

DEFINE_SYSREG_RW_FUNCS(scr_el3)
DEFINE_SYSREG_RW_FUNCS(hcr_el2)

DEFINE_SYSREG_RW_FUNCS(vbar_el1)
DEFINE_SYSREG_RW_FUNCS(vbar_el2)
DEFINE_SYSREG_RW_FUNCS(vbar_el3)

DEFINE_SYSREG_RW_FUNCS(sctlr_el1)
DEFINE_SYSREG_RW_FUNCS(sctlr_el2)
DEFINE_SYSREG_RW_FUNCS(sctlr_el3)

DEFINE_SYSREG_RW_FUNCS(actlr_el1)
DEFINE_SYSREG_RW_FUNCS(actlr_el2)
DEFINE_SYSREG_RW_FUNCS(actlr_el3)

DEFINE_SYSREG_RW_FUNCS(esr_el1)
DEFINE_SYSREG_RW_FUNCS(esr_el2)
DEFINE_SYSREG_RW_FUNCS(esr_el3)

DEFINE_SYSREG_RW_FUNCS(afsr0_el1)
DEFINE_SYSREG_RW_FUNCS(afsr0_el2)
DEFINE_SYSREG_RW_FUNCS(afsr0_el3)

DEFINE_SYSREG_RW_FUNCS(afsr1_el1)
DEFINE_SYSREG_RW_FUNCS(afsr1_el2)
DEFINE_SYSREG_RW_FUNCS(afsr1_el3)

DEFINE_SYSREG_RW_FUNCS(far_el1)
DEFINE_SYSREG_RW_FUNCS(far_el2)
DEFINE_SYSREG_RW_FUNCS(far_el3)

DEFINE_SYSREG_RW_FUNCS(mair_el1)
DEFINE_SYSREG_RW_FUNCS(mair_el2)
DEFINE_SYSREG_RW_FUNCS(mair_el3)

DEFINE_SYSREG_RW_FUNCS(amair_el1)
DEFINE_SYSREG_RW_FUNCS(amair_el2)
DEFINE_SYSREG_RW_FUNCS(amair_el3)

DEFINE_SYSREG_READ_FUNC(rvbar_el1)
DEFINE_SYSREG_READ_FUNC(rvbar_el2)
DEFINE_SYSREG_READ_FUNC(rvbar_el3)

DEFINE_SYSREG_RW_FUNCS(rmr_el1)
DEFINE_SYSREG_RW_FUNCS(rmr_el2)
DEFINE_SYSREG_RW_FUNCS(rmr_el3)

DEFINE_SYSREG_RW_FUNCS(tcr_el1)
DEFINE_SYSREG_RW_FUNCS(tcr_el2)
DEFINE_SYSREG_RW_FUNCS(tcr_el3)

DEFINE_SYSREG_RW_FUNCS(ttbr0_el1)
DEFINE_SYSREG_RW_FUNCS(ttbr0_el2)
DEFINE_SYSREG_RW_FUNCS(ttbr0_el3)

DEFINE_SYSREG_RW_FUNCS(ttbr1_el1)

DEFINE_SYSREG_RW_FUNCS(vttbr_el2)

DEFINE_SYSREG_RW_FUNCS(cptr_el2)
DEFINE_SYSREG_RW_FUNCS(cptr_el3)

DEFINE_SYSREG_RW_FUNCS(cpacr_el1)
DEFINE_SYSREG_RW_FUNCS(cntfrq_el0)
DEFINE_SYSREG_RW_FUNCS(cnthp_ctl_el2)
DEFINE_SYSREG_RW_FUNCS(cnthp_tval_el2)
DEFINE_SYSREG_RW_FUNCS(cnthp_cval_el2)
DEFINE_SYSREG_RW_FUNCS(cntps_ctl_el1)
DEFINE_SYSREG_RW_FUNCS(cntps_tval_el1)
DEFINE_SYSREG_RW_FUNCS(cntps_cval_el1)
DEFINE_SYSREG_RW_FUNCS(cntp_ctl_el0)
DEFINE_SYSREG_RW_FUNCS(cntp_tval_el0)
DEFINE_SYSREG_RW_FUNCS(cntp_cval_el0)
DEFINE_SYSREG_READ_FUNC(cntpct_el0)
DEFINE_SYSREG_RW_FUNCS(cnthctl_el2)

DEFINE_SYSREG_RW_FUNCS(vtcr_el2)

#define get_cntp_ctl_enable(x)  (((x) >> CNTP_CTL_ENABLE_SHIFT) & \
					CNTP_CTL_ENABLE_MASK)
#define get_cntp_ctl_imask(x)   (((x) >> CNTP_CTL_IMASK_SHIFT) & \
					CNTP_CTL_IMASK_MASK)
#define get_cntp_ctl_istatus(x) (((x) >> CNTP_CTL_ISTATUS_SHIFT) & \
					CNTP_CTL_ISTATUS_MASK)

#define set_cntp_ctl_enable(x)  ((x) |= (U(1) << CNTP_CTL_ENABLE_SHIFT))
#define set_cntp_ctl_imask(x)   ((x) |= (U(1) << CNTP_CTL_IMASK_SHIFT))

#define clr_cntp_ctl_enable(x)  ((x) &= ~(U(1) << CNTP_CTL_ENABLE_SHIFT))
#define clr_cntp_ctl_imask(x)   ((x) &= ~(U(1) << CNTP_CTL_IMASK_SHIFT))

DEFINE_SYSREG_RW_FUNCS(tpidr_el3)

DEFINE_SYSREG_RW_FUNCS(cntvoff_el2)

DEFINE_SYSREG_RW_FUNCS(vpidr_el2)
DEFINE_SYSREG_RW_FUNCS(vmpidr_el2)

DEFINE_SYSREG_READ_FUNC(isr_el1)

DEFINE_SYSREG_RW_FUNCS(mdcr_el2)
DEFINE_SYSREG_RW_FUNCS(mdcr_el3)
DEFINE_SYSREG_RW_FUNCS(hstr_el2)
DEFINE_SYSREG_RW_FUNCS(pmcr_el0)

/* GICv3 System Registers */

DEFINE_RENAME_SYSREG_RW_FUNCS(icc_sre_el1, S3_0_C12_C12_5)
DEFINE_RENAME_SYSREG_RW_FUNCS(icc_sre_el2, S3_4_C12_C9_5)
DEFINE_RENAME_SYSREG_RW_FUNCS(icc_sre_el3, S3_6_C12_C12_5)
DEFINE_RENAME_SYSREG_RW_FUNCS(icc_pmr_el1, S3_0_C4_C6_0)
DEFINE_RENAME_SYSREG_READ_FUNC(icc_rpr_el1, S3_0_C12_C11_3)
DEFINE_RENAME_SYSREG_RW_FUNCS(icc_igrpen1_el3, S3_6_c12_c12_7)
DEFINE_RENAME_SYSREG_RW_FUNCS(icc_igrpen1_el1, S3_0_C12_C12_7)
DEFINE_RENAME_SYSREG_RW_FUNCS(icc_igrpen0_el1, S3_0_c12_c12_6)
DEFINE_RENAME_SYSREG_READ_FUNC(icc_hppir0_el1, S3_0_c12_c8_2)
DEFINE_RENAME_SYSREG_READ_FUNC(icc_hppir1_el1, S3_0_c12_c12_2)
DEFINE_RENAME_SYSREG_READ_FUNC(icc_iar0_el1, S3_0_c12_c8_0)
DEFINE_RENAME_SYSREG_READ_FUNC(icc_iar1_el1, S3_0_c12_c12_0)
DEFINE_RENAME_SYSREG_WRITE_FUNC(icc_eoir0_el1, S3_0_c12_c8_1)
DEFINE_RENAME_SYSREG_WRITE_FUNC(icc_eoir1_el1, S3_0_c12_c12_1)
DEFINE_RENAME_SYSREG_WRITE_FUNC(icc_sgi0r_el1, S3_0_c12_c11_7)
DEFINE_RENAME_SYSREG_RW_FUNCS(icc_sgi1r, S3_0_C12_C11_5)
DEFINE_RENAME_SYSREG_RW_FUNCS(icc_asgi1r, S3_0_C12_C11_6)
DEFINE_RENAME_SYSREG_RW_FUNCS(icc_bpr1_el1, S3_0_c12_c12_3)
DEFINE_RENAME_SYSREG_RW_FUNCS(icc_ctlr_el1, S3_0_C12_C12_4)

DEFINE_RENAME_SYSREG_READ_FUNC(amcfgr_el0, S3_3_C13_C2_1)
DEFINE_RENAME_SYSREG_READ_FUNC(amcgcr_el0, S3_3_C13_C2_2)
DEFINE_RENAME_SYSREG_READ_FUNC(amcg1idr_el0, S3_3_C13_C2_6)
DEFINE_RENAME_SYSREG_RW_FUNCS(amcr_el0, S3_3_C13_C2_0)
DEFINE_RENAME_SYSREG_RW_FUNCS(amcntenclr0_el0, S3_3_C13_C2_4)
DEFINE_RENAME_SYSREG_RW_FUNCS(amcntenset0_el0, S3_3_C13_C2_5)
DEFINE_RENAME_SYSREG_RW_FUNCS(amcntenclr1_el0, S3_3_C13_C3_0)
DEFINE_RENAME_SYSREG_RW_FUNCS(amcntenset1_el0, S3_3_C13_C3_1)

DEFINE_RENAME_SYSREG_RW_FUNCS(pmblimitr_el1, S3_0_C9_C10_0)

DEFINE_RENAME_SYSREG_WRITE_FUNC(zcr_el3, S3_6_C1_C2_0)
DEFINE_RENAME_SYSREG_WRITE_FUNC(zcr_el2, S3_4_C1_C2_0)

DEFINE_RENAME_IDREG_READ_FUNC(id_aa64smfr0_el1, S3_0_C0_C4_5)
DEFINE_RENAME_SYSREG_RW_FUNCS(smcr_el3, S3_6_C1_C2_6)

DEFINE_RENAME_SYSREG_READ_FUNC(erridr_el1, S3_0_C5_C3_0)
DEFINE_RENAME_SYSREG_WRITE_FUNC(errselr_el1, S3_0_C5_C3_1)

DEFINE_RENAME_SYSREG_READ_FUNC(erxfr_el1, S3_0_C5_C4_0)
DEFINE_RENAME_SYSREG_RW_FUNCS(erxctlr_el1, S3_0_C5_C4_1)
DEFINE_RENAME_SYSREG_RW_FUNCS(erxstatus_el1, S3_0_C5_C4_2)
DEFINE_RENAME_SYSREG_READ_FUNC(erxaddr_el1, S3_0_C5_C4_3)
DEFINE_RENAME_SYSREG_READ_FUNC(erxmisc0_el1, S3_0_C5_C5_0)
DEFINE_RENAME_SYSREG_READ_FUNC(erxmisc1_el1, S3_0_C5_C5_1)

/* FEAT_TCR2 Register */
DEFINE_RENAME_SYSREG_RW_FUNCS(tcr2_el2, S3_4_C2_C0_3)

/* DynamIQ Shared Unit power management */
DEFINE_RENAME_SYSREG_RW_FUNCS(clusterpwrdn_el1, S3_0_c15_c3_6)

/* CPU Power/Performance Management registers */
DEFINE_RENAME_SYSREG_RW_FUNCS(cpuppmcr_el3, S3_6_C15_C2_0)
DEFINE_RENAME_SYSREG_RW_FUNCS(cpumpmmcr_el3, S3_6_C15_C2_1)

#define IS_IN_EL(x) \
	(GET_EL(read_CurrentEl()) == MODE_EL##x)

#define IS_IN_EL1() IS_IN_EL(1)
#define IS_IN_EL2() IS_IN_EL(2)
#define IS_IN_EL3() IS_IN_EL(3)

static inline unsigned int get_current_el(void)
{
	return GET_EL(read_CurrentEl());
}

static inline unsigned int get_current_el_maybe_constant(void)
{
#if defined(IMAGE_AT_EL1)
	return 1;
#elif defined(IMAGE_AT_EL2)
	return 2;	/* no use-case in TF-A */
#elif defined(IMAGE_AT_EL3)
	return 3;
#else
	/*
	 * If we do not know which exception level this is being built for
	 * (e.g. built for library), fall back to run-time detection.
	 */
	return get_current_el();
#endif
}

/*
 * Check if an EL is implemented from AA64PFR0 register fields.
 */
static inline uint64_t el_implemented(unsigned int el)
{
	if (el > 3U) {
		return ID_AA64PFR0_EL_IMPL_NONE;
	} else {
		unsigned int shift = ID_AA64PFR0_EL1_SHIFT * el;

		return (read_id_aa64pfr0_el1() >> shift) & ID_AA64PFR0_ELX_MASK;
	}
}

/*
 * TLBIPAALLOS instruction
 * (TLB Inivalidate GPT Information by PA,
 * All Entries, Outer Shareable)
 */
static inline void tlbipaallos(void)
{
	__asm__("SYS #6,c8,c1,#4");
}

/*
 * Invalidate TLBs of GPT entries by Physical address, last level.
 *
 * @pa: the starting address for the range
 *      of invalidation
 * @size: size of the range of invalidation
 */
void gpt_tlbi_by_pa_ll(uint64_t pa, size_t size);


/* Previously defined accessor functions with incomplete register names  */

#define read_current_el()		read_CurrentEl()

#define read_midr()				read_midr_el1()

#define read_mpidr()			read_mpidr_el1()

#define read_scr()				read_scr_el3()
#define write_scr(_v)			write_scr_el3(_v)

#define read_hcr()				read_hcr_el2()
#define write_hcr(_v)			write_hcr_el2(_v)

#define read_cpacr()			read_cpacr_el1()
#define write_cpacr(_v)			write_cpacr_el1(_v)

#define read_clusterpwrdn()		read_clusterpwrdn_el1()
#define write_clusterpwrdn(_v)	write_clusterpwrdn_el1(_v)

#if ERRATA_SPECULATIVE_AT
/*
 * Assuming SCTLR.M bit is already enabled
 * 1. Enable page table walk by clearing TCR_EL1.EPDx bits
 * 2. Execute AT instruction for lower EL1/0
 * 3. Disable page table walk by setting TCR_EL1.EPDx bits
 */
#define AT(_at_inst, _va)	\
{	\
	assert((read_sctlr_el1() & SCTLR_M_BIT) != 0ULL);	\
	write_tcr_el1(read_tcr_el1() & ~(TCR_EPD0_BIT | TCR_EPD1_BIT));	\
	isb();	\
	_at_inst(_va);	\
	write_tcr_el1(read_tcr_el1() | (TCR_EPD0_BIT | TCR_EPD1_BIT));	\
	isb();	\
}
#else
#define AT(_at_inst, _va)	_at_inst(_va)
#endif

#endif /* ARCH_HELPERS_H */
