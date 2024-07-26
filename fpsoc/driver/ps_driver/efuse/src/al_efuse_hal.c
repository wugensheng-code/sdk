/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_efuse_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 * @brief  Set the efuse bits
 * @param  Offset: The offset of the efuse bits
 * @param  Num: The number of the efuse bits
 * @param  Val: The value pointer of the efuse bits
 *              If Val is NULL, set all efuse bits to 1
 *              If Val is not NULL, set the efuse bits to 1 according to Val
 *              ex: Val[0] -> [Offset + 31, Offset + 0]
 *                  Val[1] -> [Offset + 63, Offset + 32]
 *                  ...
 * @return None
*/
AL_VOID AlEfuse_Hal_SetBits(AL_U32 Offset, AL_U32 Num, AL_U32 *Val)
{
    AlEfuse_Dev_SetBits(Offset, Num, Val);
}