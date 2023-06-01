#ifndef AL_SYS_TIMER_H
#define AL_SYS_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "al_type.h"

AL_VOID AlSys_Delay(AL_U32 Us);

AL_U64 AlSys_GetTimer(AL_VOID);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* AL_SYS_TIMER_H */