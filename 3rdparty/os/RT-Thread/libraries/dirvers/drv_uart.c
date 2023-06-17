#include "board.h"
#include <rtdevice.h>
#include "drv_uart.h"
#include "al_uart_hal.h"
#include <rtthread.h>

#define RT_USING_SERIAL_V2
#ifdef RT_USING_SERIAL_V2

#if !defined(BSP_USING_UART0) && !defined(BSP_USING_UART1)
    #error "Please define at least one BSP_USING_UARTx"
    /* this driver can be enabled at menuconfig -> 
    Hardware Drivers Config -> On-chip Peripheral Drivers -> Enable UART */
#endif

//#define DRV_DEBUG
#define DBG_TAG              "drv.usart"
#ifdef DRV_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif /* DRV_DEBUG */
#include <rtdbg.h>

#if !defined(BSP_USING_UART0) && !defined(BSP_USING_UART1)
    #error "Please define at least one BSP_USING_UARTx"
    /* this driver can be enabled at menuconfig -> 
    Hardware Drivers Config -> On-chip Peripheral Drivers -> Enable UART */
#endif

enum
{
#ifdef BSP_USING_UART0
    UART0_INDEX,
#endif

#ifdef BSP_USING_UART1
    UART1_INDEX,
#endif
};

static struct al_uart_config uart_config[] = {
#ifdef BSP_USING_UART0
    {
    .name = "uart0",
    .UART_InitStruct = {
        .BaudRate     = 115200,
        .Parity       = UART_NO_PARITY,
        .WordLength   = UART_CHAR_8BITS,
        .StopBits     = UART_STOP_1BIT,     
    },
    .DevId = 0,
    .IntrId = 89,
    },
#endif

#ifdef BSP_USING_UART1
    {
    .name = "uart1",
    .UART_InitStruct = {
        .BaudRate     = 115200,
        .Parity       = UART_NO_PARITY,
        .WordLength   = UART_CHAR_8BITS,
        .StopBits     = UART_STOP_1BIT,     
    },
    .DevId = 1,
    .IntrId = 90,
    },
#endif 
};

static struct al_uart uart_obj[sizeof(uart_config) / sizeof(uart_config[0])] = {0};

#define UART_IN_THRE_EMPTY_INTR(Status)             (Status == UART_THR_EMPTY)
#define UART_IN_BUSY_DETECT_INTR(Status)            (Status == UART_BUSY_DETECT)
#define UART_IN_RECV_AVAIL_INTR(Status)             (Status == UART_RECEIVED_DATA_AVAILABLE)
#define UART_IN_CHAR_RECEV_TIMEOUT(Status)          (Status == UART_CHARACTER_TIMEOUT)
#define UART_IN_RECV_LINE_STATUS_INTR(Status)       (Status == UART_RECEIVER_LINE_STATUS)


static AL_VOID RT_AlUart_Dev_BusBusyHandler(AL_UART_DevStruct *Uart)
{
#ifdef UART_DEBUG
    AL_LOG(AL_ERR_LEVEL_DEBUG,"uart bus busy, bus reset then reinitialize\r\n");
#endif
    /* soft reset uart bus */
    if (Uart->BaseAddr == UART0__BASE_ADDR) {
        AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART0__SRST__N__SHIFT, AL_FUNC_DISABLE);
        AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART0__SRST__N__SHIFT, AL_FUNC_ENABLE);
    } else {
        AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART1__SRST__N__SHIFT, AL_FUNC_DISABLE);
        AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART1__SRST__N__SHIFT, AL_FUNC_ENABLE);
    }
    /* Reinitialize with the original configuration of the uart port in question */
    AlUart_Dev_Init(Uart, &(Uart->Configs), Uart->DevId);
}

static AL_VOID RT_AlUart_Dev_RecvDataHandler(AL_UART_DevStruct *Uart, AL_BOOL Timeout)
{
    while ((AlUart_ll_IsRxDataReady(Uart->BaseAddr) == AL_TRUE) &&
        (Uart->RecvBuffer.HandledCnt < Uart->RecvBuffer.RequestedCnt)) {
        Uart->RecvBuffer.BufferPtr[Uart->RecvBuffer.HandledCnt] = (AL_U8)AlUart_ll_RecvByte(Uart->BaseAddr);
        Uart->RecvBuffer.HandledCnt ++;
    }
    if (Uart->RecvBuffer.HandledCnt == Uart->RecvBuffer.RequestedCnt || Timeout) {
        // AlUart_ll_SetRxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);
        if (Uart->EventCallBack) {
            AL_UART_EventStruct UartEvent = {
                .Event        = (AL_UART_RECEIVE_DONE | (Timeout ? AL_UART_RECEIVE_TIMEOUT : 0x0)),
                .EventData    = Uart->SendBuffer.HandledCnt
            };
            Uart->EventCallBack(UartEvent, Uart->EventCallBackRef);
        }
        /* Clear Status */
        AlUart_Dev_ClrRxBusy(Uart);
    }
}

