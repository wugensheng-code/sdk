/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_efuse_dev.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2024-07-25
 * @brief   efuse driver
 */

#ifndef __AL_EFUSE_DEV_H_
#define __AL_EFUSE_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_efuse_ll.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
AL_VOID AlEfuse_Dev_SetBits(AL_U32 Offset, AL_U32 Num, AL_U32 *Val);

#ifdef __cplusplus
}
#endif

#endif
