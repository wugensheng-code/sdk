/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_efuse_ll.h"

/************************** Constant Definitions *****************************/
#define AL_EFUSE_LOG_MASK   0x7

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/

/**
 *
 * This function sets bits in the efuse device starting from a given offset.
 * If the Val parameter is NULL, it sets the specified number of bits to 1.
 * Otherwise, it sets the bits according to the values in the Val array.
 * The function also logs the operations performed.
 *
 * @param Offset The starting offset where bits will be set.
 * @param Num The number of bits to set.
 * @param Val A pointer to an array of values to set the bits. If NULL, all bits are set to 1.
 * @return AL_VOID This function does not return a value.
 *
 */
AL_VOID AlEfuse_Dev_SetBits(AL_U32 Offset, AL_U32 Num, AL_U32 *Val)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "---Set efuse bit---\r\n");

    AlEfuse_ll_ClkCtrl(AL_TRUE);

    if (Val == AL_NULL) {
        AL_U32 Index = Offset;
        AL_U32 BitNum = Num;
        while (BitNum--) {
            #ifndef AL_EFUSE_SIMU
            AlEfuse_ll_WriteBit(Index++);
            #endif
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "Efuse bit start from %d to %d all set to 1.\r\n",
               Offset, Offset + Num - 1);
    } else {
        for (AL_U32 i = 0; i < Num; i++) {
            if ((i & AL_EFUSE_LOG_MASK) == 0) {
                if (i != 0) {
                    AL_LOG(AL_LOG_LEVEL_INFO, "\r\n");
                }
                AL_LOG(AL_LOG_LEVEL_INFO, "[%03d : %03d] ", i,
                       Num > i + AL_EFUSE_LOG_MASK ? i + AL_EFUSE_LOG_MASK : Num - 1);
            }
            if (Val[i>>5] & (1 << (i & 0x1F))) {
                AL_LOG(AL_LOG_LEVEL_INFO, "1", Offset);
                #ifndef AL_EFUSE_SIMU
                AlEfuse_ll_WriteBit(Offset + i);
                #endif
            } else {
                AL_LOG(AL_LOG_LEVEL_INFO, "0", Offset);
            }
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "\r\n");
    }

    AlEfuse_ll_ClkCtrl(AL_FALSE);

    AL_LOG(AL_LOG_LEVEL_INFO, "-------------------\r\n");
}
