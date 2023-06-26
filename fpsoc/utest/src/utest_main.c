#include "al_hal.h"
#include "al_cli.h"

AL_U32 main(AL_VOID)
{
    AlCli_Init(AL_CLI_CONSOLE_UART);

    AlCli_Main(AL_NULL);

    return 0;
}
