/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_efuse_simu.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2024-07-25
 * @brief   efuse simulation example
 * @example al_efuse_simu.c
 */

/***************************** Include Files *********************************/
#include "al_efuse_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_EFUSE_SIMU

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
static AL_VOID AlEfuse_Test_Simulation(AL_VOID);

/************************** Function Definitions ******************************/

/**
 *
 * This function initializes the test, logs the start and end of the test,
 * and calls the AlEfuse_Test_Simulation function to perform the actual test.
 *
 * @return AL_S32 Returns AL_OK on success.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Efuse simulation test\r\n");

    AlEfuse_Test_Simulation();

    AL_LOG(AL_LOG_LEVEL_INFO, "Efuse simulation test done\r\n");

    return Ret;
}

/**
 *
 * This function sets specific bits in the efuse simulation at predefined offsets.
 * It uses the AlEfuse_Hal_SetBits function to set the bits and logs the values
 * being set.
 *
 * @return AL_VOID This function does not return a value.
 */
AL_VOID AlEfuse_Test_Simulation(AL_VOID)
{
    AL_U32 Offset = 0;
    AL_U32 Num = 64;
    AL_U32 Val[2] = {0xA5005A00, 0x01234567};

    AL_LOG(AL_LOG_LEVEL_INFO, "---Test val---\r\n");
    AL_LOG(AL_LOG_LEVEL_INFO, "Val: 0x%08x, 0x%08x\r\n", Val[0], Val[1]);
    AL_LOG(AL_LOG_LEVEL_INFO, "---------------\r\n");

    for (AL_U32 i = 1; i <= Num; i++) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Offset: %d\r\n", Offset);
        AL_LOG(AL_LOG_LEVEL_INFO, "Num: %d\r\n", i);
        AlEfuse_Hal_SetBits(0, i, Val);
    }

    AlEfuse_Hal_SetBits(0, Num, AL_NULL);
}
