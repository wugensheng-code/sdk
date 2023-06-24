#ifndef AL_CLI_H
#define AL_CLI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

#include "al_cli_common.h"
#include "al_cli_cmd.h"
#include "al_cli_console.h"

#define AL_CLI_MAX_ARGC_NUM     16

typedef struct
{
    AL_CLI_CmdListStruct *CmdList;

    AL_U8 CliInputBuf[AL_CLI_BUF_SIZE];

    AL_U32 CmdCurPos;
    AL_U32 CmdEndPos;

    AL_U8 EchoEnable;
} AL_CLI_CmdInfoStruct;

AL_S32 AlCli_Init(AL_CLI_ConsoleTypeEnum ConsoleType);

AL_VOID AlCli_Main(AL_VOID *Data);

#ifdef __cplusplus
}
#endif

#endif /* AL_CLI_H */