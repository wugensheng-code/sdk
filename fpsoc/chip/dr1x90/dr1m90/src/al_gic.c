
/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_chip.h"
#include "al_gicv3.h"
#include "al_gicv3_private.h"
#include "al_type.h"

static AL_U32 CpuId;

/* The GICv3 driver only needs to be initialized in EL3 */
static AL_UINTPTR RdistBaseAddrs[AL_ARM_CORE_COUNT];

static AL_U32 AlGic_MpidrToCorePos(AL_UINTPTR Mpidr)
{
    AL_U32 ClusterId, CpuId;

    CpuId = Mpidr & MPIDR_AFFLVL_MASK;
    ClusterId = Mpidr & MPIDR_CLUSTER_MASK;
    CpuId += (ClusterId >> CLUSTER_TO_CPUID_SHIFT);

    if (CpuId >= AL_ARM_CORE_COUNT)
        return -1;

    return CpuId;
}

static AL_U32 AlGic_CorePos(void)
{
    AL_UINTPTR Mpidr = GICV3_SYSREG_READ(mpidr_el1);
    return AlGic_MpidrToCorePos(Mpidr);
}

const AL_GICV3_DrvDataStruct GicData = {
    .CpuId = &CpuId,
    .GicdBase = AL_DR1M90_GICD_BASE,
    .GicrBase = AL_DR1M90_GICR_BASE,
    .RdistNum = ARRAY_SIZE(RdistBaseAddrs),
    .RdistBaseAddrs = RdistBaseAddrs,
    .MpidrToCorePos = AlGic_MpidrToCorePos,
};

/******************************************************************************
 * Anlogic common helper to initialize the GIC. Only invoked
 * in EL1
 *****************************************************************************/
void AlGic_Init(void)
{
    *(GicData.CpuId) = AlGic_CorePos();
    AlGicv3_DriverInit(&GicData);

    if (*(Gicv3DrvData->CpuId) == 0)
    {
        AlGicv3_DistInit();
    }
    else
    {
        AlGicv3_Rdist_WaitMasterAwake(GicData.RdistBaseAddrs[0]);
        AlGicv3_DisableOwnSpiInterrupt();
    }

    AlGicv3_RdistInit(AlGic_CorePos());
    AlGicv3_CpuIfEnable(AlGic_CorePos());
}
