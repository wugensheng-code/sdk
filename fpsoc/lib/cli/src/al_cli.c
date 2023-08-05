#include "al_cli.h"

#include <stdio.h>
#include <string.h>

/* Record all registered commands and command information obtained from the console */
AL_CLI_CmdInfoStruct *CliCmdInfo = AL_NULL;

static AL_CLI_ConsoleStruct *CliConsole;

static AL_S32 AlCli_ProcessCmd(AL_S32 Argc, AL_S8 *Argv[])
{
    const AL_CLI_CmdStruct *Cmd = AL_NULL;
    AL_S32 Ret = AL_OK;

    if (Argv == AL_NULL || Argc < 1) {
        return AL_CLI_ERROR_ILLEGAL_PARAM;
    }

    AL_CLI_PRINTF("\n");

#ifdef USE_RTOS
#ifdef RTOS_RTTHREAD
    Ret = msh_exec(Argv[0], strlen(Argv[0]));
    if (Ret == AL_OK)
    {
        AL_CLI_PRINTF("\n");
        return AL_CLI_OK;
    }
#elif RTOS_FREERTOS

#endif
#endif

    Cmd = AlCli_LookUpCmdByName(CliCmdInfo->CmdList, Argv[0], strlen(Argv[0]));
    if (Cmd == AL_NULL) {
        return AL_CLI_ERROR_CMD_NOT_EXIST;
    }

    Ret = Cmd->AlCli_CmdFun(Argc, Argv);
    if (Ret == AL_OK)
    {
        AL_CLI_PRINTF("\n");
        return AL_CLI_OK;
    }
}

static AL_S32 AlCli_SplitCmdBuf2Argv(AL_S8 *CmdBuf, AL_U32 CmdLen, AL_S8 *Argv[AL_CLI_MAX_ARGC_NUM])
{
    AL_S8 *Ptr;
    AL_U32 Position;
    AL_U32 Argc;
    AL_U32 Index;

    Ptr = CmdBuf;
    Position = 0;
    Argc = 0;

    while (Position < CmdLen) {
        /* strip bank and tab */
        while ((*Ptr == ' ' || *Ptr == '\t') && Position < CmdLen) {
            *Ptr = '\0';
            Ptr++;
            Position++;
        }

        if (Argc >= AL_CLI_MAX_ARGC_NUM) {
            AL_CLI_PRINTF("Too many args ! We only Use:\n");
            for (Index = 0; Index < Argc; Index++)
            {
                AL_CLI_PRINTF("%s ", Argv[Index]);
            }
            AL_CLI_PRINTF("\n");
            break;
        }

        if (Position >= CmdLen)
            break;

        /* handle string */
        if (*Ptr == '"') {
            Ptr++;
            Position++;
            Argv[Argc] = Ptr;
            Argc++;

            /* skip this string */
            while (*Ptr != '"' && Position < CmdLen) {
                if (*Ptr == '\\') {
                    if (*(Ptr + 1) == '"') {
                        Ptr++;
                        Position++;
                    }
                }
                Ptr++;
                Position++;
            }
            if (Position >= CmdLen)
                break;

            /* skip '"' */
            *Ptr = '\0';
            Ptr++;
            Position++;
        } else {
            Argv[Argc] = Ptr;
            Argc++;
            while ((*Ptr != ' ' && *Ptr != '\t') && Position < CmdLen) {
                Ptr++;
                Position++;
            }
            if (Position >= CmdLen)
                break;
        }
    }

    return Argc;
}

static AL_S32 AlCli_HandleCmd(AL_S8 *CmdBuf, AL_U32 CmdLen)
{
    AL_S32 Argc;
    AL_S8 *Argv[AL_CLI_MAX_ARGC_NUM];

    if (CmdBuf == AL_NULL || CmdLen < 1) {
        return AL_CLI_ERROR_ILLEGAL_PARAM;
    }

    Argc = AlCli_SplitCmdBuf2Argv(CmdBuf, CmdLen, Argv);
    if (Argc == 0) {
        return AL_CLI_ERROR_INVALID_CMD;
    }

    return AlCli_ProcessCmd(Argc, Argv);
}

static AL_S32 AlCli_ConoleReadChar(AL_S8 *Char)
{
    if (Char == AL_NULL)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    return AlCli_ConsoleRead(CliConsole->DevConsole, Char, 1);
}

