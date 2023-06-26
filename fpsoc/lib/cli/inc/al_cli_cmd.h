#ifndef AL_CLI_CMD_H
#define AL_CLI_CMD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

/* command function define */
typedef AL_S32 (*AlCli_CmdFunction)(AL_S32 argc, AL_S8 *argv[]);

typedef struct
{
    const AL_S8       *Name;
    const AL_S8       *Help;
    AlCli_CmdFunction AlCli_CmdFun;
} AL_CLI_CmdStruct;

typedef struct AL_CLI_CmdList
{
    AL_CLI_CmdStruct        *CliCmd;
    struct AL_CLI_CmdList   *Next;
} AL_CLI_CmdListStruct;

typedef AL_S8 (*AlCli_SectionFunc)(AL_S32 argc, AL_S8 **argv);

/* cli region table */
struct cli_region
{
    const char *Name;       /* the name of cli cmd*/
    const char *Desc;       /* description of cli cmd */
    AlCli_SectionFunc Func; /* the cli function */
};

#define SECTION(x) __attribute__((section(x)))
#define USED __attribute__((used))

#define AL_CLI_CMD_REGISTER(Name, Cmd, Desc)                             \
    const char __clisym_##Cmd##_Name[] SECTION(".rodata") = #Cmd;        \
    const char __clisym_##Cmd##_Desc[] SECTION(".rodata") = #Desc;       \
    static void Name##_stub(AL_S32 Argc, AL_S8 *Argv[])   \
    {                                                                    \
        Name(Argc, Argv);                                                \
    }                                                                    \
    USED const struct cli_region __clisym_##Cmd SECTION("cli_section") = \
        {                                                                \
            __clisym_##Cmd##_Name,                                       \
            __clisym_##Cmd##_Desc,                                       \
            (AlCli_SectionFunc)&Name##_stub};

AL_S32 AlCli_InitCmdList(AL_CLI_CmdListStruct **List);

AL_S32 AlCli_RegisterDefaultCmds(AL_CLI_CmdListStruct *List);

AL_S32 AlCli_RegisterUsrCmds(AL_CLI_CmdListStruct *List);

const AL_CLI_CmdStruct *AlCli_LookUpCmdByName(AL_CLI_CmdListStruct *List, AL_S8 *CmdName, AL_U32 CmdLen);

#ifdef __cplusplus
}
#endif

#endif /* AL_CLI_CMD_H */