/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_EFUSE_HW_H_
#define __AL_EFUSE_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Include Files ********************************/
#include "al_core.h"

/********************************* Exported Constant ****************************/
#define AL_EFUSE_CFG_BASE_ADDR          TOP_CRP_BASE_ADDR

#define AL_EFUSE_CFG_CLK_OFFSET         0x40
#define AL_EFUSE_CFG_CLK_MODE_WIDTH     1
#define AL_EFUSE_CFG_CLK_MODE_SHIFT     2

#define AL_EFUSE_CFG_CTRL_OFFSET        0x410
#define AL_EFUSE_CFG_CTRL_ADDR_WIDTH    12
#define AL_EFUSE_CFG_CTRL_ADDR_SHIFT    0
#define AL_EFUSE_CFG_CTRL_WRITE_WIDTH   1
#define AL_EFUSE_CFG_CTRL_WRITE_SHIFT   17
#define AL_EFUSE_CFG_CTRL_DONE_WIDTH    1
#define AL_EFUSE_CFG_CTRL_DONE_SHIFT    20

#ifdef __cplusplus
}
#endif

#endif
