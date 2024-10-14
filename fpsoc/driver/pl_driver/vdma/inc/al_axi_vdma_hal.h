/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_VDMA_HAL_H_
#define AL_AXI_VDMA_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_axi_vdma_dev.h"

typedef struct
{
    AlAxiVdma_Dev_Struct        Dev;
} AlAxiVdma_Hal_Struct;

AL_S32 AlAxiVdma_Hal_Init(AlAxiVdma_Hal_Struct **Handle, AL_U32 DevId,
                          AlAxiVdma_InitConfig *InitConfig, AlAxiVdma_EventCallBack CallBack);
AL_S32 AlAxiVdma_Hal_Start(AlAxiVdma_Hal_Struct *Handle, AlAxiVdma_ChannelEnum Channel);
AL_S32 AlAxiVdma_Hal_Stop(AlAxiVdma_Hal_Struct *Handle, AlAxiVdma_ChannelEnum Channel);

#ifdef __cplusplus
}
#endif

#endif
