/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_iic_dev.h"

extern AlAxiIic_HwConfigStruct AlAxiIic_HwConfig[AL_AXI_IIC_NUM_INSTANCE];

AL_S32 AlAxiIic_Dev_CheckID(AlAxiIic_DevStruct *Iic)
{
    AL_U32 ReadId = 0;
    AL_U64 BaseAddr = Iic->HwConfig.BaseAddress;

    ReadId = AlAxiIic_ReadId(BaseAddr);
    if (ReadId != AXI_IIC_ID)
    {
        return ReadId;
    }
    else
    {
        return AL_OK;
    }
}

AlAxiIic_HwConfigStruct *AlAxiIic_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AlAxiIic_HwConfigStruct *ConfigPtr = AL_NULL;
    for (Index = 0; Index < AL_AXI_IIC_NUM_INSTANCE; Index++) 
    {
        if (AlAxiIic_HwConfig[Index].DeviceId == DevId)
        {
            ConfigPtr = &AlAxiIic_HwConfig[Index];
            break;
        }
    }
    return ConfigPtr;
}

AL_S32 AlAxiIic_Dev_Init(AlAxiIic_DevStruct *Iic, AlAxiIic_HwConfigStruct *HwConfig, AlAxiIic_InitStruct *InitConfig)
{
    AL_S32 Status;
    AL_U64 BaseAddr;

    Iic->HwConfig = *HwConfig;
    Iic->InitConfig = *InitConfig;

    BaseAddr = Iic->HwConfig.BaseAddress;
    Iic->FifoSize = AlAxiIic_GetFifoSize(BaseAddr);
    Iic->Status = AL_AXI_IIC_IDLE;

    AlAxiIic_Dev_Reset(Iic);
    /* Check IIC ID */
    Status = AlAxiIic_Dev_CheckID(Iic);
    if (Status != AL_OK)
    {
        return Status;
    }
    
    AlAxiIic_ll_SetEnable(BaseAddr, AL_AXI_IIC_FUNC_ENABLE);
    AlAxiIic_SetMode(BaseAddr, Iic->InitConfig.IIC_Mode);
    AlAxiIic_SetAddressMode(BaseAddr, Iic->InitConfig.IIC_AddressMode);
    AlAxiIic_Dev_SetSpeedMode(BaseAddr, Iic->InitConfig.SpeedMode, Iic->InitConfig.Taclk);

    if (Iic->InitConfig.IIC_Mode == AL_AXI_IIC_SLAVE)
    {
        AlAxiIic_SetSlaveAddr(BaseAddr, Iic->InitConfig.SlaveAddr);
    }

    AL_INTR_AttrStrct intr_attr = { LEVEL_HIGH_TRIGGER, 4 };
    AlIntr_RegHandler(Iic->HwConfig.IntrId, &intr_attr, AlAxiIic_Dev_IrqHandler, Iic);
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    return AL_OK;
}

AL_VOID AlAxiIic_Dev_CreateTrans(AlAxiIic_DevStruct *Iic, AlAxiIic_StopModeEnum StopMode)
{
    AL_U64 BaseAddr = Iic->HwConfig.BaseAddress;

    AlAxiIic_SetPhaseStart(BaseAddr, AL_AXI_IIC_FUNC_ENABLE);
    AlAxiIic_SetPhaseAddr(BaseAddr, AL_AXI_IIC_FUNC_ENABLE);
    AlAxiIic_SetPhaseData(BaseAddr, AL_AXI_IIC_FUNC_ENABLE);

    if (StopMode == AL_AXI_IIC_STOP)
    {
        AlAxiIic_SetPhaseStop(BaseAddr, AL_AXI_IIC_FUNC_ENABLE);
    }
    else
    {
        AlAxiIic_SetPhaseStop(BaseAddr, AL_AXI_IIC_FUNC_DISABLE);
    }
}

AL_S32 AlAxiIic_Dev_MasterSendData(AlAxiIic_DevStruct *Iic, AL_U32 SlaveAddr, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_S32 i = 0;   
    AL_U64 BaseAddr = Iic->HwConfig.BaseAddress;
    
    AlAxiIic_SetSlaveAddr(BaseAddr, SlaveAddr);

    AlAxiIic_SetDir(BaseAddr, MASTER_TX_SLAVE_RX);
    AlAxiIic_SetDataCnt(BaseAddr, SendSize);
    AlAxiIic_Dev_StartTrans(Iic);
    Iic->Status = AL_AXI_IIC_MASTER_TX_ING;
    for (i = 0; i < SendSize; i++)
    {
        while (AlAxiIic_GetStatus(BaseAddr, SR_FIFO_FULL));
        AlAxiIic_SetData(BaseAddr, SendBuf[i]);
    }
    return AL_OK;
}

