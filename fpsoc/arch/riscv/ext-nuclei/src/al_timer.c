#include "al_chip.h"
#include "core_feature_base.h"
#include "ext_timer.h"

extern uint32_t SystemCoreClock;

static uint32_t get_timer_freq()
{
    return SystemCoreClock;
}

uint32_t measure_cpu_freq(uint32_t n)
{
    uint32_t start_mcycle, delta_mcycle;
    uint32_t start_mtime, delta_mtime;
    uint32_t mtime_freq = get_timer_freq();

    // Don't start measuruing until we see an mtime tick
    uint32_t tmp = (uint32_t)SysTimer_GetLoadValue();
    do {
        start_mtime = (uint32_t)SysTimer_GetLoadValue();
        start_mcycle = __RV_CSR_READ(CSR_MCYCLE);
    } while (start_mtime == tmp);

    do {
        delta_mtime = (uint32_t)SysTimer_GetLoadValue() - start_mtime;
        delta_mcycle = __RV_CSR_READ(CSR_MCYCLE) - start_mcycle;
    } while (delta_mtime < n);

    return (delta_mcycle / delta_mtime) * mtime_freq
           + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}

uint32_t get_cpu_freq()
{
    uint32_t cpu_freq;

    // warm up
    measure_cpu_freq(1);
    // measure for real
    cpu_freq = measure_cpu_freq(100);

    return cpu_freq;
}

void _delay_us(uint64_t count)
{
    uint64_t start_mtime, delta_mtime;
    uint64_t delay_ticks = (get_timer_freq() * (uint64_t)count) / 1000000;

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
void _delay_ms(uint64_t count)
{
    uint64_t start_mtime, delta_mtime;
    uint64_t delay_ticks = (get_timer_freq() * (uint64_t)count) / 1000;

    start_mtime = SysTimer_GetLoadValue();

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < delay_ticks);
}

uint64_t get_SystickTimer(void)
{
    return SysTimer->MTIMER;
}

uint64_t get_MTimerOutValue(uint64_t count)
{
    return (get_timer_freq() * count) / 1000000;
}

uint64_t get_Us(uint64_t start, uint64_t end)
{
    uint64_t freq = get_timer_freq();
    return ((end - start)/(freq/1000000));
}

void AlDelay_Us(unsigned long long Count)
{
    _delay_us(Count);
}

void AlDelay_Ms(unsigned long long Count)
{
    _delay_ms(Count);
}

void AlDelay_S(unsigned long long Count)
{
    uint64_t start_mtime, delta_mtime;
    uint64_t delay_ticks = (get_timer_freq() * (uint64_t)Count);

    start_mtime = SysTimer_GetLoadValue();

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < delay_ticks);
}

unsigned long long AlDelay_CalcUs(unsigned long long Start, unsigned long long End)
{
    return get_Us(Start, End);
}

unsigned long long AlSys_GetTimer(void)
{
    return SysTimer->MTIMER;
}