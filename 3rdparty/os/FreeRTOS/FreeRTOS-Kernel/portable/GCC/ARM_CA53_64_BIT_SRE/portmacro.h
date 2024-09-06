/*
 * FreeRTOS Kernel V10.5.1
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <assert.h>
/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the given hardware
 * and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#define portCHAR char
#define portFLOAT float
#define portDOUBLE double
#define portLONG long
#define portSHORT short
#define portSTACK_TYPE size_t
#define portBASE_TYPE long

    typedef portSTACK_TYPE StackType_t;
    typedef portBASE_TYPE BaseType_t;
    typedef uint64_t UBaseType_t;

    typedef uint64_t TickType_t;
#define portMAX_DELAY ((TickType_t)0xffffffffffffffff)

/* 32-bit tick type on a 32-bit architecture, so reads of the tick count do
not need to be guarded with a critical section. */
#define portTICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Hardware specifics. */
#define portSTACK_GROWTH (-1)
#define portTICK_PERIOD_MS ((TickType_t)1000 / configTICK_RATE_HZ)
#define portBYTE_ALIGNMENT 16
#define portPOINTER_SIZE_TYPE uint64_t

/*-----------------------------------------------------------*/
#ifdef SMP

    // static inline void vPortRecursiveLock(uint32_t ulLockNum, BaseType_t uxAcquire)
    // {
    // #warning "Not implemented"
    // }

    static inline UBaseType_t uxDisableInterrupts()
    {
        unsigned long flags;

        __asm volatile("mrs %0, daif\n"
                       "msr daifset, #2\n"
                       //        "dsb sy\n"
                       //        "isb sy\n"
                       : "=r"(flags)
                       :
                       : "memory");

        return flags;
    }

    static inline void vEnableInterrupts()
    {
        __asm volatile("mrs x0, daif\n"
                       "msr daifclr, #2\n"
                       //        "dsb sy\n"
                       //        "isb sy\n"
                       :
                       :
                       : "memory");
    }

    static inline void vRestoreInterrupts(UBaseType_t flags)
    {
        __asm volatile("and x2, %0, #128\n"
                       "mrs x1, daif\n"
                       "bic x1, x1, #128\n"
                       "orr x1, x1, x2\n"
                       "msr daif, x1\n"
                       //        "dsb sy\n"
                       //        "isb sy\n"
                       :
                       : "r"(flags)
                       : "memory");
    }

    static inline BaseType_t xPortGetCoreID()
    {
        register BaseType_t xCoreID;

        __asm volatile("mrs  x0, mpidr_el1\n"
                       "and  %0, x0, #0xff\n"
                       : "=r"(xCoreID)
                       :
                       : "memory");

        return xCoreID;
    }

#endif

    /*-----------------------------------------------------------*/

    /* Task utilities. */
    extern uint64_t ullPortYieldRequired[];
/* Called at the end of an ISR that can cause a context switch. */
#define portEND_SWITCHING_ISR(xSwitchRequired)                                                                         \
    {                                                                                                                  \
                                                                                                                       \
        if (xSwitchRequired != pdFALSE)                                                                                \
        {                                                                                                              \
            ullPortYieldRequired[portGET_CORE_ID()] = pdTRUE;                                                          \
        }                                                                                                              \
    }

#define portYIELD_FROM_ISR(x) portEND_SWITCHING_ISR(x)
#if defined(GUEST)
#define portYIELD() __asm volatile("SVC 0" ::: "memory")
#else
#define portYIELD() __asm volatile("SMC 0" ::: "memory")
#endif
    /*-----------------------------------------------------------
     * Critical section control
     *----------------------------------------------------------*/

    extern void vPortEnterCritical(void);
    extern void vPortExitCritical(void);
    extern UBaseType_t uxPortSetInterruptMask(void);
    extern void vPortClearInterruptMask(UBaseType_t uxNewMaskValue);
    extern void vPortInstallFreeRTOSVectorTable(void);

#define portDISABLE_INTERRUPTS()                                                                                       \
    __asm volatile("MSR DAIFSET, #3" ::: "memory");                                                                    \
    __asm volatile("DSB SY");                                                                                          \
    __asm volatile("ISB SY");

#define portENABLE_INTERRUPTS()                                                                                        \
    __asm volatile("MSR DAIFCLR, #3" ::: "memory");                                                                    \
    __asm volatile("DSB SY");                                                                                          \
    __asm volatile("ISB SY");

/* These macros do not globally disable/enable interrupts.  They do mask off
interrupts that have a priority below configMAX_API_CALL_INTERRUPT_PRIORITY. */
#define portENTER_CRITICAL() vTaskEnterCritical();
#define portEXIT_CRITICAL() vTaskExitCritical();
#define portSET_INTERRUPT_MASK_FROM_ISR() uxPortSetInterruptMask()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x) vPortClearInterruptMask(x)