AL_S32 AlAxiIic_Dev_MasterRecvData(AlAxiIic_DevStruct *Iic, AL_U32 SlaveAddr, AL_U8 *RecvBuf, AL_U32 RecvSize)
{
    AL_S32 i = 0;
    
    AL_U64 BaseAddr = Iic->HwConfig.BaseAddress;

    AlAxiIic_SetSlaveAddr(BaseAddr, SlaveAddr);

    AlAxiIic_SetDir(BaseAddr, MASTER_RX_SLAVE_TX);
    AlAxiIic_SetDataCnt(BaseAddr, RecvSize);
    AlAxiIic_Dev_StartTrans(Iic);
    Iic->Status = AL_AXI_IIC_MASTER_RX_ING;
    for (i = 0; i < RecvSize; i++)
    {
        while (AlAxiIic_GetStatus(BaseAddr, SR_FIFO_EMPTY));
        RecvBuf[i] = (AL_U8)AlAxiIic_GetData(BaseAddr);
    }
    return AL_OK;
}

AL_S32 AlAxiIic_Dev_SlaveSendData(AlAxiIic_DevStruct *Iic, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_U64 BaseAddr = Iic->HwConfig.BaseAddress;

    Iic->TransBuffer.BuffPtr = SendBuf;
    Iic->TransBuffer.ReqCnt = SendSize;
    Iic->TransBuffer.DataCnt = 0;

    AlAxiIic_SetInt(BaseAddr, INT_ADDR_HIT, AL_AXI_IIC_FUNC_ENABLE);
    Iic->Status = AL_AXI_IIC_SLAVE_TX_ING;
    return AL_OK;
}

AL_S32 AlAxiIic_Dev_SlaveRecvData(AlAxiIic_DevStruct *Iic, AL_U8 *RecvBuf)
{
    AL_U64 BaseAddr = Iic->HwConfig.BaseAddress;

    Iic->RecvBuffer.BuffPtr = RecvBuf;
    Iic->RecvBuffer.DataCnt = 0;

    AlAxiIic_SetInt(BaseAddr, INT_ADDR_HIT, AL_AXI_IIC_FUNC_ENABLE);
    Iic->Status = AL_AXI_IIC_SLAVE_RX_ING;
    // while (!(Iic->Status == AL_AXI_IIC_SLAVE_RX_DONE));
    return AL_OK;
}

AL_VOID AlAxiIic_Dev_IrqHandler(AL_VOID *Instance)
{
    AlAxiIic_DevStruct *Iic = (AlAxiIic_DevStruct*)Instance;
    AL_U64 BaseAddr = Iic->HwConfig.BaseAddress;

    AL_U32 IntStatus = AlAxiIic_GetStatusAll(BaseAddr);
    
    if (IntStatus & AlAxiIic_GetInt(BaseAddr, INT_ADDR_HIT))
    {
        AlAxiIic_ClearIrq(BaseAddr, SR_ADDR_HIT);
        if (AlAxiIic_GetDir(BaseAddr) == MASTER_TX_SLAVE_RX)
        {
            AlAxiIic_SetInt(BaseAddr, INT_CMPL, AL_AXI_IIC_FUNC_ENABLE);
            AlAxiIic_SetInt(BaseAddr, INT_FIFO_FULL, AL_AXI_IIC_FUNC_ENABLE);
        }
        else if (AlAxiIic_GetDir(BaseAddr) == MASTER_RX_SLAVE_TX)
        {
            AlAxiIic_SetInt(BaseAddr, INT_CMPL, AL_AXI_IIC_FUNC_ENABLE);
            AlAxiIic_SetInt(BaseAddr, INT_FIFO_EMPTY, AL_AXI_IIC_FUNC_ENABLE);
        }
    }
    else if (IntStatus & AlAxiIic_GetInt(BaseAddr, INT_FIFO_FULL))
    {
        AlAxiIic_ClearIrq(BaseAddr, SR_FIFO_FULL);
        while (!AlAxiIic_GetStatus(BaseAddr, SR_FIFO_EMPTY))
        {
            Iic->RecvBuffer.BuffPtr[Iic->RecvBuffer.DataCnt] = AlAxiIic_GetData(BaseAddr);
            Iic->RecvBuffer.DataCnt++;
        }
        if (AlAxiIic_GetStatus(BaseAddr, SR_CMPL))
        {
            Iic->Status = AL_AXI_IIC_SLAVE_RX_DONE;
            AlAxiIic_EventStruct EventStruct = 
            {
                .Event = AL_AXI_IIC_SLAVE_RX_DONE,
                .EventData = Iic->RecvBuffer.DataCnt
            };
            Iic->EventCallBack(&EventStruct, Iic->EventCallBackRef);
        }
    }
    else if (IntStatus & AlAxiIic_GetInt(BaseAddr, INT_CMPL))
    {
        AlAxiIic_ClearIrq(BaseAddr, SR_CMPL);
        if (Iic->Status == AL_AXI_IIC_SLAVE_RX_ING)
        {   
            while (!AlAxiIic_GetStatus(BaseAddr, SR_FIFO_EMPTY))
            {
                Iic->RecvBuffer.BuffPtr[Iic->RecvBuffer.DataCnt] = AlAxiIic_GetData(BaseAddr);
                Iic->RecvBuffer.DataCnt++;
            }
            Iic->Status = AL_AXI_IIC_SLAVE_RX_DONE;
            AlAxiIic_EventStruct EventStruct = 
            {
                .Event = EVENT_RX_DONE,
                .EventData = Iic->RecvBuffer.DataCnt
            };
            Iic->EventCallBack(&EventStruct, Iic->EventCallBackRef);
        }
        else if (Iic->Status == AL_AXI_IIC_SLAVE_TX_ING)
        {
            Iic->Status = AL_AXI_IIC_SLAVE_TX_DONE;
            AlAxiIic_EventStruct EventStruct = 
            {
                .Event = EVENT_TX_DONE,
                .EventData = 0
            };
            Iic->EventCallBack(&EventStruct, Iic->EventCallBackRef);
        }
        else if (Iic->Status == AL_AXI_IIC_MASTER_TX_ING)
        {
            Iic->Status = AL_AXI_IIC_MASTER_TX_DONE;
            AlAxiIic_EventStruct EventStruct = 
            {
                .Event = EVENT_TX_DONE,
                .EventData = 0
            };
            Iic->EventCallBack(&EventStruct, Iic->EventCallBackRef);
        }
        else if (Iic->Status == AL_AXI_IIC_MASTER_RX_ING)
        {
            Iic->Status = AL_AXI_IIC_MASTER_RX_DONE;
            AlAxiIic_EventStruct EventStruct = 
            {
                .Event = EVENT_RX_DONE,
                .EventData = 0
            };
            Iic->EventCallBack(&EventStruct, Iic->EventCallBackRef);
        }
        AlAxiIic_Dev_ClrFifo(Iic);
    }
    else if (IntStatus & AlAxiIic_GetInt(BaseAddr, INT_FIFO_EMPTY))
    {
        AlAxiIic_ClearIrq(BaseAddr, SR_FIFO_EMPTY);
        while (!AlAxiIic_GetStatus(BaseAddr, SR_FIFO_FULL))
        {
            AlAxiIic_SetData(BaseAddr, Iic->TransBuffer.BuffPtr[Iic->TransBuffer.DataCnt]);
            if (Iic->TransBuffer.DataCnt < Iic->TransBuffer.ReqCnt)
            {
                Iic->TransBuffer.DataCnt++;
            }
        }
    }
}

