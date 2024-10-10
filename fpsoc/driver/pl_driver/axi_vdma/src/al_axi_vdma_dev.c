/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



/***************************** Include Files *********************************/
#include "al_axi_vdma_dev.h"

#define INITIALIZATION_POLLING   100000

#define AL_AXI_VDMA_VIRT_TO_PHYS(VirtAddr) (VirtAddr)

extern AlAxiVdma_HwConfigStruct AlAxiVdma_HwCfg[AL_AXI_VDMA_NUM_INSTANCE];

AlAxiVdma_HwConfigStruct *AlAxiVdma_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AlAxiVdma_HwConfigStruct *ConfigPtr = AL_NULL;
    for (Index = 0; Index < AL_AXI_VDMA_NUM_INSTANCE; Index++)
    {
        if (AlAxiVdma_HwCfg[Index].DeviceId == DevId)
        {
            ConfigPtr = &AlAxiVdma_HwCfg[Index];
            break;
        }
    }
    return ConfigPtr;
}

AL_BOOL AlAxiVdma_Dev_IsRuningMm2s(AlAxiVdma_Dev_Struct *Vdma)
{
    AL_BOOL Status;

    Status = AlAxiVdma_ll_IsHaltMm2s(Vdma->HwConfig.BaseAddress);
    if (Status) {
        return 0;
    }

    Status = AlAxiVdma_ll_IsRunMm2s(Vdma->HwConfig.BaseAddress);
    if (!Status) {
        return 0;
    }

    return 1;
}

AL_U32 AlAxiVdma_Dev_StartMm2s(AlAxiVdma_Dev_Struct *Vdma, AlAxiVdma_ReadChannelCfg ReadChannelCfg)
{
    AL_BOOL Status;

    if (!AlAxiVdma_Dev_IsRuningMm2s(Vdma)) {
        AlAxiVdma_ll_SetMm2sStart(Vdma->HwConfig.BaseAddress, AL_TRUE);
    }

    if (AlAxiVdma_Dev_IsRuningMm2s(Vdma)) {
        AlAxiVdma_ll_SetMm2sVsize(Vdma->HwConfig.BaseAddress, ReadChannelCfg.Mm2sVsize);
    } else {
        return -1;
    }

    return AL_OK;
}

AL_U32 AlAxiVdma_Dev_SetMm2sConfig(AlAxiVdma_Dev_Struct *Vdma, AlAxiVdma_ReadChannelCfg ReadChannelCfg)
{

    AlAxiVdma_ll_SetMm2sIrqDelayCount(Vdma->HwConfig.BaseAddress, ReadChannelCfg.DelayCount);
    AlAxiVdma_ll_SetMm2sIrqFrameCount(Vdma->HwConfig.BaseAddress, ReadChannelCfg.FrameCount);
    AlAxiVdma_ll_SetMm2sRepeatEn(Vdma->HwConfig.BaseAddress, ReadChannelCfg.RepeatEn);
    AlAxiVdma_ll_SetMm2sErrIrqEn(Vdma->HwConfig.BaseAddress, ReadChannelCfg.ErrIrqEn);
    AlAxiVdma_ll_SetMm2sDlyCntIrqEn(Vdma->HwConfig.BaseAddress, ReadChannelCfg.DlyCntIrqEn);
    AlAxiVdma_ll_SetMm2sFrmCntIrqEn(Vdma->HwConfig.BaseAddress, ReadChannelCfg.FrmCntIrqEn);
    AlAxiVdma_ll_SetMm2sRdPntrNum(Vdma->HwConfig.BaseAddress, ReadChannelCfg.PntrNum);
    AlAxiVdma_ll_SetMm2sGenlockSrc(Vdma->HwConfig.BaseAddress, ReadChannelCfg.GenlockSrc);
    AlAxiVdma_ll_SetMm2sFrameCntEn(Vdma->HwConfig.BaseAddress, ReadChannelCfg.FrameCntEn);
    AlAxiVdma_ll_SetMm2sGenlockEn(Vdma->HwConfig.BaseAddress, ReadChannelCfg.GenlockEn);
    AlAxiVdma_ll_SetMm2sCircularPark(Vdma->HwConfig.BaseAddress, ReadChannelCfg.CircularPark);
    AlAxiVdma_ll_SetMm2sRegIndex(Vdma->HwConfig.BaseAddress, ReadChannelCfg.RegIndex);

    for (AL_U32 i = 0; i < Vdma->HwConfig.MaxFrameStoreNum; i++) {
        AlAxiVdma_ll_SetMm2sStartAddr((Vdma->HwConfig.BaseAddress + i * 4), ReadChannelCfg.FrameStoreStartAddr[i]);
    }

    AlAxiVdma_ll_SetMm2sHsize(Vdma->HwConfig.BaseAddress, ReadChannelCfg.Mm2sHsize);
    AlAxiVdma_ll_SetMm2sFrameDelay(Vdma->HwConfig.BaseAddress, ReadChannelCfg.FrameDelay);
    AlAxiVdma_ll_SetMm2sStride(Vdma->HwConfig.BaseAddress,ReadChannelCfg.Stride);

    return AL_OK;
}


