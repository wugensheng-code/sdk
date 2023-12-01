/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <rtthread.h>
#include "rtthread_demo_config.h"
#include "lwip/tcpip.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the rt_tick_from_millisecond() macro. */
#define mainSOFTWARE_TIMER_PERIOD_MS    rt_tick_from_millisecond(1000)
#define TASKDLYMS                       rt_tick_from_millisecond(100)
#define mainQUEUE_LENGTH                (1)

static void prvSetupHardware(void);
extern void idle_task(void);

static rt_thread_t Task1Led_Handler;
static rt_thread_t Task2Can_Send_Handler;
static rt_thread_t Task3Can_DmaRecv_Handler;
static rt_thread_t Task4Wdt_Handler;
static rt_thread_t Task5Dmacahb_Handler;
static rt_thread_t Task6Iic_Handler;
static rt_thread_t Task7Uart_Handler;
// static rt_thread_t Task8Usb_Handler;
// static rt_thread_t Task9Gbe_Handler;
static rt_thread_t Task10Mmc_Handler;
static rt_thread_t Task11Qspi_Handler;
static rt_thread_t Task12Iis_Handler;
static rt_thread_t Task13Dma_Handler;
static rt_thread_t Task14Dmacahb2_Handler;
static rt_thread_t Task15UartProc_Handler;

static AL_Lock Lock_CanInit;
static AL_Lock Lock_DmaInit;
static AL_Lock Lock_DmacahbInit;
AL_Lock Lock_Log;
static AL_MailBox MailBox_CanRecv;
static rt_mq_t Queue_UartRecv;

extern struct rt_thread *rt_current_thread;

AL_U32 rtthreadRunTimeTicks;

void prvSetupHardware(void)
{

}

void task1_led(void* pvParameters);
void task2_can_send(void* pvParameters);
void task3_can_dmarecv(void* pvParameters);
void task4_wdt(void* pvParameters);
void task5_dmacahb(void* pvParameters);
void task6_Iic(void* pvParameters);
void task7_Uart(void* pvParameters);
void task9_Gbe(void* pvParameters);
void task10_Mmc(void* pvParameters);
void task11_Qspi(void* pvParameters);
void task12_Iis(void* pvParameters);
void task13_Dma(void* pvParameters);
void task14_dmacahb2(void* pvParameters);
void task15_UartProc(void* pvParameters);

typedef AL_VOID (*Task15Func)(AL_VOID);
static AL_S32 task3_DmacahbChEventCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef);
static AL_S32 task15_UartRecvProcess(AL_U8 *RecvMem);
static AL_VOID task15_CmdHelp(AL_VOID);
AL_VOID ConfigureTimerForRunTimeStates(AL_VOID);
static AL_VOID Ttc_DefEventHandler(AL_TTC_EventStruct TtcEvent, AL_VOID *CallbackRef);

static AL_U8 task15_ProcName[][UART_CMD_LEN_MAX] = {
    "help",
    "task_state"
};

static Task15Func task15_ProcList[] = {
    task15_CmdHelp,
};

