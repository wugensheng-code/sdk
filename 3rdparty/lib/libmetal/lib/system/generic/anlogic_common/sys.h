/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/anlogic_common/sys.h
 * @brief	generic anlogic_common system primitives for libmetal.
 */

#ifndef __METAL_GENERIC_SYS__H__
#error "Include metal/sys.h instead of metal/generic/@PROJECT_MACHINE@/sys.h"
#endif

#ifndef __METAL_GENERIC_ANLOGIC_COMMON_SYS__H__
#define __METAL_GENERIC_ANLOGIC_COMMON_SYS__H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	metal_anlogic_irq_isr
 *
 * Anlogic interrupt ISR can be registered to the Anlogic embeddedsw
 * IRQ controller driver.
 *
 * @param[in] arg input argument, interrupt vector id.
 */
void metal_anlogic_irq_isr(void *arg);

/**
 * @brief	metal_anlogic_irq_int
 *
 * Anlogic interrupt controller initialization. It will initialize
 * the metal Anlogic IRQ controller data structure.
 *
 * @return 0 for success, or negative value for failure
 */
int metal_anlogic_irq_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __METAL_GENERIC_ANLOGIC_COMMON_SYS__H__ */
