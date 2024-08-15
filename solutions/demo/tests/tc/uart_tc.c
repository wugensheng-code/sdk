#include <common.h>

#define BUF_SIZE    0x12
#define AL_UART_TIME_OUT_MS 10000
#define AL_UART_DEVID 1

AL_U8 SendBuffer[BUF_SIZE];	/* Buffer for Transmitting Data */
AL_U8 RecvBuffer[BUF_SIZE];	/* Buffer for Receiving Data */

static AL_UART_InitStruct UART_InitStruct = {
        .BaudRate           = 115200,
        .Parity             = AL_UART_NO_PARITY,
        .WordLength         = AL_UART_CHAR_8BITS,
        .StopBits           = AL_UART_STOP_1BIT,
        .HwFlowCtl          = AL_FALSE,
        .CharTimeoutEnable  = AL_TRUE
    };


static int AlUart_Test_Universal_Setup(void **state) {
    AL_UART_HalStruct *UartHandle;

    AL_S32 Ret = AlUart_Hal_Init(&UartHandle, AL_UART_DEVID, &UART_InitStruct, AL_NULL);

    *state = UartHandle;

    assert_int_equal(Ret, AL_OK);
}

/* A test case that does nothing and succeeds. */
static void AlUart_SendDataPolling_Tc(void **state) {

    AL_UART_HalStruct *UartHandle = (AL_UART_HalStruct *)*state;
    AL_U32 RecvSize;
    TickType_t Start = xTaskGetTickCount();

    AL_U8 *Data = (AL_U8 *)pvPortMalloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));
    for (int i = 0; i < BUF_SIZE-2; i++) {
        Data[i] = i + 'A';
    }

    Data[BUF_SIZE-2] = '\r';
    Data[BUF_SIZE-1] = '\n';

    AL_S32 Ret = AlUart_Hal_Init(&UartHandle, AL_UART_DEVID, &UART_InitStruct, AL_NULL);
    assert_int_equal(Ret, AL_OK);

    while ((xTaskGetTickCount() - Start) < TEST_TIME)
    {
        Ret = AlUart_Hal_SendDataPolling(UartHandle, Data, BUF_SIZE);
        assert_int_equal(Ret, AL_OK);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void AlUart_SendDataBlock_Tc(void **state) {

    AL_UART_HalStruct *UartHandle = (AL_UART_HalStruct *)*state;

    AL_U8 *Data = (AL_U8 *)pvPortMalloc(BUF_SIZE);
    TickType_t Start = xTaskGetTickCount();

    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));
    for (int i = 0; i < BUF_SIZE-2; i++) {
        Data[i] = i + 'A';
    }

    Data[BUF_SIZE-2] = '\r';
    Data[BUF_SIZE-1] = '\n';

    AL_S32 Ret = AlUart_Hal_Init(&UartHandle, AL_UART_DEVID, &UART_InitStruct, AL_NULL);
    assert_int_equal(Ret, AL_OK);

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while ((xTaskGetTickCount() - Start) < TEST_TIME)
    {
        Ret = AlUart_Hal_SendDataBlock(UartHandle, Data, BUF_SIZE, AL_UART_TIME_OUT_MS);
        assert_int_equal(Ret, AL_OK);
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    return;
}

CMOCKA_TC_DEFINE(AlUart_SendDataPolling_Tc, AlUart_Test_Universal_Setup, NULL, NULL);

CMOCKA_TC_DEFINE(AlUart_SendDataBlock_Tc, AlUart_Test_Universal_Setup, NULL, NULL);

