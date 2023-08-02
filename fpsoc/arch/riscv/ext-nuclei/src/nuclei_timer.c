#include "al_chip.h"
#include "al_type.h"
#include "core_feature_base.h"
#include "ext_timer.h"

extern AL_U32 SystemCoreClock;

static AL_U32 get_timer_freq()
{
    return SystemCoreClock;
}

AL_U32 measure_cpu_freq(AL_U32 n)
{
    AL_U32 start_mcycle, delta_mcycle;
    AL_U32 start_mtime, delta_mtime;
    AL_U32 mtime_freq = get_timer_freq();

    // Don't start measuruing until we see an mtime tick
    AL_U32 tmp = (AL_U32)SysTimer_GetLoadValue();
    do {
        start_mtime = (AL_U32)SysTimer_GetLoadValue();
        start_mcycle = __RV_CSR_READ(CSR_MCYCLE);
    } while (start_mtime == tmp);

    do {
        delta_mtime = (AL_U32)SysTimer_GetLoadValue() - start_mtime;
        delta_mcycle = __RV_CSR_READ(CSR_MCYCLE) - start_mcycle;
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

void _delay_us(AL_U64 count)
{
    AL_U64 start_mtime, delta_mtime;
    AL_U64 delay_ticks = (get_timer_freq() * (AL_U64)count) / 1000000;

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
void _delay_ms(AL_U64 count)
{
    AL_U64 start_mtime, delta_mtime;
    AL_U64 delay_ticks = (get_timer_freq() * (AL_U64)count) / 1000;

    start_mtime = SysTimer_GetLoadValue();

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < delay_ticks);
}

AL_U64 get_SystickTimer(void)
{
    return SysTimer->MTIMER;
}

AL_U64 get_MTimerOutValue(AL_U64 count)
{
    return (get_timer_freq() * count) / 1000000;
}

AL_U64 get_Us(AL_U64 start, AL_U64 end)
{
    AL_U64 freq = get_timer_freq();
    return ((end - start)/(freq/1000000));
}

void AlSys_UDelay(AL_U64 Usec)
{
    _delay_us(Usec);
}

void AlSys_MDelay(AL_U64 Msec)
{
    _delay_ms(Msec);
}

void AlDelay_SDelay(AL_U64 Second)
{
    AL_U64 start_mtime, delta_mtime;
    AL_U64 delay_ticks = (get_timer_freq() * Second);

    start_mtime = SysTimer_GetLoadValue();

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < delay_ticks);
}

AL_U64 AlSys_GetTimer(void)
{
    return SysTimer->MTIMER;
}

AL_U64 AlSys_GetFreq(void)
{
	return get_timer_freq();
}