static AL_VOID RT_AlUart_Dev_ErrorHandler(AL_UART_DevStruct *Uart, AL_UART_InterruptEnum IntrStatus)
{
    AL_UART_EventStruct UartEvent;

    if (AlUart_ll_IsOeIntr(Uart->BaseAddr)) {
        UartEvent.Event = AL_UART_EVENT_OVER_RUN_ERR;
        UartEvent.EventData = (AL_U32)(AL_UINTPTR)AL_NULL;
    }
    if (AlUart_ll_IsPeIntr(Uart->BaseAddr)) {
        UartEvent.Event = AL_UART_EVENT_PARITY_ERR;
        UartEvent.EventData = (AL_U32)(AL_UINTPTR)AL_NULL;
    }
    if (AlUart_ll_IsFeIntr(Uart->BaseAddr)) {
        UartEvent.Event = AL_UART_EVENT_FRAMING_ERR;
        UartEvent.EventData = (AL_U32)(AL_UINTPTR)AL_NULL;
    }
    if (AlUart_ll_IsBiIntr(Uart->BaseAddr)) {
        UartEvent.Event = AL_UART_EVENT_BREAK_INTR;
        UartEvent.EventData = (AL_U32)(AL_UINTPTR)AL_NULL;
    }
    /* trigger EventCallBack */
    if (Uart->EventCallBack) {
        (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
    }

    /* Clear Status */
    AlUart_Dev_ClrTxBusy(Uart);
}

static AL_VOID RT_AlUart_Dev_SendDataHandler(AL_UART_DevStruct *Uart)
{
    if (Uart->SendBuffer.HandledCnt == Uart->SendBuffer.RequestedCnt) {
        AlUart_ll_SetTxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);

        if (Uart->EventCallBack) {
            AL_UART_EventStruct event = {
                .Event         = AL_UART_SEND_DONE,
                .EventData     = Uart->SendBuffer.HandledCnt
            };
            (*Uart->EventCallBack)(event, Uart->EventCallBackRef);
        }
        /* Clear Status */
        AlUart_Dev_ClrTxBusy(Uart);
    } else {
        while (!(AlUart_ll_IsTransmitFifoFull(Uart->BaseAddr)) &&
            (Uart->SendBuffer.HandledCnt < Uart->SendBuffer.RequestedCnt)) {
            AlUart_ll_SendByte(Uart->BaseAddr, Uart->SendBuffer.BufferPtr[Uart->SendBuffer.HandledCnt]);
            // AlUart_ll_SendByte(Uart->BaseAddr, data);
            Uart->SendBuffer.HandledCnt ++;
        }
    }
}

static void uart_isr(struct rt_serial_device *serial)
{
    struct al_uart *uart;

    RT_ASSERT(serial != RT_NULL);


    uart = rt_container_of(serial, struct al_uart, serial);

    AL_UART_DevStruct *Uart_dev = uart->handle.Dev;

    AL_UART_InterruptEnum IntrStatus = AlUart_ll_GetIntrStatus(Uart_dev->BaseAddr);

    if (UART_IN_RECV_AVAIL_INTR(IntrStatus) || UART_IN_CHAR_RECEV_TIMEOUT(IntrStatus)) {

        struct rt_serial_rx_fifo *rx_fifo;
        rx_fifo = (struct rt_serial_rx_fifo *) serial->serial_rx;
        RT_ASSERT(rx_fifo != RT_NULL);
    

        RT_AlUart_Dev_RecvDataHandler(Uart_dev, UART_IN_CHAR_RECEV_TIMEOUT(IntrStatus));
        
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
    } else if (UART_IN_THRE_EMPTY_INTR(IntrStatus)) {

        RT_AlUart_Dev_SendDataHandler(Uart_dev);
   
    } else if (UART_IN_RECV_LINE_STATUS_INTR(IntrStatus)) {
        RT_AlUart_Dev_ErrorHandler(Uart_dev, IntrStatus);
    } else if (UART_IN_BUSY_DETECT_INTR(IntrStatus)) {
        RT_AlUart_Dev_BusBusyHandler(Uart_dev);
    } else ;

}

