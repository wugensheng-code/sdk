#ifndef AL_SYS_TIMER_H
#define AL_SYS_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "al_type.h"

AL_U64 AlSys_GetFreq(AL_VOID);

AL_U64 AlSys_GetTimer(AL_VOID);

AL_VOID AlSys_UDelay(AL_U64 Usec);

AL_VOID AlSys_MDelay(AL_U64 Msec);

AL_VOID AlDelay_SDelay(AL_U64 Second);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* AL_SYS_TIMER_H */