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

#define AL_AXI_GPIO_NUM_INSTANCE 2 // test 

/***************************** Include Files ********************************/
#include "al_core.h"

#define AL_AXI_GPIO_DATA        0x00
#define AL_AXI_GPIO_TRI         0x04
#define AL_AXI_GPIO_DATA2       0x08
#define AL_AXI_GPIO_TRI2        0x0c
#define AL_AXI_GPIO_GIER        0x11c
#define AL_AXI_GPIO_ISR         0x120
#define AL_AXI_GPIO_IER         0x128

typedef struct {
    AL_U32                  DeviceId    ;
    AL_U32                  BaseAddress ;
    AL_U32                  IntrId      ;
} AlAxiGpio_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
