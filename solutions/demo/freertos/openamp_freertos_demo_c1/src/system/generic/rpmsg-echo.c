/*
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * This is a sample demonstration application that showcases usage of rpmsg
 * This application is meant to run on the remote CPU running baremetal code.
 * This application echoes back data that was sent to it by the host core.
 */
#if defined(RTOS_FREERTOS)
/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#endif

#include "al_core.h"

#include <stdlib.h>
#include <stdio.h>
#include <openamp/open_amp.h>
#include <openamp/version.h>
#include <metal/alloc.h>
#include <metal/version.h>
#include "platform_info.h"
#include "rpmsg-echo.h"

#include "al_printf.h"
#include "al_can_hal.h"
#include "al_gpio_hal.h"
#include "dry.h"
#include "al_aarch64_core.h"

#define SHUTDOWN_MSG	0xEF56A55A

#define LPRINTF(format, ...) AL_LOG(AL_LOG_LEVEL_INFO, format, ##__VA_ARGS__)
//#define LPRINTF(format, ...)
#define LPERROR(format, ...) AL_LOG(AL_LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

static struct rpmsg_endpoint lept;
static int shutdown_req = 0;

/*-----------------------------------------------------------------------------*
 *  RPMSG endpoint callbacks
 *-----------------------------------------------------------------------------*/
static int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
			     uint32_t src, void *priv)
{
	(void)priv;
	(void)src;

	/* On reception of a shutdown we signal the application to terminate */
	if ((*(unsigned int *)data) == SHUTDOWN_MSG) {
		LPRINTF("shutdown message is received.\r\n");
		
		/* Send ACK back to host */
		if (rpmsg_send(ept, data, len) < 0) {
			LPERROR("rpmsg_send failed\r\n");
		}
		LPRINTF("Send ACK back to host \r\n");
		
		shutdown_req = 1;
		return RPMSG_SUCCESS;
	}

	/* Send data back to host */
	if (rpmsg_send(ept, data, len) < 0) {
		LPERROR("rpmsg_send failed\r\n");
	}
	return RPMSG_SUCCESS;
}

static void rpmsg_service_unbind(struct rpmsg_endpoint *ept)
{
	(void)ept;
	LPRINTF("unexpected Remote endpoint destroy\r\n");
	shutdown_req = 1;
}

/*-----------------------------------------------------------------------------*
 *  Application
 *-----------------------------------------------------------------------------*/
int app(struct rpmsg_device *rdev, void *priv)
{
	int ret;

	/* Initialize RPMSG framework */
	LPRINTF("Try to create rpmsg endpoint.\r\n");
	ret = rpmsg_create_ept(&lept, rdev, RPMSG_SERVICE_NAME,
			       RPMSG_ADDR_ANY, RPMSG_ADDR_ANY,
			       rpmsg_endpoint_cb,
			       rpmsg_service_unbind);
	if (ret) {
		LPERROR("Failed to create endpoint.\r\n");
		return -1;
	}

	LPRINTF("Successfully created rpmsg endpoint.\r\n");
	while(1) {
		platform_poll(priv);
		/* we got a shutdown request, exit */
		if (shutdown_req) {
			break;
		}
	}
	rpmsg_destroy_ept(&lept);

	return 0;
}

/*-----------------------------------------------------------------------------*
 *  Application entry point
 *-----------------------------------------------------------------------------*/
void openamp_sample(void* Parameters)
{
	void *platform;
	struct rpmsg_device *rpdev;
	int ret;

	LPRINTF("openamp lib version: %s (", openamp_version());
	LPRINTF("Major: %d, ", openamp_version_major());
	LPRINTF("Minor: %d, ", openamp_version_minor());
	LPRINTF("Patch: %d)\r\n", openamp_version_patch());

	LPRINTF("libmetal lib version: %s (", metal_ver());
	LPRINTF("Major: %d, ", metal_ver_major());
	LPRINTF("Minor: %d, ", metal_ver_minor());
	LPRINTF("Patch: %d)\r\n", metal_ver_patch());

	LPRINTF("Starting application...\r\n");

	int argc = 0;
	char *argv[] = {NULL};

	/* Initialize platform */
	ret = platform_init(argc, argv, &platform);
	if (ret) {
		LPERROR("Failed to initialize platform.\r\n");
		ret = -1;
	} else {
		rpdev = platform_create_rpmsg_vdev(platform, 0,
						   VIRTIO_DEV_DEVICE,
						   NULL, NULL);
		if (!rpdev) {
			LPERROR("Failed to create rpmsg virtio device.\r\n");
			ret = -1;
		} else {
			app(rpdev, platform);
			platform_release_rpmsg_vdev(rpdev, platform);
			ret = 0;
		}
	}

	LPRINTF("Stopping application...\r\n");
	platform_cleanup(platform);

	AlGicv3_CpuIfDisable(1);
	psci_cpu_pwrdown();

	return ret;
}


