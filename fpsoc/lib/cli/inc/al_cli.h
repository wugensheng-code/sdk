/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_CLI_H_
#define __AL_CLI_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "al_core.h"

#include "al_cli_common.h"
#include "al_cli_cmd.h"
#include "al_cli_console.h"

#ifdef USE_RTOS
#ifdef RTOS_RTTHREAD
#include <msh.h>
#else

#endif
#endif

#define AL_CLI_MAX_ARGC_NUM     16

typedef struct
{
    AL_CLI_CmdListStruct *CmdList;

    AL_U8 CliInputBuf[AL_CLI_BUF_SIZE];

    AL_U32 CmdCurPos;
    AL_U32 CmdEndPos;

    AL_U8 EchoEnable;

    AL_CHAR *prompt;
} AL_CLI_CmdInfoStruct;

AL_S32 AlCli_Init(AL_CLI_ConsoleTypeEnum ConsoleType);

AL_VOID AlCli_Main(AL_VOID *Data);

#ifdef __cplusplus
}
#endif

#endif /* AL_CLI_H */
