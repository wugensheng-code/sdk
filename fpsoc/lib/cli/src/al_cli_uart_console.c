#include "al_cli_uart_console.h"
#include "al_cli_console.h"
#include "al_cli_common.h"

#include "al_uart_hal.h"
#include "al_log.h"

AL_CLI_CircularQueueStruct UartQueue;

AL_U8 UartRecvBuffer[AL_CLI_UART_BUFFER_MAX_SIZE];

extern AL_UART_HalStruct Log;

AL_S32 AlCli_CircularQueueInit(AL_CLI_CircularQueueStruct *Queue)
{
    if (Queue == AL_NULL)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    Queue->Base = (QueueElemType *)AL_CLI_MALLOC((AL_CLI_QUEUE_MAX_SIZE) * sizeof(QueueElemType));

    if (Queue->Base == AL_NULL)
        return AL_CLI_ERROR_MALLOC_FAILED;

    Queue->Front = Queue->Rear = 0;

    return AL_CLI_OK;
}

AL_S32 AlCli_CircularQueueSend(AL_CLI_CircularQueueStruct *Queue, QueueElemType Elem)
{
    if (Queue == AL_NULL)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    AL_DISABLE_INTERRUPT();

    if ((Queue->Rear + 1) % AL_CLI_QUEUE_MAX_SIZE == Queue->Front) {
        AL_ENABLE_INTERRUPT();
        return AL_CLI_ERROR_UART_QUEUE_SEND_FAILED;
    }

    Queue->Base[Queue->Rear] = Elem;
    Queue->Rear= (Queue->Rear+ 1) % AL_CLI_QUEUE_MAX_SIZE;

    AL_ENABLE_INTERRUPT();

    return AL_CLI_OK;
}

AL_S32 AlCli_CircularQueueRecv(AL_CLI_CircularQueueStruct *Queue, QueueElemType *Elem)
{
    if (Queue == AL_NULL)
        return AL_CLI_ERROR_ILLEGAL_PARAM;

    AL_DISABLE_INTERRUPT();

    if (Queue->Front == Queue->Rear) {
        AL_ENABLE_INTERRUPT();
        return AL_CLI_ERROR_UART_QUEUE_RECV_FAILED;
    }

    *Elem = Queue->Base[Queue->Front];

    Queue->Front = (Queue->Front + 1) % AL_CLI_QUEUE_MAX_SIZE;

    AL_ENABLE_INTERRUPT();

    return AL_CLI_OK;
}

AL_S32 AlCli_UartConsoleInit()
{
    /*
      Uart console use uart1, uart1 has been initialized during log initialization
     */

    return AL_CLI_OK;
}

AL_S32 AlCli_UartConsoleDeInit()
{
    // ToDo

    return AL_CLI_OK;
}

AL_S32 AlCli_UartRead(AL_VOID *Buf, AL_U32 Len, AL_U32 *RecvSize)
{
    AL_S32 RetValue;

    if (Buf == AL_NULL || Len < 1 || RecvSize == AL_NULL) {
        return AL_CLI_ERROR_ILLEGAL_PARAM;
    }

    RetValue = AlUart_Hal_RecvDataBlock(&Log, Buf, Len, RecvSize, AL_WAITFOREVER);
    if (RetValue != AL_OK)
        return RetValue;

    return AL_CLI_OK;
}

AL_S32 AlCli_UartConsoleRead(AL_VOID *Buf, AL_U32 Len)
{
    AL_S32 RetValue;
    AL_U32 RecvSize = 0;

    RetValue = AlCli_UartRead(Buf, Len, &RecvSize);
    if ((RetValue == AL_CLI_OK) && (RecvSize == Len)) {
        return RecvSize;
    }

    return AL_CLI_OK;
}

AL_S32 AlCli_UartConsoleWrite(const AL_VOID *Buf, AL_U32 Len)
{
    AL_S32 RetValue;

    RetValue = AlLog_Write(Buf, Len);
    if (RetValue != AL_CLI_OK) {
        return AL_CLI_ERROR_UART_CONSOLE_WRITE_FAILED;
    }

    return AL_CLI_OK;
}