/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site.  These are
not required for this port but included in case common demo code that uses these
macros is used. */
#define portTASK_FUNCTION_PROTO(vFunction, pvParameters) void vFunction(void *pvParameters)
#define portTASK_FUNCTION(vFunction, pvParameters) void vFunction(void *pvParameters)

    /* Prototype of the FreeRTOS tick handler.  This must be installed as the
    handler for whichever peripheral is used to generate the RTOS tick. */
    void FreeRTOS_Tick_Handler(void);

    /* Any task that uses the floating point unit MUST call vPortTaskUsesFPU()
    before any floating point instructions are executed. */
    void vPortTaskUsesFPU(void);
#define portTASK_USES_FLOATING_POINT() vPortTaskUsesFPU()

#define portLOWEST_INTERRUPT_PRIORITY (((uint32_t)configUNIQUE_INTERRUPT_PRIORITIES) - 1UL)
#define portLOWEST_USABLE_INTERRUPT_PRIORITY (portLOWEST_INTERRUPT_PRIORITY - 1UL)

/* Architecture specific optimisations. */
#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#endif

#if configUSE_PORT_OPTIMISED_TASK_SELECTION == 1

/* Store/clear the ready priorities in a bit map. */
#define portRECORD_READY_PRIORITY(uxPriority, uxReadyPriorities) (uxReadyPriorities) |= (1UL << (uxPriority))
#define portRESET_READY_PRIORITY(uxPriority, uxReadyPriorities) (uxReadyPriorities) &= ~(1UL << (uxPriority))

    /*-----------------------------------------------------------*/

#define portGET_HIGHEST_PRIORITY(uxTopPriority, uxReadyPriorities)                                                     \
    uxTopPriority = (31 - __builtin_clz(uxReadyPriorities))

#endif /* configUSE_PORT_OPTIMISED_TASK_SELECTION */

#ifdef configASSERT
    void vPortValidateInterruptPriority(void);
#define portASSERT_IF_INTERRUPT_PRIORITY_INVALID() vPortValidateInterruptPriority()
#endif /* configASSERT */

#ifdef SMP

#define portGET_CORE_ID() xPortGetCoreID()
#define portDISABLE_INTERRUPTS() uxDisableInterrupts()
#define portENABLE_INTERRUPTS() vEnableInterrupts()
#define portSET_INTERRUPT_MASK() uxDisableInterrupts()
#define portCLEAR_INTERRUPT_MASK(x) vRestoreInterrupts(x)

#define portHAS_NESTED_INTERRUPTS 1
    UBaseType_t uxPortSetInterruptMask(void);
    void vPortClearInterruptMask(UBaseType_t);
#define portSET_INTERRUPT_MASK_FROM_ISR() uxPortSetInterruptMask()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x) vPortClearInterruptMask(x)

#define portRELEASE_ISR_LOCK() vPortRecursiveLock(ISR_LOCK, pdFALSE)
#define portGET_ISR_LOCK() vPortRecursiveLock(ISR_LOCK, pdTRUE)

#define portRELEASE_TASK_LOCK() vPortRecursiveLock(TASK_LOCK, pdFALSE)
#define portGET_TASK_LOCK() vPortRecursiveLock(TASK_LOCK, pdTRUE)

    extern void vInterruptCore(uint32_t ulInterruptID, uint32_t ulCoreID);
/* Use PPI 0 as the yield core intrrupt */
#define portYIELD_CORE_INT_ID 15
#define portYIELD_CORE(xCoreID) vInterruptCore(portYIELD_CORE_INT_ID, (uint32_t)xCoreID)

#define portENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#define portEXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

#define portCRITICAL_NESTING_IN_TCB 1
#endif

#define portNOP() __asm volatile("NOP")
#define portINLINE __inline

/*-----------------------------------------------------------
 * Critical section locks
 *----------------------------------------------------------*/
