/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_ipc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_IpcMailBox_HalStruct AlIpcMailBoxHandle[IPC_MAILBOX_NUM];
static AL_IpcSpinLock_HalStruct AlIpcSpinLockHandle[IPC_SPINLOCK_NUM];
static AL_IpcIntr_HalStruct AlIpcIntrHandle[AL_IPC_INTR_NUM_INSTANCE];

extern AL_IPCINTR_HwConfigStruct AlIpcIntr_HwConfig[AL_IPC_INTR_NUM_INSTANCE];

/************************** Function Prototypes ******************************/
static AL_VOID AlIpc_Hal_IntrClear(AL_IPC_IntrEnum IntrType);
/********************************************************/

static AL_IPCINTR_HwConfigStruct *AlIpcIntr_Dev_LookupConfig(AL_IPC_IntrEnum IntrType)
{
    AL_U32 Index = 0;
    AL_IPCINTR_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < 4; Index++) {
        if (AlIpcIntr_HwConfig[Index].Type == IntrType) {
            ConfigPtr = &AlIpcIntr_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}


AL_S32 AlIpc_Hal_SpinLockInit(AL_IpcSpinLock_HalStruct **Handle, AL_U32 Number)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);
    AL_ASSERT((Number < IPC_SPINLOCK_NUM), AL_IPC_ERR_ILLEGAL_PARAM);

    *Handle = &AlIpcSpinLockHandle[Number];
    (*Handle)->Number = Number;

    AlIpc_ll_SpinLockInitThis(Number);

    return AL_OK;
}

/*
 *
*/
AL_S32 AlIpc_Hal_SpinLockTake(AL_IpcSpinLock_HalStruct *Handle, AL_U32 Timeout)
{
    AL_U32 Take = 0;
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);

    AL_LOG(AL_LOG_LEVEL_INFO, "begin waiting AlIpc_Hal_SpinLockTake 0x%x \r\n", Timeout);

    while (AL_WAIT_COND_UNTIL_TIMEOUT((Take == 1), Timeout)) {
        Take = AlIpc_ll_SpinLockTryTake(Handle->Number);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "end waiting AlIpc_Hal_SpinLockTake 0x%x \r\n", Take);

    return (Take == 1) ? AL_OK : AL_IPC_ERR_TIMEOUT;
}

AL_S32 AlIpc_Hal_SpinLockRelease(AL_IpcSpinLock_HalStruct *Handle)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);
    AlIpc_ll_SpinLockRelease(Handle->Number);
    return AL_OK;
}

AL_S32 AlIPC_Hal_MboxInit(AL_IpcMailBox_HalStruct **Handle, AL_U32 Number)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);
    AL_ASSERT((Number < IPC_MAILBOX_NUM), AL_IPC_ERR_ILLEGAL_PARAM);

    AlIpc_ll_MboxWrite(Number, 0x0UL);

    *Handle = &AlIpcMailBoxHandle[Number];
    (*Handle)->Number = Number;

    return AL_OK;
}

AL_S32 AlIpc_Hal_MboxRead(AL_IpcMailBox_HalStruct *Handle, AL_U32 *Data)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);

    *Data = AlIpc_ll_MboxRead(Handle->Number);

    return AL_OK;
}

AL_S32 AlIpc_Hal_MboxWrite(AL_IpcMailBox_HalStruct *Handle, AL_U32 Data)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);

    AlIpc_ll_MboxWrite(Handle->Number, Data);

    return AL_OK;
}

static AL_VOID AlIpc_IntrHandler(AL_VOID *Handle)
{
    AL_IpcIntr_HalStruct *Intr = (AL_IpcIntr_HalStruct *)Handle;
    Intr->Func(Intr->Arg);

    AlIpc_Hal_IntrClear(Intr->Type);
}

AL_S32 AlIpc_Hal_IntrInit(AL_IpcIntr_HalStruct **Handle, AL_IPC_IntrEnum IntrType, Callback Func, AL_VOID *Arg)
{
    AL_IPCINTR_HwConfigStruct *HwConfig = AL_NULL;
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);

    HwConfig = AlIpcIntr_Dev_LookupConfig(IntrType);
    if (HwConfig != AL_NULL) {
        *Handle = &AlIpcIntrHandle[IntrType];
        (*Handle)->Type = HwConfig->Type;
        (*Handle)->Func = Func;
        (*Handle)->Arg = Arg;
    } else {
        return AL_IPC_ERR_ILLEGAL_PARAM;
    }

    return AlIntr_RegHandler(HwConfig->IntrNum, AL_NULL, AlIpc_IntrHandler, Handle);
}

AL_S32 AlIpc_Hal_IntrNotify(AL_IPC_IntrEnum IntrType)
{
    AL_ASSERT((IntrType <= AL_IPC_RPU2APU_ACK), AL_IPC_ERR_ILLEGAL_PARAM);

    AlIpc_ll_SetInter(IntrType, AL_FUNC_ENABLE);
    return AL_OK;
}

static AL_VOID AlIpc_Hal_IntrClear(AL_IPC_IntrEnum IntrType)
{
    AlIpc_ll_SetInter(IntrType, AL_FUNC_DISABLE);
}
