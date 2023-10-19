/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_LOG_H_
#define __AL_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "al_type.h"
#include "al_uart_hal.h"

#if ((LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1))
extern AL_UART_HalStruct *AlLog;
#endif

#define AL_LOG_UART0 0
#define AL_LOG_UART1 1

AL_S32 AlLog_Init();

AL_U32 AlLog_Write(const void* Data, AL_U32 Len);

AL_VOID AlLog_WriteByte(AL_S8 Data);

#ifdef __cplusplus
}
#endif

#endif