#define ISR_LOCK (0u)
#define TASK_LOCK (1u)
#define portRTOS_LOCK_COUNT (2u)
#define portMAX_CORE_COUNT configNUMBER_OF_CORES

    /* Which core owns the lock */
    volatile static uint64_t ucOwnedByCore[portMAX_CORE_COUNT];
    /* Lock count a core owns */
    volatile static uint64_t ucRecursionCountByLock[portRTOS_LOCK_COUNT];
    /* Index 0 is used for ISR lock and Index 1 is used for task lock */
    static uint32_t ulGateWord[portRTOS_LOCK_COUNT];

    void vSpinLock(uint32_t *ulGateWord);
    int32_t lSpinTrylock(uint32_t *ulGateWord);
    void vSpinUnlock(uint32_t *ulGateWord);

    /* Read 64b value shared between cores */
    static inline uint64_t uxGet64(volatile uint64_t *x)
    {
        __asm("dsb sy");
        return *x;
    }

    /* Write 64b value shared between cores */
    static inline void vSet64(volatile uint64_t *x, uint64_t value)
    {
        *x = value;
        __asm("dsb sy");
    }

    // TODO inline spinlocks

    static inline void vPortRecursiveLock(uint32_t ulLockNum, BaseType_t uxAcquire)
    {
        uint32_t ulCoreNum = (uint32_t)portGET_CORE_ID();
        uint32_t ulLockBit = 1u << ulLockNum;

        /* Lock acquire */
        if (uxAcquire)
        {
            /* Check if spinlock is available */
            /* If spinlock is not available check if the core owns the lock */
            /* If the core owns the lock wait increment the lock count by the core */
            /* If core does not own the lock wait for the spinlock */
            if (lSpinTrylock(&ulGateWord[ulLockNum]) != 0)
            {
                /* Check if the core owns the spinlock */
                if (uxGet64(&ucOwnedByCore[ulCoreNum]) & ulLockBit)
                {
                    // configASSERT(uxGet64(&ucRecursionCountByLock[ulLockNum]) != 255u);
                    assert(uxGet64(&ucRecursionCountByLock[ulLockNum]) != 255u);
                    vSet64(&ucRecursionCountByLock[ulLockNum], (uxGet64(&ucRecursionCountByLock[ulLockNum]) + 1));
                    return;
                }

                /* Preload the gate word into the cache */
                uint32_t dummy = ulGateWord[ulLockNum];
                dummy++;

                while (lSpinTrylock(&ulGateWord[ulLockNum]) != 0)
                    __asm volatile("wfe");
            }

            /* Add barrier to ensure lock is taken before we proceed */
            __asm__ __volatile__("dmb sy" ::: "memory");

            /* Assert the lock count is 0 when the spinlock is free and is acquired */
            // configASSERT(uxGet64(&ucRecursionCountByLock[ulLockNum]) == 0);
            assert(uxGet64(&ucRecursionCountByLock[ulLockNum]) == 0);

            /* Set lock count as 1 */
            vSet64(&ucRecursionCountByLock[ulLockNum], 1);
            /* Set ucOwnedByCore */
            vSet64(&ucOwnedByCore[ulCoreNum], (uxGet64(&ucOwnedByCore[ulCoreNum]) | ulLockBit));
        }
        /* Lock release */
        else
        {
            /* Assert the lock is not free already */
            // configASSERT((uxGet64(&ucOwnedByCore[ulCoreNum]) & ulLockBit) != 0);
            assert(uxGet64(&ucRecursionCountByLock[ulLockNum]) != 255u);
            // configASSERT(uxGet64(&ucRecursionCountByLock[ulLockNum]) != 0);
            assert(uxGet64(&ucRecursionCountByLock[ulLockNum]) != 0);

            /* Reduce ucRecursionCountByLock by 1 */
            vSet64(&ucRecursionCountByLock[ulLockNum], (uxGet64(&ucRecursionCountByLock[ulLockNum]) - 1));

            if (!uxGet64(&ucRecursionCountByLock[ulLockNum]))
            {
                vSet64(&ucOwnedByCore[ulCoreNum], (uxGet64(&ucOwnedByCore[ulCoreNum]) & ~ulLockBit));
                vSpinUnlock(&ulGateWord[ulLockNum]);
                __asm volatile("sev");
                /* Add barrier to ensure lock is taken before we proceed */
                __asm__ __volatile__("dmb sy" ::: "memory");
            }
        }
    }

/* The number of bits to shift for an interrupt priority is dependent on the
number of bits implemented by the interrupt controller. */
#if configUNIQUE_INTERRUPT_PRIORITIES == 16
#define portPRIORITY_SHIFT 4
#define portMAX_BINARY_POINT_VALUE 3
#elif configUNIQUE_INTERRUPT_PRIORITIES == 32
#define portPRIORITY_SHIFT 3
#define portMAX_BINARY_POINT_VALUE 2
#elif configUNIQUE_INTERRUPT_PRIORITIES == 64
#define portPRIORITY_SHIFT 2
#define portMAX_BINARY_POINT_VALUE 1
#elif configUNIQUE_INTERRUPT_PRIORITIES == 128
#define portPRIORITY_SHIFT 1
#define portMAX_BINARY_POINT_VALUE 0
#elif configUNIQUE_INTERRUPT_PRIORITIES == 256
#define portPRIORITY_SHIFT 0
#define portMAX_BINARY_POINT_VALUE 0
#else
#error Invalid configUNIQUE_INTERRUPT_PRIORITIES setting.  configUNIQUE_INTERRUPT_PRIORITIES must be set to the number of unique priorities implemented by the target hardware
#endif

#define portMEMORY_BARRIER() __asm volatile("" ::: "memory")

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* PORTMACRO_H */
