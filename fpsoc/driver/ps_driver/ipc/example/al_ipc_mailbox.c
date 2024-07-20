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
/**
 * This function performs a series of operations to test the IPC mailbox functionality. It initializes a mailbox,
 * writes a value to it, reads the value back, and checks if the written and read values match. This process is repeated
 * for three different mailbox instances. It logs information about the operations being performed and their outcomes.
 *
 * @return 0 Always returns 0 indicating the program executed.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U32 Value = 0xffee;
    AL_U32 OutValue0, OutValue1, OutValue2;

    AL_IpcMailBox_HalStruct *Mailbox_Handle0, *Mailbox_Handle1, *Mailbox_Handle2;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox test\r\n");

    for (int i = 0; i < 3; i++) {
        Ret = AlIPC_Hal_MboxInit(&Mailbox_Handle0, 0);
        if ( Ret != AL_OK) {
            break;
        }
        Ret = AlIPC_Hal_MboxInit(&Mailbox_Handle1, 1);
        if ( Ret != AL_OK) {
            break;
        }
        Ret = AlIPC_Hal_MboxInit(&Mailbox_Handle2, 2);
        if ( Ret != AL_OK) {
            break;
        }

        Ret = AlIpc_Hal_MboxWrite(Mailbox_Handle0, Value);
        if ( Ret != AL_OK) {
            break;
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox write 0 0x%x\r\n", Value);

        Ret = AlIpc_Hal_MboxWrite(Mailbox_Handle1, Value + 1);
        if ( Ret != AL_OK) {
            break;
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox write 1 0x%x\r\n", Value + 1);

        Ret = AlIpc_Hal_MboxWrite(Mailbox_Handle2, Value + 2);
        if ( Ret != AL_OK) {
            break;
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox write 2 0x%x\r\n", Value + 2);


        Ret = AlIpc_Hal_MboxRead(Mailbox_Handle0, &OutValue0);
        if ( Ret != AL_OK) {
            break;
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox Read 0 0x%x\r\n", OutValue0);


        Ret = AlIpc_Hal_MboxRead(Mailbox_Handle1, &OutValue1);
        if ( Ret != AL_OK) {
            break;
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox Read 0 0x%x\r\n", OutValue1);

        Ret = AlIpc_Hal_MboxRead(Mailbox_Handle2, &OutValue2);
        if ( Ret != AL_OK) {
            break;
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox Read 0 0x%x\r\n", OutValue2);

        if (OutValue0 != Value || OutValue1 != Value + 1 || OutValue2 != Value + 2) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "value: 0x%x 0x%x 0x%x OutValue: 0x%x 0x%x 0x%x\r\n");
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
