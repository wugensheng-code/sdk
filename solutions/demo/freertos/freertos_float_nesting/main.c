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

#include "fpu.h"
#include "al_tc_hal.h"

/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the pdMS_TO_TICKS() macro. */
#define mainSOFTWARE_TIMER_PERIOD_MS    pdMS_TO_TICKS(1000)
#define TASKDLYMS                       pdMS_TO_TICKS(100)
#define mainQUEUE_LENGTH                (1)

static void prvSetupHardware(void);
extern void idle_task(void);
static void vExampleTimerCallback(TimerHandle_t xTimer);

/* The queue used by the queue send and queue receive tasks. */
static QueueHandle_t xQueue = NULL;

static TaskHandle_t StartTask1_Handler;
static TaskHandle_t StartTask2_Handler;

void prvSetupHardware(void)
{

}



void AlTc0_Handler(void)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "tc 0 enter\r\n");

    double __fpu_context_intr_0[20];

    for (int i = 0; i < 20; i++) {  
        __fpu_context_intr_0[i] = 6.33 + i * 0.1; // 将每个元素初始化为3.14  
    }  

    double *__fpu_context = __fpu_context_intr_0;

    RESTORE_FPU_CONTEXT();


    SAVE_FPU_CONTEXT()

    AlIntr_SetLocalInterrupt(AL_FUNC_DISABLE);

    for (int i = 0; i < 20; i++) {
        printf("ttc0 __fpu_context[%d] %f\r\n", i, __fpu_context[i]);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_LOG(AL_LOG_LEVEL_INFO, "tc 0 exit\r\n");
}


void AlTc1_Handler(void)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "tc 1 enter\r\n");

    double __fpu_context_intr_1[20];

    for (int i = 0; i < 20; i++) {  
        __fpu_context_intr_1[i] = 6.33 + i * 0.1; // 将每个元素初始化为3.14  
    }  

    double *__fpu_context = __fpu_context_intr_1;

    RESTORE_FPU_CONTEXT();


    SAVE_FPU_CONTEXT()

    AlIntr_SetLocalInterrupt(AL_FUNC_DISABLE);

    for (int i = 0; i < 20; i++) {
        printf("ttc 1 __fpu_context[%d] %f\r\n", i, __fpu_context[i]);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_LOG(AL_LOG_LEVEL_INFO, "tc 1 exit\r\n");
}


void start_task1(void* pvParameters);
void start_task2(void* pvParameters);

int main(void)
{
    TimerHandle_t xExampleSoftwareTimer = NULL;

    /* Configure the system ready to run the demo.  The clock configuration
    can be done here if it was not done before main() was called. */
    prvSetupHardware();

    xQueue = xQueueCreate(/* The number of items the queue can hold. */
                 mainQUEUE_LENGTH,
                 /* The size of each item the queue holds. */
                 sizeof(uint32_t));

    if (xQueue == NULL) {
        printf("Unable to create xQueue due to low memory.\n");
        while (1);
    }
    xTaskCreate((TaskFunction_t)start_task1, (const char*)"start_task1",
                (uint16_t)512, (void*)NULL, (UBaseType_t)1,
                (TaskHandle_t*)&StartTask1_Handler);

    xTaskCreate((TaskFunction_t)start_task2, (const char*)"start_task2",
                (uint16_t)512, (void*)NULL, (UBaseType_t)1,
                (TaskHandle_t*)&StartTask2_Handler);

    xExampleSoftwareTimer =
        xTimerCreate((const char*)"ExTimer", mainSOFTWARE_TIMER_PERIOD_MS,
                     pdTRUE, (void*)0, vExampleTimerCallback);

    xTimerStart(xExampleSoftwareTimer, 0);
    // printf("Before StartScheduler\r\n");

    vTaskStartScheduler();

    printf("OS should never run to here\r\n");

    while (1);
}