AL_VOID RT_AlUart_Dev_IntrHandler(AL_VOID *Instance)
{

    /* enter interrupt */
    rt_interrupt_enter();


    uart_isr(&(uart_obj[UART0_INDEX].serial));



    /* leave interrupt */
    rt_interrupt_leave();
}


static rt_err_t al_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct al_uart *uart;
    
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = rt_container_of(serial, struct al_uart, serial);

    if (AlUart_Hal_Init(&uart->handle, 0, &uart->config->UART_InitStruct) != AL_OK)
    {
        return -RT_ERROR;
    }

    (AL_VOID)AlIntr_RegHandler(uart->config->IntrId, AL_NULL, RT_AlUart_Dev_IntrHandler, &uart->config->UART_InitStruct);
    
    return RT_EOK;
}

static int al_putc(struct rt_serial_device *serial, char c)
{   
    struct al_uart *uart;
    RT_ASSERT(serial != RT_NULL);

    uart = rt_container_of(serial, struct al_uart, serial);

    while (AlUart_Hal_SendDataPolling(&uart->handle, &c, 1) != AL_OK);

    return 1;
    
}

static int al_getc(struct rt_serial_device *serial)
{
    int ch;
    struct al_uart *uart;
    AL_U32 RecvSize;

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct al_uart, serial);

    ch = -1;
    if (AlUart_ll_IsRxDataReady(uart->handle.Dev->BaseAddr) == AL_TRUE)
    {
        AlUart_Hal_RecvData(&uart->handle, &ch, 1);
    }
    
    return (int)ch;
}

static rt_size_t al_transmit(struct rt_serial_device        *serial,
                                       rt_uint8_t           *buf,
                                       rt_size_t             size,
                                       rt_uint32_t           tx_flag)
{
    struct al_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(buf != RT_NULL);

    uart = rt_container_of(serial, struct al_uart, serial);

    while (AlUart_Hal_SendDataPolling(&uart->handle, buf, size) != AL_OK);

    return size;
}

static rt_err_t al_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct al_uart *uart;
#ifdef RT_SERIAL_USING_DMA
    rt_ubase_t ctrl_arg = (rt_ubase_t)arg;
#endif

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct al_uart, serial);

    switch (cmd)
    {
    /* disable interrupt */
    case RT_DEVICE_CTRL_CLR_INT:
        ECLIC_DisableIRQ(uart->config->IntrId);
        AlUart_ll_SetLineIntr(uart->handle.Dev->BaseAddr, AL_FUNC_DISABLE);
        AlUart_ll_SetThreIntr(uart->handle.Dev->BaseAddr, AL_FUNC_DISABLE);
        AlUart_ll_SetTxIntr(uart->handle.Dev->BaseAddr, AL_FUNC_DISABLE);
        AlUart_ll_SetRxIntr(uart->handle.Dev->BaseAddr, AL_FUNC_DISABLE);

        break;

    /* enable interrupt */
    case RT_DEVICE_CTRL_SET_INT:
        ECLIC_EnableIRQ(uart->config->IntrId);
        AlUart_ll_SetLineIntr(uart->handle.Dev->BaseAddr, AL_FUNC_ENABLE);
        AlUart_ll_SetThreIntr(uart->handle.Dev->BaseAddr, AL_FUNC_ENABLE);
        AlUart_ll_SetTxIntr(uart->handle.Dev->BaseAddr, AL_FUNC_ENABLE);
        AlUart_ll_SetRxIntr(uart->handle.Dev->BaseAddr, AL_FUNC_ENABLE);


        break;

    }
    return RT_EOK;
}

static const struct rt_uart_ops al_uart_ops = 
{
    .configure = al_configure,
    .control = al_control,
    .putc = al_putc,
    .getc = al_getc,
    .dma_transmit = al_transmit,
};


int rt_hw_uart_init(void)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    rt_err_t result = 0;

    for (rt_size_t i = 0; i < sizeof(uart_obj) / sizeof(struct al_uart); i++)
    {
        /* init UART object */
        uart_obj[i].config = &uart_config[i];
        uart_obj[i].serial.ops = &al_uart_ops;
        uart_obj[i].serial.config = config;
        /* register UART device */
        result = rt_hw_serial_register(&uart_obj[i].serial,
                                        uart_obj[i].config->name,
                                       RT_DEVICE_FLAG_RDWR
                                       | RT_DEVICE_FLAG_INT_RX
                                       | RT_DEVICE_FLAG_INT_TX
                                       , NULL);
        RT_ASSERT(result == RT_EOK);
    }

    RT_ASSERT(result == RT_EOK);

    return result;
}

#endif