/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */

#include <stdio.h>
#include <stdlib.h>

#include "freertos_demo_config.h"

/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the pdMS_TO_TICKS() macro. */
#define mainSOFTWARE_TIMER_PERIOD_MS    pdMS_TO_TICKS(1000)
#define TASKDLYMS                       pdMS_TO_TICKS(100)
#define mainQUEUE_LENGTH                (1)

static void prvSetupHardware(void);

/* The queue used by the queue send and queue receive tasks. */
static QueueHandle_t xQueue = NULL;

static TaskHandle_t Task1Led_Handler;
static TaskHandle_t Task2Can_Send_Handler;
static TaskHandle_t Task3Can_DmaRecv_Handler;
static TaskHandle_t Task4Wdt_Handler;
static TaskHandle_t Task5Dmacahb_Handler;
static TaskHandle_t Task6Iic_Handler;
static TaskHandle_t Task7Uart_Handler;
static TaskHandle_t Task8Usb_Handler;
static TaskHandle_t Task9Gbe_Handler;
static TaskHandle_t Task10Mmc_Handler;
static TaskHandle_t Task11Qspi_Handler;
static TaskHandle_t Task12Iis_Handler;
static TaskHandle_t Task13Dma_Handler;
static TaskHandle_t Task14Dmacahb2_Handler;
static TaskHandle_t Task15UartProc_Handler;
static TaskHandle_t Task16AsyncPrint_Handler;

static AL_Lock Lock_CanInit;
static AL_Lock Lock_DmaInit;
static AL_Lock Lock_DmacahbInit;
AL_Lock Lock_Log;
static AL_MailBox MailBox_CanRecv;
static QueueHandle_t Queue_UartRecv;

AL_CAN_HalStruct *CanHandle = AL_NULL;

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
void task16_AsyncPrint(void* pvParameters);

typedef AL_VOID (*Task15Func)(AL_VOID);
static AL_S32 task3_DmacahbChEventCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef);
static AL_S32 task15_UartRecvProcess(AL_U8 *RecvMem);
static AL_VOID task15_CmdHelp(AL_VOID);
static AL_VOID task15_CmdTaskState(AL_VOID);
AL_VOID ConfigureTimerForRunTimeStates(AL_VOID);
static AL_VOID Ttc_DefEventHandler(AL_TTC_EventStruct TtcEvent, AL_VOID *CallbackRef);

static AL_U8 task15_ProcName[][UART_CMD_LEN_MAX] = {
    "help",
    "task_state"
};

static Task15Func task15_ProcList[] = {
    task15_CmdHelp,
    task15_CmdTaskState,
};