#if defined(RTOS_FREERTOS)

#define TASKDLYMS                       pdMS_TO_TICKS(10000)
static TaskHandle_t openamp_sample_Handler;
static TaskHandle_t freertos_sample_Handler;
static TaskHandle_t can_sample_Handler;
static TaskHandle_t async_print_sample_Handler;

#define IntrPin 21

void freertos_sample(void* Parameters)
{
    int cnt = 0;

    al_printf("Enter to task_2\r\n");
    while (1) {
        al_printf("task2 is running %d.....\r\n", cnt++);
        drystone_main();
        vTaskDelay(TASKDLYMS);
    }
}

#define AL_CAN_EX_DEVICE_ID             (1)
#define AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS (1000)
#define AL_CAN_EX_MEM_SIZE              (1)
#define AL_CAN_EX_ARRAY_SIZE            (256)

static AL_CAN_InitStruct InLoopbackConfig = {
    .OpsMode        = AL_CAN_MODE_IN_LOOPBACK,
    .RunMode        = AL_CAN_RUN_INTR,
    .Type           = AL_CAN_TYPE_2_0B,
    .SlowBitRate    = AL_CAN_ARBITRATION_0_25M,
    .TransMode      = AL_CAN_TRANS_PTB,
    .RbAfwl         = AL_CAN_RB_LIMIT_8
};

static AL_CAN_FrameStruct ExFrame = {
    .Id             = 0x10,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_8,
    .Data           = {0x01020304, 0x05060708}
};
void can_sample(void* Parameters)
{
	AL_U32 Count = 0;
	AL_U32 Ret = AL_OK;
    AL_CAN_FrameStruct Frame;
    AL_CAN_HalStruct *Handle = AL_NULL;
    al_printf("Enter to task_3\r\n");

	al_printf("Task3 Can send hal init\r\n");
	Ret = AlCan_Hal_Init(&Handle, AL_CAN_EX_DEVICE_ID, &InLoopbackConfig, AL_NULL);
    if (Ret != AL_OK) {
        al_printf("Hal Init error:0x%x\r\n", Ret);
    }
	al_printf("Task3 Can send hal init success\r\n");

	while (1) {
		al_printf("Task3 Can send count:%d\r\n", Count++);
        Ret = AlCan_Hal_SendFrameBlock(Handle, &ExFrame, AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS);
        if (Ret != AL_OK) {
            al_printf("Send Frame Error:0x%x\r\n", Ret);
        }

        Ret = AlCan_Hal_RecvFrameBlock(Handle, &Frame, AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS);
        if (Ret != AL_OK) {
            al_printf("Recv Frame Error:0x%x\r\n", Ret);
        } else {
            AlCan_Dev_DisplayFrame(&Frame);

            Ret = memcmp(ExFrame.Data, Frame.Data, AL_CAN_LEN_8);
            if (Ret != AL_OK) {
                al_printf("Data check error:0x%x\r\n", Ret);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void async_print_sample(void* pvParameters)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Async print init success\r\n");

    while (1) {
        AlPrint_AsyncPrintf();
        vTaskDelay(pdMS_TO_TICKS(50));
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
    AL_LOG(AL_LOG_LEVEL_INFO, "malloc failed\n");
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
    AL_LOG(AL_LOG_LEVEL_INFO, "Stack Overflow\n");
    while (1);
}
/*-----------------------------------------------------------*/

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

int start_freertos(void)
{

    xTaskCreate((TaskFunction_t)openamp_sample, (const char*)"start_task1",
                (uint16_t)1024, (void*)NULL, (UBaseType_t)2,
                (TaskHandle_t*)&openamp_sample_Handler);

    xTaskCreate((TaskFunction_t)freertos_sample, (const char*)"start_task2",
                (uint16_t)1024, (void*)NULL, (UBaseType_t)3,
                (TaskHandle_t*)&freertos_sample_Handler);

	xTaskCreate((TaskFunction_t)can_sample, (const char*)"start_task3",
                (uint16_t)1024, (void*)NULL, (UBaseType_t)4,
                (TaskHandle_t*)&can_sample_Handler);

	// xTaskCreate((TaskFunction_t)async_print_sample, (const char*)"start_task4",
    //             (uint16_t)1024, (void*)NULL, (UBaseType_t)5,
    //             (TaskHandle_t*)&async_print_sample_Handler);

    vTaskStartScheduler();

    AL_LOG(AL_LOG_LEVEL_INFO, "OS should never run to here\r\n");

    while (1);
}

#endif

int main()
{

#if defined(RTOS_FREERTOS)

	start_freertos();

#else

	openamp_sample((void*)AL_NULL);

#endif

	return 0;
}

