#include "al_type.h"
#include "io.h"
#include "sysregs.h"
#include "al_params.h"

static void __delay(AL_U64 count)
{
    AL_U64 tEnd, tCur;
    tCur = read_sysreg(CNTPCT_EL0);
    tEnd = tCur +count;
    do {
        tCur = read_sysreg(CNTPCT_EL0);
    } while (tCur < tEnd);
}

/**
 * @desc  : get cpu system counter freq
 * @param {*}
 * @return: system counter freq
 */
static AL_U64 syscnt_get_freq(void)
{
    return SYSTEM_CLOCK;
}

 AL_U64 get_SystickTimer(void)
{
    return read_sysreg(CNTPCT_EL0);
}

uint64_t get_MTimerOutValue(uint64_t count)
{
    AL_U64 freq = syscnt_get_freq();
    return (count*(freq/1000000));
}

void _delay_us(AL_U64 count)
{
    AL_U64 freq = syscnt_get_freq();
    AL_U64 delay_cnt = (AL_U64)(count*(freq/1000000));
    __delay(delay_cnt);
}

static void _delay_ms(AL_U64 count)
{
    AL_U64 freq = syscnt_get_freq();
    AL_U64 delay_cnt = (AL_U64)(count*(freq/1000));
    __delay(delay_cnt);
}

static void _delay_s(AL_U64 count)
{
    AL_U64 freq = syscnt_get_freq();
    AL_U64 delay_cnt = (AL_U64)(count*freq);
    __delay(delay_cnt);
}

AL_U64 get_Us(AL_U64 start, AL_U64 end)
{
    AL_U64 freq = syscnt_get_freq();
    return ((end - start)/(freq/1000000));
}

void AlSys_UDelay(AL_U32 Usec)
{
    _delay_us(Usec);
}

void AlSys_MDelay(AL_U32 Msec)
{
    _delay_ms(Msec);
}

void AlDelay_SDelay(AL_U64 Second)
{
    _delay_s(Second);
}

 AL_U64 AlSys_GetTimer(void)
{
    return get_SystickTimer();
}

 AL_U64 AlSys_GetFreq(void)
{
    return syscnt_get_freq();
}