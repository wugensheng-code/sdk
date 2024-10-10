/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



/***************************** Include Files *********************************/
#include "al_axi_gpio_dev.h"

extern AlAxiGpio_HwConfigStruct AlAxiGpio_HwCfg[AL_AXI_GPIO_NUM_INSTANCE];

AL_S32 AlAxiGpio_Dev_Init(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_HwConfigStruct *HwConfig)
{
    AL_ASSERT(Handle != AL_NULL, AL_AXI_GPIO_ERR_ILLEGAL_PARAM);

    Handle->HwConfig = *HwConfig;
    return AL_OK;
}

AL_S32 AlAxiGpio_Dev_WritePin(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Bit, AL_U32 Value)
{

    AL_ASSERT((Handle != AL_NULL) && (Channel < AXI_GPIO_MAX_CHANNEL) && (Bit < AXI_GPIO_MAX_BIT), AL_AXI_GPIO_ERR_ILLEGAL_PARAM);
    AL_U32 BaseAddr = Handle->HwConfig.BaseAddress;

    if (Channel == AL_AXI_GPIO_CHANNEL1)
    {
        AlAxiGpio_ll_SetDir(BaseAddr, Bit, AL_AXI_GPIO_OUT);
        AlAxiGpio_ll_SetData(BaseAddr, Bit, Value);
    }
    else if (Channel == AL_AXI_GPIO_CHANNEL2)
    {
        AlAxiGpio_ll_SetDir2(BaseAddr, Bit, AL_AXI_GPIO_OUT);
        AlAxiGpio_ll_SetData2(BaseAddr, Bit, Value);
    }
    return AL_OK;
}

AL_U32 AlAxiGpio_Dev_ReadPin(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Bit)
{
    AL_ASSERT((Handle != AL_NULL) && (Channel < AXI_GPIO_MAX_CHANNEL) && (Bit < AXI_GPIO_MAX_BIT), AL_AXI_GPIO_ERR_ILLEGAL_PARAM);

    AL_U32 BaseAddr = Handle->HwConfig.BaseAddress;
    AL_U32 Value;

    if (Channel == AL_AXI_GPIO_CHANNEL1)
    {
        AlAxiGpio_ll_SetDir(BaseAddr, Bit, AL_AXI_GPIO_IN);
        Value = AlAxiGpio_ll_GetData(BaseAddr, Bit);
    }
    else if (Channel == AL_AXI_GPIO_CHANNEL2)
    {
        AlAxiGpio_ll_SetDir2(BaseAddr, Bit, AL_AXI_GPIO_IN);
        Value = AlAxiGpio_ll_GetData2(BaseAddr, Bit);
    }

    return Value;
}

AL_S32 AlAxiGpio_Dev_WriteChannel(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Value)
{
    AL_ASSERT((Handle != AL_NULL) && (Channel < AXI_GPIO_MAX_CHANNEL), AL_AXI_GPIO_ERR_ILLEGAL_PARAM);

    AL_U32 BaseAddr = Handle->HwConfig.BaseAddress;
    if (Channel == AL_AXI_GPIO_CHANNEL1)
    {
        AlAxiGpio_ll_SetChannelDir(BaseAddr, AL_AXI_GPIO_CHANNEL_OUT);
        AlAxiGpio_ll_SetChannelData(BaseAddr, Value);
    }
    else if (Channel == AL_AXI_GPIO_CHANNEL2)
    {
        AlAxiGpio_ll_SetChannelDir2(BaseAddr, AL_AXI_GPIO_CHANNEL_OUT);
        AlAxiGpio_ll_SetChannelData2(BaseAddr, Value);
    }

    return AL_OK;
}