int main(void)
{
    AL_S32 Ret = AL_OK;
    TimerHandle_t xExampleSoftwareTimer = NULL;

    /* Configure the system ready to run the demo.  The clock configuration
    can be done here if it was not done before main() was called. */
    prvSetupHardware();

    xTaskCreate((TaskFunction_t)task1_led, (const char*)"task1_led",
                (uint16_t)GPIO_LED_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)GPIO_LED_TASK_PRIORITY,
                (TaskHandle_t*)&Task1Led_Handler);

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

    Queue_UartRecv = xQueueCreate(UART_RECV_MEM_SIZE, UART_RECV_MEM_LENGTH);
    if (Queue_UartRecv == AL_NULL) {
        while(1);
    }

    xTaskCreate((TaskFunction_t)task2_can_send, (const char*)"task2_can_send",
                (uint16_t)CAN_SEND_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)CAN_SEND_TASK_PRIORITY,
                (TaskHandle_t*)&Task2Can_Send_Handler);

    xTaskCreate((TaskFunction_t)task3_can_dmarecv, (const char*)"task3_can_dmarecv",
                (uint16_t)CAN_DMARECV_TASK_STACK_SIZE,
                (void*)NULL, (UBaseType_t)CAN_DMARECV_TASK_PRIORITY,
                (TaskHandle_t*)&Task3Can_DmaRecv_Handler);

    xTaskCreate((TaskFunction_t)task4_wdt, (const char*)"task4_wdt",
                (uint16_t)WDT_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)WDT_TASK_PRIORITY,
                (TaskHandle_t*)&Task4Wdt_Handler);

    xTaskCreate((TaskFunction_t)task5_dmacahb, (const char*)"task5_dmacahb",
                (uint16_t)DMACAHB_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)DMACAHB_TASK_PRIORITY,
                (TaskHandle_t*)&Task5Dmacahb_Handler);

    xTaskCreate((TaskFunction_t)task6_Iic, (const char*)"task6_Iic",
                (uint16_t)IIC_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)IIC_TASK_PRIORITY,
                (TaskHandle_t*)&Task6Iic_Handler);

    xTaskCreate((TaskFunction_t)task7_Uart, (const char*)"task7_Uart",
                (uint16_t)UART_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)UART_TASK_PRIORITY,
                (TaskHandle_t*)&Task7Uart_Handler);

    xTaskCreate((TaskFunction_t)task9_Gbe, (const char*)"task9_Gbe",
                (uint16_t)GBE_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)GBE_TASK_PRIORITY,
                (TaskHandle_t*)&Task9Gbe_Handler);

    xTaskCreate((TaskFunction_t)task10_Mmc, (const char*)"task10_Mmc",
                (uint16_t)MMC_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)MMC_TASK_PRIORITY,
                (TaskHandle_t*)&Task10Mmc_Handler);

    xTaskCreate((TaskFunction_t)task11_Qspi, (const char*)"task11_Qspi",
                (uint16_t)QSPI_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)QSPI_TASK_PRIORITY,
                (TaskHandle_t*)&Task11Qspi_Handler);

    xTaskCreate((TaskFunction_t)task12_Iis, (const char*)"task12_Iis",
                (uint16_t)IIS_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)IIS_TASK_PRIORITY,
                (TaskHandle_t*)&Task12Iis_Handler);

    xTaskCreate((TaskFunction_t)task13_Dma, (const char*)"task13_Dma",
                (uint16_t)DMA_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)DMA_TASK_PRIORITY,
                (TaskHandle_t*)&Task13Dma_Handler);

    xTaskCreate((TaskFunction_t)task14_dmacahb2, (const char*)"task14_dmacahb2",
                (uint16_t)DMACAHB_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)DMACAHB_TASK_PRIORITY,
                (TaskHandle_t*)&Task14Dmacahb2_Handler);

    xTaskCreate((TaskFunction_t)task15_UartProc, (const char*)"task15_UartProc",
                (uint16_t)UART_PROC_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)UART_PROC_TASK_PRIORITY,
                (TaskHandle_t*)&Task15UartProc_Handler);

    xTaskCreate((TaskFunction_t)task16_AsyncPrint, (const char*)"task16_AsyncPrint",
                (uint16_t)PRINT_TASK_STACK_SIZE, (void*)NULL,
                (UBaseType_t)PRINT_TASK_PRIORITY,
                (TaskHandle_t*)&Task16AsyncPrint_Handler);

    vTaskStartScheduler();

    AL_LOG(AL_LOG_LEVEL_ERROR, "OS should never run to here\r\n");

    while (1);
}

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
        vTaskDelay(GPIO_LED_TOGGLE_TIME);
        AlGpio_Hal_WritePin(GPIO, GPIO_LED_PIN_NUM, 0x1);
        vTaskDelay(GPIO_LED_TOGGLE_TIME);
    }
}

void task2_can_send(void* pvParameters)
{
    AL_U32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Task2 Can send hal init\r\n");

    Ret = AlOsal_Lock_Take(&Lock_CanInit, portMAX_DELAY);
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

            vTaskDelay(CAN_SEND_INTERVAL_TIME);
        }

    }
}

