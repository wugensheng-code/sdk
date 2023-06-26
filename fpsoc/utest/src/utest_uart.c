#include "al_hal.h"
#include "al_core.h"
#include "al_utest.h"
#include "al_cli.h"

#include <stdio.h>
#include <string.h>

/**
 * @brief  Io ctl cmd enum
 */
typedef enum
{
    AL_UART_IOCTL_SET_BAUD_RATE,
    AL_UART_IOCTL_GET_BAUD_RATE,
    AL_UART_IOCTL_SET_DATA_WIDTH,
    AL_UART_IOCTL_GET_DATA_WIDTH,
    AL_UART_IOCTL_SET_STOP_BITS,
    AL_UART_IOCTL_GET_STOP_BITS,
    AL_UART_IOCTL_SET_PARITY,
    AL_UART_IOCTL_GET_PARITY
} AL_Uart_IoCtlCmdEnum;

#define AL_UART_HAL_LOCK(Handle)        do {} while (0)
#define AL_UART_HAL_UNLOCK(Handle)      do {} while (0)

#if (LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1)
extern AL_UART_HalStruct Log;
#endif

/**
 * This function excute operations to set or get uart status
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   Cmd is io ctl operation to AL_Uart_IoCtlCmdEnum
 * @param   Data is pointer reference to Cmd
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
static AL_S32 AlUart_Dev_IoCtl(AL_UART_DevStruct *Uart, AL_Uart_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Uart == AL_NULL) {
        return AL_UART_ERR_NULL_PTR;
    }

    if(AlUart_ll_IsUartBusy(Uart->BaseAddr)){
        AL_LOG(AL_LOG_LEVEL_ERROR, "access during a transaction\r\n");
        return AL_UART_ERR_BUSY;
    }

    switch (Cmd)
    {
    case AL_UART_IOCTL_SET_BAUD_RATE: {
        AL_U32 BaudRate = *(AL_U32 *)Data;
        AlUart_ll_SetBaudRate(Uart->BaseAddr, BaudRate, Uart->InputClockHz);
        break;
    }
    case AL_UART_IOCTL_GET_BAUD_RATE: {
        AL_U32 *BaudRate = (AL_U32 *)Data;
        *BaudRate = AlUart_ll_GetBaudRate(Uart->BaseAddr, Uart->InputClockHz);
        break;
    }
    case AL_UART_IOCTL_SET_DATA_WIDTH: {
        AL_U32 DataWidth = *(AL_U32 *)Data;
        AlUart_ll_SetDataWidth(Uart->BaseAddr, DataWidth);
        break;
    }
    case AL_UART_IOCTL_GET_DATA_WIDTH: {
        AL_U32 *DataWidth = (AL_U32 *)Data;
        *DataWidth = AlUart_ll_GetDataWidth(Uart->BaseAddr);
        break;
    }
    case AL_UART_IOCTL_SET_STOP_BITS: {
        AL_U32 StopBits = *(AL_U32 *)Data;
        AlUart_ll_SetStopBitsLength(Uart->BaseAddr, StopBits);
        break;
    }
    case AL_UART_IOCTL_GET_STOP_BITS: {
        AL_U32 *StopBits = (AL_U32 *)Data;
        *StopBits = AlUart_ll_GetStopBitsLength(Uart->BaseAddr);
        break;
    }
    case AL_UART_IOCTL_SET_PARITY: {
        AL_U32 Parity = *(AL_U32 *)Data;
        AlUart_ll_SetParity(Uart->BaseAddr, Parity);
        break;
    }
    case AL_UART_IOCTL_GET_PARITY: {
        AL_U32 *Parity = (AL_U32 *)Data;
        *Parity = AlUart_ll_GetParity(Uart->BaseAddr);
        break;
    }
    default:
        return AL_UART_ERR_NOT_SUPPORT;
        break;
    }

    return Ret;
}


/**
 * This function excute operations to set or check uart status.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Cmd is io ctl cmd to AL_Uart_IoCtlCmdEnum
 * @param   Data Pointer to cmd args
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
static AL_S32 AlUart_Hal_IoCtl(AL_UART_HalStruct *Handle, AL_Uart_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_UART_ERR_NULL_PTR;
    }

    AL_UART_HAL_LOCK(Handle);

    Ret = AlUart_Dev_IoCtl(Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart io ctl cmd error:%d\r\n", Ret);
    }

    AL_UART_HAL_UNLOCK(Handle);

    return Ret;
}


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
