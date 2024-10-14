/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



/***************************** Include Files *********************************/
#include "al_axi_vdma_hal.h"

AlAxiVdma_Hal_Struct AXI_VDMA_Handle[AL_AXI_VDMA_NUM_INSTANCE];


static AL_VOID AlAxiVdma_Hal_DefEventHandler(AlAxiVdma_EventStruct VdmaEvent, AL_VOID *CallbackRef)
{
    AlAxiVdma_Hal_Struct *Handle = (AlAxiVdma_Hal_Struct *)CallbackRef;

    switch (VdmaEvent.Events)
    {
    case AlAxiVdma_Event_Mm2S:
        break;
    case AlAxiVdma_Event_S2Mm:
        break;
    default:
        break;
    }
}

AL_S32 AlAxiVdma_Hal_Init(AlAxiVdma_Hal_Struct **Handle, AL_U32 DevId,
                          AlAxiVdma_InitConfig *InitConfig, AlAxiVdma_EventCallBack CallBack)
{
    AlAxiVdma_HwConfigStruct *HwConfig;
    AL_S32 Ret;
    AL_INTR_AttrStrct VdmaRstAttrStrct = {POSTIVE_EDGE_TRIGGER, 4};

    HwConfig = AlAxiVdma_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AXI_VDMA_Handle[DevId];
    } else {
        return;
    }

    Ret = AlAxiVdma_Dev_Init(&(*Handle)->Dev, DevId, InitConfig);
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

    return Ret;
}

AL_S32 AlAxiVdma_Hal_Start(AlAxiVdma_Hal_Struct *Handle, AlAxiVdma_ChannelEnum Channel)
{
    if ((Handle->Dev.HwConfig.HasMm2s)) {
        AlAxiVdma_Dev_StartMm2s(&(Handle->Dev), Handle->Dev.InitConfig.ReadChannelCfg);
    }

    if (Handle->Dev.HwConfig.HasS2Mm) {
        AlAxiVdma_Dev_StartS2Mm(&(Handle->Dev), Handle->Dev.InitConfig.WriteChannelCfg);
    }
}

AL_S32 AlAxiVdma_Hal_Stop(AlAxiVdma_Hal_Struct *Handle, AlAxiVdma_ChannelEnum Channel)
{
    if ((Handle->Dev.HwConfig.HasMm2s) && (Channel == AL_VDMA_MM2S_CHANNEL)) {
        AlAxiVdma_ll_SetMm2sStart(Handle->Dev.HwConfig.BaseAddress, 0);
    }

    if ((Handle->Dev.HwConfig.HasS2Mm) && (Channel == AL_VDMA_S2MM_CHANNEL)) {
        AlAxiVdma_ll_SetS2MmStart(Handle->Dev.HwConfig.BaseAddress, 0);
    }
}