AL_U32 AlAxiGpio_Dev_ReadChannel(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel)
{
    AL_ASSERT((Handle != AL_NULL) && (Channel < AXI_GPIO_MAX_CHANNEL), AL_AXI_GPIO_ERR_ILLEGAL_PARAM);

    AL_U32 BaseAddr = Handle->HwConfig.BaseAddress;
    AL_U32 Value;

    if (Channel == AL_AXI_GPIO_CHANNEL1)
    {
        AlAxiGpio_ll_SetChannelDir(BaseAddr, AL_AXI_GPIO_CHANNEL_IN);
        Value = AlAxiGpio_ll_GetChannelData(BaseAddr);
    }
    else if (Channel == AL_AXI_GPIO_CHANNEL2)
    {
        AlAxiGpio_ll_SetChannelDir2(BaseAddr, AL_AXI_GPIO_CHANNEL_IN);
        Value = AlAxiGpio_ll_GetChannelData2(BaseAddr);
    }

    return Value;
}

AL_VOID AlAxiGpio_Dev_IntrHandler(AL_VOID *Instance)
{
    AlAxiGpio_Dev_Struct *Handle = (AlAxiGpio_Dev_Struct *)Instance;
    AL_U32 IntStatus = AlAxiGpio_ll_GetIntStatus(Handle->HwConfig.BaseAddress);
    AL_U32 Status = 0;
    AL_U32 Channel = 0;

    if ((IntStatus & BIT(AL_AXI_GPIO_CHANNEL1)) && (IntStatus & BIT(AL_AXI_GPIO_CHANNEL2))) {
        AlAxiGpio_ll_ClearAllInt(Handle->HwConfig.BaseAddress);
    } else if((IntStatus & BIT(AL_AXI_GPIO_CHANNEL1)) && (!(IntStatus & BIT(AL_AXI_GPIO_CHANNEL2)))) {
        AlAxiGpio_ll_ClearInt(Handle->HwConfig.BaseAddress, AL_AXI_GPIO_CHANNEL1);
    } else if((IntStatus & BIT(AL_AXI_GPIO_CHANNEL2)) && (!(IntStatus & BIT(AL_AXI_GPIO_CHANNEL1)))) {
        AlAxiGpio_ll_ClearInt(Handle->HwConfig.BaseAddress, AL_AXI_GPIO_CHANNEL2);
    }

    AlAxiGpio_EventStruct Event =
    {
        .Channel = (IntStatus & BIT(AL_AXI_GPIO_CHANNEL1)) | (IntStatus & BIT(AL_AXI_GPIO_CHANNEL2)),
        .EventData = IntStatus
    };
    Handle->EventCallBack(Event, Handle->EventCallBackRef);
}

AL_S32 AlAxiGpio_Dev_IntInit(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel)
{
    AL_ASSERT((Handle != AL_NULL) && (Channel < AXI_GPIO_MAX_CHANNEL), AL_AXI_GPIO_ERR_ILLEGAL_PARAM);

    AL_U32 BaseAddr = Handle->HwConfig.BaseAddress;

    switch (Channel)
    {
    case AL_AXI_GPIO_CHANNEL1:
        AlAxiGpio_ll_SetChannelInt(BaseAddr, AL_AXI_GPIO_CHANNEL1, AL_AXI_GPIO_INT_ENABLE);
        break;
    case AL_AXI_GPIO_CHANNEL2:
        AlAxiGpio_ll_SetChannelInt(BaseAddr, AL_AXI_GPIO_CHANNEL2, AL_AXI_GPIO_INT_ENABLE);
        break;
    default:
        break;
    }

    AlIntr_RegHandler(Handle->HwConfig.IntrId, AL_NULL, AlAxiGpio_Dev_IntrHandler, Handle);

    return AL_OK;
}

AL_S32 AlAxiGpio_Dev_RegisterEventCallBack(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_EventCallBack CallBack, AL_VOID *CallBackRef)
{
    Handle->EventCallBack = CallBack;
    Handle->EventCallBackRef = CallBackRef;
    return AL_OK;
}

AlAxiGpio_HwConfigStruct *AlAxiGpio_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AlAxiGpio_HwConfigStruct *HwConfig = AL_NULL;

    for (Index = 0; Index < AL_AXI_GPIO_NUM_INSTANCE; Index++)
    {
        if (AlAxiGpio_HwCfg[Index].DeviceId == DevId)
        {
            HwConfig = &AlAxiGpio_HwCfg[Index];
            break;
        }
    }

    return HwConfig;
}
