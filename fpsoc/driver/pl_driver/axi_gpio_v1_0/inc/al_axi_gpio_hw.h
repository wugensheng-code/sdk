/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_GPIO_HW_H_
#define AL_AXI_GPIO_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

// test, parameters should from TD HPF
#define AL_AXI_GPIO_NUM_INSTANCE    2 
#define AXI_GPIO0_IRQn              114
#define AXI_GPIO1_IRQn              115
#define AXI_GPIO0__BASE_ADDR        0x80000000
#define AXI_GPIO1__BASE_ADDR        0x80100000

/***************************** Include Files ********************************/
#include "al_core.h"

#define AL_AXI_GPIO_DATA        0x00
#define AL_AXI_GPIO_TRI         0x04
#define AL_AXI_GPIO_DATA2       0x08
#define AL_AXI_GPIO_TRI2        0x0c
#define AL_AXI_GPIO_ISR         0x104
#define AL_AXI_GPIO_IER         0x100

typedef struct {
    AL_U32                  DeviceId    ;
    AL_U32                  BaseAddress ;
    AL_U32                  IntrId      ;
} AlAxiGpio_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
