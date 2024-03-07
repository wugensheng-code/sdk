/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_SYS_TIMER_H_
#define __AL_SYS_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "al_compiler.h"
#include "al_type.h"

AL_VOID AlSys_StartTimer(AL_VOID);

AL_VOID AlSys_StopTimer(AL_VOID);

AL_U64 AlSys_GetTimerFreq(AL_VOID);

AL_U64 AlSys_GetTimerTickCount(AL_VOID);

AL_VOID AlSys_UDelay(AL_U64 Usec);

AL_VOID AlSys_MDelay(AL_U64 Msec);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* AL_SYS_TIMER_H */