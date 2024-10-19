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
/**
 * Main function to test IPC spin lock functionality.
 * This function performs a series of operations to test the IPC spin lock functionality. It initializes a spin lock,
 * attempts to take the spin lock, checks if the spin lock can be taken again (which should fail to ensure the lock is working correctly),
 * and then releases the spin lock. This process is repeated for four different spin lock instances. It logs information
 * about the operations being performed and their outcomes. The function aims to verify the correct behavior of spin locks in preventing concurrent access.
 *
 * @return AL_OK if all tests pass successfully, indicating that the spin locks are functioning as expected. Returns AL_ERROR if any test fails, indicating a problem with the spin lock implementation.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_IpcSpinLock_HalStruct *Spinlock_Handle;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc spin lock test\r\n");

    for (int i = 0; i < 4; i++)
    {
        AlIpc_Hal_SpinLockInit(&Spinlock_Handle, i);

        AL_LOG(AL_LOG_LEVEL_INFO, "try take the spin lock\r\n");

        Ret = AlIpc_Hal_SpinLockTake(Spinlock_Handle, AL_WAITING_NO);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Successfully obtained the spin lock\r\n");
        }

        Ret = AlIpc_Hal_SpinLockTake(Spinlock_Handle, AL_WAITING_NO);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Attempt to obtain the same spin lock, acquisition failed \r\n");
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Successfully obtained the same spin lock, testing failed \r\n");
            Ret = AL_ERROR;
            break;
        }

        AlIpc_Hal_SpinLockRelease(Spinlock_Handle);

        AL_LOG(AL_LOG_LEVEL_INFO, "release the spin lock\r\n");
    }

    if (Ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc spinlock test success\r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc spinlock test failed\r\n");
    }

    return Ret;
}
