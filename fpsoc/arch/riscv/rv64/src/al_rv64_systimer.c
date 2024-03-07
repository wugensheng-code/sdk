/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"
#include "al_chip.h"
#include "al_rv64_core.h"
#include "al_rv64_systimer.h"
#include "al_systimer.h"

AL_U64 AlSys_GetTimerFreq(AL_VOID)
{
    extern AL_U64 SystemCoreClock;
    return SystemCoreClock / 5;
}

AL_U64 AlSys_GetTimerTickCount(AL_VOID)
{
    return SysTimer->MTIMER;
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
    AlSys_DelayTicks(Usec * AlSys_GetTimerFreq() / 1000000);
}


AL_VOID AlSys_MDelay(AL_U64 Msec)
{
    AlSys_DelayTicks(Msec * AlSys_GetTimerFreq() / 1000);
}