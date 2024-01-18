/* FreeRTOS kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */

#include <stdio.h>
#include <stdlib.h>

#include "al_core.h"
#include "al_usb_hal.h"

#define mainSOFTWARE_TIMER_PERIOD_MS    pdMS_TO_TICKS(1000)
#define TASKDLYMS                       pdMS_TO_TICKS(100)
#define mainQUEUE_LENGTH                (1)


#ifdef ENABLE_MMU
#include "al_mmu.h"
#endif

#define AL_USB_DEVICE_ID     0

extern void usbh_initialize(void);
extern void usbh_class_test(void);

extern void USBH_IRQHandler(void);
AL_VOID AlUsb_Dev_IntrHandler(AL_VOID *Instance)
{
    USBH_IRQHandler();
}

/* The queue used by the queue send and queue receive tasks. */
static QueueHandle_t xQueue = NULL;

static TaskHandle_t StartTask1_Handler;
static TaskHandle_t StartTask2_Handler;

void start_task1(void* pvParameters)
{
    int cnt = 0;
    printf("Enter to task_1\r\n");
    while (1) {
        //printf("task1 is running %d.....\r\n", cnt++);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void start_task2(void* pvParameters)
{
    AL_USB_HalStruct *USB0;
    AL_S32 ret = AlUsb_Hal_Init(&USB0, AL_USB_DEVICE_ID, AlUsb_Dev_IntrHandler);
    if (ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlUsb_Hal_Init success");
    }
    else {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlUsb_Hal_Init failed");
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    printf("Enter to task_2 USB MSC INIT \r\n");

    usbh_initialize();
    usbh_class_test();

    int cnt = 0;
    printf("Enter to task_2\r\n");
    while (1) {
        //printf("task2 is running %d.....\r\n", cnt++);
        vTaskDelay(pdMS_TO_TICKS(6000));
    }
}

int main()
{
    AL_S8 *str = "Hello World From Anlogic!";

    printf("str = %s \r\n", str);

#if ENABLE_MMU
    printf("MMU ENABLE \r\n");
    extern AL_U32 _no_cache_section_start;
    mmu_settlb((AL_UINTPTR) &(_no_cache_section_start), NORM_NONCACHE);
#else
    printf("MMU DISABLE \r\n");
    AlCache_DisableMmu();
#endif

    TimerHandle_t xExampleSoftwareTimer = NULL;

    /* Configure the system ready to run the demo.  The clock configuration
    can be done here if it was not done before main() was called. */
    //prvSetupHardware();

    xQueue = xQueueCreate(/* The number of items the queue can hold. */
                 mainQUEUE_LENGTH,
                 /* The size of each item the queue holds. */
                 sizeof(uint32_t));

    if (xQueue == NULL) {
        printf("Unable to create xQueue due to low memory.\n");
        while (1);
    }
    xTaskCreate((TaskFunction_t)start_task1, (const char*)"start_task1",
                (uint16_t)256, (void*)NULL, (UBaseType_t)1,
                (TaskHandle_t*)&StartTask1_Handler);
    
    xTaskCreate((TaskFunction_t)start_task2, (const char*)"start_task2",
                (uint16_t)256, (void*)NULL, (UBaseType_t)2,
                (TaskHandle_t*)&StartTask2_Handler);

    vTaskStartScheduler();

    for (;;) {
        ;
    }
    return 0;
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