AL_BOOL AlAxiVdma_Dev_IsRuningS2Mm(AlAxiVdma_Dev_Struct *Vdma)
{
    AL_BOOL Status;

    Status = AlAxiVdma_ll_IsHaltS2Mm(Vdma->HwConfig.BaseAddress);
    if (Status) {
        return 0;
    }

    Status = AlAxiVdma_ll_IsRunS2Mm(Vdma->HwConfig.BaseAddress);
    if (!Status) {
        return 0;
    }

    return 1;
}

AL_U32 AlAxiVdma_Dev_StartS2Mm(AlAxiVdma_Dev_Struct *Vdma, AlAxiVdma_WriteChannelCfg WriteChannelCfg)
{
    AL_BOOL Status;

    if (!AlAxiVdma_Dev_IsRuningS2Mm(Vdma)) {
        AlAxiVdma_ll_SetS2MmStart(Vdma->HwConfig.BaseAddress, AL_TRUE);
    }

    if (AlAxiVdma_Dev_IsRuningS2Mm(Vdma)) {
        AlAxiVdma_ll_SetS2MmVsize(Vdma->HwConfig.BaseAddress, WriteChannelCfg.S2MmVsize);
    } else {
        return -1;
    }

    return AL_OK;
}

AL_U32 AlAxiVdma_Dev_SetS2MmConfig(AlAxiVdma_Dev_Struct *Vdma, AlAxiVdma_WriteChannelCfg WriteChannelCfg)
{

    AlAxiVdma_ll_SetS2MmIrqDelayCount(Vdma->HwConfig.BaseAddress, WriteChannelCfg.DelayCount);
    AlAxiVdma_ll_SetS2MmIrqFrameCount(Vdma->HwConfig.BaseAddress, WriteChannelCfg.FrameCount);
    AlAxiVdma_ll_SetS2MmRepeatEn(Vdma->HwConfig.BaseAddress, WriteChannelCfg.RepeatEn);
    AlAxiVdma_ll_SetS2MmErrIrqEn(Vdma->HwConfig.BaseAddress, WriteChannelCfg.ErrIrqEn);
    AlAxiVdma_ll_SetS2MmDlyCntIrqEn(Vdma->HwConfig.BaseAddress, WriteChannelCfg.DlyCntIrqEn);
    AlAxiVdma_ll_SetS2MmFrmCntIrqEn(Vdma->HwConfig.BaseAddress, WriteChannelCfg.FrmCntIrqEn);
    AlAxiVdma_ll_SetS2MmRdPntrNum(Vdma->HwConfig.BaseAddress, WriteChannelCfg.PntrNum);
    AlAxiVdma_ll_SetS2MmGenlockSrc(Vdma->HwConfig.BaseAddress, WriteChannelCfg.GenlockSrc);
    AlAxiVdma_ll_SetS2MmFrameCntEn(Vdma->HwConfig.BaseAddress, WriteChannelCfg.FrameCntEn);
    AlAxiVdma_ll_SetS2MmGenlockEn(Vdma->HwConfig.BaseAddress, WriteChannelCfg.GenlockEn);
    AlAxiVdma_ll_SetS2MmCircularPark(Vdma->HwConfig.BaseAddress, WriteChannelCfg.CircularPark);
    AlAxiVdma_ll_SetS2MmRegIndex(Vdma->HwConfig.BaseAddress, WriteChannelCfg.RegIndex);

    for (AL_U32 i = 0; i < Vdma->HwConfig.MaxFrameStoreNum; i++) {
        AlAxiVdma_ll_SetS2MmStartAddr((Vdma->HwConfig.BaseAddress + i * 4), WriteChannelCfg.FrameStoreStartAddr[i]);
    }

    AlAxiVdma_ll_SetS2MmHsize(Vdma->HwConfig.BaseAddress, WriteChannelCfg.S2MmHsize);
    AlAxiVdma_ll_SetS2MmFrameDelay(Vdma->HwConfig.BaseAddress, WriteChannelCfg.FrameDelay);
    AlAxiVdma_ll_SetS2MmStride(Vdma->HwConfig.BaseAddress,WriteChannelCfg.Stride);

    return AL_OK;
}

