/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_CLI_UART_CONSOLE_H
#define AL_CLI_UART_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

#ifdef SUPPORT_OS
// Todo
#else

#define AL_CLI_QUEUE_MAX_SIZE   1024
#define AL_CLI_UART_BUFFER_MAX_SIZE     2048

typedef char                    QueueElemType;

typedef struct
{
    QueueElemType *Base;
    AL_U32 Front;
    AL_U32 Rear;
} AL_CLI_CircularQueueStruct;

AL_S32 AlCli_CircularQueueInit(AL_CLI_CircularQueueStruct *Queue);
AL_S32 AlCli_CircularQueueSend(AL_CLI_CircularQueueStruct *Queue, QueueElemType  Elem);
AL_S32 AlCli_CircularQueueRecv(AL_CLI_CircularQueueStruct *Queue, QueueElemType *Elem);

/* ToDo:  */
#define AL_DISABLE_INTERRUPT()
#define AL_ENABLE_INTERRUPT()

#endif /* SUPPORT_OS */

AL_S32 AlCli_UartConsoleInit();

AL_S32 AlCli_UartConsoleDeInit();

AL_S32 AlCli_UartConsoleRead(AL_VOID *Buf, AL_U32 Len);

AL_S32 AlCli_UartConsoleWrite(const AL_VOID *Buf, AL_U32 Len);

#ifdef __cplusplus
}
#endif

#endif /* AL_CLI_UART_CONSOLE_H */