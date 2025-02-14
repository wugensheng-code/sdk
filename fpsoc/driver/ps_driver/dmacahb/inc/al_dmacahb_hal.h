/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_DMACAHB_HAL_H_
#define __AL_DMACAHB_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_dmacahb_dev.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/
typedef struct
{
    AL_DMACAHB_ChStruct     Channel;
    AL_Lock                 Lock;
    AL_MailBox              EventQueue;
    AL_MailBox              SrcEventQueue;
} AL_DMACAHB_HalStruct;

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
AL_S32 AlDmacAhb_Hal_Init(AL_DMACAHB_HalStruct **Handle, AL_U32 DevId, AL_DMACAHB_ChInitStruct *InitConfig,
                          AL_DMACAHB_ChEventCallBack CallBack);

AL_S32 AlDmacAhb_Hal_DeInit(AL_DMACAHB_HalStruct *Handle);

AL_S32 AlDmacAhb_Hal_Start(AL_DMACAHB_HalStruct *Handle);

AL_S32 AlDmacAhb_Hal_StartBlock(AL_DMACAHB_HalStruct *Handle, AL_U32 Timeout);

AL_S32 AlDmacAhb_Hal_IoCtl(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_IoCtlCmdEnum Cmd, AL_VOID *Data);

AL_S32 AlDmacAhb_Hal_WaitTransDoneOrTimeout(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_EventStruct *Event, AL_U32 TimeoutMs);

AL_S32 AlDmacAhb_Hal_WaitSrcTransDoneOrTimeout(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_EventStruct *Event, AL_U32 TimeoutMs);

#ifdef __cplusplus
}
#endif

#endif
