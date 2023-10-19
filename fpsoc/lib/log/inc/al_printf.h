/*
 * Copyright (c) 2023, AnPRINTFic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_PRINTF_H_
#define __AL_PRINTF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

#define PRINT_SCALE_HEX             16
#define PRINT_SCALE_DECIMAL         10
#define PRINT_SCALE_EXPONENTIAL     'E'

#define OUTPUT_BUFFER_SIZE          32
#define OUTPUT_BUFFER_SIZE_LONG     64

#ifdef AL_PRINT_ASYNC

#ifndef RTOS_FREERTOS
#error "AL_PRINTF_ASYNC use for FREERTOS print task"
#endif

#define RING_BUF_SIZE 0x10000000

typedef struct {
    AL_U32 ValidSize;
    AL_U32 PrintPos;
    AL_U32 FillPos;
    AL_S8 *Buf;
} RingBufStruct;

extern RingBufStruct RingBuf;

AL_VOID AlPrint_AsyncPrintf(AL_VOID);
#endif

AL_VOID al_printf(AL_CHAR *Format, ...);

#ifdef __cplusplus
}
#endif

#endif
