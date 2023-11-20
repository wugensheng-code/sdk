/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_cli_console.h"
#include "al_cli_uart_console.h"
#include "al_cli_common.h"

#define AL_CLICONSOLE_NUM_INSTANCES     1

AL_CLI_DevConsoleStruct UartConsole = {
    .Name          = (AL_S8 *)"uart-console",
    .ConsoleRead   = AlCli_UartConsoleRead,
    .ConsoleWrite  = AlCli_UartConsoleWrite,
    .ConsoleInit   = AlCli_UartConsoleInit,
    .ConsoleDeinit = AlCli_UartConsoleDeInit
};

AL_CLI_ConsoleStruct ConsoleList[AL_CLICONSOLE_NUM_INSTANCES] = {
    {
        .ConsoleType    = AL_CLI_CONSOLE_UART,
        .DevConsole     = &UartConsole,
    }
};

AL_CLI_ConsoleStruct *AlCli_LookUpConsole(AL_CLI_ConsoleTypeEnum ConsoleType)
{
    AL_U32 Index;
    AL_CLI_ConsoleStruct *CliConsole = AL_NULL;

    for (Index = 0; Index < AL_CLICONSOLE_NUM_INSTANCES; Index++) {
        if (ConsoleList[Index].ConsoleType == ConsoleType) {
            CliConsole = &ConsoleList[Index];
            break;
        }
    }

    return CliConsole;
}

AL_S32 AlCli_ConsoleInit(AL_CLI_DevConsoleStruct *Console)
{
    if (Console == AL_NULL || Console->ConsoleInit == AL_NULL)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    return (Console->ConsoleInit());
}

AL_S32 AlCli_ConsoleRead(AL_CLI_DevConsoleStruct *Console, AL_VOID *Buf, AL_U32 Len)
{
    if (Console == AL_NULL || Console->ConsoleRead == AL_NULL || Buf == AL_NULL || Len < 1)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    return Console->ConsoleRead(Buf, Len);
}

AL_S32 AlCli_ConsoleWrite(AL_CLI_DevConsoleStruct *Console, const AL_VOID *Buf, AL_U32 Len)
{
    if (Console == AL_NULL || Console->ConsoleWrite == AL_NULL || Buf == AL_NULL || Len < 1)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    return Console->ConsoleWrite(Buf, Len);
}