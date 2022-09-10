/*
 * @Author: Guangxiang Rao
 * @Date: 2022-04-06 11:13:18
 * @LastEditors: Guangxiang Rao
 * @LastEditTime: 2022-05-04 23:40:21
 * @Description: gic 500 initialization and do_irq_hander.
 * @FilePath: /alsoc-embedded-DE/bsp/drives/gic-500/src/gic_v3.c
 */

#include "gic.h"
#include "io.h"
#include "config.h"
#include "cpu.h"
#include "type.h"
#include "delay.h"
#include "sysregs.h"
#include "gic_v3_mask.h"
#include "gic_v3_addr.h"
#include "gic_v3_value.h"
#include "compiler_attributes.h"


#ifdef CONFIG_PRINTK
#define gic_print(...) gic_print(__VA_ARGS__)
#else
#define gic_print(...)
#endif

#define GICV3_SPECIAL_START				(1020)
#define GICV3_SPECIAL_END				(1023)

#define GICV3_SPECIAL_NUM				(GICV3_SPECIAL_END - GICV3_SPECIAL_START +1)
#define IRQ_MAX_NUM					(256)

typedef void (*interrupt_fn)();

static interrupt_fn irq_handler_list[IRQ_MAX_NUM + GICV3_SPECIAL_NUM];
static interrupt_fn fiq_handler_list[IRQ_MAX_NUM + GICV3_SPECIAL_NUM];

static void request_interrupt(u32 int_id, void* handler, interrupt_fn *fn)
{
	/*
	 * > IRQ_MAX_NUM && (not in the range of special range)
	*/
	if (int_id >= IRQ_MAX_NUM && (int_id < GICV3_SPECIAL_START || int_id > GICV3_SPECIAL_END)) {
		gic_print("init id error int_id = %d\n", int_id);
		return;
	}

	if (int_id < IRQ_MAX_NUM) {
		fn[int_id] = handler;
		gicv3_enable_irq(int_id);
	} else {
		fn[int_id - GICV3_SPECIAL_START + IRQ_MAX_NUM] = handler;
	}
}

void request_fiq(u32 int_id, void* handler)
{
	request_interrupt(int_id, handler, fiq_handler_list);
}

void request_irq(u32 int_id, void* handler)
{
	request_interrupt(int_id, handler, irq_handler_list);
}


static void gic_do_wait_for_rwp(void __iomem *rwp_addr)
{
	u32_t count = 1000000;	/* 1s! */

	while (readl(rwp_addr) & GICD_CTLR_RWP) {
		count--;
		if (!count) {
			gic_print("RWP timeout, gone fishing\n");
			return;
		}
		cpu_relax();
		_delay_ms(1);
	}
}

static void gic_dist_wait(void)
{
	gic_do_wait_for_rwp(GICD_BASE);
}

static void gic_redist_wait(void)
{
	gic_do_wait_for_rwp(GICR_RD_BASE);
}

static u64 gic_mpidr_to_affinity(unsigned long mpidr)
{
	u64 aff;
	aff = ((u64)MPIDR_AFFINITY_LEVEL(mpidr, 3) << 32 |
		MPIDR_AFFINITY_LEVEL(mpidr, 2) << 16 |
		MPIDR_AFFINITY_LEVEL(mpidr, 1) << 8  |
		MPIDR_AFFINITY_LEVEL(mpidr, 0));

	return aff;
}

