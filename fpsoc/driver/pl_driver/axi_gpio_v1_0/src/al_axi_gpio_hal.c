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
    AL_ASSERT(Handle != AL_NULL, AL_AXI_GPIO_ERR_ILLEGAL_PARAM);

    AL_S32 Ret = AL_OK;
    AlAxiGpio_HwConfigStruct *HwConfig;

    HwConfig = AlAxiGpio_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL)
    {
        (*Handle) = &AXI_GPIO_Handle[DevId];
    }
    else
    {
        return AL_ERR_ILLEGAL_PARAM;
    }

    Ret = AlAxiGpio_Dev_Init(&(*Handle)->GpioDevStruct, HwConfig);

    if (CallBack != AL_NULL)
    {
        Ret = AlAxiGpio_Dev_RegisterEventCallBack(&(*Handle)->GpioDevStruct, CallBack, (void *)(*Handle));
    }

    return Ret;
}

AL_S32 AlAxiGpio_Hal_WritePin(AlAxiGpio_Hal_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Bit, AL_U32 Value)
{
    AL_ASSERT((Handle != AL_NULL) && (Channel < AXI_GPIO_MAX_CHANNEL) && (Bit < AXI_GPIO_MAX_BIT), AL_AXI_GPIO_ERR_ILLEGAL_PARAM);

    AL_S32 Ret = 0;

    Ret = AlAxiGpio_Dev_WritePin(&(Handle->GpioDevStruct), Channel, Bit, Value);

    return Ret;
}

AL_U32 AlAxiGpio_Hal_ReadPin(AlAxiGpio_Hal_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Bit)
{
    AL_ASSERT((Handle != AL_NULL) && (Channel < AXI_GPIO_MAX_CHANNEL) && (Bit < AXI_GPIO_MAX_BIT), AL_AXI_GPIO_ERR_ILLEGAL_PARAM);

    AL_U32 Ret = 0;

    Ret = AlAxiGpio_Dev_ReadPin(&(Handle->GpioDevStruct), Channel, Bit);

    return Ret;
}

AL_S32 AlAxiGpio_Hal_WriteChannel(AlAxiGpio_Hal_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Value)
{
    AL_ASSERT((Handle != AL_NULL) && (Channel < AXI_GPIO_MAX_CHANNEL), AL_AXI_GPIO_ERR_ILLEGAL_PARAM);
    AL_S32 Ret = 0;

    Ret = AlAxiGpio_Dev_WriteChannel(&(Handle->GpioDevStruct), Channel, Value);

    return Ret;
}

AL_U32 AlAxiGpio_Hal_ReadChannel(AlAxiGpio_Hal_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel)
{
    AL_ASSERT((Handle != AL_NULL) && (Channel < AXI_GPIO_MAX_CHANNEL), AL_AXI_GPIO_ERR_ILLEGAL_PARAM);

    AL_U32 Ret = 0;

    Ret = AlAxiGpio_Dev_ReadChannel(&(Handle->GpioDevStruct), Channel);

    return Ret;
}

AL_S32 AlAxiGpio_Hal_IntInit(AlAxiGpio_Hal_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel)
{
    AL_ASSERT((Handle != AL_NULL) && (Channel < AXI_GPIO_MAX_CHANNEL), AL_AXI_GPIO_ERR_ILLEGAL_PARAM);

    AL_S32 Ret = 0;

    Ret = AlAxiGpio_Dev_IntInit(&(Handle->GpioDevStruct), Channel);

    return Ret;
}
