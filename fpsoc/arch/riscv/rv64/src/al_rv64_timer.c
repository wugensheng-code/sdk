/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"
#include "al_chip.h"
#include "al_rv_core.h"
#include "al_rv64_timer.h"
#include "al_systimer.h"


AL_U32 measure_cpu_freq(AL_U32 n)
{
    AL_U32 start_mcycle, delta_mcycle;
    AL_U32 start_mtime, delta_mtime;
    AL_U32 mtime_freq = AlSys_GetTimerFreq();

    // Don't start measuruing until we see an mtime tick
    AL_U32 tmp = (AL_U32)SysTimer_GetLoadValue();
    do {
        start_mtime = (AL_U32)SysTimer_GetLoadValue();
        start_mcycle = ARCH_SYSREG_READ(CSR_MCYCLE);
    } while (start_mtime == tmp);

    do {
        delta_mtime = (AL_U32)SysTimer_GetLoadValue() - start_mtime;
        delta_mcycle = ARCH_SYSREG_READ(CSR_MCYCLE) - start_mcycle;
    } while (delta_mtime < n);

    return (delta_mcycle / delta_mtime) * mtime_freq
           + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}

AL_U32 get_cpu_freq()
{
    AL_U32 cpu_freq;

    // warm up
    measure_cpu_freq(1);
    // measure for real
    cpu_freq = measure_cpu_freq(100);

    return cpu_freq;
}

AL_VOID _delay_us(AL_U64 count)
{
    AL_U64 start_mtime, delta_mtime;
    AL_U64 delay_ticks = (AlSys_GetTimerFreq() * (AL_U64)count) / 1000000;

    start_mtime = SysTimer_GetLoadValue();

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < delay_ticks);
}

/**
 * \brief      delay a time in milliseconds
 * \details
 *             provide API for delay
 * \param[in]  count: count in milliseconds
 * \remarks
 */
AL_VOID _delay_ms(AL_U64 count)
{
    AL_U64 start_mtime, delta_mtime;
    AL_U64 delay_ticks = (AlSys_GetTimerFreq() * (AL_U64)count) / 1000;

    start_mtime = SysTimer_GetLoadValue();

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < delay_ticks);
}

AL_U64 AlSys_GetTimerTickCount(AL_VOID)
{
    return SysTimer->MTIMER;
}


AL_VOID AlSys_UDelay(AL_U64 Usec)
{
    _delay_us(Usec);
}

AL_VOID AlSys_MDelay(AL_U64 Msec)
{
    _delay_ms(Msec);
}
