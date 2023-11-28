/*
    FreeRTOS Kernel V10.3.1

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

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H


/* Here is a good place to include header files that are required across
your application. */
/* Configuration for Cortex A */
#define configUNIQUE_INTERRUPT_PRIORITIES       #{{configUNIQUE_INTERRUPT_PRIORITIES}}
#define configMAX_API_CALL_INTERRUPT_PRIORITY   #{{configMAX_API_CALL_INTERRUPT_PRIORITY}}
#define configSETUP_TICK_INTERRUPT()            vSetupTickInterrupt()
#define configCLEAR_TICK_INTERRUPT              clear_tick

#define USER_MODE_TASKS                         #{{USER_MODE_TASKS}}

#define configUSE_PREEMPTION                    #{{configUSE_PREEMPTION}}
#define configUSE_PORT_OPTIMISED_TASK_SELECTION #{{configUSE_PORT_OPTIMISED_TASK_SELECTION}}
#define configUSE_TICKLESS_IDLE                 #{{configUSE_TICKLESS_IDLE}}
#define configCPU_CLOCK_HZ                      #{{configCPU_CLOCK_HZ}}
#define configRTC_CLOCK_HZ                      #{{configRTC_CLOCK_HZ}}
#define configTICK_RATE_HZ                      #{{configTICK_RATE_HZ}}
#define configMAX_PRIORITIES                    #{{configMAX_PRIORITIES}}
#define configMINIMAL_STACK_SIZE                #{{configMINIMAL_STACK_SIZE}}
#define configMAX_TASK_NAME_LEN                 #{{configMAX_TASK_NAME_LEN}}
#define configUSE_16_BIT_TICKS                  #{{configUSE_16_BIT_TICKS}}
#define configIDLE_SHOULD_YIELD                 #{{configIDLE_SHOULD_YIELD}}
#define configUSE_TASK_NOTIFICATIONS            #{{configUSE_TASK_NOTIFICATIONS}}
#define configUSE_MUTEXES                       #{{configUSE_MUTEXES}}
#define configUSE_RECURSIVE_MUTEXES             #{{configUSE_RECURSIVE_MUTEXES}}
#define configUSE_COUNTING_SEMAPHORES           #{{configUSE_COUNTING_SEMAPHORES}}
#define configQUEUE_REGISTRY_SIZE               #{{configQUEUE_REGISTRY_SIZE}}
#define configUSE_QUEUE_SETS                    #{{configUSE_QUEUE_SETS}}
#define configUSE_TIME_SLICING                  #{{configUSE_TIME_SLICING}}
#define configUSE_NEWLIB_REENTRANT              #{{configUSE_NEWLIB_REENTRANT}}
#define configENABLE_BACKWARD_COMPATIBILITY     #{{configENABLE_BACKWARD_COMPATIBILITY}}
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS #{{configNUM_THREAD_LOCAL_STORAGE_POINTERS}}

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         #{{configSUPPORT_STATIC_ALLOCATION}}
#define configSUPPORT_DYNAMIC_ALLOCATION        #{{configSUPPORT_DYNAMIC_ALLOCATION}}
#define configTOTAL_HEAP_SIZE                   #{{configTOTAL_HEAP_SIZE}}
#define configAPPLICATION_ALLOCATED_HEAP        #{{configAPPLICATION_ALLOCATED_HEAP}}

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                     #{{configUSE_IDLE_HOOK}}
#define configUSE_TICK_HOOK                     #{{configUSE_TICK_HOOK}}
#define configCHECK_FOR_STACK_OVERFLOW          #{{configCHECK_FOR_STACK_OVERFLOW}}
#define configUSE_MALLOC_FAILED_HOOK            #{{configUSE_MALLOC_FAILED_HOOK}}
#define configUSE_DAEMON_TASK_STARTUP_HOOK      #{{configUSE_DAEMON_TASK_STARTUP_HOOK}}

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           #{{configGENERATE_RUN_TIME_STATS}}
#define configUSE_TRACE_FACILITY                #{{configUSE_TRACE_FACILITY}}
#define configUSE_STATS_FORMATTING_FUNCTIONS    #{{configUSE_STATS_FORMATTING_FUNCTIONS}}

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   #{{configUSE_CO_ROUTINES}}
#define configMAX_CO_ROUTINE_PRIORITIES         #{{configMAX_CO_ROUTINE_PRIORITIES}}

/* Software timer related definitions. */
#define configUSE_TIMERS                        #{{configUSE_TIMERS}}
#define configTIMER_TASK_PRIORITY               #{{configTIMER_TASK_PRIORITY}}
#define configTIMER_QUEUE_LENGTH                #{{configTIMER_QUEUE_LENGTH}}
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

#define configKERNEL_INTERRUPT_PRIORITY         #{{configKERNEL_INTERRUPT_PRIORITY}}
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    #{{configMAX_SYSCALL_INTERRUPT_PRIORITY}}

/* Define to trap errors during development. */
#define configASSERT( x ) if( ( x ) == 0 ) {taskDISABLE_INTERRUPTS(); for( ;; );}

/* FreeRTOS MPU specific definitions. */
//#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                #{{INCLUDE_vTaskPrioritySet}}
#define INCLUDE_uxTaskPriorityGet               #{{INCLUDE_uxTaskPriorityGet}}
#define INCLUDE_vTaskDelete                     #{{INCLUDE_vTaskDelete}}
#define INCLUDE_vTaskSuspend                    #{{INCLUDE_vTaskSuspend}}
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 #{{INCLUDE_vTaskDelayUntil}}
#define INCLUDE_vTaskDelay                      #{{INCLUDE_vTaskDelay}}
#define INCLUDE_xTaskGetSchedulerState          #{{INCLUDE_xTaskGetSchedulerState}}
#define INCLUDE_xTaskGetCurrentTaskHandle       #{{INCLUDE_xTaskGetCurrentTaskHandle}}
#define INCLUDE_uxTaskGetStackHighWaterMark     #{{INCLUDE_uxTaskGetStackHighWaterMark}}
#define INCLUDE_xTaskGetIdleTaskHandle          #{{INCLUDE_xTaskGetIdleTaskHandle}}
#define INCLUDE_eTaskGetState                   #{{INCLUDE_eTaskGetState}}
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          #{{INCLUDE_xTimerPendFunctionCall}}
#define INCLUDE_xTaskAbortDelay                 #{{INCLUDE_xTaskAbortDelay}}
#define INCLUDE_xTaskGetHandle                  #{{INCLUDE_xTaskGetHandle}}
#define INCLUDE_xTaskResumeFromISR              1

/* A header file that defines trace macro can be included here. */
#if (configGENERATE_RUN_TIME_STATS == 1)
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#define portGET_RUN_TIME_COUNTER_VALUE()           (AlSys_GetTimerTickCount() >> 17)
#endif

#endif /* FREERTOS_CONFIG_H */
