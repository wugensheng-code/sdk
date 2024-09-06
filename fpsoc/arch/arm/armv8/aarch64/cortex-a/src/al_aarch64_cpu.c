/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "al_chip.h"
#include "al_core.h"

#define CPU_SYSCTRL_S_APU0_RESET_VECTOR_H ((TOP_S_BASE_ADDR) + 0x00000300U)
#define CPU_SYSCTRL_S_APU0_RESET_VECTOR_L ((TOP_S_BASE_ADDR) + 0x00000304U)
#define CPU_SYSCTRL_S_APU1_RESET_VECTOR_H ((TOP_S_BASE_ADDR) + 0x00000308U)
#define CPU_SYSCTRL_S_APU1_RESET_VECTOR_L ((TOP_S_BASE_ADDR) + 0x0000030CU)
#define CPU_SYSCTRL_S_XPU_SRST ((TOP_S_BASE_ADDR) + 0x00000334U)

typedef enum
{
    Core0,
    Core1,
} AL_CpuNumEnum;

static AL_VOID AlCpu_Reset(AL_U64 CpuId, AL_U64 VectorAddr)
{
    switch (CpuId)
    {
    case Core0:
        AL_REG32_WRITE(CPU_SYSCTRL_S_APU0_RESET_VECTOR_H, VectorAddr >> 34);
        AL_REG32_WRITE(CPU_SYSCTRL_S_APU0_RESET_VECTOR_L, (VectorAddr >> 2) & 0xffffffff);
        AL_REG32_SET_BIT(CPU_SYSCTRL_S_XPU_SRST, 4, 1); /// release level reset
        AL_REG32_SET_BIT(CPU_SYSCTRL_S_XPU_SRST, 0, 0); /// trigger pulse reset
        break;
    case Core1:
        AL_REG32_WRITE(CPU_SYSCTRL_S_APU1_RESET_VECTOR_H, VectorAddr >> 34);
        AL_REG32_WRITE(CPU_SYSCTRL_S_APU1_RESET_VECTOR_L, (VectorAddr >> 2) & 0xffffffff);
        AL_REG32_SET_BIT(CPU_SYSCTRL_S_XPU_SRST, 5, 1); /// release level reset
        AL_REG32_SET_BIT(CPU_SYSCTRL_S_XPU_SRST, 1, 0); /// trigger pulse reset
        break;
    default:
        break;
    }

    return;
}

static AL_VOID AlCpu_Up(AL_U64 CpuId, AL_U64 ResetAddr)
{
    AlCache_DisableDCache();

    AlCpu_Reset(CpuId, ResetAddr);

    AlCache_EnableDCache();
}

AL_VOID AlCpu_WakeUpMultipleCpus(AL_U64 CpuId, AL_U64 ResetAddr)
{
    static volatile int CpuPowerStatus[2] = {0};
    AL_U32 CpuIndex = 0;

    if (CpuId == 0)
    {
        CpuPowerStatus[CpuId] = 1;

        for (CpuId = 1; CpuId < 2; ++CpuId)
        {
            AlCpu_Up(CpuId, ResetAddr);
        }

        CpuIndex = 0;

        while (CpuIndex < 2)
        {

            if (CpuPowerStatus[CpuIndex] != 0)
            {
                CpuIndex++;
            }
        }
    }
    else
    {
        CpuPowerStatus[CpuId] = 1;
    }
}