void start_task1(void* pvParameters)
{

    double __fpu_context_tsk_1[20];
    AL_U32 Ret = AL_OK;
    int cnt = 0;
    AL_TC_HalStruct *TtcHandle;

    AL_TC_TimerInitStruct PwmInitConfigs = {
        .CountDec            = AL_TC_CountUp,
        .ClkSrc              = AL_TC_PCLK,
        .EnablePrescale      = AL_TRUE,
        .PrescaleVal         = 10,
    };

    Ret =  AlTc_Hal_Init(&TtcHandle, 0, &PwmInitConfigs,  AlTc0_Handler);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    AL_INTR_AttrStrct test_ttc_attr = {
            .TrigMode   =  LEVEL_HIGH_TRIGGER,
            .Priority   =  0,
            .VectorMode =  NON_VECTOR_INTERRUPT,
    };
    (AL_VOID)AlIntr_RegHandler(TtcHandle->Dev.IntrNum, &test_ttc_attr,  AlTc_Dev_IntrHandler, &TtcHandle->Dev);

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

     AlTc_Hal_EnableIntervalMode(TtcHandle);
     AlTc_Hal_SetIntervalMaxVal(TtcHandle, 10000);

     AlTc_Hal_EnableIntr(TtcHandle, AL_TC_IntrInterval, AL_TRUE);

     AlTc_Hal_EnableWaveOutput(TtcHandle, AL_TRUE);
     AlTc_Hal_EnableCounter(TtcHandle, AL_TRUE);


    for (int i = 0; i < 20; i++) {  
        __fpu_context_tsk_1[i] = 1.11 + i * 0.1; // 将每个元素初始化为3.14  
    }  

    double *__fpu_context = __fpu_context_tsk_1;

    RESTORE_FPU_CONTEXT();

    printf("Enter to task_1\r\n");
    while (1) {
        SAVE_FPU_CONTEXT()

        for (int i = 0; i < 20; i++) {
            printf("task 1 __fpu_context[%d] %f\r\n", i, __fpu_context[i]);
            vTaskDelay(TASKDLYMS);
        }
        // vTaskDelay(TASKDLYMS);
    }
}

void start_task2(void* pvParameters)
{

    double __fpu_context_tsk_2[20];

    AL_U32 Ret = AL_OK;
    int cnt = 0;
    AL_TC_HalStruct *TtcHandle;

    AL_TC_TimerInitStruct PwmInitConfigs = {
        .CountDec            = AL_TC_CountUp,
        .ClkSrc              = AL_TC_PCLK,
        .EnablePrescale      = AL_TRUE,
        .PrescaleVal         = 15,
    };

    Ret =  AlTc_Hal_Init(&TtcHandle, 1, &PwmInitConfigs,  AlTc1_Handler);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    AL_INTR_AttrStrct test_ttc_attr = {
            .TrigMode   =  LEVEL_HIGH_TRIGGER,
            .Priority   =  1,
            .VectorMode =  NON_VECTOR_INTERRUPT,
    };
    (AL_VOID)AlIntr_RegHandler(TtcHandle->Dev.IntrNum, &test_ttc_attr,  AlTc_Dev_IntrHandler, &TtcHandle->Dev);

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

     AlTc_Hal_EnableIntervalMode(TtcHandle);
     AlTc_Hal_SetIntervalMaxVal(TtcHandle, 10000);

     AlTc_Hal_EnableIntr(TtcHandle, AL_TC_IntrInterval, AL_TRUE);

     AlTc_Hal_EnableWaveOutput(TtcHandle, AL_TRUE);
     AlTc_Hal_EnableCounter(TtcHandle, AL_TRUE);

    for (int i = 0; i < 20; i++) {  
        __fpu_context_tsk_2[i] = 6.33 + i * 0.1; // 将每个元素初始化为3.14  
    }  

    double *__fpu_context = __fpu_context_tsk_2;

    RESTORE_FPU_CONTEXT();

    printf("Enter to task_2\r\n");
    while (1) {
        SAVE_FPU_CONTEXT()

        for (int j = 0; j < 20; j++) {
            printf("task 2 __fpu_context[%d] %f\r\n", j, __fpu_context[j]);
            vTaskDelay(TASKDLYMS);
        }
        // vTaskDelay(TASKDLYMS);
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
