/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al9000_mpu.h"

static volatile uint32_t* TOP_CFG_S_PROTEN_SET = (void*)0xF88060B0;
static volatile uint32_t* TOP_CFG_S_PROTEN_CLR = (void*)0xF88060B4;
static volatile uint32_t* TOP_CFG_S_GROUPID    = (void*)0xF8806100;

void mpu_enable(mpu_handle inst)
{
    uint32_t val = inst->ctrl;
    val |= 0x1;
    inst->ctrl = val;
}

void mpu_disable(mpu_handle inst)
{
    uint32_t val = inst->ctrl;
    val &= ~0x1;
    inst->ctrl = val;
}

void mpu_group_enable(mpu_handle inst, uint32_t rid, uint32_t base, uint32_t size, uint32_t flag, uint32_t group_mask)
{
    uint32_t val = inst->ctrl;
    inst->ctrl = rid << 8;
    inst->region[rid].sar = (base) >> 12;
    inst->region[rid].ear = (base + size - 1) >> 12;
    inst->region[rid].groupid = group_mask;
    inst->region[rid].rasr = flag | 0x1;
    if (val & 0x1)
        mpu_enable(inst);
}

void mpu_group_disable(mpu_handle inst, uint32_t rid)
{
    uint32_t val = inst->ctrl;
    inst->ctrl = rid << 8;
    inst->region[rid].rasr = 0x0;
    if (val & 0x1)
        mpu_enable(inst);
}

void mpu_groupid_set(enum mpu_groupid_t idx, uint32_t gid)
{
    TOP_CFG_S_GROUPID[idx] = gid & 0x1F;
}

void tzpc_prot_set(enum tzpc_t flag)
{
    *TOP_CFG_S_PROTEN_SET = (uint32_t)flag;
}

void tzpc_prot_clr(enum tzpc_t flag)
{
    *TOP_CFG_S_PROTEN_CLR = (uint32_t)flag;
}
