/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_TTC_HAL_H_
#define __AL_TTC_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_ttc_dev.h"


/**************************** Type Definitions *******************************/
typedef struct
{
    AL_TTC_DevStruct      Dev;

} AL_TTC_HalStruct;


/************************** Function Prototypes ******************************/

AL_S32 AlTtc_Hal_Init(AL_TTC_HalStruct **Handle, AL_U32 DevId,
                      AL_TTC_TimerInitStruct *InitConfig, AL_TTC_EventCallBack Callback);
AL_VOID AlTtc_Hal_EnableOverflowMode(AL_TTC_HalStruct *Handle);
AL_VOID AlTtc_Hal_EnableIntervalMode(AL_TTC_HalStruct *Handle);
AL_S32 AlTtc_Hal_SetIntervalMaxVal(AL_TTC_HalStruct *Handle, AL_U16 Value);
AL_U16 AlTtc_Hal_GetCounterVal(AL_TTC_HalStruct *Handle);
AL_S32 AlTtc_Hal_EnableMatchMode(AL_TTC_HalStruct *Handle, AL_BOOL State);
AL_S32 AlTtc_Hal_SetMatchVal(AL_TTC_HalStruct *Handle, AL_TTC_MatchNumEnum MatchNum, AL_U16 Value);
AL_S32 AlTtc_Hal_EnableEventTimerMode(AL_TTC_HalStruct *Handle, AL_BOOL State);
AL_S32 AlTtc_Hal_SelExtClkEdge(AL_TTC_HalStruct *Handle, AL_TTC_ClkEdgeEnum ClkEdge);
AL_S32 AlTtc_Hal_SetEventTimerLevel(AL_TTC_HalStruct *Handle, AL_TTC_LevelEnum Level);
AL_S32 AlTtc_Hal_EventTimerOv(AL_TTC_HalStruct *Handle, AL_BOOL State);
AL_U16 AltTtc_Hal_GetEventTimerVal(AL_TTC_HalStruct *Handle);
AL_S32 AlTtc_Hal_SetWaveformPolarity(AL_TTC_HalStruct *Handle, AL_TTC_ClkEdgeEnum ClkEdge);
AL_S32 AlTtc_Hal_EnableWaveOutput(AL_TTC_HalStruct *Handle, AL_BOOL State);
AL_S32 AlTtc_Hal_EnableCounter(AL_TTC_HalStruct *Handle, AL_BOOL State);
AL_S32 AlTtc_Hal_EnableIntr(AL_TTC_HalStruct *Handle, AL_TTC_IntrTypeEnum IntrType);
AL_S32 AlTtc_Hal_IoCtl(AL_TTC_HalStruct *Handle, AL_TTC_IoCtlCmdEnum Cmd, AL_TTC_IoctlParamUnion *IoctlParam);

#ifdef __cplusplus
}
#endif

#endif
