/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_ipc_Hal.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-03-29
 * @brief   IPC Halice driver
 */

#ifndef __AL_IPC_HAL_H_
#define __AL_IPC_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************* Exported Includes ************************************/
#include "al_errno.h"
#include "al_ipc_ll.h"
#include "al_core.h"

#define AL_IPC_ERR_NULL_PTR                 (AL_DEF_ERR(AL_IPC, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_IPC_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_IPC, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_IPC_ERR_NOT_READY                (AL_DEF_ERR(AL_IPC, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY))
#define AL_IPC_ERR_TIMEOUT                  (AL_DEF_ERR(AL_IPC, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT))
#define AL_IPC_ERR_BUSY                     (AL_DEF_ERR(AL_IPC, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY))


/******************************* Exported Typedef ************************************/
/**
 * @brief  Callback Pointer
 */
typedef void (Callback)(void *);

/**
 * @brief  IpcMailBox Hal Struct
 */
typedef struct
{
    AL_U32                       Number;
} AL_IpcMailBox_HalStruct;

/**
 * @brief  IpcIntr Hal Struct
 */
typedef struct
{
    AL_IPC_IntrEnum              Type;
    Callback                     *Func;
    AL_VOID                      *Arg;
} AL_IpcIntr_HalStruct;

/**
 * @brief  IpcSpinLock Hal Struct
 */
typedef struct
{
    AL_U32                       Number;
} AL_IpcSpinLock_HalStruct;

AL_S32 AlIpc_Hal_SpinLockInit(AL_IpcSpinLock_HalStruct **Handle, AL_U32 Number);
AL_S32 AlIpc_Hal_SpinLockTake(AL_IpcSpinLock_HalStruct *Handle, AL_U32 Timeout);
AL_S32 AlIpc_Hal_SpinLockRelease(AL_IpcSpinLock_HalStruct *Handle);
AL_S32 AlIPC_Hal_MboxInit(AL_IpcMailBox_HalStruct **Handle, AL_U32 Number);
AL_S32 AlIpc_Hal_MboxRead(AL_IpcMailBox_HalStruct *Handle,  AL_U32 *Data);
AL_S32 AlIpc_Hal_MboxWrite(AL_IpcMailBox_HalStruct *Handle,  AL_U32 Data);
AL_S32 AlIpc_Hal_IntrInit(AL_IpcIntr_HalStruct **Handle, AL_IPC_IntrEnum IntrType, Callback Func, AL_VOID *Arg);
AL_S32 AlIpc_Hal_IntrNotify(AL_IPC_IntrEnum IntrType);

#ifdef __cplusplus
}
#endif

#endif