int demo(void)
{
    AL_S32 Ret = AL_OK;

    /* Configure the system ready to run the demo.  The clock configuration
    can be done here if it was not done before main() was called. */
    prvSetupHardware();

    ConfigureTimerForRunTimeStates();

    Task1Led_Handler = rt_thread_create((const char*)"led", task1_led, (void*)NULL,
                (uint16_t)GPIO_LED_TASK_STACK_SIZE,
                (rt_ubase_t)GPIO_LED_TASK_PRIORITY,
                THREAD_TICK);
    if (Task1Led_Handler != RT_NULL)
        rt_thread_startup(Task1Led_Handler);

    Ret = AlOsal_Lock_Init(&Lock_Log, "Log");
    if (Ret != AL_OK) {
        while (1);
    }

    Ret = AlOsal_Lock_Init(&Lock_CanInit, "CanInit");
    if (Ret != AL_OK) {
        while (1);
    }

    Ret = AlOsal_Lock_Init(&Lock_DmaInit, "DmaInit");
    if (Ret != AL_OK) {
        while (1);
    }

    Ret = AlOsal_Lock_Init(&Lock_DmacahbInit, "DmacahbInit");
    if (Ret != AL_OK) {
        while (1);
    }

    Ret = AlOsal_Mb_Init(&MailBox_CanRecv, "CanRecv");
    if (Ret != AL_OK) {
        while (1);
    }

    Queue_UartRecv = rt_mq_create("uart_recv", UART_RECV_MEM_LENGTH, UART_RECV_MEM_SIZE, RT_IPC_FLAG_PRIO);
    if (Queue_UartRecv == AL_NULL) {
        while(1);
    }

    Task2Can_Send_Handler = rt_thread_create((const char*)"can_send", task2_can_send, (void*)NULL,
                (uint16_t)CAN_SEND_TASK_STACK_SIZE,
                (rt_ubase_t)CAN_SEND_TASK_PRIORITY,
                THREAD_TICK);
    if (Task2Can_Send_Handler != RT_NULL)
        rt_thread_startup(Task2Can_Send_Handler);

    Task3Can_DmaRecv_Handler = rt_thread_create((const char*)"can_dmarecv", task3_can_dmarecv, (void*)NULL,
                (uint16_t)CAN_DMARECV_TASK_STACK_SIZE,
                (rt_ubase_t)CAN_DMARECV_TASK_PRIORITY,
                THREAD_TICK);
    if (Task3Can_DmaRecv_Handler != RT_NULL)
        rt_thread_startup(Task3Can_DmaRecv_Handler);

    Task4Wdt_Handler = rt_thread_create((const char*)"wdt", task4_wdt, (void*)NULL,
                (uint16_t)WDT_TASK_STACK_SIZE,
                (rt_ubase_t)WDT_TASK_PRIORITY,
                THREAD_TICK);
    if (Task4Wdt_Handler != RT_NULL)
        rt_thread_startup(Task4Wdt_Handler);

    Task5Dmacahb_Handler = rt_thread_create((const char*)"dmacahb", task5_dmacahb, (void*)NULL,
                (uint16_t)DMACAHB_TASK_STACK_SIZE,
                (rt_ubase_t)DMACAHB_TASK_PRIORITY,
                THREAD_TICK);
    if (Task5Dmacahb_Handler != RT_NULL)
        rt_thread_startup(Task5Dmacahb_Handler);

    Task6Iic_Handler = rt_thread_create((const char*)"Iic", task6_Iic, (void*)NULL,
                (uint16_t)IIC_TASK_STACK_SIZE,
                (rt_ubase_t)IIC_TASK_PRIORITY,
                THREAD_TICK);
    if (Task6Iic_Handler != RT_NULL)
        rt_thread_startup(Task6Iic_Handler);

    Task7Uart_Handler = rt_thread_create((const char*)"Uart", task7_Uart, (void*)NULL,
                (uint16_t)UART_TASK_STACK_SIZE,
                (rt_ubase_t)UART_TASK_PRIORITY,
                THREAD_TICK);
    if (Task7Uart_Handler != RT_NULL)
        rt_thread_startup(Task7Uart_Handler);

    Task10Mmc_Handler = rt_thread_create((const char*)"Mmc", task10_Mmc, (void*)NULL,
                (uint16_t)MMC_TASK_STACK_SIZE,
                (rt_ubase_t)MMC_TASK_PRIORITY,
                THREAD_TICK);
    if (Task10Mmc_Handler != RT_NULL)
        rt_thread_startup(Task10Mmc_Handler);

    Task11Qspi_Handler = rt_thread_create((const char*)"Qspi", task11_Qspi, (void*)NULL,
                (uint16_t)QSPI_TASK_STACK_SIZE,
                (rt_ubase_t)QSPI_TASK_PRIORITY,
                THREAD_TICK);
    if (Task11Qspi_Handler != RT_NULL)
        rt_thread_startup(Task11Qspi_Handler);

    Task12Iis_Handler = rt_thread_create((const char*)"Iis", task12_Iis, (void*)NULL,
                (uint16_t)IIS_TASK_STACK_SIZE,
                (rt_ubase_t)IIS_TASK_PRIORITY,
                THREAD_TICK);
    // if (Task12Iis_Handler != RT_NULL)
    //     rt_thread_startup(Task12Iis_Handler);

    Task13Dma_Handler = rt_thread_create((const char*)"Dma", task13_Dma, (void*)NULL,
                (uint16_t)DMA_TASK_STACK_SIZE,
                (rt_ubase_t)DMA_TASK_PRIORITY,
                THREAD_TICK);
    if (Task13Dma_Handler != RT_NULL)
        rt_thread_startup(Task13Dma_Handler);

    Task14Dmacahb2_Handler = rt_thread_create((const char*)"dmacahb2", task14_dmacahb2, (void*)NULL,
                (uint16_t)DMACAHB_TASK_STACK_SIZE,
                (rt_ubase_t)DMACAHB_TASK_PRIORITY,
                THREAD_TICK);
    if (Task14Dmacahb2_Handler != RT_NULL)
        rt_thread_startup(Task14Dmacahb2_Handler);

    Task15UartProc_Handler = rt_thread_create((const char*)"UartProc", task15_UartProc, (void*)NULL,
                (uint16_t)UART_PROC_TASK_STACK_SIZE,
                (rt_ubase_t)UART_PROC_TASK_PRIORITY,
                THREAD_TICK);
    if (Task15UartProc_Handler != RT_NULL)
        rt_thread_startup(Task15UartProc_Handler);
    
    return AL_OK;
}
MSH_CMD_EXPORT(demo, demo);

