/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_MMU_H_
#define __AL_MMU_H_

#ifndef __ASSEMBLER__
#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

#define MEM_2M_ALIGN(Mem) ((Mem & (~0x200000UL)) == 0)

AL_S64 mmu_settlb(AL_UINTPTR addr, AL_U64 attr);

#ifdef __cplusplus
}
#endif
#endif

#define EXECUTE_NEVER   ((0x1 << 53) | (0x1 << 54))
#define NON_SECURE      (0x1 << 5)
#define DEVICE_MEM      (0x409UL | EXECUTE_NEVER)
#define RESERVED        (0x0UL)

#ifdef SUPPORT_NONSECURE
#define NORM_CACHE        0x605UL
#define NORM_NONCACHE     0x601UL
#define NORM_RO_CACHE     0x685UL
#else
#define NORM_CACHE        0x705UL
#define NORM_NONCACHE     0x701UL
#define NORM_RO_CACHE     0x785UL
#endif

#endif /* AL_MMU_H */
