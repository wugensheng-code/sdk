/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/dr1m90/sys.h
 * @brief	generic dr1m90 system primitives for libmetal.
 */

#ifndef __METAL_GENERIC_SYS__H__
#error "Include metal/sys.h instead of metal/generic/@PROJECT_MACHINE@/sys.h"
#endif

#include <metal/system/@PROJECT_SYSTEM@/anlogic_common/sys.h>
#include "al_intr.h"

#ifndef __METAL_GENERIC_DR1M90_SYS__H__
#define __METAL_GENERIC_DR1M90_SYS__H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef METAL_INTERNAL

#define UNUSED(x) (void)x

static inline void sys_irq_enable(unsigned int vector)
{
	(AL_VOID)AlIntr_SetInterrupt(vector, AL_FUNC_ENABLE);
}

static inline void sys_irq_disable(unsigned int vector)
{
	(AL_VOID)AlIntr_SetInterrupt(vector, AL_FUNC_DISABLE);
}

#endif /* METAL_INTERNAL */

#ifdef __cplusplus
}
#endif

#endif /* __METAL_GENERIC_DR1M90_SYS__H__ */