void task1_led(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_GPIO_HalStruct *GPIO;

    AL_LOG(AL_LOG_LEVEL_INFO, "Task1 GPIO LED hal init\r\n");
    Ret = AlGpio_Hal_Init(&GPIO, GPIO_LED_DEVICE_ID, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task1 GPIO LED hal init error\r\n");
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task1 GPIO LED hal init success\r\n");

    while (1) {
        AlGpio_Hal_WritePin(GPIO, GPIO_LED_PIN_NUM, 0x0);
        rt_thread_delay(GPIO_LED_TOGGLE_TIME);
        AlGpio_Hal_WritePin(GPIO, GPIO_LED_PIN_NUM, 0x1);
        rt_thread_delay(GPIO_LED_TOGGLE_TIME);
    }
}

void task2_can_send(void* pvParameters)
{
    AL_U32 Ret = AL_OK;
    AL_CAN_HalStruct *CanHandle = AL_NULL;

    AL_LOG(AL_LOG_LEVEL_INFO, "Task2 Can send hal init\r\n");

    Ret = AlOsal_Lock_Take(&Lock_CanInit, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task2 Can send hal init lock take error\r\n");
        while (1);
    }

    Ret = AlCan_Hal_Init(&CanHandle, CAN_DEVICE_ID, &Task2_CanConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task2 Can send hal init error\r\n");
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task2 Can send hal init success\r\n");

    AlOsal_Lock_Release(&Lock_CanInit);

    while (1) {
        for (AL_U32 i = 0; i < (sizeof(Task2_FrameList) / sizeof(typeof(Task2_FrameList[0]))); i++) {
            Ret = AlCan_Hal_SendFrameBlock(CanHandle, Task2_FrameList[i], CAN_SEND_DATA_TIMEOUT_MS);
            if (Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task2 Send Frame Error:0x%x\r\n", Ret);
            } else {
                AL_LOG(AL_LOG_LEVEL_DEBUG, "Task2 Send Frame: %d\r\n", i);
            }

            rt_thread_delay(CAN_SEND_INTERVAL_TIME);
        }

    }
}

void task3_can_dmarecv(void* pvParameters)
{
    AL_U32 Ret = AL_OK;
    AL_CAN_FrameStruct Frame = {0};
    AL_DMACAHB_HalStruct *DmaHandle = AL_NULL;
    AL_DMACAHB_EventStruct DmaEvent = {0};
    AL_U32 *RecvMem = (AL_U32 *)rt_malloc(CAN_DMA_RECV_DATA);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task3 Can recv dmacahb hal init\r\n");
    /* Wait for can send init done forever */
    Ret = AlOsal_Lock_Take(&Lock_CanInit, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task3 Can send hal init lock take error\r\n");
        while (1);
    }


    Ret = AlOsal_Lock_Take(&Lock_DmacahbInit, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task2 Can send hal init lock take error\r\n");
        while (1);
    }

    Ret = AlDmacAhb_Hal_Init(&DmaHandle, CAN_DMACAHB_DEVICE_ID, &Task3_DmaChConfig, task3_DmacahbChEventCallBack);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task3 Can recv dmacahb hal Init error:0x%x\r\n", Ret);
        while (1);
    }

    AlOsal_Lock_Release(&Lock_DmacahbInit);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task3 Can recv dmacahb hal init success\r\n");

    AlOsal_Lock_Release(&Lock_CanInit);

    while (1) {
        Ret = AlOsal_Mb_Receive(&MailBox_CanRecv, &DmaEvent, CAN_RECV_DATA_TIMEOUT_MS);
        if (Ret != AL_OK) {
            if (Ret == AL_ERR_UNAVAILABLE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task3 Can Dma data recv timeout: 0x%x\r\n", Ret);
                continue;
            } else {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task3 Can Dma data recv error: 0x%x\r\n", Ret);
                return;
            }
        } else {
            #ifdef ENABLE_MMU
            AlCache_InvalidateDcacheRange((AL_UINTPTR)RecvMem, (AL_UINTPTR)(RecvMem + CAN_DMA_RECV_DATA));
            #endif
            AlCan_Dev_DecodeFrame(RecvMem, &Frame);
            AlCan_Dev_DisplayFrame(&Frame);
        }
    }
}

static AL_S32 task3_DmacahbChEventCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_DMACAHB_HalStruct *Handle = (AL_DMACAHB_HalStruct *)CallBackRef;

    AL_ASSERT((Event != AL_NULL) && (CallBackRef != AL_NULL), AL_ERR_NULL_PTR);

    switch (Event->EventId)
    {
    case AL_DMACAHB_EVENT_TRANS_COMP:
    case AL_DMACAHB_EVENT_BLOCK_TRANS_COMP:
        AlOsal_Mb_Send(&Handle->EventQueue, Event);
        break;
    case AL_DMACAHB_EVENT_ERR:
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task3 Dma trans error\r\n");
        break;
    case AL_DMACAHB_EVENT_RELOAD:
        AlOsal_Mb_Send(&MailBox_CanRecv, Event);
        break;
    default:
        break;
    }
    return AL_OK;
}


