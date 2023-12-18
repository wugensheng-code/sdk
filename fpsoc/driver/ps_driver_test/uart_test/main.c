/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_uart_test_config.h"
#include "al_uart_hal.h"

static AL_S32 AlUart_Test_SendDataPolling(AL_VOID);
static AL_S32 AlUart_Test_SendDataBlock(AL_VOID);
static AL_S32 AlUart_Test_RecvAndSendBlock(AL_VOID);
static AL_S32 AlUart_Test_RecvAndSendNonBlock(AL_VOID);
static AL_S32 AlUart_Test_UartAutoFlowControl(AL_VOID);
static AL_S32 AlUart_Test_SendDmaBlock(AL_VOID);
static AL_S32 AlUart_Test_RecvDmaBlock(AL_VOID);
static AL_S32 AlUart_Test_Loopback(AL_VOID);

AL_S32 main(AL_VOID)
{
#if CONFIG_AlUart_Test_SendDataPolling_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AlUart Send Data Polling only example testing...\r\n");
    AL_S32 Ret = AlUart_Test_SendDataPolling();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlUart Send Data Polling only example test error\r\n");
        return Ret;
    }
#endif

#if CONFIG_AlUart_Test_SendDataBlock_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AlUart Send Data Block only example testing...\r\n");
    AL_S32 Ret = AlUart_Test_SendDataBlock();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlUart Send Data Block only example test error\r\n");
        return Ret;
    }
#endif

#if CONFIG_AlUart_Test_RecvAndSendBlock_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AlUart Recv Send Data Block Loop Back example testing...\r\n");
    AL_S32 Ret = AlUart_Test_RecvAndSendBlock();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlUart Recv Send Data Block Loop Back example test error\r\n");
        return Ret;
    }
#endif

#if CONFIG_AlUart_Test_RecvAndSendNonBlock_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AlUart Recv Send Data No Block Loop Back example testing...\r\n");
    AL_S32 Ret = AlUart_Test_RecvAndSendNonBlock();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlUart Recv Send Data No Block Loop Back example test error\r\n");
        return Ret;
    }
#endif

#if CONFIG_AlUart_Test_UartAutoFlowControl_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "Use uart0 and uart1 for auto flow central testing...\r\n");
    AL_S32 Ret = AlUart_Test_UartAutoFlowControl();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart AutoFlowControl test error\r\n");
        return Ret;
    }
#endif

#if CONFIG_AlUart_Test_SendDmaBlock_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AlUart Send Data DMA Block only example testing...\r\n");
    AL_S32 Ret = AlUart_Test_SendDmaBlock();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart Send Data DMA Block test error\r\n");
        return Ret;
    }
#endif

#if CONFIG_AlUart_Test_RecvDmaBlock_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AlUart Recv Data DMA Block only example testing...\r\n");
    AL_S32 Ret = AlUart_Test_RecvDmaBlock();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart Recv Data DMA Block test error\r\n");
        return Ret;
    }
#endif

#if CONFIG_AlUart_Test_Loopback_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AlUart Loopback testing...\r\n");
    AL_S32 Ret = AlUart_Test_Loopback();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlUart Loopback testing error\r\n");
        return Ret;
    }
#endif
    while (1);
}

static AL_S32 AlUart_Test_SendDataPolling(AL_VOID)
{
    AL_UART_HalStruct *UartHandle;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));
    for (int i = 0; i < BUF_SIZE; i++) {
        Data[i] = i + 'A';
    }

    AL_S32 Ret = AlUart_Hal_Init(&UartHandle, AL_UART_DEVID, &UART_InitStruct, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlUart_Hal_Init error\r\n");
        return Ret;
    }

    while (1) {
        Ret = AlUart_Hal_SendDataPolling(UartHandle, Data, BUF_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlUart_Hal_SendDataPolling Error\r\n");
            return Ret;
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "\r\n");
    }

}

