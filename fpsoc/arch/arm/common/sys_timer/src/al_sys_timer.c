/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"

#if defined __aarch64__
#include "al_aarch64_core.h"
#elif defined __arm__
#include "al_aarch32_core.h"
#endif

#include "al_top.h"
#include "al_systimer.h"


AL_VOID AlSys_StartTimer(AL_VOID)
{
    ARCH_COPROCR_SYSREG_WRITE(cntp_ctl_el0, 1);
    Altop_Syscnts_CounterCtrl(AL_FUNC_ENABLE);
}


AL_VOID AlSys_StopTimer(AL_VOID)
{
    Altop_Syscnts_CounterCtrl(AL_FUNC_DISABLE);
}

AL_U64 AlSys_GetTimerFreq(AL_VOID)
{
    extern AL_U64 SystemCoreClock;
    return SystemCoreClock;
}

AL_U64 AlSys_GetTimerTickCount(AL_VOID)
{
    return (AL_U64)ARCH_COPROCR_SYSREG_READ_64(cntpct_el0);
}


AL_VOID AlSys_DelayTicks(AL_U64 TickCount)
{
    AL_U64 TickEnd, TickCur;

    TickCur = AlSys_GetTimerTickCount();
    TickEnd = TickCur + TickCount;

    do {
        TickCur = AlSys_GetTimerTickCount();
    } while (TickCur < TickEnd);
}


AL_VOID AlSys_UDelay(AL_U64 Usec)
{
    AlSys_DelayTicks((AL_U64)Usec * AlSys_GetTimerFreq() / 1000000);
}


AL_VOID AlSys_MDelay(AL_U64 Msec)
{
    AlSys_DelayTicks((AL_U64)Msec * AlSys_GetTimerFreq() / 1000);
}
