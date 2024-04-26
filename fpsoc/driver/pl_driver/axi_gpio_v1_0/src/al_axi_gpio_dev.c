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
    Handle->HwConfig = *HwConfig;
    return AL_OK;
}

AL_S32 AlAxiGpio_Dev_WritePin(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel, AL_U32 Bit, AL_U32 Value)
{
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
    AlAxiGpio_Dev_Struct *Handle = Instance;
    AL_U32 BaseAddr = Handle->HwConfig.BaseAddress;
    AL_U32 IntStatus = AlAxiGpio_ll_GetIntStatus(BaseAddr);
    
    /* Channel 1 Interrupt */
    if (IntStatus & (AlAxiGpio_ll_GetChannelInt(BaseAddr, AL_AXI_GPIO_CHANNEL1) << AL_AXI_GPIO_CHANNEL1))
    {
        AlAxiGpio_ll_ClearInt(BaseAddr, AL_AXI_GPIO_CHANNEL1);
        AlAxiGpio_EventStruct Event = 
        {
            .EventData = AL_AXI_GPIO_CHANNEL1
        };
        Handle->EventCallBack(&Event, Handle->EventCallBackRef);
    }
    /* Channel 2 Interrupt */
    else if (IntStatus & (AlAxiGpio_ll_GetChannelInt(BaseAddr, AL_AXI_GPIO_CHANNEL2) << AL_AXI_GPIO_CHANNEL2))
    {
        AlAxiGpio_ll_ClearInt(BaseAddr, AL_AXI_GPIO_CHANNEL2);
        AlAxiGpio_EventStruct Event = 
        {
            .EventData = AL_AXI_GPIO_CHANNEL2
        };
        Handle->EventCallBack(&Event, Handle->EventCallBackRef);
    }
}

AL_S32 AlAxiGpio_Dev_IntInit(AlAxiGpio_Dev_Struct *Handle, AlAxiGpio_ll_ChannelEnum Channel)
{
    AL_U32 BaseAddr = Handle->HwConfig.BaseAddress;

    AlAxiGpio_ll_SetGlobalInt(BaseAddr, AL_AXI_GPIO_INT_ENABLE);
    switch (Channel)
    {
    case AL_AXI_GPIO_CHANNEL1: 
        AlAxiGpio_ll_SetChannelInt(BaseAddr, AL_AXI_GPIO_CHANNEL1, AL_AXI_GPIO_INT_ENABLE); 
        break;
    case AL_AXI_GPIO_CHANNEL2: 
        AlAxiGpio_ll_SetChannelInt(BaseAddr, AL_AXI_GPIO_CHANNEL2, AL_AXI_GPIO_INT_ENABLE); 
        break;
    case AL_AXI_GPIO_ALL_CHAN: 
        AlAxiGpio_ll_SetChannelInt(BaseAddr, AL_AXI_GPIO_CHANNEL1, AL_AXI_GPIO_INT_ENABLE);
        AlAxiGpio_ll_SetChannelInt(BaseAddr, AL_AXI_GPIO_CHANNEL2, AL_AXI_GPIO_INT_ENABLE); 
        break;
    default: break;
    }

    AL_INTR_AttrStrct intr_attr = { LEVEL_HIGH_TRIGGER, 4 };
    AlIntr_RegHandler(Handle->HwConfig.IntrId, &intr_attr, AlAxiGpio_Dev_IntrHandler, Handle);
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
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