#include "al_cli_cmd.h"
#include "al_cli_common.h"

#include <string.h>

static void AlCli_HelpCmd(AL_S32 Argc, AL_S8 **Argv);

static const AL_CLI_CmdStruct DefaultCmd[] = {
    {"help", "print this", AlCli_HelpCmd},
};

static AL_CLI_CmdListStruct *CliCmdList;

static void AlCli_HelpCmd(AL_S32 Argc, AL_S8 **Argv)
{
    AL_CLI_CmdListStruct *ListPtr = CliCmdList->Next;

    AL_CLI_PRINTF("\r\nAnLogic Cli Commands:\r\n");
    AL_CLI_PRINTF("\r\n");

    /* Display all command, show name and help */
    while (ListPtr != AL_NULL) {
        if (ListPtr->CliCmd && ListPtr->CliCmd->Name) {
            AL_CLI_PRINTF("  %-15s :%s\r\n", ListPtr->CliCmd->Name, ListPtr->CliCmd->Help ? ListPtr->CliCmd->Help : "");
        }
        ListPtr = ListPtr->Next;
    }
    AL_CLI_PRINTF("\r\n");
}

const AL_CLI_CmdStruct *AlCli_LookUpCmdByName(AL_CLI_CmdListStruct *List, AL_S8 *CmdName, AL_U32 CmdLen)
{
    AL_CLI_CmdListStruct *ListPtr = List->Next;
    AL_CLI_CmdStruct *Cmd = AL_NULL;

    if (List == AL_NULL || CmdName == AL_NULL || CmdLen < 1)
        return AL_NULL;

    while (ListPtr != AL_NULL) {

        /* Only support full match */
        if ((strlen(ListPtr->CliCmd->Name) == CmdLen) && !(strncmp(CmdName, ListPtr->CliCmd->Name, CmdLen))) {
            Cmd = ListPtr->CliCmd;
            break;
        }

        ListPtr = ListPtr->Next;
    }

    return Cmd;
}

AL_S32 AlCli_UnRegisterCmd(AL_CLI_CmdListStruct *List, const AL_CLI_CmdStruct *Cmd)
{
    AL_CLI_CmdListStruct *ListPtr = List;
    AL_CLI_CmdListStruct *ListNode;

    if (List == AL_NULL || Cmd->Name == AL_NULL)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    while (ListPtr->Next != AL_NULL) {

        if (!(strcmp(Cmd->Name, ListPtr->Next->CliCmd->Name))) {
            ListNode = ListPtr->Next->Next;
            AL_CLI_FREE(ListPtr->Next);
            ListPtr->Next = ListNode;
            return AL_CLI_OK;
        }

        ListPtr = ListPtr->Next;
    }

    return AL_CLI_ERROR_CMD_NOT_EXIST;
}

AL_S32 AlCli_RegisterCmd(AL_CLI_CmdListStruct *List, const AL_CLI_CmdStruct *Cmd)
{
    AL_CLI_CmdListStruct *ListNode;
    AL_CLI_CmdListStruct *ListPtr = List;

    if (List == AL_NULL || Cmd == AL_NULL || Cmd->Name == AL_NULL || Cmd->AlCli_CmdFun == AL_NULL)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    while (ListPtr->Next != AL_NULL) {
        ListPtr = ListPtr->Next;
    }

    ListNode = AL_CLI_CALLOC(1, sizeof(AL_CLI_CmdListStruct));
    if (ListNode == AL_NULL)
        return AL_CLI_ERROR_MALLOC_FAILED;

    ListNode->CliCmd = Cmd;
    ListNode->Next = AL_NULL;
    ListPtr->Next = ListNode;

    return AL_CLI_OK;
}

AL_S32 AlCli_RegisterCmds(AL_CLI_CmdListStruct *List, const AL_CLI_CmdStruct *Cmds, AL_U32 CmdNum)
{
    AL_S32 Index;
    AL_S32 RetValue;

    if (List == AL_NULL || Cmds == AL_NULL || CmdNum < 1)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    for (Index = 0; Index < CmdNum; Index++) {
        if ((RetValue = AlCli_RegisterCmd(List, &Cmds[Index])) != 0) {
            return RetValue;
        }
    }

    return AL_CLI_OK;
}

AL_S32 AlCli_RegisterDefaultCmds(AL_CLI_CmdListStruct *List)
{
    AL_S32 RetValue;

    if (List == AL_NULL)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    RetValue = AlCli_RegisterCmds(List, DefaultCmd, sizeof(DefaultCmd) / sizeof(AL_CLI_CmdStruct));
    if (RetValue != AL_CLI_OK) {
        return RetValue;
    }

    return AL_CLI_OK;
}

/* defined in the link script */
extern AL_U32 _cli_section_start, _cli_section_end;

AL_S32 AlCli_RegisterUsrCmds(AL_CLI_CmdListStruct *List)
{
    AL_S32 RetValue;
    AL_U32 Ptr;
    struct cli_region *UsrCmd;
    AL_CLI_CmdStruct *Cmd;

    if (List == AL_NULL)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    for (Ptr = (AL_U32)&_cli_section_start; Ptr < (AL_U32)&_cli_section_end;) {
        UsrCmd = (struct cli_region *)Ptr;
        Ptr += sizeof(struct cli_region);

        Cmd = (AL_CLI_CmdStruct *)AL_CLI_MALLOC(sizeof(AL_CLI_CmdStruct));
        if (Cmd == NULL) {
            return AL_CLI_ERROR_MALLOC_FAILED;
        }

        Cmd->Name = UsrCmd->Name;
        Cmd->Help = UsrCmd->Desc;
        Cmd->AlCli_CmdFun = (AlCli_CmdFunction)(UsrCmd->Func);

        RetValue = AlCli_RegisterCmd(List, Cmd);
        if (RetValue != AL_CLI_OK) {
            return RetValue;
        }
    }

    return AL_CLI_OK;
}

AL_S32 AlCli_InitCmdList(AL_CLI_CmdListStruct **List)
{
    if (List == AL_NULL)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    *List = AL_CLI_CALLOC(1, sizeof(AL_CLI_CmdListStruct));
    if (*List == AL_NULL)
        return AL_CLI_ERROR_MALLOC_FAILED;

    /* Record the header of the command list. The help command will be used */
    CliCmdList = *List;

    return AL_CLI_OK;
}