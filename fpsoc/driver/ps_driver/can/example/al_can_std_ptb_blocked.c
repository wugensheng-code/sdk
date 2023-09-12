/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_dmacahb_single_mode_blocked.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   can std ptb blocked example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include "al_can_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_CAN_EX_DEVICE_ID             (0)
#define AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS (100)
#define AL_CAN_EX_MEM_SIZE              (1)
#define AL_CAN_EX_ARRAY_SIZE            (256)
#define AL_CAN_EX_DELAY_COUNT           (200)
#define AL_CAN_EX_DELAY_MS              (5)

/************************** Variable Definitions *****************************/
static AL_CAN_InitStruct StdPtbConfig = {
    .OpsMode        = AL_CAN_MODE_NORMAL,
    .RunMode        = AL_CAN_RUN_INTR,
    .Type           = AL_CAN_TYPE_2_0B,
    .SlowBitRate    = AL_CAN_ARBITRATION_0_25M,
    .TransMode      = AL_CAN_TRANS_PTB,
    .RbAfwl         = AL_CAN_RB_LIMIT_8
};

static AL_CAN_FrameStruct ExFrame = {
    .Id             = 0x10,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_8,
    .Data           = {0x01020304, 0x05060708}
};

/************************** Function Prototypes ******************************/
static AL_S32 AlCan_Test_StdPtbBlocked(AL_VOID);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Can std ptb blocked test\r\n");

    Ret = AlCan_Test_StdPtbBlocked();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Can std ptb blocked test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Can std ptb blocked test success\r\n");

    return Ret;
}

static AL_S32 AlCan_Test_StdPtbBlocked(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 DelayCount = 0;
    AL_CAN_FrameStruct Frame;
    AL_CAN_HalStruct *Handle = AL_NULL;

    Ret = AlCan_Hal_Init(&Handle, AL_CAN_EX_DEVICE_ID, &StdPtbConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        AlSys_MDelay(AL_CAN_EX_DELAY_MS);

        if (((DelayCount++) % AL_CAN_EX_DELAY_COUNT) == 0) {
            Ret = AlCan_Hal_SendFrameBlock(Handle, &ExFrame, AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS);
            if (Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame Error:0x%x\r\n", Ret);
            } else {
                AL_LOG(AL_LOG_LEVEL_INFO, "Send Frame: %d\r\n", (DelayCount / AL_CAN_EX_DELAY_COUNT));
            }
        }

        Ret = AlCan_Hal_RecvFrame(Handle, &Frame);
        if (Ret != AL_OK) {
            if (Ret == AL_ERR_UNAVAILABLE) {
                continue;
            } else {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame Error:0x%x\r\n", Ret);
            }
        } else {
            AlCan_Dev_DisplayFrame(&Frame);
        }
    }

    return Ret;
}
