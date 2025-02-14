/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_CHIP_H_
#define __AL_CHIP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"
#include "al_hwcfg.h"
#include "soc_plat.h"

AL_VOID AlChip_Init(AL_VOID);

/*----------------------------------------------------------------------------
* Define Interrupt Number
*----------------------------------------------------------------------------*/
typedef enum {
    SOC_SECURE_PHYSICAL_TIMER         =  29,
    SOC_NONE_SECURE_PHYSICAL_TIMER    =  30,
    SOC_REQ0_IRQn                     =  43,
    SOC_ACK0_IRQn                     =  44,
    SOC_REQ1_IRQn                     =  45,
    SOC_ACK1_IRQn                     =  46,
    SOC_REQ2_IRQn                     =  47,
    SOC_ACK2_IRQn                     =  48,
    SOC_REQ3_IRQn                     =  49,
    SOC_ACK3_IRQn                     =  50,
    SOC_PMU0_IRQn                     =  52,
    SOC_PMU1_IRQn                     =  53,
    SOC_ADC_IRQn                      =  56,
    SOC_WDT0_IRQn                     =  58,
    SOC_WDT1_IRQn                     =  59,
    SOC_TC0_TC0_IRQn                 =  61,
    SOC_TC0_TC1_IRQn                 =  62,
    SOC_TC0_TC2_IRQn                 =  63,
    SOC_TC1_TC0_IRQn                 =  64,
    SOC_TC1_TC1_IRQn                 =  65,
    SOC_TC1_TC2_IRQn                 =  66,
    SOC_DMAC_ABORT_IRQn               =  69,
    SOC_DMAC_CH0_IRQ                  =  70,
    SOC_DMAC_CH1_IRQ                  =  71,
    SOC_DMAC_CH2_IRQ                  =  72,
    SOC_DMAC_CH3_IRQ                  =  73,
    SOC_DMAC_CH4_IRQ                  =  74,
    SOC_DMAC_CH5_IRQ                  =  75,
    SOC_DMAC_CH6_IRQ                  =  76,
    SOC_DMAC_CH7_IRQ                  =  77,
    SOC_DMACAHB_IRQn                  =  78,
    SOC_SMC_IRQn                      =  79,
    SOC_QSPI_IRQn                     =  80,
    GPIO0_IRQn                        =  84,
    SOC_USB0_IRQn                     =  88,
    SOC_USB1_IRQn                     =  89,
    SOC_GBE0_IRQn                     =  90,
    SOC_GBE1_IRQn                     =  91,
    SOC_MMC0_IRQn                     =  94,
    SOC_MMC1_IRQn                     =  95,
    SOC_IIC0_IRQn                     =  96,
    SOC_IIC1_IRQn                     =  97,
    SOC_SPI0_MST_IRQn                 =  98,
    SOC_SPI0_SLV_IRQn                 =  99,
    SOC_SPI1_MST_IRQn                 =  100,
    SOC_SPI1_SLV_IRQn                 =  101,
    SOC_UART0_IRQn                    =  102,
    SOC_UART1_IRQn                    =  103,
    SOC_CAN0_IRQn                     =  104,
    SOC_CAN1_IRQn                     =  105,
    NPU_SOFT_IRQn                     =  129,
    SOC_XMON_IRQn                     =  142,
    SOC_MPU_IRQn                      =  143,
    NPU_HARD_IRQn                     =  144,
    SOC_APU2RPU_REQ_IRQn              =  148,
    SOC_APU2RPU_ACK_IRQn              =  149,
    SOC_RPU2APU_REQ_IRQn              =  150,
    SOC_RPU2APU_ACK_IRQn              =  151,
    SOC_INT_MAX                       =  256,
} AL_IrqNumEnum;

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


#define AL_DEFAULT_ATTR(Name)  AL_INTR_AttrStrct Name = {   \
            .TrigMode   =  LEVEL_HIGH_TRIGGER,              \
            .Priority   =  137,                             \
};


#ifdef __cplusplus
}
#endif
#endif /* AL_CHIP_H */
