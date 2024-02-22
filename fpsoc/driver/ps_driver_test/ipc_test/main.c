/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_uart_nonblocked.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   uart nonblocked example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_ipc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
#if CONFIG_AlIpc_Test_Mailbox_TEST
    AL_S32 Ret = AL_OK;
    AL_U32 Value = 0xffee;
    AL_IpcMailBox_HalStruct *Mailbox_Handle;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox test\r\n");

    for (int i = 0; i < 3; i++) 
    {
        Ret = AlIPC_Hal_MboxInit(&Mailbox_Handle, i);

        AlIpc_Hal_MboxWrite(Mailbox_Handle, Value, AL_WAITING_NO);

        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox write 0x%x\r\n", Value);

        Ret = AlIpc_Hal_MboxRead(Mailbox_Handle, &Value, AL_WAITING_NO);

        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox read 0x%x\r\n", Value);

        Value++;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox test success\r\n");
    return Ret;
#endif

#if CONFIG_AlIpc_Test_Spinlock_TEST
    AL_S32 Ret = AL_OK;
    AL_U32 Key = 0;
    AL_U32 Same_Key = 0;
    AL_IpcSpinLock_HalStruct *Spinlock_Handle;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc spin lock test\r\n");

    for (int i = 0; i < 4; i++) 
    {
        AlIpc_Hal_SpinLockInit(&Spinlock_Handle, i);

        AL_LOG(AL_LOG_LEVEL_INFO, "try take the spin lock\r\n");

        Key = AlIpc_Hal_SpinLockTake(i, AL_WAITING_NO);

        if (Key) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Successfully obtained the spin lock\r\n");
        }

        Same_Key = AlIpc_Hal_SpinLockTake(i, AL_WAITING_NO);

        if (!Same_Key) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Attempt to obtain the same spin lock, acquisition failed\r\n");
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Successfully obtained the same spin lock, testing failed\r\n")
        }

        AlIpc_Hal_SpinLockRelease(i);

        AL_LOG(AL_LOG_LEVEL_INFO, "release the spin lock\r\n");
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc spinlock test success\r\n");
    return Ret;
#endif
}