/*
 * this function get a character by console and handle it
 */
static AL_S32 AlCli_GetCmd(AL_U8 *Buf, AL_U32 BufSize)
{
    AL_S8 Character;
    AL_S32 Index;

    if (Buf == AL_NULL || BufSize < 1) {
        return AL_CLI_ERROR_ILLEGAL_PARAM;
    }

    /* read a character until \r or \n */
    while (AlCli_ConoleReadChar(&Character) == 1) {
        if (CliCmdInfo->CmdEndPos >= BufSize) {
            AL_CLI_PRINTF("\r\nError: input buffer overflow\r\n");
            return AL_CLI_ERROR_BUF_OVERFLOW;
        }

        /* received null or error */
        if (Character == '\0' || Character == 0xFF) {
            continue;
        }

        /* handle end of line, break */
        if (Character == '\r' || Character == '\n') {
            CliCmdInfo->CliInputBuf[CliCmdInfo->CmdEndPos] = '\0';
            memcpy(Buf, CliCmdInfo->CliInputBuf, BufSize);
            memset(CliCmdInfo->CliInputBuf, 0, BufSize);
            CliCmdInfo->CmdCurPos = 0;
            CliCmdInfo->CmdEndPos = 0;
            return AL_CLI_OK;
        }

        /* handle backspace*/
        if (Character == 0x7f || Character == 0x08) {

            if (CliCmdInfo->CmdCurPos == 0)
                continue;

            CliCmdInfo->CmdCurPos--;
            CliCmdInfo->CmdEndPos--;

            if (CliCmdInfo->CmdEndPos >  CliCmdInfo->CmdCurPos)
            {
                int i;

                AL_CLI_MEMMOVE(&CliCmdInfo->CliInputBuf[CliCmdInfo->CmdCurPos],
                           &CliCmdInfo->CliInputBuf[CliCmdInfo->CmdCurPos + 1],
                           CliCmdInfo->CmdEndPos - CliCmdInfo->CmdCurPos);
                CliCmdInfo->CliInputBuf[CliCmdInfo->CmdEndPos] = 0;

                AL_CLI_PRINTF("\b%s  \b", &CliCmdInfo->CliInputBuf[CliCmdInfo->CmdCurPos]);

                for (i = CliCmdInfo->CmdCurPos; i <= CliCmdInfo->CmdEndPos; i++)
                    AL_CLI_PRINTF("\b");
            }
            else
            {
                AL_CLI_PRINTF("\b \b");
                CliCmdInfo->CliInputBuf[CliCmdInfo->CmdEndPos] = 0;
            }

            continue;
        }

        /* others: handle normal character */
        if (CliCmdInfo->CmdCurPos < CliCmdInfo->CmdEndPos) {
            memmove(&CliCmdInfo->CliInputBuf[CliCmdInfo->CmdCurPos + 1],
                    &CliCmdInfo->CliInputBuf[CliCmdInfo->CmdCurPos],
                    CliCmdInfo->CmdEndPos - CliCmdInfo->CmdCurPos);
            CliCmdInfo->CliInputBuf[CliCmdInfo->CmdEndPos] = Character;

            for (Index = CliCmdInfo->CmdCurPos; Index < CliCmdInfo->CmdEndPos; Index++) {
                AL_CLI_PRINTF("\b");
            }
        } else {
            CliCmdInfo->CliInputBuf[CliCmdInfo->CmdEndPos] = Character;
            if (CliCmdInfo->EchoEnable) {
                AL_CLI_PRINTF("%c", Character);
            }
        }

        /*
          Todo:
          1.Some special key handling
          2.history command
         */

        CliCmdInfo->CmdCurPos++;
        CliCmdInfo->CmdEndPos++;
        if (CliCmdInfo->CmdEndPos >= BufSize) {
            CliCmdInfo->CmdCurPos = 0;
            CliCmdInfo->CmdEndPos = 0;
        }

        Character = 0;
    }

    /* If reach here, it's probably a console read error */
    return AL_CLI_ERROR_CONSOLE_READ;
}