static AL_S32 AlUart_Test_SendDataBlock(AL_VOID)
{
    AL_UART_HalStruct *UartHandle;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));
    for (int i = 0; i < BUF_SIZE; i++) {
        Data[i] = i + 'A';
    }

    AL_S32 Ret = AlUart_Hal_Init(&UartHandle, AL_UART_DEVID, &UART_InitStruct, AL_NULL);
    if (Ret != AL_OK){
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlUart_Hal_Init error\r\n");
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        Ret = AlUart_Hal_SendDataBlock(UartHandle, Data, BUF_SIZE, AL_UART_TIME_OUT_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlUart_Hal_SendDataBlock Error! Ret: 0x%x\r\n", Ret);
            return Ret;
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "\r\n");
    }

}

static AL_S32 AlUart_Test_RecvAndSendBlock(AL_VOID)
{
    AL_UART_HalStruct *UartHandle;
    AL_U32 RealSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));

    AL_S32 Ret = AlUart_Hal_Init(&UartHandle, AL_UART_DEVID, &UART_InitStruct, AL_NULL);
    if (Ret != AL_OK){
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Send less than %d Bytes data and will show back\r\n", BUF_SIZE);
    while (1) {
        Ret = AlUart_Hal_RecvDataBlock(UartHandle, Data, BUF_SIZE, &RealSize, AL_UART_TIME_OUT_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Uart receive data timeout or less that %d Bytes data\r\n", BUF_SIZE);
            return Ret;
        }

        if (Ret == AL_OK) {
            Ret = AlUart_Hal_SendDataBlock(UartHandle, Data, RealSize, AL_UART_TIME_OUT_MS);
            if (Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Uart send data error\r\n");
                return Ret;
            } else {
                return AL_OK;
            }
        }
    }
}

static AL_S32 AlUart_Test_RecvAndSendNonBlock(AL_VOID)
{
    AL_UART_HalStruct *UartHandle;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));

    AL_S32 Ret = AlUart_Hal_Init(&UartHandle, AL_UART_DEVID, &UART_InitStruct, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Send less than %d Bytes data and will show back\r\n", BUF_SIZE);

    while (1) {
        Ret = AlUart_Hal_RecvData(UartHandle, Data, BUF_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Uart receive data error\r\n");
            return Ret;
        }

        while (AlUart_Dev_IsRxBusy(&UartHandle->Dev));

        Ret = AlUart_Hal_SendData(UartHandle, Data, UartHandle->Dev.RecvBuffer.HandledCnt);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Uart send data error\r\n");
            return Ret;
        }

        while (AlUart_Dev_IsTxBusy(&UartHandle->Dev));

        return AL_OK;
    }

}

static AL_S32 AlUart_Test_UartAutoFlowControl(AL_VOID)
{
    AL_UART_HalStruct *UartHandle0;
    AL_UART_HalStruct *UartHandle1;
    AL_U32 Index;

    AL_UART_InitStruct UART_InitStruct_Auto = {
        .BaudRate           = 115200,
        .Parity             = AL_UART_NO_PARITY,
        .WordLength         = AL_UART_CHAR_8BITS,
        .StopBits           = AL_UART_STOP_1BIT,
        .CharTimeoutEnable  = AL_TRUE,
        .HwFlowCtl          = AL_TRUE,
    };

    AL_GPIO_HalStruct *GPIO;

    AlGpio_Hal_Init(&GPIO, 0, AL_NULL);

    for (Index = 0; Index < BUF_SIZE; Index++) {
        SendBuffer[Index] = '0' + Index;
        RecvBuffer[Index] = 0;
    }

    AlUart_Hal_Init(&UartHandle0, 0, &UART_InitStruct_Auto, AL_NULL);
    AlUart_Hal_Init(&UartHandle1, 1, &UART_InitStruct_Auto, AL_NULL);


    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlUart_Hal_SendDataPolling(UartHandle0, SendBuffer, BUF_SIZE);
    AlUart_Hal_RecvDataPolling(UartHandle1, RecvBuffer, BUF_SIZE);


    /* If PS-LED is lighting, uart0 and uart1 AutoFlowControl test is success */
    for (Index = 0; Index < BUF_SIZE; Index++) {
        if (SendBuffer[Index] != RecvBuffer[Index]) {
            return -1;
        } else {
            AlGpio_Hal_WritePin(GPIO, 14, 0x1);
        }
    }

    return 0;
}

