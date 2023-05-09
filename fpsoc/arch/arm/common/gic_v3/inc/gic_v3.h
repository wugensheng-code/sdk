#ifndef __GIC_V3_H
#define __GIC_V3_H

#include "type.h"
#include "sysregs.h"
#include "barriers.h"
#include "compiler_attributes.h"
#include "stdio.h"

#define DEFAULT_PMR_VALUE	    0xf0

#define read_gicreg(r)			read_sysreg_s(SYS_ ## r)
#define write_gicreg(v, r)		write_sysreg_s(v, SYS_ ## r)

typedef enum {
	GROUP_0 = 0,
	GROUP_1_SECURE = 1,
	GROUP_1_NONSECURE = 2,
} gic_group;

typedef void (*interrupt_fn)(void *instance);


/*
 * Low-level accessors
 *
 * These system registers are 32 bits, but we make sure that the compiler
 * sets the GP register's most significant bits to 0 with an explicit cast.
 */

static inline void gic_write_eoir(u32 irq)
{
	write_sysreg_s(irq, SYS_ICC_EOIR1_EL1);
	isb();
}

static __always_inline void gic_write_dir(u32 irq)
{
	write_sysreg_s(irq, SYS_ICC_DIR_EL1);
	isb();
}

static inline u64 gic_read_iar_common(void)
{
	u64 irqstat;

	irqstat = read_sysreg_s(SYS_ICC_IAR1_EL1);

	dsb(sy);

	return irqstat;
}

static inline u64 gic_read_iar0_common(void)
{
	u64 irqstat;

	irqstat = read_sysreg_s(SYS_ICC_IAR0_EL1);

	dsb(sy);

	return irqstat;
}

/*
 * Cavium ThunderX erratum 23154
 *
 * The gicv3 of ThunderX requires a modified version for reading the
 * IAR status to ensure data synchronization (access to icc_iar1_el1
 * is not sync'ed before and after).
 */
static inline u64 gic_read_iar_cavium_thunderx(void)
{
	u64 irqstat;

	nops(8);
	irqstat = read_sysreg_s(SYS_ICC_IAR1_EL1);
	nops(4);
	mb();

	return irqstat;
}

static inline void gic_write_ctlr(u32 val)
{
	write_sysreg_s(val, SYS_ICC_CTLR_EL1);
	isb();
}

static inline u32 gic_read_ctlr(void)
{
	return read_sysreg_s(SYS_ICC_CTLR_EL1);
}

static inline void gic_write_grpen1(u32 val)
{
	write_sysreg_s(val, SYS_ICC_IGRPEN1_EL1);
	isb();
}

static inline void gic_write_sgi1r(u64 val)
{
	write_sysreg_s(val, SYS_ICC_SGI1R_EL1);
}

static inline u32 gic_read_sre(void)
{
	return read_sysreg_s(SYS_ICC_SRE_EL1);
}

static inline void gic_write_sre(u32 val)
{
	write_sysreg_s(val, SYS_ICC_SRE_EL1);
	isb();
}

static inline void gic_write_bpr1(u32 val)
{
	write_sysreg_s(val, SYS_ICC_BPR1_EL1);
}

static inline u32 gic_read_pmr(void)
{
	return read_sysreg_s(SYS_ICC_PMR_EL1);
}

static __always_inline void gic_write_pmr(u32 val)
{
	write_sysreg_s(val, SYS_ICC_PMR_EL1);
}

static inline u32 gic_read_rpr(void)
{
	return read_sysreg_s(SYS_ICC_RPR_EL1);
}


#define irq_disable()	__disable_irq()
#define irq_enable()	__enable_irq()

/* functions */

/**
 * @desc  :  init gic-500 to enable interrupt of soc
 * @param {*}
 * @return {*}
 */
void gicv3_init(void);

/**
 * @desc  : register device interrupt function to interrupt handler function
 * @param {u32} int_id: device interrupt id
 * @param {void*} handler, function point of device interrupt function
 * @return {*}
 */
void request_irq(u32 int_id, interrupt_fn* handler, void *ref);
void request_fiq(u32 int_id, interrupt_fn* handler, void *ref);
/**
 * @desc  : disable irq
 * @param {int} irq_num
 * @return {*}
 */
void gicv3_disable_irq(int irq_num);

/**
 * @desc  : enable irq
 * @param {int} irq_num
 * @return {*}
 */
void gicv3_enable_irq(int irq_num);

/*
*/
int gicv3_set_irq_priority(int int_id, int priority);

#endif /* __GIC_V3_H */
