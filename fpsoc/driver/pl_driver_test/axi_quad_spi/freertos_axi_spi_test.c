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

#include "al_axi_qspi_hal.h"
#include "al_intr.h"
#include "al_reg_io.h"
#include "al_spinor.h"
#include "al_top.h"


/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the pdMS_TO_TICKS() macro. */
#define SOFTWARE_TIMER_PERIOD_MS        pdMS_TO_TICKS(1000)
#define TASKDLYMS                       pdMS_TO_TICKS(1000)
#define QUEUE_LENGTH                    (1)

/***************** Macros (Inline Functions) Definitions *********************/
#define QSPI_FLASH_PAGE_SIZE                     256
#define AL_AXI_QSPI_TEST_SIZE                   1024
#define FLASH_READ_BUF_SIZE        (1 * 1024 * 1024)
#define FLASH_SIZE_IN_BYTES       (32 * 1024 * 1024)
#define TIMEOUT_FOR_TEST                      100000

/************************** Variable Definitions *****************************/
static AL_U8 SendData[AL_AXI_QSPI_TEST_SIZE] = { 0 };
static AL_U8 RecvData[FLASH_READ_BUF_SIZE] = { 0 };

/************************** Function Prototypes ******************************/
static AL_U32 AlAxiQspi_Flash_Test(AL_VOID);
static AL_U32 AlAxiQspi_FlashReadStatusReg(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *StatusReg);
static AL_U32 AlAxiQspi_FlashReadId(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Id, AL_U32 IdSize);
static AL_U32 AlAxiQspi_FlashWren(AlAxiQspi_HalStruct *AlAxiQspiHandle);
static AL_U32 AlAxiQspi_FlashEraseChip(AlAxiQspi_HalStruct *AlAxiQspiHandle);
static AL_U32 AlAxiQspi_FlashEraseSector(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U32 Addr);
static AL_U32 AlAxiQspi_FlashWaitWip(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U32 Msleep);
static AL_U32 AlAxiQspi_FlashRead(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size);
static AL_U32 AlAxiQspi_FlashWritePage(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size);
static AL_U32 AlAxiQspi_FlashWrite(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size);


static void prvSetupHardware(void);
extern void idle_task(void);
static void vExampleTimerCallback(TimerHandle_t xTimer);

/* The queue used by the queue send and queue receive tasks. */
static QueueHandle_t xQueue = NULL;

static TaskHandle_t StartTask1_Handler;
static TaskHandle_t StartTask2_Handler;
static TaskHandle_t StartTaskAxiQspi_Handler;

void prvSetupHardware(void)
{

}

void start_task1(void* pvParameters);
void start_task2(void* pvParameters);
void axi_spi_task(void* pvParameters);

int main(void)
{
    TimerHandle_t xExampleSoftwareTimer = NULL;

    /* Configure the system ready to run the demo.  The clock configuration
    can be done here if it was not done before main() was called. */
    prvSetupHardware();

    printf("Start freertos test.\r\n");

    xQueue = xQueueCreate(/* The number of items the queue can hold. */
                 QUEUE_LENGTH,
                 /* The size of each item the queue holds. */
                 sizeof(uint32_t));

    if (xQueue == NULL) {
        printf("Unable to create xQueue due to low memory.\n");
        while (1);
    }
    xTaskCreate((TaskFunction_t)start_task1, (const char*)"start_task1",
                (uint16_t)256, (void*)NULL, (UBaseType_t)2,
                (TaskHandle_t*)&StartTask1_Handler);

    xTaskCreate((TaskFunction_t)start_task2, (const char*)"start_task2",
                (uint16_t)256, (void*)NULL, (UBaseType_t)1,
                (TaskHandle_t*)&StartTask2_Handler);

    xTaskCreate((TaskFunction_t)axi_spi_task, (const char*)"axi_spi_task",
                (uint16_t)256, (void*)NULL, (UBaseType_t)3,
                (TaskHandle_t*)&StartTaskAxiQspi_Handler);

    xExampleSoftwareTimer =
        xTimerCreate((const char*)"ExTimer", SOFTWARE_TIMER_PERIOD_MS,
                     pdTRUE, (void*)0, vExampleTimerCallback);

    xTimerStart(xExampleSoftwareTimer, 0);
    // printf("Before StartScheduler\r\n");

    vTaskStartScheduler();

    printf("OS should never run to here\r\n");

    while (1);
}

