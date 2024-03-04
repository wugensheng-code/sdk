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
AL_VOID CallBack2Rpu(AL_VOID * arg)
{
    AL_S32 Ret
    Ret = AlIpc_Hal_IntrNotify(AL_IPC_RPU2APU_REQ);
    if (Ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc notify sucess\r\n");
    }
}

AL_VOID CallBack2Apu(AL_VOID * arg)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Inter test success\r\n");
}

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_IpcIntr_HalStruct *IpcInter2Rpu;
    AL_IpcIntr_HalStruct *IpcInter2Apu;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Inter test\r\n");

#ifdef RPU

    Ret = AlIpc_Hal_IntrInit(&IpcInter2Rpu, AL_IPC_APU2RPU_REQ, CallBack2Apu, AL_NULL);
    Ret = AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    if (Ret || Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc Inter test error\r\n");
    }

#endif

#ifdef APU

    Ret = AlIpc_Hal_IntrInit(&IpcInter2Apu, AL_IPC_RPU2APU_REQ, CallBack2Rpu, AL_NULL);
    Ret = AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    Ret = AlIpc_Hal_IntrNotify(AL_IPC_APU2RPU_REQ);

    if (Ret || Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc Inter test error\r\n");
    }

#endif

    return Ret;
}
