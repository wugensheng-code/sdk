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
 * @brief   can fd stb priority blocked example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include "al_can_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_CAN_EX_DEVICE_ID             (0)
#define AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS (1000)
#define AL_CAN_EX_MEM_SIZE              (1)
#define AL_CAN_EX_ARRAY_SIZE            (256)
#define AL_CAN_EX_DELAY_COUNT           (200)
#define AL_CAN_EX_DELAY_MS              (5)

/************************** Variable Definitions *****************************/
static AL_CAN_InitStruct FdStbPriorityConfig = {
    .OpsMode        = AL_CAN_MODE_NORMAL,
    .RunMode        = AL_CAN_RUN_INTR,
    .Type           = AL_CAN_TYPE_FD,
    .SlowBitRate    = AL_CAN_0_5M,
    .FastBitRate    = AL_CAN_1_M,
    .TransMode      = AL_CAN_TRANS_STB_PRIO,
    .RbAfwl         = AL_CAN_RB_LIMIT_8
};

static AL_CAN_FrameStruct ExFrame = {
    .Id             = 0xb,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_32,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, \
                        0x090a0b0c, 0x0d0e0f00},
};
/************************** Function Prototypes ******************************/
static AL_S32 AlCan_Test_FdStbPriorityBlocked(AL_VOID);

/************************** Function Definitions ******************************/
/**
 * The main entry point for the CAN FD STB priority blocked test.
 *
 * This function starts by logging the beginning of the CAN FD STB priority blocked test.
 * It then calls the AlCan_Test_FdStbPriorityBlocked function to perform the test.
 * Depending on the return value of the test function, it logs either a success or failure message.
 * The function returns the result of the test.
 *
 * @return AL_OK if the test is successful, an error code otherwise.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Can fd stb priority blocked test\r\n");

    Ret = AlCan_Test_FdStbPriorityBlocked();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Can fd stb priority blocked test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Can fd stb priority blocked test success\r\n");

    return Ret;
}

/**
 *
 * This function initializes the CAN hardware with a specific configuration for STB priority mode and
 * enters a loop where it periodically sends CAN frames using a blocking send operation.
 * The function also attempts to receive CAN frames in each iteration of the loop. If sending a frame fails,
 * an error is logged. If receiving a frame fails with an error other than AL_ERR_UNAVAILABLE, an error is logged.
 * Otherwise, received frames are displayed. The loop runs indefinitely, simulating continuous operation.
 *
 * @return AL_OK if the loop runs without critical errors,
 *         AL_ERR_UNAVAILABLE if a frame is not available for reception,
 *         or other error codes for different errors encountered during the test.
 */
static AL_S32 AlCan_Test_FdStbPriorityBlocked(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 DelayCount = 0;
    AL_CAN_FrameStruct Frame;
    AL_CAN_HalStruct *Handle = AL_NULL;

    Ret = AlCan_Hal_Init(&Handle, AL_CAN_EX_DEVICE_ID, &FdStbPriorityConfig, AL_NULL);
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
