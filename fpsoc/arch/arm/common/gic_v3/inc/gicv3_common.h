/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef GICV3_COMMON_H
#define GICV3_COMMON_H

/*******************************************************************************
 * GIC Distributor interface general definitions
 ******************************************************************************/
/* Constants to categorise interrupts */
#define MIN_SGI_ID        (0U)
#define MIN_SEC_SGI_ID    (8U)
#define MIN_PPI_ID        (16U)
#define MIN_SPI_ID        (32U)
#define MAX_SPI_ID        (1019U)

#define TOTAL_SPI_INTR_NUM      (MAX_SPI_ID - MIN_SPI_ID + (1U))
#define TOTAL_PCPU_INTR_NUM     (MIN_SPI_ID - MIN_SGI_ID)

/* Mask for the priority field common to all GIC interfaces */
#define GIC_PRI_MASK    (0xf0U)

/* Mask for the configuration field common to all GIC interfaces */
#define GIC_CFG_MASK        (0x3U)
#define GIC_CFG_MASK_SIZE   (2U)

/* Constant to indicate a spurious interrupt in all GIC versions */
#define GIC_SPURIOUS_INTERRUPT  (1023U)

/* Interrupt configurations: 2-bit fields with LSB reserved */
#define GIC_INTR_CFG_LEVEL  (0 << 1)
#define GIC_INTR_CFG_EDGE   (1 << 1)

/* Highest possible interrupt priorities */
#define GIC_HIGHEST_SEC_PRIORITY    (0x00U)
#define GIC_HIGHEST_NS_PRIORITY     (0x80U)

/*******************************************************************************
 * Common GIC Distributor interface register offsets
 ******************************************************************************/
#define GICD_CTLR       (0x0U)
#define GICD_TYPER      (0x4U)
#define GICD_IIDR       (0x8U)
#define GICD_IGROUPR    (0x80U)
#define GICD_ISENABLER  (0x100U)
#define GICD_ICENABLER  (0x180U)
#define GICD_ISPENDR    (0x200U)
#define GICD_ICPENDR    (0x280U)
#define GICD_ISACTIVER  (0x300U)
#define GICD_ICACTIVER  (0x380U)
#define GICD_IPRIORITYR (0x400U)
#define GICD_ICFGR      (0xc00U)
#define GICD_NSACR      (0xe00U)

/* GICD_CTLR bit definitions */
#define CTLR_ENABLE_G0_SHIFT    0
#define CTLR_ENABLE_G0_MASK     (0x1U)
#define CTLR_ENABLE_G0_BIT      BITS_32(CTLR_ENABLE_G0_SHIFT,1)

/*******************************************************************************
 * Common GIC Distributor interface register constants
 ******************************************************************************/
#define PIDR2_ARCH_REV_SHIFT    4
#define PIDR2_ARCH_REV_MASK     (0xfU)

/* GIC revision as reported by PIDR2.ArchRev register field */
#define ARCH_REV_GICV1  (0x1U)
#define ARCH_REV_GICV2  (0x2U)
#define ARCH_REV_GICV3  (0x3U)
#define ARCH_REV_GICV4  (0x4U)

#define IGROUPR_SHIFT       5
#define ISENABLER_SHIFT     5
#define ICENABLER_SHIFT     ISENABLER_SHIFT
#define ISPENDR_SHIFT       5
#define ICPENDR_SHIFT       ISPENDR_SHIFT
#define ISACTIVER_SHIFT     5
#define ICACTIVER_SHIFT     ISACTIVER_SHIFT
#define IPRIORITYR_SHIFT    2
#define ITARGETSR_SHIFT     2
#define ICFGR_SHIFT         4
#define NSACR_SHIFT         4

/* GICD_TYPER shifts and masks */
#define TYPER_IT_LINES_NO_SHIFT     (0U)
#define TYPER_IT_LINES_NO_MASK      (0x1fU)

/* Value used to initialize Normal world interrupt priorities four at a time */
#define GICD_IPRIORITYR_DEF_VAL     (GIC_HIGHEST_NS_PRIORITY    |    \
                                    (GIC_HIGHEST_NS_PRIORITY << 8)    |    \
                                    (GIC_HIGHEST_NS_PRIORITY << 16)    |    \
                                    (GIC_HIGHEST_NS_PRIORITY << 24))

/*******************************************************************************
 * GIC500/GIC600 Re-distributor interface registers & constants
 ******************************************************************************/

/* GICR_WAKER implementation-defined bit definitions */
#define WAKER_SL_SHIFT      0
#define WAKER_QSC_SHIFT     31

#define WAKER_SL_BIT    (1U << WAKER_SL_SHIFT)
#define WAKER_QSC_BIT   (1U << WAKER_QSC_SHIFT)

#define IIDR_MODEL_ARM_GIC_600      U(0x0200043b)
#define IIDR_MODEL_ARM_GIC_600AE    U(0x0300043b)
#define IIDR_MODEL_ARM_GIC_700      U(0x0400043b)

#define PIDR_COMPONENT_ARM_DIST     U(0x492)
#define PIDR_COMPONENT_ARM_REDIST   U(0x493)
#define PIDR_COMPONENT_ARM_ITS      U(0x494)

#endif /* ARM_GICV3_COMMON_H */
