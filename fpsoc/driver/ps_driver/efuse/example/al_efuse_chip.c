/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_efuse_chip.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2024-07-25
 * @brief   efuse chip example
 */

/***************************** Include Files *********************************/
#include "al_efuse_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_EFUSE_TEST_FIELD2_OFFSET 1792
#define AL_EFUSE_TEST_FIELD2_NUM    256

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
static AL_VOID AlEfuse_Test_Chip(AL_VOID);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Efuse chip once program test\r\n");

    AlEfuse_Test_Chip();

    AL_LOG(AL_LOG_LEVEL_INFO, "Efuse chip once program test done\r\n");

    return Ret;
}

AL_VOID AlEfuse_Test_Chip(AL_VOID)
{
    AL_U32 Offset = AL_EFUSE_TEST_FIELD2_OFFSET;
    AL_U32 Val = 0b101;

    AlEfuse_Hal_SetBits(Offset, 1, AL_NULL);

    AlEfuse_Hal_SetBits(Offset + 2, 2, AL_NULL);

    AlEfuse_Hal_SetBits(Offset + 6, 2, &Val);
}