void gicv3_dist_init(u32 int_group)
{
	u32 i;
	u32 spi_itline_nums, cpu_nums, gic_irqs;
	u32 gicd_typer;
	u64 affinity;

	/* Disable the distributor */
	writel_relaxed(0, GICD_CTLR);
	gic_dist_wait();

	gicd_typer = readl_relaxed(GICD_TYPER);
	spi_itline_nums = gicd_typer & GICD_ITLines_Numbers;
	gic_irqs = min((spi_itline_nums + 1) * 32, 1020);

	gic_print("support irq amount: %d\n", gic_irqs);

	/*
	 * Set all global interrupts to be level triggered, active low.
	 */
	for (i = 32; i < gic_irqs; i += 16)
		writel_relaxed(GICD_INT_ACTLOW_LVLTRIG, GICD_ICFGR + i / 4);

	/*
	 * Set priority on all global interrupts.
	 */
	for (i = 32; i < gic_irqs; i += 4)
		writel_relaxed(GICD_INT_DEF_PRI_X4, GICD_IPRIORITYR + i);

	/*
	 * Deactivate and disable all SPIs. Leave the PPI and SGIs
	 * alone as they are in the redistributor registers on GICv3.
	 */
	for (i = 32; i < gic_irqs; i += 32) {
		writel_relaxed(GICD_INT_EN_CLR_X32, GICD_ACTIVE_CLEAR + i / 8);
		writel_relaxed(GICD_INT_EN_CLR_X32, GICD_ICENABLER + i / 8);
	}
	gic_dist_wait();
	/* there setting depend the running secure state */
	if (int_group == GROUP_1_NONSECURE) {
		 /* group 1 non-secure*/
		 /* gicd.ctrl.ds = 1 */
		for (i = 32; i < gic_irqs; i += 32) {
			writel_relaxed(~0x0, GICD_IGROUPR + i / 8);
			writel_relaxed(0, GICD_IGRPMODR + i / 8);
		}
	} else if (int_group == GROUP_1_SECURE) {
		 /* group 1 secure*/
		 /* gicd.ctrl.ds = 1 */
		for (i = 32; i < gic_irqs; i += 32) {
			writel_relaxed(0x0, GICD_IGROUPR + i / 8);
			writel_relaxed(~0x0, GICD_IGRPMODR + i / 8);
		}
	} else {
		for (i = 32; i < gic_irqs; i += 32) {
			writel_relaxed(0x0, GICD_IGROUPR + i / 8);
			writel_relaxed(0x0, GICD_IGRPMODR + i / 8);
		}
	}

	writel_relaxed(0x37, GICD_CTLR);
	gic_dist_wait();
	/*
	 * Set all global interrupts to the boot CPU only. ARE must be enabled.
	 */
	affinity = gic_mpidr_to_affinity(0);
	for (i = 32; i < gic_irqs; i++)
		writeq_relaxed(affinity, GICD_IROUTER + i * 8);

	gic_dist_wait();
}

void gic_enable_redist(bool enable)
{
	u32 count = 1000000;	/* 1s! */
	u32 val;

	val = readl_relaxed(GICR_WAKER);
	if (enable)
		/* Wake up this CPU redistributor */
		val &= ~GICR_WAKER_ProcessorSleep;
	else
		val |= GICR_WAKER_ProcessorSleep;
	writel_relaxed(val, GICR_WAKER);

	if (!enable) {		/* Check that GICR_WAKER is writeable */
		val = readl_relaxed(GICR_WAKER);
		if (!(val & GICR_WAKER_ProcessorSleep))
			return;	/* No PM support in this redistributor */
	}

	while (--count) {
		val = readl_relaxed(GICR_WAKER);
		if (enable ^ (bool)(val & GICR_WAKER_ChildrenAsleep))
			break;
		cpu_relax();
		_delay_ms(1);
	}

	if (!count)
		gic_print("redistributor failed to %s...\n", enable ? "wakeup" : "sleep");
}

void gicv3_redist_init(u32 int_group)
{
	u32 i;
	u32 nr = 1;
	gic_enable_redist(true);

	if (int_group == GROUP_1_NONSECURE) {
		/* Configure SGIs/PPIs as non-secure Group-1 */
		writel_relaxed(~0x0, GICR_SGI_IGROUPR0);
		writel_relaxed(0x0, GICR_SGI_IGRPMOD0);
	} else if (int_group == GROUP_1_SECURE) {
		/* Configure SGIs/PPIs as secure Group-1 */
		writel_relaxed(0x0, GICR_SGI_IGROUPR0);
		writel_relaxed(~0x0, GICR_SGI_IGRPMOD0);
	} else {

		writel_relaxed(0x0, GICR_SGI_IGROUPR0);
		writel_relaxed(0x0, GICR_SGI_IGRPMOD0);
	}

	/*
	 * Deal with the banked PPI and SGI interrupts - disable all
	 * private interrupts. Make sure everything is deactivated.
	 */
	writel_relaxed(GICD_INT_EN_CLR_X32, GICR_SGI_ACTIVE_CLEAR);
	writel_relaxed(GICD_INT_EN_CLR_X32, GICR_SGI_ENABLE_CLEAR);

	/*
	 * Set priority on PPI and SGI interrupts
	 */
	for (i = 0; i < 8; i += 1)
		writel_relaxed(GICD_INT_DEF_PRI_X4, GICR_SGI_PRI + i * 4);

	/* SGI: edge-triggered */
	writel_relaxed(GICR_SGI_EDGE_TRIG, GICR_SGI_ICFGR0);

	/* PPI: level-triggered */
	writel_relaxed(GICR_SGI_LEVEL_TRIG, GICR_SGI_ICFGR1);

	gic_redist_wait();
}


