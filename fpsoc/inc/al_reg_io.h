/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_REG_IO_H_
#define __AL_REG_IO_H_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "al_core.h"
#include "al_utils_def.h"

#define AL_REG8_WRITE(reg_addr, value)             do {*(volatile AL_U8 *)(AL_UINTPTR)(reg_addr) = value;} while (0)
#define AL_REG16_WRITE(reg_addr, value)            do {*(volatile AL_U16 *)(AL_UINTPTR)(reg_addr) = value;} while (0)
#define AL_REG32_WRITE(reg_addr, value)            do {*(volatile AL_U32 *)(AL_UINTPTR)(reg_addr) = value;} while (0)
#define AL_REG64_WRITE(reg_addr, value)            do {*(volatile AL_U64 *)(AL_UINTPTR)(reg_addr) = value;} while (0)

#define AL_REG8_READ(reg_addr)                     (*(volatile AL_U8 *)(AL_UINTPTR)(reg_addr))
#define AL_REG16_READ(reg_addr)                    (*(volatile AL_U16 *)(AL_UINTPTR)(reg_addr))
#define AL_REG32_READ(reg_addr)                    (*(volatile AL_U32 *)(AL_UINTPTR)(reg_addr))
#define AL_REG64_READ(reg_addr)                    (*(volatile AL_U64 *)(AL_UINTPTR)(reg_addr))

#define AL_REG16_GET_BITS(reg_addr, shift, size)   (((AL_REG16_READ(reg_addr)) & BITS_16((shift),(size))) >> (shift))
#define AL_REG32_GET_BITS(reg_addr, shift, size)   (((AL_REG32_READ(reg_addr)) & BITS_32((shift),(size))) >> (shift))
#define AL_REG64_GET_BITS(reg_addr, shift, size)   (((AL_REG64_READ(reg_addr)) & BITS_64((shift),(size))) >> (shift))

#define AL_REG16_GET_BIT(reg_addr, shift)          (AL_BOOL)(((AL_REG16_READ(reg_addr)) & BIT(shift)) >> (shift))
#define AL_REG32_GET_BIT(reg_addr, shift)          (AL_BOOL)(((AL_REG32_READ(reg_addr)) & BIT(shift)) >> (shift))
#define AL_REG64_GET_BIT(reg_addr, shift)          (AL_BOOL)(((AL_REG64_READ(reg_addr)) & BIT(shift)) >> (shift))

#define AL_REG16_SET_BITS(reg_addr, shift, size, value)  do {                                             \
    AL_REG16_WRITE(reg_addr, ((AL_REG16_READ(reg_addr)) & (~BITS_16(shift, size))) | ((value) << (shift))); \
} while (0)

#define AL_REG32_SET_BITS(reg_addr, shift, size, value)  do {                                             \
    AL_REG32_WRITE(reg_addr, ((AL_REG32_READ(reg_addr)) & (~BITS_32(shift, size))) | ((value) << (shift))); \
} while (0)

#define AL_REG64_SET_BITS(reg_addr, shift, size, value)  do {                                               \
    AL_REG64_WRITE(reg_addr, ((AL_REG64_READ(reg_addr)) & (~BITS_64(shift, size))) | ((value) << (shift))); \
} while (0)

#define AL_REG16_SET_BIT(reg_addr, shift, value)         do {                                      \
    AL_REG16_WRITE(reg_addr, ((AL_REG16_READ(reg_addr)) & (~BIT(shift))) | (((value) << (shift)) & BIT(shift))); \
} while (0)

#define AL_REG32_SET_BIT(reg_addr, shift, value)         do {                                      \
    AL_REG32_WRITE(reg_addr, ((AL_REG32_READ(reg_addr)) & (~BIT(shift))) | (((value) << (shift)) & BIT(shift))); \
} while (0)

#define AL_REG64_SET_BIT(reg_addr, shift, value)         do {                                      \
    AL_REG64_WRITE(reg_addr, ((AL_REG64_READ(reg_addr)) & (~BIT(shift))) | (((value) << (shift)) & BIT(shift))); \
} while (0)

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /*AL_REG_IO_H */