void task4_wdt(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_WDT_HalStruct *WdtHandle = AL_NULL;

    AL_LOG(AL_LOG_LEVEL_INFO, "Task4 Wdt hal init\r\n");

    Ret = AlWdt_Hal_Init(&WdtHandle, WDT_DEVICE_ID, &Task4_WdtConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task4 Wdt hal Init error:0x%x\r\n", Ret);
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task4 Wdt hal init success\r\n");

    while (1) {
        (AL_VOID)AlWdt_Hal_Feed(WdtHandle);
        AL_LOG(AL_LOG_LEVEL_INFO, "Task4 Wdt feed dog\r\n");
        rt_thread_delay(WDT_FEED_TIME);
    }
}

void task5_dmacahb(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_U8 InitData = 0;
    AL_DMACAHB_HalStruct *DmaHandle = AL_NULL;
    AL_DMACAHB_ChTransStruct *ChTransCfg = AL_NULL;
    AL_U8 *MemSrc = (AL_U8 *)rt_malloc(DMACAHB_ARRAY_SIZE);
    AL_U8 *MemDst = (AL_U8 *)rt_malloc(DMACAHB_ARRAY_SIZE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task5 Dmacahb hal init\r\n");

    Ret = AlOsal_Lock_Take(&Lock_DmacahbInit, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task5 dmacahb hal init lock take error\r\n");
        while (1);
    }

    Ret = AlDmacAhb_Hal_Init(&DmaHandle, DMACAHB_DEVICE_ID, &Task5_DmaChConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task5 Dmacahb hal Init error:0x%x\r\n", Ret);
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task5 Dmacahb hal init success\r\n");

    AlOsal_Lock_Release(&Lock_DmacahbInit);

    ChTransCfg = &DmaHandle->Channel.Trans;
    ChTransCfg->SrcAddr = (AL_REG)MemSrc;
    ChTransCfg->DstAddr = (AL_REG)MemDst;
    ChTransCfg->TransSize = DMACAHB_ARRAY_SIZE / (1 << Task5_DmaChConfig.SrcTransWidth);

    while (1) {
        memset((void*)(uintptr_t)(ChTransCfg->SrcAddr), InitData++, DMACAHB_ARRAY_SIZE);
        AL_LOG(AL_LOG_LEVEL_INFO, "Task5 Data trans %d\r\n", InitData);

        Ret = AlDmacAhb_Hal_StartBlock(DmaHandle, DMACAHB_TRANS_TIMEOUT_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task5 Trans error:0x%x\r\n", Ret);
        }

        Ret = memcmp((void*)(uintptr_t)(ChTransCfg->SrcAddr), (void*)(uintptr_t)(ChTransCfg->DstAddr), DMACAHB_ARRAY_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task5 Data check error:0x%x\r\n", Ret);
        }

        memset((void*)(uintptr_t)(ChTransCfg->DstAddr), 0, DMACAHB_ARRAY_SIZE);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange(ChTransCfg->DstAddr, ChTransCfg->DstAddr + DMACAHB_ARRAY_SIZE);
        #endif
        rt_thread_delay(DMACAHB_INTERVAL_TIME);
    }
}

void task6_Iic(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_U8 InitData = 0;
    AL_U8 Channel = IIC_EEPROM_CHANNEL;
    AL_U16 SlaveAddr = IIC_EEPROM_START_ADDR;
    AL_IIC_HalStruct *IicHandle = AL_NULL;
    AL_U8 *MemWrite = (AL_U8 *)rt_malloc(IIC_ADDR_SIZE + IIC_EEPROM_PAGE_SIZE);
    AL_U8 *MemRead = (AL_U8 *)rt_malloc(IIC_EEPROM_PAGE_SIZE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task6 Iic hal init\r\n");

    Ret = AlIic_Hal_Init(&IicHandle, DMACAHB_DEVICE_ID, &task6_IicConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task6 Iic hal Init error:0x%x\r\n", Ret);
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task6 Iic hal init success\r\n");

    Ret = AlIic_Hal_MasterSendDataBlock(IicHandle, IIC_MUX_ADDRESS, &Channel, 1, IIC_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task6 Iic Mux Init error:0x%x\r\n", Ret);
    }

    MemWrite[0] = (AL_U8) (IIC_EEPROM_START_ADDR);

    while (1) {
        memset(&MemWrite[1], InitData++, IIC_EEPROM_PAGE_SIZE);
        AL_LOG(AL_LOG_LEVEL_INFO, "Task6 iic Data trans %d\r\n", InitData);

        Ret = AlIic_Hal_MasterSendDataBlock(IicHandle, IIC_EEPROM_ADDRESS, MemWrite,
                                            IIC_ADDR_SIZE + IIC_EEPROM_PAGE_SIZE, IIC_TEST_TIMEOUT_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task6 Iic send data error:0x%x\r\n", Ret);
        }

        /* Wait write complete */
        rt_thread_delay(IIC_WAIT_WRITE_COMPLETE);

        /* Send read address */
        Ret = AlIic_Hal_MasterSendDataBlock(IicHandle, IIC_EEPROM_ADDRESS, (AL_U8 *)&SlaveAddr,
                                            IIC_ADDR_SIZE, IIC_TEST_TIMEOUT_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task6 Iic send read addr error:0x%x\r\n", Ret);
        }

        /* Read data from eeprom */
        Ret = AlIic_Hal_MasterRecvDataBlock(IicHandle, IIC_EEPROM_ADDRESS, MemRead,
                                            IIC_EEPROM_PAGE_SIZE, IIC_TEST_TIMEOUT_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task6 Iic recv data error:0x%x\r\n", Ret);
        }

        Ret = memcmp(&MemWrite[1], MemRead, IIC_EEPROM_PAGE_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task6 Iic read&write data check error:0x%x\r\n", Ret);
        }

        memset(MemRead, 0, IIC_EEPROM_PAGE_SIZE);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange((uintptr_t)MemRead, (uintptr_t)(MemRead + IIC_EEPROM_PAGE_SIZE));
        #endif

        rt_thread_delay(IIC_INTERVAL_TIME);
    }
}

extern AL_UART_HalStruct *AlLog;
void task7_Uart(void* pvParameters)
{
    AL_U32 Ret = AL_OK;
    AL_UART_HalStruct *UartHandle = AlLog;
    AL_U8 *RecvMem = (AL_U8 *)rt_malloc(UART_RECV_MEM_LENGTH);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task7 uart hal init\r\n");

    while (1) {
        Ret = AlUart_Hal_RecvDataBlock(UartHandle, &RecvMem[UART_RECV_SIZE_LENGTH], UART_RECV_MEM_LENGTH, (AL_U32 *)&RecvMem[0], AL_WAITFOREVER);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task7 uart recv data Error: 0x%x\r\n", Ret);
        }

        // Ret = task15_UartRecvProcess(RecvMem);
        // if (Ret != AL_OK) {
        //     AL_LOG(AL_LOG_LEVEL_ERROR, "Task7 Uart proc error:0x%x\r\n", Ret);
        // }

        Ret = rt_mq_send(Queue_UartRecv, RecvMem, UART_RECV_MEM_LENGTH);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task7 uart recv data send queue Error: 0x%x\r\n", Ret);
        }

        rt_thread_delay(UART_INTERVAL_TIME);
    }
}

void task9_Gbe(void* pvParameters)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Task9 Gbe init\r\n");

    /* Initialize the LwIP stack */
    tcpip_init(NULL, NULL);

    netif_config();

    rt_thread_suspend(rt_current_thread);

    while(1);
}

void task10_Mmc(void* pvParameters)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_U32 BlockCnt = MMC_BLOCK_COUNT;
    AL_MMC_HalStruct *MmcHandle = AL_NULL;
    AL_U8 *MemWrite = (AL_U8 *)rt_malloc(MMC_BLOCK_LEN);
    AL_U8 *MemRead = (AL_U8 *)rt_malloc(MMC_BLOCK_LEN);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task10 Mmc hal init\r\n");

    Ret = AlMmc_Hal_Init(&MmcHandle, MMC_DEVICE_ID, &task10_MmcSdmaConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task10 Mmc hal Init error:0x%x\r\n", Ret);
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task10 Mmc hal init success\r\n");

    while (1) {
        memset(MemWrite, (AL_U8)InitData++, MMC_BLOCK_LEN);
        AL_LOG(AL_LOG_LEVEL_INFO, "Task10 mmc Data trans %d\r\n", InitData);

        Ret = AlMmc_Hal_WriteBlocked(MmcHandle, MemWrite, MMC_BLOCK_OFFSET, BlockCnt, MMC_BLOCK_TIMEOUT_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task10 Mmc write %d block error 0x%x\r\n", BlockCnt, Ret);
        }

        Ret = AlMmc_Hal_ReadBlocked(MmcHandle, MemRead, MMC_BLOCK_OFFSET, BlockCnt, MMC_BLOCK_TIMEOUT_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task10 Mmc read %d block error 0x%x\r\n", BlockCnt, Ret);
        }

        Ret = memcmp(MemWrite, MemRead, MMC_BLOCK_LEN);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task10 Mmc Data check error:0x%x\r\n", Ret);
        }

        memset(MemRead, 0, MMC_BLOCK_LEN);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange((uintptr_t)MemRead, (uintptr_t)(MemRead + MMC_BLOCK_LEN));
        #endif

        rt_thread_delay(MMC_INTERVAL_TIME);
    }
}

void task11_Qspi(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_QSPI_HalStruct *QspiHandle = AL_NULL;
    AL_U8 *FlashId = (AL_U8 *)rt_malloc(10);
    AL_U8 *SendData = (AL_U8 *)rt_malloc(500);
    AL_U8 *RecvData = (AL_U8 *)rt_malloc(500);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task11 Qspi hal init\r\n");

    Ret = AlQspi_Hal_Init(&QspiHandle, &task11_QspiX4Config, AL_NULL, QSPI_DEVICE_ID);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task11 Qspi hal Init error:0x%x\r\n", Ret);
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task11 Qspi hal init success\r\n");

    while (1) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Task11 qspi Data read&write %d\r\n", InitData++);
        AlQspi_rtthread_ReadId(QspiHandle, SendData, FlashId);
        AlQspi_rtthread_ReadStatus(QspiHandle, SendData, RecvData);
        if((FlashId[0] != 0x01) && (FlashId[0] != 0x20) && (FlashId[0] != 0x0) && (FlashId[0] != 0xff)) {
            if( (FlashId[0] != 0x9d) && (FlashId[0] != 0xc2) ){
                Ret = AlQspi_rtthread_SetQuad(QspiHandle, SendData, RecvData, 0x31, 0x35, 1);
            }else{
                Ret = AlQspi_rtthread_SetQuad(QspiHandle, SendData, RecvData, 0x01, 0x05, 6);
            }
            if(Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task11 AlQspi set quad error\r\n");
            }
        }

        AlQspi_rtthread_WrEn(QspiHandle, SendData);
        AlQspi_rtthread_EraseChip(QspiHandle, SendData);
        AlQspi_rtthread_WaitWip(QspiHandle, SendData, RecvData);

        AlQspi_rtthread_X4_1_1_4_Read(QspiHandle, SendData, 0, RecvData);
        for (AL_U32 i = 0; i < 240; i++) {
            if(0xff != RecvData[i]) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task11 AlQspi test erase norflash error\r\n");
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task11 Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            }
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "Task11 AlQspi test erase norflash success\r\n");

        AlQspi_rtthread_WrEn(QspiHandle, SendData);
        AlQspi_rtthread_Write_X4_1_1_4_Page(QspiHandle, SendData, 0);
        AlQspi_rtthread_WaitWip(QspiHandle, SendData, RecvData);

        AlQspi_rtthread_X4_1_1_4_Read(QspiHandle, SendData, 0, RecvData);
        for (AL_U32 i = 0; i < 230; i++) {
            if (i != RecvData[i]) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task11 Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            }
        }

        rt_thread_delay(QSPI_INTERVAL_TIME);
    }
}

