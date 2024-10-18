/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



/***************************** Include Files *********************************/
#include "al_vdma_hal.h"

AlAxiVdma_Hal_Struct AXI_VDMA_Handle[AL_VDMA_NUM_INSTANCE];


static AL_VOID AlAxiVdma_Hal_DefEventHandler(AlAxiVdma_EventStruct VdmaEvent, AL_VOID *CallbackRef)
{
    AlAxiVdma_Hal_Struct *Handle = (AlAxiVdma_Hal_Struct *)CallbackRef;

    switch (VdmaEvent.Events)
    {
    case AlAxiVdma_Event_Mm2s_FRAMECNT:
        break;
    case AlAxiVdma_Event_Mm2s_ERRIRQ:
        break;
    case AlAxiVdma_Event_S2Mm_FRAMECNT:
        break;
    case AlAxiVdma_Event_S2Mm_ERRIRQ:
        break;
    default:
        break;
    }
}

AL_S32 AlAxiVdma_Hal_Init(AlAxiVdma_Hal_Struct **Handle, AL_U32 DevId, AlAxiVdma_EventCallBack CallBack)
{
    AlAxiVdma_HwConfigStruct *HwConfig;
    AL_S32 Ret;
    AL_INTR_AttrStrct VdmaRstAttrStrct = {POSTIVE_EDGE_TRIGGER, 4};

    HwConfig = AlAxiVdma_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AXI_VDMA_Handle[DevId];
    } else {
        return AL_AXI_VDMA_ERR_ILLEGAL_PARAM;
    }

    Ret = AlAxiVdma_Dev_Init(&(*Handle)->Dev, DevId);
    if (Ret != AL_OK) {
        return Ret;
    }

    if(CallBack == AL_NULL) {
        Ret = AlAxiVdma_Dev_RegisterEventCallBack(&(*Handle)->Dev, AlAxiVdma_Hal_DefEventHandler, (AL_VOID *)Handle);
    } else {
        Ret = AlAxiVdma_Dev_RegisterEventCallBack(&(*Handle)->Dev, CallBack, (AL_VOID *)Handle);
    }

    if ((*Handle)->Dev.HwConfig.HasMm2s) {
        (AL_VOID)AlIntr_RegHandler((*Handle)->Dev.HwConfig.Mm2s_IntrId, &VdmaRstAttrStrct, AlAxiVdma_Dev_Mm2sIntrHandler, &(*Handle)->Dev);
    }

    if ((*Handle)->Dev.HwConfig.HasS2Mm) {
        (AL_VOID)AlIntr_RegHandler((*Handle)->Dev.HwConfig.S2Mm_IntrId, &VdmaRstAttrStrct, AlAxiVdma_Dev_S2MmIntrHandler, &(*Handle)->Dev);
    }

    Ret = AlOsal_Lock_Init(&(*Handle)->Mm2s_Lock, "AxiVdma_Mm2s_Lock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Lock_Init(&(*Handle)->S2mm_Lock, "AxiVdma_S2mm_Lock");
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}

AL_S32 AlAxiVdma_Hal_Start(AlAxiVdma_Hal_Struct *Handle, AlAxiVdma_ChannelEnum Channel, AlAxiVdma_ChannelCfg *ChannelCfg)
{
    AL_S32 Ret = AL_OK;

    AlAxiVdma_EventStruct VdmaEvent = {0};

    AL_ASSERT((Handle != AL_NULL), AL_AXI_VDMA_ERR_ILLEGAL_PARAM);
    AL_ASSERT((ChannelCfg != AL_NULL), AL_AXI_VDMA_ERR_ILLEGAL_PARAM);

    if ((Handle->Dev.HwConfig.HasMm2s) && (Channel == AL_VDMA_MM2S_CHANNEL)) {
        AlOsal_Lock_Take(&(Handle->Mm2s_Lock), AL_WAITFOREVER);

        AlAxiVdma_Dev_SetMm2sConfig(&(Handle->Dev), ChannelCfg);

        Ret = AlAxiVdma_Dev_StartMm2s(&(Handle->Dev), ChannelCfg);
        if (Ret != AL_OK) {
            (AL_VOID)AlOsal_Lock_Release(&Handle->Mm2s_Lock);
            return Ret;
        }

        AlOsal_Lock_Release(&(Handle->Mm2s_Lock));
    }

    if (Handle->Dev.HwConfig.HasS2Mm) {
        AlOsal_Lock_Take(&(Handle->S2mm_Lock), AL_WAITFOREVER);

        AlAxiVdma_Dev_SetS2MmConfig(&(Handle->Dev), ChannelCfg);

        Ret = AlAxiVdma_Dev_StartS2Mm(&(Handle->Dev), ChannelCfg);
        if (Ret != AL_OK) {
            (AL_VOID)AlOsal_Lock_Release(&Handle->S2mm_Lock);
            return Ret;
        }

        AlOsal_Lock_Release(&(Handle->S2mm_Lock));
    }
}

AL_S32 AlAxiVdma_Hal_Stop(AlAxiVdma_Hal_Struct *Handle, AlAxiVdma_ChannelEnum Channel)
{
    AL_S32 Ret = AL_OK;

    if ((Handle->Dev.HwConfig.HasMm2s) && (Channel == AL_VDMA_MM2S_CHANNEL)) {

        AlOsal_Lock_Take(&(Handle->Mm2s_Lock), AL_WAITFOREVER);

        AlAxiVdma_ll_SetMm2sStart(Handle->Dev.HwConfig.BaseAddress, 0);

        AlOsal_Lock_Release(&(Handle->Mm2s_Lock));
    }

    if ((Handle->Dev.HwConfig.HasS2Mm) && (Channel == AL_VDMA_S2MM_CHANNEL)) {

        AlOsal_Lock_Take(&(Handle->S2mm_Lock), AL_WAITFOREVER);

        AlAxiVdma_ll_SetS2MmStart(Handle->Dev.HwConfig.BaseAddress, 0);

        AlOsal_Lock_Release(&(Handle->S2mm_Lock));
    }
}