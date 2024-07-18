/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_TC_HAL_H_
#define __AL_TC_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_tc_dev.h"


/**************************** Type Definitions *******************************/
typedef struct
{
    AL_TC_DevStruct      Dev;

} AL_TC_HalStruct;


/************************** Function Prototypes ******************************/

AL_S32 AlTc_Hal_Init(AL_TC_HalStruct **Handle, AL_U32 DevId,
                      AL_TC_TimerInitStruct *InitConfig, AL_TC_EventCallBack Callback);
AL_VOID AlTc_Hal_EnableOverflowMode(AL_TC_HalStruct *Handle);
AL_VOID AlTc_Hal_EnableIntervalMode(AL_TC_HalStruct *Handle);
AL_VOID AlTc_Hal_SetIntervalMaxVal(AL_TC_HalStruct *Handle, AL_U16 Value);
AL_U16 AlTc_Hal_GetCounterVal(AL_TC_HalStruct *Handle);
AL_VOID AlTc_Hal_EnableMatchMode(AL_TC_HalStruct *Handle, AL_BOOL State);
AL_VOID AlTc_Hal_SetMatchVal(AL_TC_HalStruct *Handle, AL_TC_MatchNumEnum MatchNum, AL_U16 Value);
AL_VOID AlTc_Hal_EnableEventTimerMode(AL_TC_HalStruct *Handle, AL_BOOL State);
AL_S32 AlTc_Hal_SetExtClkEdge(AL_TC_HalStruct *Handle, AL_TC_ClkEdgeEnum ClkEdge);
AL_VOID AlTc_Hal_SetEventTimerLevel(AL_TC_HalStruct *Handle, AL_TC_LevelEnum Level);
AL_VOID AlTc_Hal_SetEventTimerOvState(AL_TC_HalStruct *Handle, AL_BOOL State);
AL_U16 AlTc_Hal_GetEventTimerVal(AL_TC_HalStruct *Handle);
AL_VOID AlTc_Hal_SetWaveformPolarity(AL_TC_HalStruct *Handle, AL_TC_ClkEdgeEnum ClkEdge);
AL_VOID AlTc_Hal_EnableWaveOutput(AL_TC_HalStruct *Handle, AL_BOOL State);
AL_VOID AlTc_Hal_EnableCounter(AL_TC_HalStruct *Handle, AL_BOOL State);
AL_VOID AlTc_Hal_EnableIntr(AL_TC_HalStruct *Handle, AL_TC_IntrTypeEnum IntrType, AL_BOOL State);
AL_S32 AlTc_Hal_IoCtl(AL_TC_HalStruct *Handle, AL_TC_IoCtlCmdEnum Cmd, AL_TC_IoctlParamUnion *IoctlParam);

#ifdef __cplusplus
}
#endif

#endif