void task3_can_dmarecv(void* pvParameters)
{
    AL_U32 Ret = AL_OK;
    AL_CAN_FrameStruct Frame = {0};
    AL_DMACAHB_HalStruct *DmaHandle = AL_NULL;
    AL_DMACAHB_EventStruct DmaEvent = {0};
    AL_DMACAHB_ChTransStruct *DmacChTrans = AL_NULL;
    AL_U8 *RecvMem = (AL_U8 *)pvPortMalloc(CAN_DMA_RECV_DATA);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task3 Can recv dmacahb hal init\r\n");
    /* Wait for can send init done forever */
    Ret = AlOsal_Lock_Take(&Lock_CanInit, portMAX_DELAY);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task3 Can send hal init lock take error\r\n");
        while (1);
    }

    Ret = AlOsal_Lock_Take(&Lock_DmacahbInit, portMAX_DELAY);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task3 Can dma hal init lock take error\r\n");
        while (1);
    }

    Ret = AlDmacAhb_Hal_Init(&DmaHandle, CAN_DMACAHB_DEVICE_ID, &Task3_DmaChConfig, task3_DmacahbChEventCallBack);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task3 Can recv dmacahb hal Init error:0x%x\r\n", Ret);
        while (1);
    }

    DmacChTrans = &(DmaHandle->Channel.Trans);
    DmacChTrans->SrcAddr        = CanHandle->Dev.BaseAddr;
    DmacChTrans->DstAddr        = (AL_REG)RecvMem;
    DmacChTrans->TransSize      = CAN_DMA_RECV_DATA >> 2;
    DmacChTrans->ReloadCountNum = AL_DMACAHB_RELOAD_CNT_MAX;   /* max AL_U32 for trans forever */

    AlDmacAhb_Hal_Start(DmaHandle);

    AlOsal_Lock_Release(&Lock_DmacahbInit);

    AlOsal_Lock_Release(&Lock_CanInit);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task3 Can recv dmacahb hal init success\r\n");

    while (1) {
        Ret = AlOsal_Mb_Receive(&MailBox_CanRecv, &DmaEvent, CAN_RECV_DATA_TIMEOUT_MS);
        if (Ret != AL_OK) {
            if (Ret == AL_ERR_UNAVAILABLE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task3 Can Dma data recv timeout: 0x%x\r\n", Ret);
                continue;
            } else {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task3 Can Dma data recv error: 0x%x\r\n", Ret);
                return Ret;
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
        AlWdt_Hal_Feed(WdtHandle);
        AL_LOG(AL_LOG_LEVEL_INFO, "Task4 Wdt feed dog\r\n");
        vTaskDelay(WDT_FEED_TIME);
    }
}

void task5_dmacahb(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_U8 InitData = 0;
    AL_DMACAHB_HalStruct *DmaHandle = AL_NULL;
    AL_DMACAHB_ChTransStruct *ChTransCfg = AL_NULL;
    AL_U8 *MemSrc = (AL_U8 *)pvPortMalloc(DMACAHB_ARRAY_SIZE);
    AL_U8 *MemDst = (AL_U8 *)pvPortMalloc(DMACAHB_ARRAY_SIZE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task5 Dmacahb hal init\r\n");

    Ret = AlOsal_Lock_Take(&Lock_DmacahbInit, portMAX_DELAY);
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
        memset(ChTransCfg->SrcAddr, InitData++, DMACAHB_ARRAY_SIZE);
        AL_LOG(AL_LOG_LEVEL_INFO, "Task5 Data trans %d\r\n", InitData);

        Ret = AlDmacAhb_Hal_StartBlock(DmaHandle, DMACAHB_TRANS_TIMEOUT_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task5 Trans error:0x%x\r\n", Ret);
        }

        Ret = memcmp(ChTransCfg->SrcAddr, ChTransCfg->DstAddr, DMACAHB_ARRAY_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task5 Data check error:0x%x\r\n", Ret);
        }

        memset(ChTransCfg->DstAddr, 0, DMACAHB_ARRAY_SIZE);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange(ChTransCfg->DstAddr, ChTransCfg->DstAddr + DMACAHB_ARRAY_SIZE);
        #endif
        vTaskDelay(DMACAHB_INTERVAL_TIME);
    }
}

void task6_Iic(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_U8 InitData = 0;
    AL_U8 Channel = IIC_EEPROM_CHANNEL;
    AL_U16 SlaveAddr = IIC_EEPROM_START_ADDR;
    AL_IIC_HalStruct *IicHandle = AL_NULL;
    AL_U8 *MemWrite = (AL_U8 *)pvPortMalloc(IIC_ADDR_SIZE + IIC_EEPROM_PAGE_SIZE);
    AL_U8 *MemRead = (AL_U8 *)pvPortMalloc(IIC_EEPROM_PAGE_SIZE);

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
        vTaskDelay(IIC_WAIT_WRITE_COMPLETE);

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
        AlCache_FlushDcacheRange(MemRead, MemRead + IIC_EEPROM_PAGE_SIZE);
        #endif

        vTaskDelay(IIC_INTERVAL_TIME);
    }
}

