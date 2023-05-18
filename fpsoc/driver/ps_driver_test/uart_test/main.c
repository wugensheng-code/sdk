#include "al_uart_test_config.h"

static AL_VOID AlUart_Test_SendDataPollingOnly(AL_VOID);
static AL_VOID AlUart_Test_SendDataBlockOnly(AL_VOID);
static AL_VOID AlUart_Test_RecvSendLoopBack(AL_VOID);

AL_U32 main(AL_VOID)
{
#if CONFIG_AlUart_Send_Polling_ONLY_TEST
    printf("AlUart Send Data Polling only example testing...\r\n");
    AlUart_Test_SendDataPollingOnly();
#endif

#if CONFIG_AlUart_Send_Block_ONLY_TEST
    printf("AlUart Send Data Block only example testing...\r\n");
    AlUart_Test_SendDataBlockOnly();
#endif

#if CONFIG_AlUart_Recv_Send_Loop_Back_TEST
    printf("AlUart Recv Send Data Loop Back example testing...\r\n");
    AlUart_Test_RecvSendLoopBack();
#endif
    while (1);
}

static AL_VOID AlUart_Test_SendDataPollingOnly(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    for(int i = 0; i < BUF_SIZE; i++){
        Data[i] = i;
    }


    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 0, &UART_InitStruct);
    if (ret != AL_OK){
        printf("AlUart_Hal_Init error!!!\r\n");
        return ret;
    }

    while(1)
    {
        ret = AlUart_Hal_SendDataPolling(&uart0_hal, Data, BUF_SIZE);
    	if (ret) {
		    printf("Send Data Polling only example Error!!!\r\n");
            return ret;
	    }
    }
    
}

static AL_VOID AlUart_Test_SendDataBlockOnly(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    for(int i = 0; i < BUF_SIZE; i++){
        Data[i] = i;
    }

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 0, &UART_InitStruct);
    if (ret != AL_OK){
        printf("AlUart_Hal_Init error!!!\r\n");
        return ret;
    }

    while(1)
    {
        ret = AlUart_Hal_SendDataBlock(&uart0_hal, Data, BUF_SIZE, TIME_OUT);
	    if (ret) {
		    printf("AlUart Send Data Block only example Error!!!\r\n");
            return ret;
	    }
    }
    
}

static AL_VOID AlUart_Test_RecvSendLoopBack(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(Data) * BUF_SIZE));

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 0, &UART_InitStruct);
    if (ret != AL_OK){
        printf("AlUart_Hal_Init error!!!\r\n");
        return ret;
    }

    while (1) {
        ret = AlUart_Hal_RecvDataBlock(&uart0_hal, Data, 5, &RecvSize, TIME_OUT);
        ret = AlUart_Hal_SendDataBlock(&uart0_hal, Data, RecvSize, TIME_OUT);
        if (ret) {
		    printf("AlUart Recv Send Data Loop Back example Error!!!\r\n");
            return ret;
        }
        printf("\r\n");
    }

}
