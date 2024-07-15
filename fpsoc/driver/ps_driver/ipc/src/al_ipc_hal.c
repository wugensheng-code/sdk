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
static AL_S32 AlIpc_Hal_IntrClear(AL_IPC_IntrEnum IntrType);
/********************************************************/
/**
 * This function searches through the available IPC interrupt hardware configurations to find the one that
 * matches the specified interrupt type.
 *
 * @param IntrType The interrupt type for which the hardware configuration is being looked up.
 * @return Pointer to the AL_IPCINTR_HwConfigStruct structure if found, otherwise NULL.
 */
static AL_IPCINTR_HwConfigStruct *AlIpcIntr_Dev_LookupConfig(AL_IPC_IntrEnum IntrType)
{
    AL_U32 Index = 0;
    AL_IPCINTR_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_IPC_INTR_NUM_INSTANCE; Index++) {
        if (AlIpcIntr_HwConfig[Index].Type == IntrType) {
            ConfigPtr = &AlIpcIntr_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

/**
 * This function initializes a spin lock for inter-process communication by setting up the handle and lock number.
 *
 * @param Handle Double pointer to the AL_IpcSpinLock_HalStruct structure where the initialized spin lock will be stored.
 * @param Number The number of the spin lock to initialize.
 * @return AL_OK on success, AL_IPC_ERR_ILLEGAL_PARAM if the parameters are invalid.
 */
AL_S32 AlIpc_Hal_SpinLockInit(AL_IpcSpinLock_HalStruct **Handle, AL_U32 Number)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);
    AL_ASSERT((Number < IPC_SPINLOCK_NUM), AL_IPC_ERR_ILLEGAL_PARAM);

    *Handle = &AlIpcSpinLockHandle[Number];
    (*Handle)->Number = Number;

    AlIpc_ll_SpinLockInitThis(Number);

    return AL_OK;
}

/**
 * This function tries to acquire a spin lock for inter-process communication, with a specified timeout for the operation.
 *
 * @param Handle Pointer to the AL_IpcSpinLock_HalStruct structure that represents the spin lock to be acquired.
 * @param Timeout The maximum time to wait for the lock to become available.
 * @return AL_OK if the lock was successfully acquired, AL_IPC_ERR_TIMEOUT if the operation timed out.
 */
AL_S32 AlIpc_Hal_SpinLockTake(AL_IpcSpinLock_HalStruct *Handle, AL_U32 Timeout)
{
    AL_U32 Take = 0;
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);

    AL_LOG(AL_LOG_LEVEL_DEBUG, "begin waiting AlIpc_Hal_SpinLockTake 0x%x \r\n", Timeout);

    while (AL_WAIT_COND_UNTIL_TIMEOUT((Take == 1), Timeout)) {
        Take = AlIpc_ll_SpinLockTryTake(Handle->Number);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "end waiting AlIpc_Hal_SpinLockTake 0x%x \r\n", Take);

    return (Take == 1) ? AL_OK : AL_IPC_ERR_TIMEOUT;
}

/**
 * This function releases a previously acquired spin lock for inter-process communication.
 *
 * @param Handle Pointer to the AL_IpcSpinLock_HalStruct structure that represents the spin lock to be released.
 * @return AL_OK if succeed.
 */
AL_S32 AlIpc_Hal_SpinLockRelease(AL_IpcSpinLock_HalStruct *Handle)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);
    AlIpc_ll_SpinLockRelease(Handle->Number);
    return AL_OK;
}

/**
 * This function initializes a mailbox for inter-process communication by setting up the handle and mailbox number.
 *
 * @param Handle Double pointer to the AL_IpcMailBox_HalStruct structure where the initialized mailbox will be stored.
 * @param Number The number of the mailbox to initialize.
 * @return AL_OK on success, AL_IPC_ERR_ILLEGAL_PARAM if the parameters are invalid.
 */
AL_S32 AlIPC_Hal_MboxInit(AL_IpcMailBox_HalStruct **Handle, AL_U32 Number)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);
    AL_ASSERT((Number < IPC_MAILBOX_NUM), AL_IPC_ERR_ILLEGAL_PARAM);

    AlIpc_ll_MboxWrite(Number, 0x0UL);

    *Handle = &AlIpcMailBoxHandle[Number];
    (*Handle)->Number = Number;

    return AL_OK;
}