extern AL_UART_HalStruct *AlLog;
void task7_Uart(void* pvParameters)
{
    AL_U32 Ret = AL_OK;
    AL_UART_HalStruct *UartHandle = AlLog;
    AL_UART_EventStruct UartEvent = {0};
    AL_U8 *RecvMem = (AL_U8 *)pvPortMalloc(UART_RECV_MEM_LENGTH);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task7 uart hal init\r\n");

    // Ret = AlUart_Hal_Init(&UartHandle, UART_DEVICE_ID, &task7_UartConfig, AL_NULL);
    // if (Ret != AL_OK) {
    //     AL_LOG(AL_LOG_LEVEL_ERROR, "Task7 uart hal Init error:0x%x\r\n", Ret);
    //     while (1);
    // }
    // AL_LOG(AL_LOG_LEVEL_INFO, "Task7 uart hal init success\r\n");

    while (1) {
        Ret = AlUart_Hal_RecvDataBlock(UartHandle, &RecvMem[UART_RECV_SIZE_LENGTH], UART_RECV_MEM_LENGTH, (AL_U8 *)&RecvMem[0], portMAX_DELAY);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task7 uart recv data Error: 0x%x\r\n", Ret);
        }

        // Ret = task15_UartRecvProcess(RecvMem);
        // if (Ret != AL_OK) {
        //     AL_LOG(AL_LOG_LEVEL_ERROR, "Task7 Uart proc error:0x%x\r\n", Ret);
        // }

        Ret = xQueueSend(Queue_UartRecv, RecvMem, UART_QUEUE_BLOCK_TIME);
        if (Ret != pdTRUE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task7 uart recv data send queue Error: 0x%x\r\n", Ret);
        }

        vTaskDelay(UART_INTERVAL_TIME);
    }
}

void task9_Gbe(void* pvParameters)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Task9 Gbe init\r\n");

    /* Initialize the LwIP stack */
    tcpip_init(NULL, NULL);

    netif_config();

    vTaskSuspend(NULL);

    while(1);
}

void task10_Mmc(void* pvParameters)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_U32 BlockCnt = MMC_BLOCK_COUNT;
    AL_MMC_HalStruct *MmcHandle = AL_NULL;
    AL_U8 *MemWrite = (AL_U8 *)pvPortMalloc(MMC_BLOCK_LEN);
    AL_U8 *MemRead = (AL_U8 *)pvPortMalloc(MMC_BLOCK_LEN);

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
        AlCache_FlushDcacheRange(MemRead, MemRead + MMC_BLOCK_LEN);
        #endif

        vTaskDelay(MMC_INTERVAL_TIME);
    }
}

void task11_Qspi(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_QSPI_HalStruct *QspiHandle = AL_NULL;
    AL_U8 *FlashId = (AL_U8 *)pvPortMalloc(10);
    AL_U8 *SendData = (AL_U8 *)pvPortMalloc(500);
    AL_U8 *RecvData = (AL_U8 *)pvPortMalloc(500);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task11 Qspi hal init\r\n");

    Ret = AlQspi_Hal_Init(&QspiHandle, &task11_QspiX4Config, AL_NULL, QSPI_DEVICE_ID);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Task11 Qspi hal Init error:0x%x\r\n", Ret);
        while (1);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Task11 Qspi hal init success\r\n");

    while (1) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Task11 qspi Data read&write %d\r\n", InitData++);
        AlQspi_freertos_ReadId(QspiHandle, SendData, FlashId);
        AlQspi_freertos_ReadStatus(QspiHandle, SendData, RecvData);
        if((FlashId[0] != 0x01) && (FlashId[0] != 0x20) && (FlashId[0] != 0x0) && (FlashId[0] != 0xff)) {
            if( (FlashId[0] != 0x9d) && (FlashId[0] != 0xc2) ){
                Ret = AlQspi_freertos_SetQuad(QspiHandle, SendData, RecvData, 0x31, 0x35, 1);
            }else{
                Ret = AlQspi_freertos_SetQuad(QspiHandle, SendData, RecvData, 0x01, 0x05, 6);
            }
            if(Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task11 AlQspi set quad error\r\n");
            }
        }

        // AlQspi_freertos_WrEn(QspiHandle, SendData);
        // AlQspi_freertos_EraseChip(QspiHandle, SendData);
        // AlQspi_freertos_WaitWip(QspiHandle, SendData, RecvData);

        // AlQspi_freertos_X4_1_1_4_Read(QspiHandle, SendData, 0, RecvData);
        // for (AL_U32 i = 0; i < 240; i++) {
        //     if(0xff != RecvData[i]) {
        //         AL_LOG(AL_LOG_LEVEL_ERROR, "Task11 AlQspi test erase norflash error\r\n");
        //         AL_LOG(AL_LOG_LEVEL_ERROR, "Task11 Error RecvData[%d]:%d\r\n", i, RecvData[i]);
        //     }
        // }
        // AL_LOG(AL_LOG_LEVEL_INFO, "Task11 AlQspi test erase norflash success\r\n");

        AlQspi_freertos_WrEn(QspiHandle, SendData);
        AlQspi_freertos_Write_X4_1_1_4_Page(QspiHandle, SendData, QSPI_ADDR_OFFSET);
        AlQspi_freertos_WaitWip(QspiHandle, SendData, RecvData);

        AlQspi_freertos_X4_1_1_4_Read(QspiHandle, SendData, QSPI_ADDR_OFFSET, RecvData);
        for (AL_U32 i = 0; i < 230; i++) {
            if (i != RecvData[i]) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task11 Error RecvData[%d]:%d\r\n", i, RecvData[i]);
                break;
            }
        }

        vTaskDelay(QSPI_INTERVAL_TIME);
    }
}

