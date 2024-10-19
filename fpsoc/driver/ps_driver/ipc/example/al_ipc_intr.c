/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_ipc_intr.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   ipc interrupt example
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
 * This function is called when an interrupt from the RPU to the APU (Application Processing Unit) is received.
 * It notifies the APU about the interrupt and logs a success message if the notification is successful.
 *
 * @param arg Pointer to user-defined data or NULL if no data is passed. This parameter is not used in the function.
 * @return None.
 */
AL_VOID CallBack2Rpu(AL_VOID * arg)
{
    AL_S32 Ret;
    Ret = AlIpc_Hal_IntrNotify(AL_IPC_RPU2APU_REQ);
    if (Ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc notify sucess\r\n");
    }
}

/**
 * This function is called when an interrupt from the APU to the RPU (Remote Processing Unit) is received.
 * It logs a message indicating the success of the interrupt test.
 *
 * @param arg Pointer to user-defined data or NULL if no data is passed. This parameter is not used in the function.
 * @return None.
 */
AL_VOID CallBack2Apu(AL_VOID * arg)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Inter test success\r\n");
}

/**
 * This function initializes IPC interrupts for communication between the APU and RPU. It sets up callback functions for handling interrupts
 * from the other processor and enables local interrupts. It also sends a notification to the other processor and checks for errors
 * during the initialization and notification process.
 *
 * @return AL_OK if the test runs successfully and all initializations and notifications are successful.
 *         Returns an error code if any initialization or notification fails.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_IpcIntr_HalStruct *IpcInter2Rpu = AL_NULL;
    AL_IpcIntr_HalStruct *IpcInter2Apu = AL_NULL;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Inter test\r\n");

#ifdef RPU
    Ret = AlIpc_Hal_IntrInit(&IpcInter2Rpu, AL_IPC_APU2RPU_REQ, CallBack2Apu, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc Intr init error\r\n");
    }

    Ret = AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc Intr enable error\r\n");
    }
#endif

#ifdef APU
    Ret = AlIpc_Hal_IntrInit(&IpcInter2Apu, AL_IPC_RPU2APU_REQ, CallBack2Rpu, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc Inter init error\r\n");
    }

    Ret = AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc Intr enable error\r\n");
    }

    Ret = AlIpc_Hal_IntrNotify(AL_IPC_APU2RPU_REQ);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc Intr Notify error\r\n");
    }
#endif

    return Ret;
}
