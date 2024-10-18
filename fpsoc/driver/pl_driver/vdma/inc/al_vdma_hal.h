/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_VDMA_HAL_H_
#define AL_VDMA_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_vdma_dev.h"

typedef struct
{
    AlAxiVdma_Dev_Struct        Dev;
    AL_Lock                 Mm2s_Lock;
    AL_Lock                 S2mm_Lock;
    AL_MailBox              Mm2s_EventQueue;
    AL_MailBox              S2mm_EventQueue;
} AlAxiVdma_Hal_Struct;

AL_S32 AlAxiVdma_Hal_Init(AlAxiVdma_Hal_Struct **Handle, AL_U32 DevId, AlAxiVdma_EventCallBack CallBack);
AL_S32 AlAxiVdma_Hal_Start(AlAxiVdma_Hal_Struct *Handle, AlAxiVdma_ChannelEnum Channel, AlAxiVdma_ChannelCfg *ChannelCfg);
AL_S32 AlAxiVdma_Hal_Stop(AlAxiVdma_Hal_Struct *Handle, AlAxiVdma_ChannelEnum Channel);

#ifdef __cplusplus
}
#endif

#endif
