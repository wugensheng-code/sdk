/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"
#include "al_core.h"

#define TSG_CTRL_EN_CNT_BIT_POS  (0)

#define TOP_NS_PLS_PROT_OFFSET                 0x80UL
#define TOP_CRP_SRST_CTRL2_OFFSET              0x78UL

AL_VOID Altop_Syscnts_CounterCtrl(AL_FUNCTION CntStatus)
{
    AL_REG32_SET_BIT(TOP_SYSCNT_S_BASE_ADDR, TSG_CTRL_EN_CNT_BIT_POS, CntStatus);
}

AL_VOID AlTop_GPPortEnable(AL_VOID)
{
    AL_U32 val = 0;

    /* Enable gp normal access */
    val = AL_REG32_READ(TOP_NS_BASE_ADDR + TOP_NS_PLS_PROT_OFFSET);
    val &= ~0x3;
    AL_REG32_WRITE(TOP_NS_BASE_ADDR + TOP_NS_PLS_PROT_OFFSET, val);

    /* Soft reset hp0, hp1, gp0, gp1 */
    val = 0;
    val = AL_REG32_READ(TOP_CRP_BASE_ADDR + TOP_CRP_SRST_CTRL2_OFFSET);
    AL_REG32_WRITE(TOP_CRP_BASE_ADDR + TOP_CRP_SRST_CTRL2_OFFSET, val & (~0x33));
    AlSys_MDelay(1);
    AL_REG32_WRITE(TOP_CRP_BASE_ADDR + TOP_CRP_SRST_CTRL2_OFFSET, val | 0x33);
}

