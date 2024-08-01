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
 *
 * This function sets bits in the efuse device starting from a given offset.
 * It delegates the actual setting of bits to the AlEfuse_Dev_SetBits function.
 *
 * @param Offset The starting offset where bits will be set.
 * @param Num The number of bits to set.
 * @param Val A pointer to an array of values to set the bits. If NULL, all bits are set to 1.
 * @return AL_VOID This function does not return a value.
 *
 */
AL_VOID AlEfuse_Hal_SetBits(AL_U32 Offset, AL_U32 Num, AL_U32 *Val)
{
    AlEfuse_Dev_SetBits(Offset, Num, Val);
}
