/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_EFUSE_LL_H_
#define __AL_EFUSE_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Include Files ********************************/
#include "al_efuse_hw.h"

/****************************** Exported Function ******************************/
/**
 * @brief  efuse clock control
 * @param  ClkEn: AL_TRUE - enable, AL_FALSE - disable
 * @return None
*/
static inline AL_VOID AlEfuse_ll_ClkCtrl(AL_BOOL ClkEn)
{
    AL_REG32_SET_BIT(AL_EFUSE_CFG_BASE_ADDR + AL_EFUSE_CFG_CLK_OFFSET,
                     AL_EFUSE_CFG_CLK_MODE_SHIFT, ClkEn);
}

/**
 * @brief  efuse write bit index
 * @param  Index: The index of the efuse bit
 * @return None
*/
static inline AL_VOID AlEfuse_ll_WriteBitIndex(AL_U32 Index)
{
    AL_REG32_SET_BITS(AL_EFUSE_CFG_BASE_ADDR + AL_EFUSE_CFG_CTRL_OFFSET,
                          AL_EFUSE_CFG_CTRL_ADDR_SHIFT, AL_EFUSE_CFG_CTRL_ADDR_WIDTH, Index);
}

/**
 * @brief  efuse start write
 * @param  None
 * @return None
*/
static inline AL_VOID AlEfuse_ll_StartWrite(AL_VOID)
{
    AL_REG32_SET_BIT(AL_EFUSE_CFG_BASE_ADDR + AL_EFUSE_CFG_CTRL_OFFSET,
                          AL_EFUSE_CFG_CTRL_WRITE_SHIFT, AL_TRUE);
}

/**
 * @brief  efuse write bit
 * @param  Index: The index of the efuse bit
 * @return None
*/
static inline AL_VOID AlEfuse_ll_WriteBit(AL_U32 Index)
{
    AlEfuse_ll_WriteBitIndex(Index);
    AlEfuse_ll_StartWrite();
    AlSys_UDelay(30);
}

#ifdef __cplusplus
}
#endif

#endif
