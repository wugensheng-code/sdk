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

/********************************************************/
/**
 * This function look up hardware config structure.
 * @param   DeviceId is hardware module id
 * @return
 *          - AL_UART_HwConfigStruct for hardware config
 * @note
*/
AL_IPCINTR_HwConfigStruct *AlIpcIntr_Dev_LookupConfig(AL_IPC_IntrEnum IntrType)
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

/**
 * This function initialize the UART mode according to the specified
 *          parameters in the AL_WDT_InitStruct and initialize the associated handle.
 * @param   Handle Pointer to a AL_WDT_HalStruct structure that contains wdt Hal instance
 * @param   HalId is hardware module id
 * @param   InitConfig pointer to a AL_WDT_InitStruct structure
 *          that contains the configuration information for the specified UART peripheral
 * @return
 *          - AL_OK for function success
 *          - Other for function failuregit
 * @note
*/
AL_VOID AlIpc_Hal_SpinLockInit(AL_IpcSpinLock_HalStruct **Handle, AL_U32 Number)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);
    AL_ASSERT((Number < IPC_SPINLOCK_NUM), AL_IPC_ERR_ILLEGAL_PARAM);

    *Handle = &AlIpcSpinLockHandle[Number];
    (*Handle)->Number = Number;
    return AlIpc_ll_SpinLockInitThis(Number);
}

AL_U32 AlIpc_Hal_SpinLockTake(AL_IpcSpinLock_HalStruct *Handle, AL_U32 Timeout)
{
    AL_U32 Key = 0;
    while (AL_WAIT_COND_UNTIL_TIMEOUT((Key == 1), Timeout)) {
        Key = AlIpc_ll_SpinLockTryTake(Handle->Number);
    }

    return Key;
}

AL_VOID AlIpc_Hal_SpinLockRelease(AL_IpcSpinLock_HalStruct *Handle)
{
    return AlIpc_ll_SpinLockRelease(Handle->Number);
}

AL_VOID AlIPC_Hal_MboxInit(AL_IpcMailBox_HalStruct **Handle, AL_U32 Number)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);
    AL_ASSERT((Number < IPC_MAILBOX_NUM), AL_IPC_ERR_ILLEGAL_PARAM);

    AlIpc_ll_MboxWrite(Number, 0x0UL);

    *Handle = &AlIpcMailBoxHandle[Number];
    (*Handle)->Number = Number;

    return;
}

AL_VOID AlIpc_Hal_MboxRead(AL_IpcMailBox_HalStruct *Handle, AL_U32 *Data)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);

    *Data = AlIpc_ll_MboxRead(Handle->Number);

    return;
}

AL_VOID AlIpc_Hal_MboxWrite(AL_IpcMailBox_HalStruct *Handle,  AL_U32 Data)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);

    AlIpc_ll_MboxWrite(Handle->Number, Data);

    return;
}

AL_VOID AlIpc_IntrHandler(AL_VOID *Handle)
{   
    AL_IpcIntr_HalStruct *Intr = *(AL_IpcIntr_HalStruct **)Handle;
    Intr->Func(Intr->Arg);
    AlIpc_Hal_IntrClear(Intr->Type);
}

AL_VOID AlIpc_Hal_IntrInit(AL_IpcIntr_HalStruct **Handle, AL_IPC_IntrEnum IntrType, Callback Func, AL_VOID *Arg)
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

    (AL_VOID)AlIntr_RegHandler(HwConfig->IntrNum, AL_NULL, AlIpc_IntrHandler, Handle);

    return;
}

AL_VOID AlIpc_Hal_IntrNotify(AL_IPC_IntrEnum IntrType)
{
    AlIpc_ll_SetInter(IntrType, AL_FUNC_ENABLE);
}

AL_VOID AlIpc_Hal_IntrClear(AL_IPC_IntrEnum IntrType)
{
    AlIpc_ll_SetInter(IntrType, AL_FUNC_DISABLE);
}
