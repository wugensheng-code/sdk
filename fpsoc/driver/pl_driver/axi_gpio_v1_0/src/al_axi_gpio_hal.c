/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



/***************************** Include Files *********************************/
#include "al_axi_gpio_hal.h"

AlAxiGpio_Hal_Struct AXI_GPIO_Handle[AL_AXI_GPIO_NUM_INSTANCE];

AL_S32 AlAxiGpio_Hal_Init(AlAxiGpio_Hal_Struct **Handle, AL_U32 DevId, AlAxiGpio_EventCallBack CallBack)
{
    AlAxiGpio_HwConfigStruct *HwConfig;
    HwConfig = AlAxiGpio_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL)
    {
        *Handle = &AXI_GPIO_Handle[DevId];
    }
    else 
    {
        return;
    }

    AlAxiGpio_Dev_Init(&(*Handle)->GpioDevStruct, HwConfig);

    if (CallBack != AL_NULL)
    {
        AlAxiGpio_Dev_RegisterEventCallBack(&(*Handle)->GpioDevStruct, CallBack, *Handle);
    }
    return AL_OK;
}

AL_S32 AlAxiGpio_Hal_WritePin(AlAxiGpio_Hal_Struct **Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Bit, AL_U32 Value)
{
    AlAxiGpio_Dev_WritePin(&(*Handle)->GpioDevStruct, Channel, Bit, Value);
    return AL_OK;
}

AL_U32 AlAxiGpio_Hal_ReadPin(AlAxiGpio_Hal_Struct **Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Bit)
{
    return AlAxiGpio_Dev_ReadPin(&(*Handle)->GpioDevStruct, Channel, Bit);
}

AL_S32 AlAxiGpio_Hal_WriteChannel(AlAxiGpio_Hal_Struct **Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Value)
{
    AlAxiGpio_Dev_WriteChannel(&(*Handle)->GpioDevStruct, Channel, Value);
}

AL_U32 AlAxiGpio_Hal_ReadChannel(AlAxiGpio_Hal_Struct **Handle, AlAxiGpio_ll_ChannelEnum Channel)
{
    return AlAxiGpio_Dev_ReadChannel(&(*Handle)->GpioDevStruct, Channel);
}

AL_S32 AlAxiGpio_Hal_IntInit(AlAxiGpio_Hal_Struct **Handle, AlAxiGpio_ll_ChannelEnum Channel)
{
    AlAxiGpio_Dev_IntInit(&(*Handle)->GpioDevStruct, Channel);
}