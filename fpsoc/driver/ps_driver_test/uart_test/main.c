#include "al_uart_test_config.h"

static AL_S32 AlUart_Test_SendDataPollingOnly(AL_VOID);
static AL_S32 AlUart_Test_SendDataBlockOnly(AL_VOID);
static AL_S32 AlUart_Test_RecvSendBlockLoopBack(AL_VOID);
static AL_S32 AlUart_Test_RecvSendNoBlockLoopBack(AL_VOID);

AL_S32 main(AL_VOID)
{
#if CONFIG_AlUart_Send_Polling_ONLY_TEST
    printf("AlUart Send Data Polling only example testing...\r\n");
    AL_S32 ret = AlUart_Test_SendDataPollingOnly();
    if (ret != AL_OK) {
        printf("AlUart Send Data Polling only example test error\r\n");
        return ret;
    }
#endif

#if CONFIG_AlUart_Send_Block_ONLY_TEST
    printf("AlUart Send Data Block only example testing...\r\n");
    AL_S32 ret = AlUart_Test_SendDataBlockOnly();
    if (ret != AL_OK) {
        printf("AlUart Send Data Block only example test error\r\n");
        return ret;
    }
#endif

#if CONFIG_AlUart_Recv_Send_Block_Loop_Back_TEST
    printf("AlUart Recv Send Data Block Loop Back example testing...\r\n");
    AL_S32 ret = AlUart_Test_RecvSendBlockLoopBack();
    if (ret != AL_OK) {
        printf("AlUart Recv Send Data Block Loop Back example test error\r\n");
        return ret;
    }
#endif

#if CONFIG_AlUart_Recv_Send_No_Block_Loop_Back_TEST
    printf("AlUart Recv Send Data No Block Loop Back example testing...\r\n");
    AL_S32 ret = AlUart_Test_RecvSendNoBlockLoopBack();
    if (ret != AL_OK) {
        printf("AlUart Recv Send Data No Block Loop Back example test error\r\n");
        return ret;
    }
#endif

    while (1);
}

static AL_S32 AlUart_Test_SendDataPollingOnly(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));
    for (int i = 0; i < BUF_SIZE; i++) {
        Data[i] = i + 'A';
    }

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 1, &UART_InitStruct, AL_NULL);
    if (ret != AL_OK) {
        printf("AlUart_Hal_Init error\r\n");
        return ret;
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        ret = AlUart_Hal_SendDataPolling(&uart0_hal, Data, BUF_SIZE);
        if (ret != AL_OK) {
            printf("AlUart_Hal_SendDataPolling Error\r\n");
            return ret;
        }
        printf("\r\n");
    }

}

static AL_S32 AlUart_Test_SendDataBlockOnly(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));
    for (int i = 0; i < BUF_SIZE; i++) {
        Data[i] = i + 'A';
    }

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 1, &UART_InitStruct, AL_NULL);
    if (ret != AL_OK){
        printf("AlUart_Hal_Init error\r\n");
        return ret;
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        ret = AlUart_Hal_SendDataBlock(&uart0_hal, Data, BUF_SIZE, UART_TIME_OUT_MS);
        if (ret != AL_OK) {
            printf("AlUart_Hal_SendDataBlock Error! ret: 0x%x\r\n", ret);
            return ret;
        }
        printf("\r\n");
    }

}

static AL_S32 AlUart_Test_RecvSendBlockLoopBack(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 1, &UART_InitStruct, AL_NULL);
    if (ret != AL_OK){
        printf("AlUart_Hal_Init error\r\n");
        return ret;
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    /* Block send recv show back test */
    printf("use Block send and recv\r\n");
    printf("send less than %d Bytes data and will show back\r\n", BUF_SIZE);

    while (1) {
        /* step 1. ues AlUart_Hal_RecvDataBlock to receve datas */
        ret = AlUart_Hal_RecvDataBlock(&uart0_hal, Data, BUF_SIZE, &RecvSize, UART_TIME_OUT_MS);
        if (ret != AL_OK) {
            switch ((AL_ERR_CODE)ret)
            {
            case AL_ERR_TIMEOUT:
            case AL_ERR_UNAVAILABLE:
            case AL_ERR_BUSY:
                break;

            case AL_ERR_NOMEM:
            case AL_ERR_NOT_READY:
            case AL_ERR_NULL_PTR:
            case AL_ERR_ILLEGAL_PARAM:
            case AL_ERR_NOT_SUPPORT:
                printf("AlUart_Hal_RecvDataBlock Error! ret: 0x%x\r\n", ret);
                return ret;

            default:
                printf("Unknown error type for AlUart_Hal_RecvDataBlock!!! ret: 0x%x\r\n", ret);
                return ret;
            }
        }

        /* step 2. ues AlUart_Hal_SendDataBlock to send datas */
        if (ret == AL_OK) {
            ret = AlUart_Hal_SendDataBlock(&uart0_hal, Data, RecvSize, UART_TIME_OUT_MS);
            if (ret != AL_OK) {
                printf("AlUart_Hal_SendDataBlock Error\r\n");
            }
        }

    }
}

static AL_S32 AlUart_Test_RecvSendNoBlockLoopBack(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 1, &UART_InitStruct, AL_NULL);
    if (ret != AL_OK){
        printf("AlUart_Hal_Init error\r\n");
        return ret;
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    /* no Block send recv show back test */
    printf("use No Block send and recv\r\n");
    printf("send less than %d Bytes data and will show back\r\n", BUF_SIZE);

    while (1) {
        /* step 1. ues AlUart_Hal_RecvData to receve datas */
        ret = AlUart_Hal_RecvData(&uart0_hal, Data, BUF_SIZE);
        if (ret != AL_OK) {
            printf("AlUart_Hal_RecvData Error\r\n");
            return ret;
        }

        /* step 2. Prevent screen brushing */
        while (AlUart_Dev_IsRxBusy(uart0_hal.Dev));

        /* step 3. ues AlUart_Hal_SendData to send datas */
        ret = AlUart_Hal_SendData(&uart0_hal, Data, uart0_hal.Dev->RecvBuffer.HandledCnt);
        if (ret != AL_OK) {
            printf("AlUart_Hal_SendData Error\r\n");
            return ret;
        }
        printf("\r\n");
    }
}
