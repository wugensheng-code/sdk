/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_iic_hal.h"

AlAxiIic_HalStruct AXI_IIC_Handle[AL_AXI_IIC_NUM_INSTANCE];

AL_S32 AlAxiIic_Hal_Init(AlAxiIic_HalStruct **Dev, AL_U32 DevId, AlAxiIic_InitStruct *InitConfig, AlAxiIic_EventCallBack EventCallBack)
{
    AL_U32 Status;
    AlAxiIic_HwConfigStruct *HwConfig = AL_NULL;

    HwConfig = AlAxiIic_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL)
    {
        *Dev = &AXI_IIC_Handle[DevId];
    }
    else 
    {
        return;
    }

    Status = AlAxiIic_Dev_Init(&(*Dev)->Iic, HwConfig, InitConfig);
    if (Status != AL_OK)
    {
        return;
    }

    if (EventCallBack != AL_NULL)
    {
        AlAxiIic_Dev_RegisterEventCallBack(&(*Dev)->Iic, EventCallBack, *Dev);
    }
    return AL_OK;
}

AL_S32 AlAxiIic_Hal_MasterSendData(AlAxiIic_HalStruct **Dev, AL_U32 SlaveAddr, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_S32 Ret = AL_OK;

    Ret = AlOsal_Lock_Take(&(*Dev)->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlAxiIic_Dev_MasterSendData(&(*Dev)->Iic, SlaveAddr, SendBuf, SendSize);
    if (Ret != AL_OK)
    {
        return Ret;
    }

    (AL_VOID)AlOsal_Lock_Release(&(*Dev)->Lock);

    return AL_OK;
}

AL_S32 AlAxiIic_Hal_MasterRecvData(AlAxiIic_HalStruct **Dev, AL_U32 SlaveAddr, AL_U8 *RecvBuf, AL_U32 RecvSize)
{
    AL_S32 Ret = AL_OK;

    Ret = AlOsal_Lock_Take(&(*Dev)->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlAxiIic_Dev_MasterRecvData(&(*Dev)->Iic, SlaveAddr, RecvBuf, RecvSize);
    if (Ret != AL_OK)
    {
        return Ret;
    }

    (AL_VOID)AlOsal_Lock_Release(&(*Dev)->Lock);
    return AL_OK;
}

AL_S32 AlAxiIic_Hal_SlaveSendData(AlAxiIic_HalStruct **Dev, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_S32 Ret = AL_OK;

    Ret = AlOsal_Lock_Take(&(*Dev)->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlAxiIic_SlaveSendData(&(*Dev)->Iic, SendBuf, SendSize);
    if (Ret != AL_OK)
    {
        return Ret;
    }

    (AL_VOID)AlOsal_Lock_Release(&(*Dev)->Lock);
    return AL_OK;
}

AL_S32 AlAxiIic_Hal_SlaveRecvData(AlAxiIic_HalStruct **Dev, AL_U8 *RecvBuf)
{
    AL_S32 Ret = AL_OK;

    Ret = AlOsal_Lock_Take(&(*Dev)->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }
    Ret = AlAxiIic_Dev_SlaveRecvData(&(*Dev)->Iic, RecvBuf);
    if (Ret != AL_OK)
    {
        return Ret;
    }
    (AL_VOID)AlOsal_Lock_Release(&(*Dev)->Lock);
    return AL_OK;
}