void task12_Iis(void* pvParameters)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_DMA_HalStruct *M2PHandle = AL_NULL;
    AL_DMA_HalStruct *P2MHandle = AL_NULL;
    AL_MailBox M2PEvent, P2MEvent;
    AL_U32 *Src = rt_malloc(IIS_DMA_DATA_SIZE);
    AL_U32 *Dst = rt_malloc(IIS_DMA_DATA_SIZE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task12 Iis dma m2p hal init\r\n");

    /* Take here and release after iis dma hal init success */
    Ret = AlOsal_Lock_Take(&Lock_DmaInit, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task12 Iis dma hal init lock take error\r\n");
        while (1);
    }

    Ret = AlDma_Hal_Init(&M2PHandle, IIS_DMA_DEVICE_ID, &task12_IisM2PConfig);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task12 Iis dma m2p hal Init error:0x%x\r\n", Ret);
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task12 Iis dma m2p hal init success\r\n");

    Ret = AlDma_Hal_Init(&P2MHandle, IIS_DMA_DEVICE_ID, &task12_IisP2MConfig);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task12 Iis dma p2m hal Init error:0x%x\r\n", Ret);
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task12 Iis dma p2m hal init success\r\n");

    /* IIS config */
    AL_REG32_WRITE(TOP_NS_BASE_ADDR + TOP_NS_PLS_PROT_REG, 0x00000001); /* Close gp port protect */
    AL_REG32_WRITE(IIS_BASE_ADDR + IIS_RESET_REG, 0x00000007);          /* Reset assert IIS */
    AL_REG32_WRITE(IIS_BASE_ADDR + IIS_RESET_REG, 0x00000000);          /* Reset release IIS */
    AL_REG32_WRITE(IIS_BASE_ADDR + IIS_CLK_CTRL_REG, 0x00000000);       /*  */
    AL_REG32_WRITE(IIS_BASE_ADDR + IIS_PERIOD_LEN_REG, 0x00000000);     /*  */

    AlOsal_Lock_Release(&Lock_DmaInit);

    while (1) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Task12 Iis dma Loop: 0x%d\r\n", InitData++);

        for (AL_U32 i = 0; i < IIS_DMA_DATA_SIZE / sizeof(AL_U32); i++) {
            Src[i] = (i + InitData) << 8;
        }

        Ret = AlDma_Hal_PeriCpySingle(M2PHandle, (AL_UINTPTR)Src, IIS_DMA_DATA_SIZE, IIS_TX_ID);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task12 Iis M2P mem copy error:0x%x\r\n", Ret);
        }

        Ret = AlDma_Hal_PeriCpySingle(P2MHandle, (AL_UINTPTR)Dst, IIS_DMA_DATA_SIZE, IIS_RX_ID);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task12 Iis M2P mem copy error:0x%x\r\n", Ret);
        }

        AL_REG32_WRITE(IIS_BASE_ADDR + IIS_CTRL_REG, 0x00000003);           /* Request DMA trans */

        Ret = AlOsal_Mb_Receive(&M2PHandle->EventQueue, (AL_VOID *)&M2PEvent, IIS_DMA_TRANS_TIMEOUT);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task12 Iis M2P Trans error: 0x%x\r\n", Ret);
        }

        Ret = AlOsal_Mb_Receive(&P2MHandle->EventQueue, (AL_VOID *)&P2MEvent, IIS_DMA_TRANS_TIMEOUT);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task12 Iis P2M Trans error: 0x%x\r\n", Ret);
        }

        for (AL_U32 i = 0 ; i < IIS_DMA_DATA_SIZE / sizeof(AL_U32); i++) {
            /* drop low 8 bit time stamp */
            Dst[i] &= 0xFFFFFF00;
        }

        Ret = memcmp(Dst, Src, IIS_DMA_DATA_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task12 Iis Data check error:0x%x\r\n", Ret);
        } else {
            AL_LOG(AL_LOG_LEVEL_INFO, "Task12 Iis Data check success: %d\r\n", InitData-1);
        }

        memset(Dst, 0, IIS_DMA_DATA_SIZE);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange((AL_UINTPTR)Dst, (AL_UINTPTR)(Dst + IIS_DMA_DATA_SIZE));
        #endif

        rt_thread_delay(IIS_INTERVAL_TIME);
    }
}