static AL_S32 AlUart_Test_SendDmaBlock(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U8 CACHE_LINE_ALIGN DmaSendData[1];
    AL_UART_HalStruct *UartHandle;

    memset((AL_VOID *)DmaSendData, 'A', 1);

    Ret = AlUart_Hal_Init(&UartHandle, AL_UART_DEVID, &UART_InitStruct, AL_NULL);
    if (Ret != AL_OK){
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    #ifdef ENABLE_MMU
    AlCache_FlushDcacheRange((AL_UINTPTR)DmaSendData, (AL_UINTPTR)(DmaSendData + 1));
    #endif

    Ret = AlUart_Hal_SendDataDmaBlock(UartHandle, DmaSendData, 1, 10000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart Send Data Dma Block error:0x%x\r\n", Ret);
        return Ret;
    }

    return AL_OK;
}

static AL_S32 AlUart_Test_RecvDmaBlock(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U8 CACHE_LINE_ALIGN DmaRecvData[32];
    AL_UART_HalStruct *UartHandle;

    Ret = AlUart_Hal_Init(&UartHandle, AL_UART_DEVID, &UART_InitStruct, AL_NULL);
    if (Ret != AL_OK){
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    Ret = AlUart_Hal_RecvDataDmaBlock(UartHandle, DmaRecvData, 32, 10000000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart Recv Data Dma Block error:0x%x\r\n", Ret);
        return Ret;
    }

#ifdef ENABLE_MMU
    AlCache_InvalidateDcacheRange((AL_UINTPTR)DmaRecvData, (AL_UINTPTR)(DmaRecvData + 32));
#endif

    AL_LOG(AL_LOG_LEVEL_INFO, "memory is %s", DmaRecvData);

    return AL_OK;
}

static AL_S32 AlUart_Test_Loopback(AL_VOID)
{
    volatile AL_UART_HalStruct *uart0_hal;
    AL_U16 Index;
    AL_U32 TEST_BUF_SIZE = 32;
    AL_UART_IoctlParamUnion UartIoctlParam = {0};
    AL_U8 SendBuffer[TEST_BUF_SIZE];
    AL_U8 RecvBuffer[TEST_BUF_SIZE];

    AL_S32 ret = AlUart_Hal_Init((AL_UART_HalStruct **)&uart0_hal, 0, &UART_InitStruct, AL_NULL);
    if(ret!= AL_OK) {
        printf("AlUart_Hal_Init error\r\n");
        return ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    UartIoctlParam.LoopBack = AL_TRUE;
    AlUart_Hal_IoCtl((AL_UART_HalStruct *)uart0_hal, AL_UART_IOCTL_SET_LOOPBACK, &UartIoctlParam);

    for(Index = 0; Index < TEST_BUF_SIZE; Index++){
        SendBuffer[Index] = '0' + Index;
        RecvBuffer[Index] = 0;
    }

    ret = AlUart_Hal_RecvData((AL_UART_HalStruct *)uart0_hal, RecvBuffer, TEST_BUF_SIZE);
    if(ret!= AL_OK) {
        printf("AlUart_Hal_RecvData error\r\n");
        return ret;
    }

    ret = AlUart_Hal_SendData((AL_UART_HalStruct *)uart0_hal, SendBuffer, TEST_BUF_SIZE);
    if(ret!= AL_OK) {
        printf("AlUart_Hal_SendData error\r\n");
        return ret;
    }

	while (1) {
		if (uart0_hal->Dev.RecvBuffer.HandledCnt == TEST_BUF_SIZE &&
            uart0_hal->Dev.SendBuffer.HandledCnt == TEST_BUF_SIZE) {
                break;
        }
	}

    for(Index = 0; Index < TEST_BUF_SIZE; Index++){
        if(RecvBuffer[Index]!= SendBuffer[Index]){
            printf("Uart Intr Example error\r\n");
            return -1;
        }
    }

    UartIoctlParam.LoopBack = AL_FALSE;
    AlUart_Hal_IoCtl((AL_UART_HalStruct *)uart0_hal, AL_UART_IOCTL_SET_LOOPBACK, &UartIoctlParam);
    return AL_OK;
}