/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_ipc_mailbox.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   ipc mailbox example
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
    AL_S32 Ret = AL_OK;
    AL_U32 Value = 0xffee;
    AL_U32 OutValue;

    AL_IpcMailBox_HalStruct *Mailbox_Handle;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox test\r\n");

    for (int i = 0; i < 3; i++)
    {
        Ret = AlIPC_Hal_MboxInit(&Mailbox_Handle, i);
        if ( Ret != AL_OK) {
            break;
        }

        Ret = AlIpc_Hal_MboxWrite(Mailbox_Handle, Value);
        if ( Ret != AL_OK) {
            break;
        }

        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox write 0x%x\r\n", Value);

        Ret = AlIpc_Hal_MboxRead(Mailbox_Handle, &OutValue);
        if ( Ret != AL_OK) {
            break;
        }

        if (Value != OutValue) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc Mailbox fail write(0x%x) read(0x%x)\r\n", Value, OutValue);
            Ret = AL_ERROR;
            break;
        }

        Value++;
    }

    if (Ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox test success\r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc Mailbox test fail\r\n");
    }

    return 0;
}