void AlCli_Main(void *data)
{
    AL_S32 RetValue;
    AL_S8  *Buf = CliConsole->CliConsoleBuf;
    AL_U32 BufSize = AL_CLI_BUF_SIZE;

    AL_CLI_PRINTF("\r\n");
    AL_CLI_PRINTF("   _           _                _       \r\n");
    AL_CLI_PRINTF("  /_\\   _ __  | |  ___    __ _ (_)  ___ \r\n");
    AL_CLI_PRINTF(" //_\\\\ | '_ \\ | | / _ \\  / _` || | / __|\r\n");
    AL_CLI_PRINTF("/  _  \\| | | || || (_) || (_| || || (__ \r\n");
    AL_CLI_PRINTF("\\_/ \\_/|_| |_||_| \\___/  \\__, ||_| \\___|\r\n");
    AL_CLI_PRINTF("                         |___/          \r\n");

    AL_CLI_PRINTF("%s", CliCmdInfo->prompt);

    while (1) {

        /* get a whole command and handle it */
        if (AlCli_GetCmd(Buf, BufSize) == AL_CLI_OK) {

            RetValue = AlCli_HandleCmd(Buf, strlen(Buf));
            switch (RetValue) {
                case AL_CLI_ERROR_INVALID_CMD:
                    AL_CLI_PRINTF("syntax error\r\n");
                    AL_CLI_PRINTF("%s", CliCmdInfo->prompt);
                    break;

                case AL_CLI_ERROR_CMD_NOT_EXIST:
                    AL_CLI_PRINTF("cmd not found\r\n");
                    AL_CLI_PRINTF("%s", CliCmdInfo->prompt);
                    break;

                default:
                    AL_CLI_PRINTF("%s", CliCmdInfo->prompt);
                    break;
            }

        }

    }
    AL_CLI_PRINTF("CLI exited\r\n");

    //Todo: clear something
}

static AL_S32 AlCli_InitCliCmd()
{
    AL_S32 RetValue;

    CliCmdInfo = (AL_CLI_CmdInfoStruct *)AL_CLI_CALLOC(1, sizeof(AL_CLI_CmdInfoStruct));
    if (CliCmdInfo == AL_NULL) {
        return AL_CLI_ERROR_MALLOC_FAILED;
    }

    CliCmdInfo->prompt = AL_CLI_PROMT;

#ifdef AL_CLI_ECHO_DISABLE
    CliCmdInfo->EchoEnable = 0;
#else
    CliCmdInfo->EchoEnable = 1;
#endif /* AL_CLI_ECHO_DISABLE */

    /* Init command link list head */
    RetValue = AlCli_InitCmdList(&(CliCmdInfo->CmdList));
    if (RetValue != AL_CLI_OK) {
        AL_CLI_FREE(CliCmdInfo);
        return RetValue;
    }

    /* register system default commands */
    RetValue = AlCli_RegisterDefaultCmds(CliCmdInfo->CmdList);
    if (RetValue != AL_CLI_OK) {
        AL_CLI_FREE(CliCmdInfo);
        return RetValue;
    }

    /* register user commands */
    RetValue = AlCli_RegisterUsrCmds(CliCmdInfo->CmdList);
    if (RetValue != AL_CLI_OK) {
        AL_CLI_FREE(CliCmdInfo);
        return RetValue;
    }

    return AL_CLI_OK;
}

static AL_S32 AlCli_InitCliConsole(AL_CLI_ConsoleTypeEnum ConsoleType)
{
    AL_S32 RetValue;

    CliConsole = AlCli_LookUpConsole(ConsoleType);
    if (CliConsole == AL_NULL) {
        return AL_CLI_ERROR_INVALID_CONSOLE;
    }

    /* Init device console */
    RetValue = AlCli_ConsoleInit(CliConsole->DevConsole);
    if (RetValue != AL_CLI_OK) {
        return RetValue;
    }

    memset(CliConsole->CliConsoleBuf, 0, AL_CLI_BUF_SIZE);

    return AL_CLI_OK;
}

AL_S32 AlCli_Init(AL_CLI_ConsoleTypeEnum ConsoleType)
{
    AL_S32 RetValue;

    /* init console by ConsoleType */
    RetValue = AlCli_InitCliConsole(ConsoleType);
    if (RetValue != AL_CLI_OK) {
        return RetValue;
    }

    /* register command */
    RetValue = AlCli_InitCliCmd();
    if (RetValue != AL_CLI_OK) {
        return RetValue;
    }

    return AL_CLI_OK;
}
