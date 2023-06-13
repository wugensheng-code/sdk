#include "al_uart_test_config.h"

static AL_S32 AlUart_Test_SendDataPollingOnly(AL_VOID);
static AL_S32 AlUart_Test_SendDataBlockOnly(AL_VOID);
static AL_S32 AlUart_Test_RecvSendBlockLoopBack(AL_VOID);
static AL_S32 AlUart_Test_RecvSendNoBlockLoopBack(AL_VOID);
static AL_S32 AlUart_Test_Ioctl(AL_VOID);

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

#if CONFIG_AlUart_Ioctl_TEST
    printf("AlUart Ioctl example testing...\r\n");
    AL_S32 ret = AlUart_Test_Ioctl();
    if (ret != AL_OK) {
        printf("AlUart Ioctl example test error\r\n");
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
        Data[i] = i;
    }

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 0, &UART_InitStruct);
    if (ret != AL_OK) {
        printf("AlUart_Hal_Init error\r\n");
        return ret;
    }

    while (1) {
        ret = AlUart_Hal_SendDataPolling(&uart0_hal, Data, BUF_SIZE);
        if (ret != AL_OK) {
		    printf("AlUart_Hal_SendDataPolling Error\r\n");
            return ret;
	    }
        printf("\r\n\r\n");
    }

}

static AL_S32 AlUart_Test_SendDataBlockOnly(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));
    for (int i = 0; i < BUF_SIZE; i++) {
        Data[i] = i;
    }

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 0, &UART_InitStruct);
    if (ret != AL_OK){
        printf("AlUart_Hal_Init error\r\n");
        return ret;
    }

    while (1) {
        ret = AlUart_Hal_SendDataBlock(&uart0_hal, Data, BUF_SIZE, TIME_OUT);
        if (ret != AL_OK) {
		    printf("AlUart_Hal_SendDataBlock Error\r\n");
            return ret;
        }
        printf("\r\n\r\n");
    }

}

static AL_S32 AlUart_Test_RecvSendBlockLoopBack(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 0, &UART_InitStruct);
    if (ret != AL_OK){
        printf("AlUart_Hal_Init error\r\n");
        return ret;
    }

    /* Block send recv show back test */
    printf("use Block send and recv\r\n");
    printf("send less than %d Bytes data and will show back\r\n", BUF_SIZE);

    while (1) {
        /* step 1. ues AlUart_Hal_RecvDataBlock to receve datas */
        ret = AlUart_Hal_RecvDataBlock(&uart0_hal, Data, BUF_SIZE, &RecvSize, TIME_OUT);
        if (ret != AL_OK) {
		    printf("AlUart_Hal_RecvDataBlock Error\r\n");
            return ret;
        }

        /* step 2. ues AlUart_Hal_SendDataBlock to send datas */
        ret = AlUart_Hal_SendDataBlock(&uart0_hal, Data, RecvSize, TIME_OUT);
        if (ret != AL_OK) {
		    printf("AlUart_Hal_SendDataBlock Error\r\n");
            return ret;
        }
        printf("\r\n");
    }
}

static AL_S32 AlUart_Test_RecvSendNoBlockLoopBack(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 0, &UART_InitStruct);
    if (ret != AL_OK){
        printf("AlUart_Hal_Init error\r\n");
        return ret;
    }

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

static AL_S32 AlUart_Test_Ioctl(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U32 IoCtl_Data;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 0, &UART_InitStruct);
    if (ret != AL_OK){
        printf("AlUart_Hal_Init error\r\n");
        return ret;
    }

    /* ioctl get current buadrate */
    ret = AlUart_Hal_IoCtl(&uart0_hal, AL_UART_IOCTL_GET_BAUD_RATE, &IoCtl_Data);
    if (ret != AL_OK){
        return ret;
    }
    printf("AlUart current baudrate = %d\r\n", IoCtl_Data);

    /* ioctl set buadrate */
    IoCtl_Data = 9600;
    printf("Set AlUart baudrate to = %d\r\n", IoCtl_Data);
    printf("Please change pc's baudrate to %d then send \"ok\" to board\r\n", IoCtl_Data);
    ret = AlUart_Hal_IoCtl(&uart0_hal, AL_UART_IOCTL_SET_BAUD_RATE, &IoCtl_Data);
    if (ret != AL_OK) {
        printf("AlUart_Hal_IoCtl error\r\n");
        return ret;
    }

    /* check user send 'o' 'k' */
    ret = AlUart_Hal_RecvDataBlock(&uart0_hal, Data, 2, &RecvSize, TIME_OUT);
    while (Data[0] != 'o' || Data[1] != 'k') {
        printf("Please send \"ok\" to board\r\n");
        ret = AlUart_Hal_RecvDataBlock(&uart0_hal, Data, 2, &RecvSize, TIME_OUT);
    }

    /* ioctl get current buadrate */
    ret = AlUart_Hal_IoCtl(&uart0_hal, AL_UART_IOCTL_GET_BAUD_RATE, &IoCtl_Data);
    if (ret != AL_OK) {
        printf("AlUart_Hal_IoCtl error\r\n");
        return ret;
    }
    printf("After set, current baudrate = %d\r\n", IoCtl_Data);

    /* ioctl get current DataWidth */
    ret = AlUart_Hal_IoCtl(&uart0_hal, AL_UART_IOCTL_GET_DATA_WIDTH, &IoCtl_Data);
    if (ret != AL_OK) {
        printf("AlUart_Hal_IoCtl error\r\n");
        return ret;
    }
    printf("AlUart DataWidth = 0x%02x\r\n", IoCtl_Data);

    /* ioctl get current StopBits */
    ret = AlUart_Hal_IoCtl(&uart0_hal, AL_UART_IOCTL_GET_STOP_BITS, &IoCtl_Data);
    if (ret != AL_OK) {
        printf("AlUart_Hal_IoCtl error\r\n");
        return ret;
    }
    printf("AlUart StopBits = 0x%02x\r\n", IoCtl_Data);

    /* ioctl get current Parity */
    ret = AlUart_Hal_IoCtl(&uart0_hal, AL_UART_IOCTL_GET_PARITY, &IoCtl_Data);
    if (ret != AL_OK) {
        printf("AlUart_Hal_IoCtl error\r\n");
        return ret;
    }
    printf("AlUart Parity = 0x%02x\r\n", IoCtl_Data);

    printf("AlUart Ioctl example test ok, done\r\n");

    return ret;
}