AL_U32 AlAxiVdma_Dev_MaskIrqS2Mm(AlAxiVdma_Dev_Struct *Vdma, AlAxiVdma_S2MmMaskIntrEnum MaskIrqS2Mm)
{
    AlAxiVdma_ll_MaskIrqS2Mm(Vdma->HwConfig.BaseAddress, (MaskIrqS2Mm << 1));

    return AL_OK;
}

AL_S32 AlAxiVdma_Dev_Init(AlAxiVdma_Dev_Struct *Vdma, AL_U32 DevId, AlAxiVdma_InitConfig *InitConfig)
{
    AL_S32 Status;
    AlAxiVdma_HwConfigStruct *VdmaHwConfig = AL_NULL;

    VdmaHwConfig = AlAxiVdma_Dev_LookupConfig(DevId);

    Vdma->HwConfig = *VdmaHwConfig;

    Vdma->InitConfig = *InitConfig;

    if (VdmaHwConfig->HasMm2s) {
        AlAxiVdma_ll_SetMm2sReset(Vdma->HwConfig.BaseAddress, 1);
        while (AlAxiVdma_ll_GetMm2sResetStatus(Vdma->HwConfig.BaseAddress));

        AlAxiVdma_Dev_SetMm2sConfig(Vdma, Vdma->InitConfig.ReadChannelCfg);
    }

    if (VdmaHwConfig->HasS2Mm) {
        AlAxiVdma_ll_SetS2MmReset(Vdma->HwConfig.BaseAddress, 1);
        while (AlAxiVdma_ll_GetS2MmResetStatus(Vdma->HwConfig.BaseAddress));

        AlAxiVdma_Dev_SetS2MmConfig(Vdma, Vdma->InitConfig.WriteChannelCfg);
    }

    if (VdmaHwConfig->HasVFlip) {
        AlAxiVdma_ll_EnableVerticalFlip(Vdma->HwConfig.BaseAddress, Vdma->InitConfig.EnableVerticalFlip);
    }

    return AL_OK;
}

static AL_U32 AlAxiVdma_Dev_GetMm2sPendingIntr(AlAxiVdma_Dev_Struct *Vdma)
{
    return (AlAxiVdma_ll_GetMm2sStatus(Vdma->HwConfig.BaseAddress) & AL_AXI_VDMA_ALL_INTR_MASK);
}

static AL_U32 AlAxiVdma_Dev_GetMm2sEnableIntr(AlAxiVdma_Dev_Struct *Vdma)
{
    return (AlAxiVdma_ll_GetMm2sControlReg(Vdma->HwConfig.BaseAddress) & AL_AXI_VDMA_ALL_INTR_MASK);
}

