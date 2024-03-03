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
AL_VOID CallBack(AL_VOID * arg)
{
    AL_LOG(AL_LOG_LEVEL_NOTICE, "IPC notification\r\n");
}

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_IpcIntr_HalStruct *Ipc_Inter;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Inter test\r\n");

#ifdef RPU

    AlIpc_Hal_IntrInit(&Ipc_Inter, AL_IPC_APU2RPU_REQ, CallBack, AL_NULL);
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

#endif

#ifdef APU

    AlIpc_Hal_IntrNotify(AL_IPC_APU2RPU_REQ);

#endif

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Inter test success\r\n");

    return Ret;
}
