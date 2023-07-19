#include "al_hal.h"
#include "al_core.h"
#include "al_utest.h"
#include "al_cli.h"

#include <stdio.h>
#include <string.h>


#if (LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1)
extern AL_UART_HalStruct Log;
#endif


AL_S32 AlUart_UnitTest(AL_U32 Argc, AL_S8 * Argv[])
{
    AL_S32 Ret = AL_OK;
    AL_UTEST_CMDENUM Id = (AL_UTEST_CMDENUM)atoi(Argv[0]);

    switch (Id) {
    case UTEST_UART_IOCTL: {
        AL_U32 SubId = atoi(Argv[1]);
        Ret = AlUart_Hal_IoCtl(&Log, SubId, Argv[2]);
    }
    break;

    case UTEST_UART_SENDBLOCK: {
        Ret = AlUart_Hal_SendDataBlock(&Log, (AL_U8 *)atoi(Argv[1]), (AL_U32)atoi(Argv[2]), (AL_U32)atoi(Argv[3]));
    }
    break;

    default:
    break;
    }

    return Ret;
}

AL_CLI_CMD_REGISTER(AlUart_UnitTest, unityuart, unity-uart);