void task12_Iis(void* pvParameters)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_DMA_HalStruct *M2PHandle = AL_NULL;
    AL_DMA_HalStruct *P2MHandle = AL_NULL;
    AL_MailBox M2PEvent, P2MEvent;
    AL_U32 *Src = pvPortMalloc(IIS_DMA_DATA_SIZE);
    AL_U32 *Dst = pvPortMalloc(IIS_DMA_DATA_SIZE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task12 Iis dma m2p hal init\r\n");

    /* Take here and release after iis dma hal init success */
    Ret = AlOsal_Lock_Take(&Lock_DmaInit, portMAX_DELAY);
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

        Ret = AlDma_Hal_PeriCpySingle(M2PHandle, Src, IIS_DMA_DATA_SIZE, IIS_TX_ID);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task12 Iis M2P mem copy error:0x%x\r\n", Ret);
        }

        Ret = AlDma_Hal_PeriCpySingle(P2MHandle, Dst, IIS_DMA_DATA_SIZE, IIS_RX_ID);
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
        AlCache_FlushDcacheRange(Dst, Dst + IIS_DMA_DATA_SIZE);
        #endif

        vTaskDelay(IIS_INTERVAL_TIME);
    }
}

void task13_Dma(void* pvParameters)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_DMA_HalStruct *DmaHandle = AL_NULL;
    AL_U32 *Src = pvPortMalloc(DMA_TRANS_DATA_SIZE);
    AL_U32 *Dst = pvPortMalloc(DMA_TRANS_DATA_SIZE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task13 Dma hal init\r\n");

    /* Take here and release after iis dma hal init success */
    Ret = AlOsal_Lock_Take(&Lock_DmaInit, portMAX_DELAY);
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

        Ret = AlDma_Hal_MemCpyBlock(DmaHandle, Dst, Src, DMA_TRANS_DATA_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task13 Dma Mem copy error:0x%x\r\n", Ret);
        }

        Ret = memcmp(Dst, Src, DMA_TRANS_DATA_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task13 Dma data check error:0x%x\r\n", Ret);
        }

        memset(Dst, 0, DMA_TRANS_DATA_SIZE);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange(Dst, Dst + DMA_TRANS_DATA_SIZE);
        #endif

        vTaskDelay(DMA_INTERVAL_TIME);
    }
}

void task14_dmacahb2(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_U8 InitData = 0;
    AL_DMACAHB_HalStruct *DmaHandle = AL_NULL;
    AL_DMACAHB_ChTransStruct *ChTransCfg = AL_NULL;
    AL_U8 *MemSrc = (AL_U8 *)pvPortMalloc(DMACAHB2_ARRAY_SIZE);
    AL_U8 *MemDst = (AL_U8 *)pvPortMalloc(DMACAHB2_ARRAY_SIZE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task14 Dmacahb2 hal init\r\n");

    Ret = AlOsal_Lock_Take(&Lock_DmacahbInit, portMAX_DELAY);
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
        memset(ChTransCfg->SrcAddr, InitData++, DMACAHB2_ARRAY_SIZE);
        AL_LOG(AL_LOG_LEVEL_INFO, "Task14 Data trans %d\r\n", InitData);

        Ret = AlDmacAhb_Hal_StartBlock(DmaHandle, DMACAHB2_TRANS_TIMEOUT_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task14 Trans error:0x%x\r\n", Ret);
        }

        Ret = memcmp(ChTransCfg->SrcAddr, ChTransCfg->DstAddr, DMACAHB2_ARRAY_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task14 Data check error:0x%x\r\n", Ret);
        }

        memset(ChTransCfg->DstAddr, 0, DMACAHB2_ARRAY_SIZE);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange(ChTransCfg->DstAddr, ChTransCfg->DstAddr + DMACAHB2_ARRAY_SIZE);
        #endif

        vTaskDelay(DMACAHB2_INTERVAL_TIME);
    }
}