void start_task1(void* pvParameters)
{
    int cnt = 0;
    printf("Enter to task_1\r\n");
    while (1) {
        printf("task1 is running %d.....\r\n", cnt++);
        vTaskDelay(TASKDLYMS);
    }
}

void start_task2(void* pvParameters)
{
    int cnt = 0;
    printf("Enter to task_2\r\n");
    while (1) {
        printf("task2 is running %d.....\r\n", cnt++);
        vTaskDelay(TASKDLYMS);
    }
}

static void vExampleTimerCallback(TimerHandle_t xTimer)
{
    /* The timer has expired.  Count the number of times this happens.  The
    timer that calls this function is an auto re-load timer, so it will
    execute periodically. */
    static int cnt = 0;
    printf("timers Callback %d\r\n", cnt++);
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
    printf("malloc failed\n");
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
    printf("Stack Overflow\n");
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








void axi_spi_task(void* pvParameters)
{
    int cnt = 0;
    AL_U32 Status = AL_OK;
    printf("Enter to axi_spi_task\r\n");

    while (1) {
        printf("axi_spi_task is running %d.....\r\n", cnt++);
        Status = AlAxiQspi_Flash_Test();
        if (Status != AL_OK) {
            printf("axi_spi_flash_test error: 0x%x\r\n", Status);
        }
    }
}








/************************** Function Definitions ******************************/

/**
 * Main function for axi-qspi flash test.
 * Write data to flash, read data from flash, and compare the data.
 * 
 * @param  None
 * @return AL_OK on success, error code on failure, check the result by log.
 */
static AL_U32 AlAxiQspi_Flash_Test(AL_VOID)
{
    AL_U8 StatusReg = 0x0U;
    AL_U8 FlashId[3] = {0};
    AL_U32 Status = AL_OK;
    AL_U32 Index = 0;
    AL_U32 CurFlashAddr = 0;
    AL_U32 TestSize = AL_AXI_QSPI_TEST_SIZE;
    AlAxiQspi_HalStruct *AlAxiQspiHandle;

    printf("Axi-qspi flash test start...\r\n");

    AlTop_GPPortEnable();
    printf("Enable GP port.\r\n");

    Status = AlAxiQspi_Hal_Init(&AlAxiQspiHandle, 0, AL_NULL, AL_NULL);
    if (Status != AL_OK) {
        printf("Axi-qspi init error: 0x%x\r\n", Status);
        return Status;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    Status = AlAxiQspi_FlashReadId(AlAxiQspiHandle, FlashId, 3);
    if (Status != AL_OK) {
        printf("Read id error: 0x%x\r\n", Status);
        return Status;
    } else {
        printf("Read flash id: 0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
    }

    Status = AlAxiQspi_FlashReadStatusReg(AlAxiQspiHandle, &StatusReg);
    if (Status != AL_OK) {
        printf("Read status register error: 0x%x\r\n", Status);
        return Status;
    } else {
        printf("Read status reg: 0x%x\r\n", StatusReg);
    }

    Status = AlAxiQspi_FlashEraseChip(AlAxiQspiHandle);
    if (Status != AL_OK) {
        printf("Erase chip error: 0x%x\r\n", Status);
        return Status;
    } else {
        printf("Erase chip finish\r\n");
    }

    Status = AlAxiQspi_FlashRead(AlAxiQspiHandle, RecvData, 0x00, QSPI_FLASH_PAGE_SIZE);
    if (Status != AL_OK) {
        printf("Flash read error: 0x%x\r\n", Status);
        return Status;
    }

    for (Index = 0; Index < QSPI_FLASH_PAGE_SIZE; Index++) {
        if (RecvData[Index] != 0xFF) {
            printf("Data error after sector erase\r\n");
            while (1);
        }
    }

    for (Index = 0; Index < TestSize; Index++) {
        SendData[Index] = (Index + 100) % 256;
    }

    Status = AlAxiQspi_FlashReadStatusReg(AlAxiQspiHandle, &StatusReg);
    if (Status != AL_OK) {
        printf("Read status register error: 0x%x\r\n", Status);
        return Status;
    } else {
        printf("Read status reg: 0x%x\r\n", StatusReg);
    }

    for (CurFlashAddr = 0; CurFlashAddr < FLASH_SIZE_IN_BYTES; CurFlashAddr += TestSize) {
        Status = AlAxiQspi_FlashWrite(AlAxiQspiHandle, SendData, CurFlashAddr, TestSize);
        if (Status != AL_OK) {
            printf("Flash write error: 0x%x\r\n", Status);
            return Status;
        }

        if ((CurFlashAddr + TestSize) % (1024 * 1024) == 0) {
            printf("%d Mbytes have been written\r\n", 
                (CurFlashAddr + TestSize) / (1024 * 1024));
        }
    }

    for (CurFlashAddr = 0; CurFlashAddr < FLASH_SIZE_IN_BYTES; CurFlashAddr += FLASH_READ_BUF_SIZE) {
        Status = AlAxiQspi_FlashRead(AlAxiQspiHandle, RecvData, CurFlashAddr, 
            FLASH_READ_BUF_SIZE);
        if (Status != AL_OK) {
            printf("Flash read error: 0x%x\r\n", Status);
            return Status;
        }

        printf("%d Mbytes have been read, then compare\r\n", 
                (CurFlashAddr + FLASH_READ_BUF_SIZE) / (1024 * 1024));
    
        for (Index = 0; Index < FLASH_READ_BUF_SIZE; Index++) {
            if (RecvData[Index] != ((Index + 100) % 256)) {
                printf("Data error after write date at Index %d\r\n", Index);
                while (1);
            }
        }

        memset(RecvData, 0, FLASH_READ_BUF_SIZE);
    }

    printf("Axi-qspi flash test success.\r\n");

    return AL_OK;
}

static AL_U32 AlAxiQspi_FlashReadId(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Id, AL_U32 IdSize)
{
    AL_U8 TempCmd = NOR_OP_RDID;
    return AlAxiQspi_Hal_TransferDataBlock(AlAxiQspiHandle, &TempCmd, 1, Id, IdSize, TIMEOUT_FOR_TEST);
}

static AL_U32 AlAxiQspi_FlashWren(AlAxiQspi_HalStruct *AlAxiQspiHandle)
{
    AL_U8 TempCmd = NOR_OP_WREN;
    return AlAxiQspi_Hal_TransferDataBlock(AlAxiQspiHandle, &TempCmd, 1, AL_NULL, 0, TIMEOUT_FOR_TEST);
    // return AlAxiQspi_Hal_SendDataBlock(AlAxiQspiHandle, &TempCmd, 1, TIMEOUT_FOR_TEST);
}

static AL_U32 AlAxiQspi_FlashEraseChip(AlAxiQspi_HalStruct *AlAxiQspiHandle)
{
    AL_U8 TempCmd = NOR_OP_CE;
    AL_U32 Status = AL_OK;

    Status = AlAxiQspi_FlashWren(AlAxiQspiHandle);
    if (Status != AL_OK) {
        return Status;
    }

    Status = AlAxiQspi_Hal_TransferDataBlock(AlAxiQspiHandle, &TempCmd, 1, AL_NULL, 0, TIMEOUT_FOR_TEST);
    if (Status != AL_OK) {
        return Status;
    }

    Status = AlAxiQspi_FlashWaitWip(AlAxiQspiHandle, 10);
    if (Status != AL_OK) {
        return Status;
    }

    return AL_OK;
}

static AL_U32 AlAxiQspi_FlashEraseSector(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U32 Addr)
{
    AL_U8 TempArray[4] = {0};
    AL_U32 Status = AL_OK;

    Status = AlAxiQspi_FlashWren(AlAxiQspiHandle);
    if (Status != AL_OK) {
        return Status;
    }

    TempArray[0] = NOR_OP_SE;
    TempArray[1] = (Addr >> 16) & 0xFF;
    TempArray[2] = (Addr >> 8) & 0xFF;
    TempArray[3] = Addr & 0xFF;

    Status = AlAxiQspi_Hal_TransferDataBlock(AlAxiQspiHandle, TempArray, 4, AL_NULL, 0, TIMEOUT_FOR_TEST);
    // Status = AlAxiQspi_Hal_SendDataBlock(AlAxiQspiHandle, TempArray, 4, TIMEOUT_FOR_TEST);
    if (Status != AL_OK) {
        return Status;
    }

    Status = AlAxiQspi_FlashWaitWip(AlAxiQspiHandle, 1);
    if (Status != AL_OK) {
        return Status;
    }

    return AL_OK;
}

static AL_U32 AlAxiQspi_FlashReadStatusReg(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *StatusReg)
{
    AL_U8 TempCmd = NOR_OP_RDSR;
    return AlAxiQspi_Hal_TransferDataBlock(AlAxiQspiHandle, &TempCmd, 1, StatusReg, 1, TIMEOUT_FOR_TEST);
}

static AL_U32 AlAxiQspi_FlashWaitWip(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U32 Msleep)
{
    AL_U32 Status = AL_OK;
    AL_U8 TempCmd = NOR_OP_RDSR;
    AL_U8 StatusReg = 0;

    StatusReg = SR_WIP;
    while (StatusReg & SR_WIP) {
        Status = AlAxiQspi_Hal_TransferDataBlock(AlAxiQspiHandle, &TempCmd, 1, &StatusReg, 1, TIMEOUT_FOR_TEST);
        if (Status != AL_OK) {
            return Status;
        }

        if (Msleep != 0) {
            AlOsal_Sleep(Msleep);
        }
    }

    return AL_OK;
}

static AL_U32 AlAxiQspi_FlashRead(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size)
{
    AL_U8 TempArray[5] = {0};
    TempArray[0] = NOR_OP_READ_4B;
    TempArray[1] = (Addr >> 24) & 0xFF;
    TempArray[2] = (Addr >> 16) & 0xFF;
    TempArray[3] = (Addr >> 8) & 0xFF;
    TempArray[4] = Addr & 0xFF;
    return AlAxiQspi_Hal_TransferDataBlock(AlAxiQspiHandle, TempArray, 5, Data, Size, TIMEOUT_FOR_TEST);
}

static AL_U32 AlAxiQspi_FlashWritePage(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size)
{
    AL_U8 TempArray[QSPI_FLASH_PAGE_SIZE + 5] = {0};
    AL_U32 Index = 0;
    AL_U32 Status = AL_OK;

    TempArray[0] = NOR_OP_PP_4B;
    TempArray[1] = (Addr >> 24) & 0xFF;
    TempArray[2] = (Addr >> 16) & 0xFF;
    TempArray[3] = (Addr >> 8) & 0xFF;
    TempArray[4] = Addr & 0xFF;
    for (Index = 0; Index < Size; Index++) {
        TempArray[Index + 5] = Data[Index];
    }

    return AlAxiQspi_Hal_TransferDataBlock(AlAxiQspiHandle, TempArray, Size + 5, AL_NULL, 0, TIMEOUT_FOR_TEST);
}

static AL_U32 AlAxiQspi_FlashWrite(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size)
{
    AL_U32 Status = 0;
    AL_U32 SendCnt = 0;
    AL_U32 CurSendSize = 0;

    while (SendCnt < Size) {
        CurSendSize = (Size - SendCnt) > QSPI_FLASH_PAGE_SIZE ? QSPI_FLASH_PAGE_SIZE : (Size - SendCnt);

        Status = AlAxiQspi_FlashWren(AlAxiQspiHandle);
        if (Status != AL_OK) {
            return Status;
        }

        Status = AlAxiQspi_FlashWritePage(AlAxiQspiHandle, &Data[SendCnt], Addr + SendCnt, CurSendSize);
        if (Status != AL_OK) {
            return Status;
        }

        Status = AlAxiQspi_FlashWaitWip(AlAxiQspiHandle, 0);
        if (Status != AL_OK) {
            return Status;
        }

        SendCnt += CurSendSize;
    }

    return AL_OK;
}



