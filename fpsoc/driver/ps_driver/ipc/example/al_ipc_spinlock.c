/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_ipc_spinlock.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   ipc spinlock example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_ipc_hal.h"
#include "al_core.h"

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

        if (Same_Key) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Attempt to obtain the same spin lock, acquisition failed \r\n");
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Successfully obtained the same spin lock, testing failed \r\n");
            Ret = AL_ERROR;
            break;
        }

        AlIpc_Hal_SpinLockRelease(i);

        AL_LOG(AL_LOG_LEVEL_INFO, "release the spin lock\r\n");
    }

    if (Ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc spinlock test success\r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc spinlock test failed\r\n");
    }

    return Ret;
}