static bool gic_enable_sre(void)
{
#if 1
	u32 val;
	val = gic_read_sre();
	if (val & ICC_SRE_EL1_SRE)
		return true;

	val |= ICC_SRE_EL1_SRE;
	gic_write_sre(val);
	val = gic_read_sre();

	return !!(val & ICC_SRE_EL1_SRE);
#else
		return 1;
#endif
}

static u32 gic_get_pribits(void)
{
	u32 pribits;

	pribits = gic_read_ctlr();
	pribits &= ICC_CTLR_EL1_PRI_BITS_MASK;
	pribits >>= ICC_CTLR_EL1_PRI_BITS_SHIFT;
	pribits++;

	return pribits;
}

static void gicv3_cpu_sysregs_init(void)
{
	/* no nmi */
	//write_gicreg(0, ICC_AP1R1_EL1);
	/* Set priority mask register */
	write_gicreg(DEFAULT_PMR_VALUE, ICC_PMR_EL1);

	gic_write_bpr1(0);
	/* eoi mode */
	gic_write_ctlr(ICC_CTLR_EL1_EOImode_drop_dir);

	isb();
	/* enable group1 */
	gic_write_grpen1(1);

}

static void gicv3_cpu_memory_map_init(void)
{
	/* Set priority mask register */
	writel_relaxed(DEFAULT_PMR_VALUE, GICC_PMR);

	writel_relaxed(0x0, GICC_BPR);
	/* enable group1, eoimode0 */
	writel_relaxed((1<0) | (1<1) | (0<9), GICC_CTRL);

}
void gicv3_cpu_init(void)
{
	int i;
	u32 pribits;

	/* enable sre */
	if (!gic_enable_sre()) {
		gic_print("GIC: unable to init gic cpu interface by setting system register.\n");
		gic_print("try to init gic cpu interface using memory-mapped register.\n");
		gicv3_cpu_memory_map_init();
	} else {
		gic_print("init gic cpu interface using system register.\n");
		gicv3_cpu_sysregs_init();
	}


}

__attribute__((weak)) void gicv3_init(void)
{
#ifdef SUPPORT_NONSECURE
    gicv3_dist_init(GROUP_1_NONSECURE);
    gicv3_redist_init(GROUP_1_NONSECURE);
#else
	gicv3_dist_init(GROUP_1_SECURE);
    gicv3_redist_init(GROUP_1_SECURE);
#endif
    gicv3_cpu_init();

    gic_print("gicv3 : enabled.\n");
}


static void gicv3_set_irq(int irq, void __iomem * reg_addr)
{
	u32 mask = 1 << (irq % 32);
	void __iomem * addr = reg_addr + (irq / 32) * 4;
	writel_relaxed(mask, addr);
}

/**
 * @desc  : diable device irq
 * @param {int} int_id, irq interrput number
 * @return {*}
 */
void gicv3_disable_irq(int int_id)
{
	void __iomem * addr;
	if (int_id<32)
		addr = GICR_SGI_ENABLE_CLEAR;
	else
		addr = GICD_ICENABLER;

	gicv3_set_irq(int_id, addr);
}

/**
 * @desc  : diable device irq
 * @param {int} int_id, irq interrput number
 * @return {*}
 */
void gicv3_enable_irq(int int_id)
{
	void __iomem * addr;
	//SGI
	if (int_id < 32)
		addr = GICR_SGI_ENABLE;
	else
		addr = GICD_ISENABLER;

	gicv3_set_irq(int_id, addr);
}

/**
 * @desc  : irq handle implement
 * @flow  : read irq number -> deal with this irq event -> 	write eoi
 */
void do_irq_handle(void)
{
	u32	int_id;

#if 0
	/* enable sre */
	if (!gic_enable_sre()) {
		/* interrupr acknowledge by read gicc_iar*/
		int_id = readl_relaxed(GICC_IAR) & 0xffffff;
	} else 
#endif	
	{
		/* interrupr acknowledge by read iar*/
		int_id = gic_read_iar_common() & 0xffffff;
	}

	/* run irq handler function */
	void (*p_func)();
	if (int_id < IRQ_MAX_NUM) {
		p_func = irq_handler_list[int_id];
	} else {
		p_func = irq_handler_list[int_id -GICV3_SPECIAL_START + IRQ_MAX_NUM];
	}

	if (!p_func) {
		gic_print("can not found your irq event handle at irq number: %d\n", int_id);
	} else {
		p_func();
	}
#if 0
	/* write end of interrupt to deactivate the interrupt */
	if (!gic_enable_sre()) {
		writel_relaxed(int_id, GICC_EOIR);
	} else 
#endif
	{
		gic_write_eoir(int_id);
	}
}

