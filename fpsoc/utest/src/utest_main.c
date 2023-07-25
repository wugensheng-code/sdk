#include "al_hal.h"
#include "al_cli.h"

AL_U32 main(AL_VOID)
{
    AlCli_Init(AL_CLI_CONSOLE_UART);
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);
    AlCli_Main(AL_NULL);

    return 0;
}
