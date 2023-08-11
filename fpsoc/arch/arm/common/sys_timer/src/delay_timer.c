#include "sysregs.h"
#include "al_type.h"
#include "al_top.h"


AL_VOID AlSys_StartTimer(AL_VOID)
{
    AL_U32 V = 1;

    asm volatile(
    "msr cntp_ctl_el0, %x[V]"
    :
    : [V]"r"(V)
    : "memory");

    Altop_Syscnts_CounterCtrl(AL_FUNC_ENABLE);
}


AL_VOID AlSys_StopTimer(AL_VOID)
{
    Altop_Syscnts_CounterCtrl(AL_FUNC_DISABLE);
}


AL_U64 AlSys_GetTimerTick(AL_VOID)
{
    return (AL_U64)read_sysreg(CNTPCT_EL0);
}

AL_U64 AlSys_GetTimerFreq(AL_VOID)
{
    extern AL_U64 SystemCoreClock;
    return SystemCoreClock;
}


AL_VOID AlSys_DelayTicks(AL_U64 TickCount)
{
    AL_U64 TickEnd, TickCur;

    TickCur = AlSys_GetTimerTick();
    TickEnd = TickCur + TickCount;

    do {
        TickCur = AlSys_GetTimerTick();
    } while (TickCur < TickEnd);
}


AL_VOID AlSys_UDelay(AL_U32 Usec)
{
    AlSys_DelayTicks((AL_U64)Usec * AlSys_GetTimerFreq() / 1000000);
}


AL_VOID AlSys_MDelay(AL_U32 Msec)
{
    AlSys_DelayTicks((AL_U64)Msec * AlSys_GetTimerFreq() / 1000);
}


