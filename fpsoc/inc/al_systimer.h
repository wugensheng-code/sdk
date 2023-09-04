#ifndef AL_SYS_TIMER_H
#define AL_SYS_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "al_compiler.h"
#include "al_type.h"

AL_VOID AlSys_StartTimer(AL_VOID);

AL_VOID AlSys_StopTimer(AL_VOID);

__STATIC_INLINE AL_U64 AlSys_GetTimerFreq(AL_VOID)
{
    extern AL_U64 SystemCoreClock;
    return SystemCoreClock;
}

AL_U64 AlSys_GetTimerTickCount(AL_VOID);

AL_VOID AlSys_UDelay(AL_U64 Usec);

AL_VOID AlSys_MDelay(AL_U64 Msec);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* AL_SYS_TIMER_H */