/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_CLI_CONSOLE_H_
#define __AL_CLI_CONSOLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

#define AL_CLI_BUF_SIZE 1024

typedef struct {
    AL_S8 *Name;
    AL_S32 (*ConsoleRead)(AL_VOID *Buff, AL_U32 len);
    AL_S32 (*ConsoleWrite)(const AL_VOID *Buff, AL_U32 len);
    AL_S32 (*ConsoleInit)();
    AL_S32 (*ConsoleDeinit)();
} AL_CLI_DevConsoleStruct;

typedef enum {
    AL_CLI_CONSOLE_UART = 0,
} AL_CLI_ConsoleTypeEnum;

typedef struct {
    AL_CLI_ConsoleTypeEnum ConsoleType;
    AL_CLI_DevConsoleStruct *DevConsole;
    AL_U8 CliConsoleBuf[AL_CLI_BUF_SIZE];
} AL_CLI_ConsoleStruct;

AL_CLI_ConsoleStruct *AlCli_LookUpConsole(AL_CLI_ConsoleTypeEnum ConsoleType);

AL_S32 AlCli_ConsoleInit(AL_CLI_DevConsoleStruct *Console);

AL_S32 AlCli_ConsoleRead(AL_CLI_DevConsoleStruct *Console, AL_VOID *Buf, AL_U32 Len);

AL_S32 AlCli_ConsoleWrite(AL_CLI_DevConsoleStruct *Console, const AL_VOID *Buf, AL_U32 Len);

#ifdef __cplusplus
}
#endif

#endif /* AL_CLI_CONSOLE_H */