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
 * @brief   can in loopback blocked example
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

/************************** Variable Definitions *****************************/
static AL_CAN_InitStruct InLoopbackConfig = {
    .OpsMode        = AL_CAN_MODE_IN_LOOPBACK,
    .RunMode        = AL_CAN_RUN_INTR,
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
static AL_S32 AlCan_Test_InLoopbackBlocked(AL_VOID);

/************************** Function Definitions ******************************/
/**
 *
 * This function initializes the test environment and starts the CAN in loopback blocked test by calling AlCan_Test_InLoopbackBlocked.
 * It logs the start of the test, checks the return value of the test function to determine success or failure, and logs the appropriate message before exiting.
 *
 * @return AL_OK if the test is successful, an error code otherwise.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Can in loopback blocked test\r\n");

    Ret = AlCan_Test_InLoopbackBlocked();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Can in loopback blocked test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Can in loopback blocked test success\r\n");

    return Ret;
}

/**
 *
 * This function initializes the CAN hardware with a specific configuration for loopback mode and enters a loop
 * where it sends and receives CAN frames in a blocked manner. The function sends a predefined CAN frame
 * and waits for its reception using a blocking call with a specified timeout. If the frame is successfully sent and received,
 * it compares the sent and received data to ensure they match. The loop continues indefinitely, simulating
 * a real-world scenario where a device continuously sends and receives CAN frames in loopback mode.
 *
 * The function logs an error and returns immediately if any of the following operations fail:
 * - Hardware initialization
 * - Sending a frame
 * - Receiving a frame
 * - Data comparison between sent and received frames
 *
 * @return AL_OK if the loop runs without errors, an error code otherwise.
 */
static AL_S32 AlCan_Test_InLoopbackBlocked(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_CAN_FrameStruct Frame;
    AL_CAN_HalStruct *Handle = AL_NULL;

    Ret = AlCan_Hal_Init(&Handle, AL_CAN_EX_DEVICE_ID, &InLoopbackConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        Ret = AlCan_Hal_SendFrameBlock(Handle, &ExFrame, AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame Error:0x%x\r\n", Ret);
        }

        Ret = AlCan_Hal_RecvFrameBlock(Handle, &Frame, AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame Error:0x%x\r\n", Ret);
        }

        AlCan_Dev_DisplayFrame(&Frame);

        Ret = memcmp(ExFrame.Data, Frame.Data, AL_CAN_LEN_8);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
            return Ret;
        }

        AlSys_MDelay(500);
    }

    return Ret;
}