AL_S32 AlAxiIic_Dev_StartTrans(AlAxiIic_DevStruct *Iic)
{
    AlAxiIic_SetCmd(Iic->HwConfig.BaseAddress, AL_AXI_IIC_CMD_TRANS);
}

AL_S32 AlAxiIic_Dev_Reset(AlAxiIic_DevStruct *Iic)
{
    AlAxiIic_SetCmd(Iic->HwConfig.BaseAddress, AL_AXI_IIC_CMD_RST);
}

AL_S32 AlAxiIic_Dev_ClrFifo(AlAxiIic_DevStruct *Iic)
{
    AlAxiIic_SetCmd(Iic->HwConfig.BaseAddress, AL_AXI_IIC_CMD_CLR_FIFO);
}

AL_S32 AlAxiIic_Dev_SetSpeedMode(AL_U64 BaseAddr, AlAxiIic_SpeedModeEnum SpeedMode, AL_U32 Taclk)
{
    AL_U32 Tsclh = 0;

    switch (SpeedMode)
    {
    case AL_AXI_IIC_STANDARD_MODE:      // T = 10000ns, Thigh = 5000ns, Tlow = 5000ns; 5000ns = 2*Taclk + (2+1+Tsclh)*Taclk
        AlAxiIic_SetSclRatio(BaseAddr, AL_AXI_IIC_SCL_RATIO_1);
        Tsclh = (5000 / Taclk) - 5; 
        break;          
    case AL_AXI_IIC_FAST_MODE:          // T = 2500ns, Thigh = 883ns, Tlow = 1666ns;
        AlAxiIic_SetSclRatio(BaseAddr, AL_AXI_IIC_SCL_RATIO_2);
        Tsclh = (883 / Taclk) - 5; 
        break;                                   
    case AL_AXI_IIC_HIGH_SPEED_MODE:    // 1000ns, Thigh = 333ns, Tlow = 666ns; 
        AlAxiIic_SetSclRatio(BaseAddr, AL_AXI_IIC_SCL_RATIO_2);
        Tsclh = (333 / Taclk) - 5; 
        break;                                   
    }
    AlAxiIic_SetSclHighTime(BaseAddr, Tsclh);
}

AL_S32 AlAxiIic_Dev_RegisterEventCallBack(AlAxiIic_DevStruct *Handle, AlAxiIic_EventCallBack CallBack, AL_VOID *CallBackRef)
{
    Handle->EventCallBack = CallBack;
    Handle->EventCallBackRef = CallBackRef;
    return AL_OK;
}