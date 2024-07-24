/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_CAN_PLAT_H_
#define __AL_CAN_PLAT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "soc_plat.h"

#ifndef CAN_CLOCK
#define CAN_CLOCK (80*MHz)
#endif

#if !defined(CANFD) || !defined(CAN20B)
#define CANFD 0
#define CAN20B 1
#endif

#if !defined(CAN0PS_ENABLE) && !defined(CAN1PS_ENABLE)
#define CAN0PS_ENABLE
#endif

#ifdef CAN0PS_ENABLE
#ifndef CAN0_TYPE
#define CAN0_TYPE CANFD
#endif

#ifndef CAN0_SLOW_RATE
#define CAN0_SLOW_RATE 250000
#endif

#ifndef CAN0_FAST_RATE
#define CAN0_FAST_RATE 1000000
#endif
#endif

#ifdef CAN1PS_ENABLE
#ifndef CAN1_TYPE
#define CAN1_TYPE CANFD
#endif

#ifndef CAN1_SLOW_RATE
#define CAN1_SLOW_RATE 250000
#endif

#ifndef CAN1_FAST_RATE
#define CAN1_FAST_RATE 1000000
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
