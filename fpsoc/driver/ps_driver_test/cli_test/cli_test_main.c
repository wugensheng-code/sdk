
#include "al_cli.h"
#include "unity_test.h"

#include <stdio.h>

int UartTxSetUp()
{
    printf("\r\nUartTxSetUp\r\n");
}

int UartTxTearDown()
{
    printf("\r\nUartTxTearDown\r\n");
}

void UartTxTest()
{
    printf("\r\nUartTxTest\r\n");
}

int UartRxSetUp()
{
    printf("\r\nUartRxSetUp\r\n");
}

int UartRxTearDown()
{
    printf("\r\nUartRxTearDown\r\n");
}

void UartRxTest()
{
    printf("\r\nUartRxTest\r\n");
}

UNITY_TESTCASE_REGISTER(UartTxTest, "UartTxTest", UartTxSetUp, UartTxTearDown);
UNITY_TESTCASE_REGISTER(UartRxTest, "UartRxTest", UartRxSetUp, UartRxTearDown);

int AlCli_UsrHello(int argc, char **argv)
{
    AL_CLI_PRINTF("\r\nhello world\r\n");
}

int AlCli_UsrNiHao(int argc, char **argv)
{
    AL_CLI_PRINTF("\r\nnihao, shijie\r\n");
}

AL_CLI_CMD_REGISTER(AlCli_UsrHello, hello, printf hello world);
AL_CLI_CMD_REGISTER(AlCli_UsrNiHao, nihao , printf nihao shijie);

int main()
{
    AlCli_Init(AL_CLI_CONSOLE_UART);

    UityTestInit();

    AlCli_Main(AL_NULL);

    while(1);

    return 0;
}