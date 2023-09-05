/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"
#include "al_core.h"

#define TSG_CTRL_EN_CNT_BIT_POS  (0)

AL_VOID Altop_Syscnts_CounterCtrl(AL_FUNCTION CntStatus)
{
    AL_REG32_SET_BIT(SYSCNT_S_BASE, TSG_CTRL_EN_CNT_BIT_POS, CntStatus);
}