void task13_Dma(void* pvParameters)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_DMA_HalStruct *DmaHandle = AL_NULL;
    AL_U32 *Src = rt_malloc(DMA_TRANS_DATA_SIZE);
    AL_U32 *Dst = rt_malloc(DMA_TRANS_DATA_SIZE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task13 Dma hal init\r\n");

    /* Take here and release after iis dma hal init success */
    Ret = AlOsal_Lock_Take(&Lock_DmaInit, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task13 dma hal init lock take error\r\n");
        while (1);
    }

    Ret = AlDma_Hal_Init(&DmaHandle, IIS_DMA_DEVICE_ID, &task13_DmaConfig);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task13 Dma hal Init error:0x%x\r\n", Ret);
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task13 Dma hal init success\r\n");

    AlOsal_Lock_Release(&Lock_DmaInit);

    while (1) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Task13 Dma loop: 0x%d\r\n", InitData++);

        for (AL_U32 i = 0; i < DMA_TRANS_DATA_SIZE / sizeof(AL_U32); i++) {
            Src[i] = i + InitData;
        }

        Ret = AlDma_Hal_MemCpyBlock(DmaHandle, (AL_UINTPTR)Dst, (AL_UINTPTR)Src, DMA_TRANS_DATA_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task13 Dma Mem copy error:0x%x\r\n", Ret);
        }

        Ret = memcmp(Dst, Src, DMA_TRANS_DATA_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task13 Dma data check error:0x%x\r\n", Ret);
        }

        memset(Dst, 0, DMA_TRANS_DATA_SIZE);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange((AL_UINTPTR)Dst, (AL_UINTPTR)(Dst + DMA_TRANS_DATA_SIZE));
        #endif

        rt_thread_delay(DMA_INTERVAL_TIME);
    }
}

