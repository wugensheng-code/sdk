/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_GPIO_HAL_H_
#define AL_AXI_GPIO_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_axi_gpio_dev.h"

typedef struct
{
    AlAxiGpio_Dev_Struct        GpioDevStruct;
} AlAxiGpio_Hal_Struct;

AL_S32 AlAxiGpio_Hal_Init(AlAxiGpio_Hal_Struct **Handle, AL_U32 DevId, AlAxiGpio_EventCallBack CallBack);
AL_S32 AlAxiGpio_Hal_WritePin(AlAxiGpio_Hal_Struct **Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Bit, AL_U32 Value);
AL_U32 AlAxiGpio_Hal_ReadPin(AlAxiGpio_Hal_Struct **Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Bit);
AL_S32 AlAxiGpio_Hal_WriteChannel(AlAxiGpio_Hal_Struct **Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Value);
AL_U32 AlAxiGpio_Hal_ReadChannel(AlAxiGpio_Hal_Struct **Handle, AlAxiGpio_ll_ChannelEnum Channel);
AL_S32 AlAxiGpio_Hal_IntInit(AlAxiGpio_Hal_Struct **Handle, AlAxiGpio_ll_ChannelEnum Channel);

#ifdef __cplusplus
}
#endif

#endif