void task15_UartProc(void* pvParameters)
{
    AL_S32 Ret = AL_OK;
    AL_U8 *RecvMem = (AL_U8 *)pvPortMalloc(UART_RECV_MEM_LENGTH);

    AL_LOG(AL_LOG_LEVEL_INFO, "Task15 Init success\r\n");

    while (1) {
        Ret = xQueueReceive(Queue_UartRecv, RecvMem, portMAX_DELAY);
        if (Ret != pdTRUE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Task15 Uart proc recv error:0x%x\r\n", Ret);
        } else {
            Ret = task15_UartRecvProcess(RecvMem);
            if (Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Task15 Uart proc error:0x%x\r\n", Ret);
            }
        }

        vTaskDelay(UART_INTERVAL_TIME);
    }
}

static AL_S32 task15_UartRecvProcess(AL_U8 *RecvMem)
{
    AL_S32 Ret = AL_OK;
    AL_U32 i = 0;
    AL_U32 Size = *(AL_U32 *)RecvMem;
    AL_U8 *Cmd = &RecvMem[UART_RECV_SIZE_LENGTH];
    AL_LOG(AL_LOG_LEVEL_INFO, "Task15 len:%d, data: %s\r\n", Size, Cmd);

    for (i = 0; i < sizeof(task15_ProcList) / sizeof(typeof(task15_ProcList[0])); i++) {
        if (!strncmp(Cmd, task15_ProcName[i], Size)) {
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


AL_U8 BufTaskList[TASK_STATE_BUF] = {0};
AL_U8 BufTaskState[TASK_STATE_BUF] = {0};
static AL_VOID task15_CmdTaskState(AL_VOID)
{
    // AL_LOG(AL_LOG_LEVEL_INFO, "Task15 cmd task state\r\n");
    memset(BufTaskList, 0, TASK_STATE_BUF);
    memset(BufTaskState, 0, TASK_STATE_BUF);
    vTaskList(BufTaskList);
    AL_LOG(AL_LOG_LEVEL_INFO, " taskName\tstatus\tpri\tStack\ttaskNum\r\n");
    AL_LOG(AL_LOG_LEVEL_INFO, "%s", BufTaskList);
    // AlLog_Write(BufTaskList, strlen(BufTaskList));
    vTaskGetRunTimeStats(BufTaskState);
    AL_LOG(AL_LOG_LEVEL_INFO, "Task Runtime State\r\n");
    AL_LOG(AL_LOG_LEVEL_INFO, "%s", BufTaskState);
    // AlLog_Write(BufTaskState, strlen(BufTaskState));
}

void task16_AsyncPrint(void* pvParameters)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Task16 Async print init success\r\n");

    while (1) {
        AlPrint_AsyncPrintf();
        vTaskDelay(PRINT_INTERVAL_TIME);
    }
}

void vApplicationTickHook(void)
{
    // BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* The RTOS tick hook function is enabled by setting configUSE_TICK_HOOK to
    1 in FreeRTOSConfig.h.

    "Give" the semaphore on every 500th tick interrupt. */

    /* If xHigherPriorityTaskWoken is pdTRUE then a context switch should
    normally be performed before leaving the interrupt (because during the
    execution of the interrupt a task of equal or higher priority than the
    running task was unblocked).  The syntax required to context switch from
    an interrupt is port dependent, so check the documentation of the port you
    are using.

    In this case, the function is running in the context of the tick interrupt,
    which will automatically check for the higher priority task to run anyway,
    so no further action is required. */
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void)
{
    /* The malloc failed hook is enabled by setting
    configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

    Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
    AL_LOG(AL_LOG_LEVEL_ERROR, "malloc failed\n");
    while (1);
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
    /* Run time stack overflow checking is performed if
    configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected.  pxCurrentTCB can be
    inspected in the debugger if the task name passed into this function is
    corrupt. */
    AL_LOG(AL_LOG_LEVEL_ERROR, "Stack Overflow\n");
    while (1);
}
/*-----------------------------------------------------------*/

extern UBaseType_t uxCriticalNesting;
void vApplicationIdleHook(void)
{
    // volatile size_t xFreeStackSpace;
    /* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
    FreeRTOSConfig.h.

    This function is called on each cycle of the idle task.  In this case it
    does nothing useful, other than report the amount of FreeRTOS heap that
    remains unallocated. */
    /* By now, the kernel has allocated everything it is going to, so
    if there is a lot of heap remaining unallocated then
    the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
    reduced accordingly. */
}