void task14_dmacahb2(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_U8 InitData = 0;
    AL_DMACAHB_HalStruct *DmaHandle = AL_NULL;
    AL_DMACAHB_ChTransStruct *ChTransCfg = AL_NULL;
    AL_U8 *MemSrc = (AL_U8 *)rt_malloc(DMACAHB2_ARRAY_SIZE);
    AL_U8 *MemDst = (AL_U8 *)rt_malloc(DMACAHB2_ARRAY_SIZE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task14 Dmacahb2 hal init\r\n");

    Ret = AlOsal_Lock_Take(&Lock_DmacahbInit, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task14 dmacahb2 hal init lock take error\r\n");
        while (1);
    }

    Ret = AlDmacAhb_Hal_Init(&DmaHandle, DMACAHB2_DEVICE_ID, &Task14_DmaChConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task14 Dmacahb2 hal Init error:0x%x\r\n", Ret);
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task14 Dmacahb2 hal init success\r\n");

    AlOsal_Lock_Release(&Lock_DmacahbInit);

    ChTransCfg = &DmaHandle->Channel.Trans;
    ChTransCfg->SrcAddr = (AL_REG)MemSrc;
    ChTransCfg->DstAddr = (AL_REG)MemDst;
    ChTransCfg->TransSize = DMACAHB2_ARRAY_SIZE / (1 << Task14_DmaChConfig.SrcTransWidth);

    while (1) {
        memset((void*)(AL_UINTPTR)(ChTransCfg->SrcAddr), InitData++, DMACAHB2_ARRAY_SIZE);
        AL_LOG(AL_LOG_LEVEL_INFO, "Task14 Data trans %d\r\n", InitData);

        Ret = AlDmacAhb_Hal_StartBlock(DmaHandle, DMACAHB2_TRANS_TIMEOUT_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task14 Trans error:0x%x\r\n", Ret);
        }

        Ret = memcmp((void*)(AL_UINTPTR)(ChTransCfg->SrcAddr), (void*)(AL_UINTPTR)(ChTransCfg->DstAddr), DMACAHB2_ARRAY_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task14 Data check error:0x%x\r\n", Ret);
        }

        memset((void*)(AL_UINTPTR)(ChTransCfg->DstAddr), 0, DMACAHB2_ARRAY_SIZE);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange(ChTransCfg->DstAddr, ChTransCfg->DstAddr + DMACAHB2_ARRAY_SIZE);
        #endif

        rt_thread_delay(DMACAHB2_INTERVAL_TIME);
    }
}

void task15_UartProc(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_U8 *RecvMem = (AL_U8 *)rt_malloc(UART_RECV_MEM_LENGTH);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task15 Init success\r\n");

    while (1) {
        Ret = rt_mq_recv(Queue_UartRecv, RecvMem, UART_RECV_MEM_LENGTH, RT_WAITING_FOREVER);
        if (Ret != AL_TRUE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task15 Uart proc recv error:0x%x\r\n", Ret);
        } else {
            Ret = task15_UartRecvProcess(RecvMem);
            if (Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task15 Uart proc error:0x%x\r\n", Ret);
            }
        }

        rt_thread_delay(UART_INTERVAL_TIME);
    }
}

static AL_S32 task15_UartRecvProcess(AL_U8 *RecvMem)
{
    AL_U32 i = 0;
    AL_U32 Size = *(AL_U32 *)RecvMem;
    AL_U8 *Cmd = &RecvMem[UART_RECV_SIZE_LENGTH];
    AL_LOG(AL_LOG_LEVEL_INFO, "Task15 len:%d, data: %s\r\n", Size, Cmd);

    for (i = 0; i < sizeof(task15_ProcList) / sizeof(typeof(task15_ProcList[0])); i++) {
        if (!strncmp((char*)Cmd, (char*)task15_ProcName[i], Size)) {
            break;
        }
    }

    if (i == sizeof(task15_ProcList) / sizeof(typeof(task15_ProcList[0]))) {
        return AL_ERR_UNAVAILABLE;
    }

    task15_ProcList[i]();

    return AL_OK;
}

static AL_VOID task15_CmdHelp(AL_VOID)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Task15 cmd help\r\n");
}

AL_VOID ConfigureTimerForRunTimeStates(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_TTC_HalStruct *TtcHandle = AL_NULL;

    AlTtc_Hal_Init(&TtcHandle, TTC_DEVICE_ID, &Ttc_Config, Ttc_DefEventHandler);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return;
    }

    AlTtc_Hal_EnableIntervalMode(TtcHandle);
    AlTtc_Hal_SetIntervalMaxVal(TtcHandle, TTC_INTERVAL_MAX_VALUE);
    AlTtc_Hal_EnableMatchMode(TtcHandle, AL_TRUE);
    AlTtc_Hal_SetMatchVal(TtcHandle, AL_TTC_Match1, TTC_MATCH_VALUE);
    AlTtc_Hal_EnableIntr(TtcHandle, AL_TTC_IntrMatch1, AL_TRUE);
    AlTtc_Hal_EnableCounter(TtcHandle, AL_TRUE);

    rtthreadRunTimeTicks = 0;
}

static AL_VOID Ttc_DefEventHandler(AL_TTC_EventStruct TtcEvent, AL_VOID *CallbackRef)
{
    switch (TtcEvent.Events)
    {
    case AL_TTC_EVENT_Interval:
        break;
    case AL_TTC_EVENT_Match1:
        rtthreadRunTimeTicks++;
        break;
    case AL_TTC_EVENT_Match2:
        break;
    case AL_TTC_EVENT_Match3:
        break;
    case AL_TTC_EVENT_Overflow:
        break;
    case AL_TTC_EVENT_EventTimer:
        break;
    default:
        break;
    }

}

int main()
{
    return 0;
}
