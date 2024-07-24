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
    .TransMode      = AL_CAN_TRANS_PTB,
    .RbAfwl         = AL_CAN_RB_LIMIT_8,
    .IsTimeStampEn  = AL_TRUE,
    .TimePos        = AL_CAN_TIMEPOS_EOF,
    .Acf[0].FilterEn        = AL_TRUE,
    .Acf[0].FilterIndex     = AL_CAN_FILTER_0,
    .Acf[0].MaskType        = AL_CAN_FILTER_MASK_BOTH,
    .Acf[0].MaskValue       = 0x7FE,
    .Acf[0].IdValue         = 0x1,
};

static AL_CAN_FrameStruct ExFrame = {
    .Id             = 0x10,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x1,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_8,
    .Data           = {0x01020304, 0x05060708}
};

/************************** Function Prototypes ******************************/
static AL_S32 AlCan_Test_StdPtbBlocked(AL_VOID);

/************************** Function Definitions ******************************/
/**
 *
 * This function initializes the test environment and starts the CAN standard PTB (Pass-Through Block) blocked test
 * by calling AlCan_Test_StdPtbBlocked. It logs the start of the test, checks the return value of the test function
 * to determine success or failure, and logs the appropriate message before exiting.
 *
 * @return AL_OK if the test is successful, an error code otherwise.
 */
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

/**
 *
 * This function initializes the CAN hardware with a specific configuration for standard PTB mode and enters
 * a loop where it sends CAN frames in a blocked manner and attempts to receive CAN frames.
 * The sending of frames is attempted every AL_CAN_EX_DELAY_COUNT iterations of the loop, with a delay of AL_CAN_EX_DELAY_MS
 * milliseconds between each iteration. If sending a frame fails, an error is logged. If receiving a frame fails
 * with an error other than AL_ERR_UNAVAILABLE, an error is logged. Otherwise, received frames are displayed.
 * The loop runs indefinitely, simulating a real-world scenario where a device continuously sends and receives CAN frames.
 *
 * @return AL_OK if the loop runs without critical errors,
 *         AL_ERR_UNAVAILABLE if a frame is not available for reception,
 *         or other error codes for different errors encountered during the test.
 */
static AL_S32 AlCan_Test_StdPtbBlocked(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 DelayCount = 0;
    AL_CAN_FrameStruct Frame;
    AL_CAN_HalStruct *Handle = AL_NULL;

    Ret = AlCan_Dev_ModifyHwConfig(AL_CAN_EX_DEVICE_ID, CAN20B, 250000, 0);

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