static AL_VOID AlAxiVdma_Dev_Mm2sHandler(AlAxiVdma_Dev_Struct *Vdma)
{
    AL_U32 PendingIntr;
    AlAxiVdma_EventStruct VdmaEvent = {
        .Events = AlAxiVdma_Event_Mm2S,
    };

    PendingIntr = AlAxiVdma_Dev_GetMm2sPendingIntr(Vdma);
    PendingIntr &= AlAxiVdma_Dev_GetMm2sEnableIntr(Vdma);


    if (Vdma->EventCallBack) {
        (*Vdma->EventCallBack)(VdmaEvent, Vdma->EventCallBackRef);
    }

    AlAxiVdma_ll_ClearMm2sIntr(Vdma->HwConfig.BaseAddress, (PendingIntr & AL_AXI_VDMA_ALL_INTR_MASK));
}

AL_VOID AlAxiVdma_Dev_Mm2sIntrHandler(AL_VOID *Instance)
{
    AlAxiVdma_Dev_Struct *Vdma = (AlAxiVdma_Dev_Struct *)Instance;
    AL_U32 Mm2SIntrStatus = AlAxiVdma_ll_GetMm2sStatus(Vdma->HwConfig.BaseAddress);

    if (Mm2SIntrStatus) {
        AlAxiVdma_Dev_Mm2sHandler(Vdma);
    }
}

static AL_U32 AlAxiVdma_Dev_GetS2MmPendingIntr(AlAxiVdma_Dev_Struct *Vdma)
{
    return (AlAxiVdma_ll_GetS2MmStatus(Vdma->HwConfig.BaseAddress) & AL_AXI_VDMA_ALL_INTR_MASK);
}

static AL_U32 AlAxiVdma_Dev_GetS2MmEnableIntr(AlAxiVdma_Dev_Struct *Vdma)
{
    return (AlAxiVdma_ll_GetS2MmControlReg(Vdma->HwConfig.BaseAddress) & AL_AXI_VDMA_ALL_INTR_MASK);
}

static AL_VOID AlAxiVdma_Dev_S2MmHandler(AlAxiVdma_Dev_Struct *Vdma)
{
    AL_U32 PendingIntr;
    AlAxiVdma_EventStruct VdmaEvent = {
        .Events = AlAxiVdma_Event_S2Mm,
    };

    PendingIntr = AlAxiVdma_Dev_GetS2MmPendingIntr(Vdma);
    PendingIntr &= AlAxiVdma_Dev_GetS2MmEnableIntr(Vdma);

    if (Vdma->EventCallBack) {
        (*Vdma->EventCallBack)(VdmaEvent, Vdma->EventCallBackRef);
    }

    AlAxiVdma_ll_ClearS2MmIntr(Vdma->HwConfig.BaseAddress, (PendingIntr & AL_AXI_VDMA_ALL_INTR_MASK));
}

AL_VOID AlAxiVdma_Dev_S2MmIntrHandler(AL_VOID *Instance)
{
    AlAxiVdma_Dev_Struct *Vdma = (AlAxiVdma_Dev_Struct *)Instance;
    AL_U32 S2MmIntrStatus = AlAxiVdma_ll_GetS2MmStatus(Vdma->HwConfig.BaseAddress);

    if (S2MmIntrStatus) {
        AlAxiVdma_Dev_S2MmHandler(Vdma);
    }
}

AL_S32 AlAxiVdma_Dev_RegisterEventCallBack(AlAxiVdma_Dev_Struct *Vdma, AlAxiVdma_EventCallBack Callback, void *CallbackRef)
{
    Vdma->EventCallBack        = Callback;
    Vdma->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

AL_S32 AlAxiVdma_Dev_UnRegisterEventCallBack(AlAxiVdma_Dev_Struct *Vdma)
{
    Vdma->EventCallBack = (AlAxiVdma_EventCallBack)AL_NULL;

    return AL_OK;
}