/**
 * This function reads a 32-bit data value from a specified mailbox for inter-process communication.
 *
 * @param Handle Pointer to the AL_IpcMailBox_HalStruct structure that represents the mailbox to read from.
 * @param Data Pointer to a 32-bit variable where the read data will be stored.
 * @return AL_OK on success, AL_IPC_ERR_ILLEGAL_PARAM if the parameters are invalid.
 */
AL_S32 AlIpc_Hal_MboxRead(AL_IpcMailBox_HalStruct *Handle, AL_U32 *Data)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);

    *Data = AlIpc_ll_MboxRead(Handle->Number);

    return AL_OK;
}

/**
 * This function writes a 32-bit data value to a specified mailbox for inter-process communication.
 *
 * @param Handle Pointer to the AL_IpcMailBox_HalStruct structure that represents the mailbox to write to.
 * @param Data The 32-bit data value to write.
 * @return AL_OK on success, AL_IPC_ERR_ILLEGAL_PARAM if the parameters are invalid.
 */
AL_S32 AlIpc_Hal_MboxWrite(AL_IpcMailBox_HalStruct *Handle, AL_U32 Data)
{
    AL_ASSERT((Handle != AL_NULL), AL_IPC_ERR_ILLEGAL_PARAM);

    AlIpc_ll_MboxWrite(Handle->Number, Data);

    return AL_OK;
}

/**
 * This function is the interrupt handler for IPC interrupts. It calls the registered callback function for the interrupt.
 *
 * @param Handle Pointer to the user-defined data structure that contains information about the IPC interrupt.
 * @return None.
 */
static AL_VOID AlIpc_IntrHandler(AL_VOID *Handle)
{
    AL_IpcIntr_HalStruct *Intr = (AL_IpcIntr_HalStruct *)Handle;
    AL_S32 Ret;

    Intr->Func(Intr->Arg);

    Ret = AlIpc_Hal_IntrClear(Intr->Type);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc cleaning interrupt failed \r\n");
    }
}

/**
 * This function initializes an interrupt for inter-process communication by setting up the handle, type, callback function, and argument.
 *
 * @param Handle Double pointer to the AL_IpcIntr_HalStruct structure where the initialized interrupt will be stored.
 * @param IntrType The type of the interrupt to initialize.
 * @param Func Pointer to the callback function to be called when the interrupt occurs.
 * @param Arg Pointer to the argument to be passed to the callback function.
 * @return AL_OK on success, AL_IPC_ERR_ILLEGAL_PARAM if the parameters are invalid or the hardware configuration is not found.
 */
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

    return AlIntr_RegHandler(HwConfig->IntrNum, AL_NULL, AlIpc_IntrHandler, (*Handle));
}

/**
 * This function triggers an interrupt for inter-process communication.
 *
 * @param IntrType The type of the interrupt to notify.
 * @return AL_OK on success, AL_IPC_ERR_ILLEGAL_PARAM if the interrupt type is invalid.
 */
AL_S32 AlIpc_Hal_IntrNotify(AL_IPC_IntrEnum IntrType)
{
    AL_ASSERT((IntrType <= AL_IPC_RPU2APU_ACK), AL_IPC_ERR_ILLEGAL_PARAM);

    AlIpc_ll_SetInter(IntrType, AL_FUNC_ENABLE);
    return AL_OK;
}

/**
 * This function clears a specified interrupt for inter-process communication.
 *
 * @param IntrType The type of the interrupt to clear.
 * @return AL_OK on success, AL_IPC_ERR_ILLEGAL_PARAM if the interrupt type is invalid.
 */
static AL_S32 AlIpc_Hal_IntrClear(AL_IPC_IntrEnum IntrType)
{
    AL_ASSERT((IntrType <= AL_IPC_RPU2APU_ACK), AL_IPC_ERR_ILLEGAL_PARAM);
    AlIpc_ll_SetInter(IntrType, AL_FUNC_DISABLE);
    return AL_OK;
}
