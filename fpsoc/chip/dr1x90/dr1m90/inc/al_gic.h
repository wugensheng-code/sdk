#ifndef _AL_GIC_H_
#define _AL_GIC_H_

#define AL_ARM_CORE_COUNT		(2)

#define CLUSTER_TO_CPUID_SHIFT      (6)

/* Base al_dr1m90 compatible GIC memory map */
#define AL_DR1M90_GIC_BASE          (0xDD000000UL)
#define AL_DR1M90_GICD_BASE         (AL_DR1M90_GIC_BASE)
#define AL_DR1M90_GICD_MSG_BASE     (AL_DR1M90_GIC_BASE + 0x10000UL)
#define AL_DR1M90_GICR_BASE         (AL_DR1M90_GIC_BASE + 0x40000UL)
#define AL_DR1M90_GICR_0_BASE       (AL_DR1M90_GICR_BASE)
#define AL_DR1M90_GICR_0_SGI_BASE   (AL_DR1M90_GIC_BASE + 0x50000UL)
#define AL_DR1M90_GICR_1_BASE       (AL_DR1M90_GIC_BASE + 0x60000UL)
#define AL_DR1M90_GICR_1_SGI_BASE   (AL_DR1M90_GIC_BASE + 0x70000UL)

/******************************************************************************
 * sgi, ppi
 ******************************************************************************/
#define ARM_IRQ_SEC_PHY_TIMER		29

#define ARM_IRQ_SEC_SGI_0		8
#define ARM_IRQ_SEC_SGI_1		9
#define ARM_IRQ_SEC_SGI_2		10
#define ARM_IRQ_SEC_SGI_3		11
#define ARM_IRQ_SEC_SGI_4		12
#define ARM_IRQ_SEC_SGI_5		13
#define ARM_IRQ_SEC_SGI_6		14
#define ARM_IRQ_SEC_SGI_7		15


void AlGic_Init(void);

#endif