__attribute__((weak)) unsigned int gic_fiq_get_int_id(void)
{
	unsigned int int_id;

	/* enable sre */
	if (!gic_enable_sre()) {
		/* interrupr acknowledge by read gicc_iar*/
		int_id = readl_relaxed(GICC_IAR) & 0xffffff;
	} else {
		/* interrupr acknowledge by read iar*/
		int_id = gic_read_iar_common() & 0xffffff;
	}
}

/**
 * @desc  : irq handle implement; this is weak function because we need to read ICC_IAR0_EL1 if group0 case
 * @flow  : read irq number -> deal with this irq event -> 	write eoi
 */
void do_fiq_handle(void)
{
	u32	int_id;
	void (*p_func)();

	int_id = gic_fiq_get_int_id();

	/* run irq handler function */
	if (int_id < IRQ_MAX_NUM) {
		p_func = fiq_handler_list[int_id];
	} else {
		p_func = fiq_handler_list[int_id - GICV3_SPECIAL_START + IRQ_MAX_NUM];
	}
	if (!p_func) {
		gic_print("can not found your irq event handle at irq number: %d\n",int_id);
	} else {
		p_func();
	}
	/* write end of interrupt to deactivate the interrupt */

	if (!gic_enable_sre()) {
		writel_relaxed(int_id, GICC_EOIR);
	} else {
		gic_write_eoir(int_id);
	}
}


void __enable_irq(void)
{
	asm volatile(
		"msr    daifclr, #0x3"
		:
		:
		: "memory");
}

void __disable_irq(void)
{
	asm volatile(
		"msr    daifset, #0x3"
		:
		:
		: "memory");
}

/**
 * @desc  : set gic irq priority
 */
int gicv3_set_irq_priority(int int_id, int priority)
{
	void __iomem * addr;
	u32 pri;

	if (int_id >= IRQ_MAX_NUM) {
		gic_print("init id error int_id = %d\n", int_id);
		return (-1);
	}

	if (int_id < 32)
		addr = GICR_SGI_PRI;
	else
		addr = GICD_IPRIORITYR;

	pri = readl_relaxed(addr + (int_id & 0xfffffffc));

	pri = pri & (~(0xffUL << ((int_id & 0x03) << 3))) | (priority << ((int_id & 0x03) << 3));

	writel_relaxed(pri, addr + (int_id & 0xfffffffc));

	return 0;
}


/**
 * @desc  : set gic group
 */
int gicv3_set_group(int int_id, int int_group)
{
	/* Disable the distributor */
	if (int_id < 32) {
		u32 sgi_igroup  = readl_relaxed(GICR_SGI_IGROUPR0);
		u32 sgi_igrpmod = readl_relaxed(GICR_SGI_IGRPMOD0);

		gic_enable_redist(true);

		if (int_group == GROUP_1_NONSECURE) {
			sgi_igroup  |= (0x01 << int_id);
			sgi_igrpmod &= (~(0x01 << int_id));
		} else if (int_group == GROUP_1_SECURE) {
			sgi_igroup  &= (~(0x01 << int_id));
			sgi_igrpmod |= (0x01 << int_id);
		} else if(int_group == GROUP_0) {
			sgi_igroup  &= (~(0x01 << int_id));
			sgi_igrpmod &= (0x01 << int_id);
		}

		writel_relaxed(sgi_igroup, GICR_SGI_IGROUPR0);
		writel_relaxed(sgi_igrpmod, GICR_SGI_IGRPMOD0);

		gic_redist_wait();
	} else {
		u32 gicd_igroup  = readl_relaxed(GICD_IGROUPR  + int_id / 32);
		u32 gicd_igrpmod = readl_relaxed(GICD_IGRPMODR + int_id / 32);

		writel_relaxed(0, GICD_CTLR);
		gic_dist_wait();

		if (int_group == GROUP_1_NONSECURE) {
			gicd_igroup  |= (0x01 << int_id);
			gicd_igrpmod &= (~(0x01 << int_id));
		} else if (int_group == GROUP_1_SECURE) {
			gicd_igroup  &= (~(0x01 << int_id));
			gicd_igrpmod |= (0x01 << int_id);
		} else if(int_group == GROUP_0) {
			gicd_igroup  &= (~(0x01 << int_id));
			gicd_igrpmod &= (0x01 << int_id);
		}

		writel_relaxed(gicd_igrpmod, GICD_IGRPMODR + int_id / 8);
		writel_relaxed(gicd_igroup, GICD_IGROUPR + int_id / 8);

		gic_dist_wait();
	}
}
