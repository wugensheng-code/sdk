/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_GPIO_DEV_H_
#define AL_AXI_GPIO_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_axi_gpio_ll.h"

#define AL_AXI_GPIO_ERR_ILLEGAL_PARAM    (AL_DEF_ERR(AL_AxiGpio, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))

typedef struct
{
    AL_U32                  Channel;
    AL_U32                  EventData;
} AlAxiGpio_EventStruct;

typedef (*AlAxiGpio_EventCallBack)(AlAxiGpio_EventStruct Event, AL_VOID *CallbackRef);

typedef struct
{
    AlAxiGpio_HwConfigStruct    HwConfig;
    AlAxiGpio_EventCallBack     EventCallBack;
    AL_VOID                     *EventCallBackRef;
} AlAxiGpio_Dev_Struct;

AL_S32 AlAxiGpio_Dev_Init(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_HwConfigStruct *HwConfig);
AL_S32 AlAxiGpio_Dev_WritePin(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Bit, AL_U32 Value);
AL_U32 AlAxiGpio_Dev_ReadPin(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Bit);
AL_S32 AlAxiGpio_Dev_WriteChannel(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Value);
AL_U32 AlAxiGpio_Dev_ReadChannel(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel);
AL_S32 AlAxiGpio_Dev_RegisterEventCallBack(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_EventCallBack CallBack, AL_VOID *CallBackRef);
AL_S32 AlAxiGpio_Dev_IntInit(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel);
AL_VOID AlAxiGpio_Dev_IntrHandler(AL_VOID *Instance);

#ifdef __cplusplus
}
#endif

#endif
