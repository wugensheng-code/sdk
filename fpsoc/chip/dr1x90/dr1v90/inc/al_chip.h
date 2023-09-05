/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_CHIP_H__
#define __AL_CHIP_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "al_core.h"

AL_VOID AlChip_Dr1V90Init(AL_VOID);

#define AlChip_Init AlChip_Dr1V90Init

#include "al_hwcfg.h"
#include "soc_plat.h"

/*----------------------------------------------------------------------------
* Define Interrupt Number
*----------------------------------------------------------------------------*/
typedef enum {
    SysTimerSW_IRQn      =  3,
    SysTimer_IRQn        =  7,
    SOC_REQ0_IRQn        =  30,
    SOC_ACK0_IRQn        =  31,
    SOC_REQ1_IRQn        =  32,
    SOC_ACK1_IRQn        =  33,
    SOC_REQ2_IRQn        =  34,
    SOC_ACK2_IRQn        =  35,
    SOC_REQ3_IRQn        =  36,
    SOC_ACK3_IRQn        =  37,
    SOC_XADC_IRQn        =  43,
    SOC_DMACAHB_IRQn     =  65,
    SOC_SMC_IRQn         =  66,
    SOC_QSPI_IRQn        =  67,
    GPIO0_IRQn           =  71,
    SOC_USB0_IRQn        =  75,
    SOC_USB1_IRQn        =  76,
    SOC_GBE0_IRQn        =  77,
    SOC_GBE1_IRQn        =  78,
    SOC_INT130_IRQn      =  130,
    SOC_MMC0_IRQn        =  81,
    SOC_MMC1_IRQn        =  82,
    SOC_IIC0_IRQn        =  83,
    SOC_IIC1_IRQn        =  84,
    SOC_SPI0_MST_IRQn    =  85,
    SOC_SPI0_SLV_IRQn    =  86,
    SOC_SPI1_MST_IRQn    =  87,
    SOC_SPI1_SLV_IRQn    =  88,
    SOC_UART0_IRQn       =  89,
    SOC_UART1_IRQn       =  90,
    SOC_CAN0_IRQn        =  91,
    SOC_CAN1_IRQn        =  92,
    SOC_INT_MAX          =  256,
    NPU_HARD_IRQn        =  131,
    NPU_SOFT_IRQn        =  116,
} AL_IrqNumEnum;

#define ECLIC__BASE_ADDR        (0x68020000UL)

#define PMP_ENTRY_NUM           (16)

#ifdef __cplusplus
}
#endif
#endif /* __AL_CHIP_H__ */
