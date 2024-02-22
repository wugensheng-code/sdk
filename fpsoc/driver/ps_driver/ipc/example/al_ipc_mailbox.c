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

AL_VOID main(AL_VOID)
{
    AL_U32 Value = 0xffee;
    AL_IpcMailBox_HalStruct *Mailbox_Handle;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox test\r\n");

    for (int i = 0; i < 3; i++) 
    {
        AlIPC_Hal_MboxInit(&Mailbox_Handle, i);

        AlIpc_Hal_MboxWrite(Mailbox_Handle, Value);

        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox write 0x%x\r\n", Value);

        AlIpc_Hal_MboxRead(Mailbox_Handle, &Value);

        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox read 0x%x\r\n", Value);

        Value++;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox test success\r\n");
    return;
}
