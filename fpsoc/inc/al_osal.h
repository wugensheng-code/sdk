/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*****************************************************************************
*****************************************************************************/

#ifndef __AL_OSAL_H_
#define __AL_OSAL_H_

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "al_errno.h"
#include "al_type.h"
#include "al_intr.h"

#ifdef USE_RTOS
#ifdef RTOS_RTTHREAD

#include <rtthread.h>
#include <rthw.h>

/*----------------------------------------------*
 * MUTEX API.*
 *----------------------------------------------*/

#define AL_WAITFOREVER           (AL_U64)RT_WAITING_FOREVER
#define AL_WAITING_NO            RT_WAITING_NO
typedef struct
{
    struct rt_mutex     Thread_Lock;
    AL_S64              Isr_Lock;
} AL_Lock;
typedef AL_Lock* AL_Lock_t;

static inline AL_S32 AlOsal_Lock_Init(AL_Lock_t Lock, const char* Name)
{
    return rt_mutex_init(&Lock->Thread_Lock, Name, RT_IPC_FLAG_PRIO);
}

static inline AL_S32 AlOsal_Lock_Take(AL_Lock_t Lock, AL_U64 Timeout)
{
    /* If the scheduler is started and in thread context */
    if (rt_interrupt_get_nest() == 0 && rt_thread_self() != RT_NULL) {
        return rt_mutex_take(&Lock->Thread_Lock, Timeout);
    }
    else {
        Lock->Isr_Lock = rt_hw_interrupt_disable();
    }
}

static inline AL_S32 AlOsal_Lock_Release(AL_Lock_t Lock)
{
    /* If the scheduler is started and in thread context */
    if (rt_interrupt_get_nest() == 0 && rt_thread_self() != RT_NULL) {
        return rt_mutex_release(&Lock->Thread_Lock);
    }
    else {
        rt_hw_interrupt_enable(Lock->Isr_Lock);
    }
}

/*----------------------------------------------*
 * Semaphore API.*
 *----------------------------------------------*/

typedef struct rt_semaphore  AL_Semaphore;
typedef struct rt_semaphore* AL_Semaphore_t;

static inline AL_S32 AlOsal_Sem_Init(AL_Semaphore_t Semaphore, const char* Name, AL_S32 Value)
{
    return rt_sem_init(Semaphore, Name, Value, RT_IPC_FLAG_PRIO);
}

static inline AL_S32 AlOsal_Sem_Take(AL_Semaphore_t Semaphore, AL_U64 Timeout)
{
    return rt_sem_take(Semaphore, Timeout);
}

static inline AL_S32 AlOsal_Sem_Release(AL_Semaphore_t Semaphore)
{
    return rt_sem_release(Semaphore);
}

/*----------------------------------------------*
 * Mailbox API.*
 *----------------------------------------------*/

typedef struct
{
    AL_Semaphore    Semaphore;
    AL_U64          Msg;
    AL_S32          size;
    volatile AL_S16 entry;                         /**< index of messages in msg_pool */
} AL_MailBox;
typedef AL_MailBox*  AL_MailBox_t;

static inline AL_S32 AlOsal_Mb_Init(AL_MailBox_t MailBox, const char* Name)
{
    AL_S64 Ret = AL_OK;

    Ret = rt_sem_init(&MailBox->Semaphore, Name, 0, RT_IPC_FLAG_PRIO);
    if (Ret != AL_OK) {
        return Ret;
    }

    MailBox->Msg   = 0;
    MailBox->size  = 1;
    MailBox->entry = 0;

    return Ret;

}

static inline AL_S32 AlOsal_Mb_Send(AL_MailBox_t MailBox, AL_VOID * Msg)
{
    MailBox->Msg = *(AL_U64 *)Msg;
    __COMPILER_BARRIER();
    MailBox->entry = 1;

    return rt_sem_release(&MailBox->Semaphore);
}

static inline AL_S32 AlOsal_Mb_Receive(AL_MailBox_t MailBox, AL_VOID* Msg, AL_U64 Timeout)
{
    AL_S32 flag = (AL_S32)rt_sem_take(&MailBox->Semaphore, AL_WAITFOREVER);

    if (flag == AL_OK) {
        MailBox->entry = 0;
        __COMPILER_BARRIER();
        *(AL_U64 *)Msg = MailBox->Msg;
        return AL_OK;
    }

    return AL_ERR_UNAVAILABLE;
}

/*----------------------------------------------*
 * Critical API for specific devices*
 *----------------------------------------------*/

static inline AL_VOID AlOsal_EnterDevCtritical(AL_U32 DevIntrId, AL_BOOL Condition)
{
    (AL_VOID)DevIntrId;
    (AL_VOID)Condition;
    rt_enter_critical();
}


static inline AL_VOID AlOsal_ExitDevCtritical(AL_U32 DevIntrId, AL_BOOL Condition)
{
    (AL_VOID)DevIntrId;
    (AL_VOID)Condition;
    rt_exit_critical();
}


/*----------------------------------------------*
 * Critical API.*
 *----------------------------------------------*/

static inline AL_U64 ALOsal_EnterCritical(AL_VOID)
{
    rt_enter_critical();
    return AL_OK;
}

static inline AL_VOID ALOsal_ExitCritical(AL_U64 flag)
{
    rt_exit_critical();
}

/*----------------------------------------------*
 * Critical API.*
 *----------------------------------------------*/

static inline AL_VOID AlOsal_Sleep(AL_U32 Time)
{
    /* If the scheduler is started and in thread context */
    if (rt_interrupt_get_nest() == 0 && rt_thread_self() != RT_NULL) {
        return rt_thread_mdelay(Time);
    }
    else {
        return;
    }
}

#endif
#ifdef RTOS_FREERTOS

#include <FreeRTOS.h>
#include "semphr.h"

extern volatile uint64_t ullCriticalNesting;
extern volatile uint64_t ullPortInterruptNesting;
/*----------------------------------------------*
 * MUTEX API.*
 *----------------------------------------------*/

#define AL_WAITFOREVER           portMAX_DELAY
#define AL_WAITING_NO            0
typedef struct
{
    StaticSemaphore_t        Thread_Lock_buffer;
    SemaphoreHandle_t        Thread_Lock;
    AL_S64                   Isr_Lock;
} AL_Lock;
typedef AL_Lock* AL_Lock_t;

static inline AL_S32 AlOsal_Lock_Init(AL_Lock_t Lock, const char* Name)
{
    Lock->Thread_Lock = xSemaphoreCreateMutexStatic(&Lock->Thread_Lock_buffer);

    configASSERT(Lock->Thread_Lock);

    return AL_OK;
}

static inline AL_S32 AlOsal_Lock_Take(AL_Lock_t Lock, AL_U64 Timeout)
{
    /* If the scheduler is started and in thread context */
    if (ullPortInterruptNesting == 0) {
        return !xSemaphoreTake(Lock->Thread_Lock, (TickType_t) Timeout);
    } else {
        AL_S32 Ret;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        Ret = xSemaphoreTakeFromISR(Lock->Thread_Lock, &xHigherPriorityTaskWoken);
        if (Ret == pdPASS) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            return AL_OK;
        } else {
            return AL_ERR_UNAVAILABLE;
        }
    }
}

static inline AL_S32 AlOsal_Lock_Release(AL_Lock_t Lock)
{
    /* If the scheduler is started and in thread context */
    if (ullPortInterruptNesting == 0) {
        return !xSemaphoreGive(Lock->Thread_Lock);
    } else {
        AL_S32 Ret;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        Ret = xSemaphoreGiveFromISR(Lock->Thread_Lock, &xHigherPriorityTaskWoken);
        if (Ret == pdPASS) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            return AL_OK;
        } else {
            return AL_ERR_UNAVAILABLE;
        }
    }
}

/*----------------------------------------------*
 * Semaphore API.*
 *----------------------------------------------*/
typedef struct
{
    StaticSemaphore_t Semaphore_Buffer;
    SemaphoreHandle_t Semaphore_Handle;
} AL_Semaphore;

typedef AL_Semaphore* AL_Semaphore_t;

static inline AL_S32 AlOsal_Sem_Init(AL_Semaphore_t Semaphore, const char* Name, AL_S32 Value, AL_S64 MaxCount)
{
    AL_UNUSED(Name);

    Semaphore->Semaphore_Handle = xSemaphoreCreateCountingStatic(MaxCount, Value, &Semaphore->Semaphore_Buffer);

    configASSERT(Semaphore->Semaphore_Handle);

    return AL_OK;
}

static inline AL_S32 AlOsal_Sem_Take(AL_Semaphore_t Semaphore, AL_U64 Timeout)
{
    return !xSemaphoreTake(Semaphore->Semaphore_Handle, (TickType_t) Timeout);
}

static inline AL_S32 AlOsal_Sem_Release(AL_Semaphore_t Semaphore)
{
    return !xSemaphoreGive(Semaphore->Semaphore_Handle);
}

/*----------------------------------------------*
 * Mailbox API.*
 *----------------------------------------------*/

typedef struct
{
    AL_Semaphore    Semaphore;
    AL_U64          Msg;
    AL_S32          size;
    volatile AL_S16 entry;                         /**< index of messages in msg_pool */
} AL_MailBox;
typedef AL_MailBox*  AL_MailBox_t;

static inline AL_S32 AlOsal_Mb_Init(AL_MailBox_t MailBox, const char* Name)
{
    MailBox->Semaphore.Semaphore_Handle = xSemaphoreCreateCountingStatic(1, 0, &MailBox->Semaphore.Semaphore_Buffer);

    configASSERT(MailBox->Semaphore.Semaphore_Handle);

    MailBox->Msg   = 0;
    MailBox->size  = 1;
    MailBox->entry = 0;

    return AL_OK;

}

static inline AL_S32 AlOsal_Mb_Send(AL_MailBox_t MailBox, AL_VOID * Msg)
{
    MailBox->Msg = *(AL_U64 *)Msg;
    __COMPILER_BARRIER();
    MailBox->entry = 1;

    /* If the scheduler is started and in thread context */
    if (ullPortInterruptNesting == 0) {
        return !xSemaphoreGive(MailBox->Semaphore.Semaphore_Handle);
    } else {
        AL_S32 Ret;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        Ret = xSemaphoreGiveFromISR(MailBox->Semaphore.Semaphore_Handle, &xHigherPriorityTaskWoken);
        if (Ret == pdPASS) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
        return AL_OK;
    }
}

static inline AL_S32 AlOsal_Mb_Receive(AL_MailBox_t MailBox, AL_VOID* Msg, AL_U64 Timeout)
{
    AL_S32 Ret;
    /* If the scheduler is started and in thread context */
    if (ullPortInterruptNesting == 0) {
        Ret = !xSemaphoreTake(MailBox->Semaphore.Semaphore_Handle, (TickType_t) Timeout);
    } else {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        Ret = xSemaphoreTakeFromISR(MailBox->Semaphore.Semaphore_Handle, &xHigherPriorityTaskWoken);
        if (Ret == pdPASS) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            Ret = AL_OK;
        } else {
            Ret = AL_ERR_UNAVAILABLE;
        }
    }

    if (Ret == AL_OK) {
        MailBox->entry = 0;
        __COMPILER_BARRIER();
        *(AL_U64 *)Msg = MailBox->Msg;
        return AL_OK;
    }

    return AL_ERR_UNAVAILABLE;
}

/*----------------------------------------------*
 * Critical API for specific devices*
 *----------------------------------------------*/

static inline AL_VOID AlOsal_EnterDevCtritical(AL_U32 DevIntrId, AL_BOOL Condition)
{
    if (Condition) {
        AlIntr_SetInterrupt(DevIntrId, AL_FUNC_DISABLE);
    }
}


static inline AL_VOID AlOsal_ExitDevCtritical(AL_U32 DevIntrId, AL_BOOL Condition)
{
    if (Condition) {
        AlIntr_SetInterrupt(DevIntrId, AL_FUNC_ENABLE);
    }
}

/*----------------------------------------------*
 * Critical API.*
 *----------------------------------------------*/

static inline AL_U64 ALOsal_EnterCritical(AL_VOID)
{
    return AlIntr_SaveLocalInterruptMask();
}

static inline AL_VOID ALOsal_ExitCritical(AL_U64 Flag)
{
    AL_UNUSED(Flag);
    (AL_VOID)AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
}

/*----------------------------------------------*
 * Critical API.*
 *----------------------------------------------*/

static inline AL_VOID AlOsal_Sleep(AL_U32 Time)
{
    /* If the scheduler is started and in thread context */
    if (ullCriticalNesting == 0)
    {
        return vTaskDelay(Time);
    }
    else
    {
        return;
    }
}

#endif
#else

/*----------------------------------------------*
 * Semaphore API.*
 *----------------------------------------------*/

#define AL_WAITFOREVER          (AL_U64)(-1UL)
#define AL_WAITING_NO           (0)
typedef struct
{
  volatile AL_S32 count;
}AL_Semaphore;
typedef volatile AL_Semaphore* AL_Semaphore_t;

static inline AL_S32 AlOsal_Sem_Init(AL_Semaphore_t Semaphore, const char* Name, AL_S32 Value)
{
    AL_UNUSED(Name);

    Semaphore->count = Value;

    return AL_OK;
}

static inline AL_S32 AlOsal_Sem_Take(AL_Semaphore_t Semaphore, AL_U64 Timeout)
{
    AL_WAIT_COND_UNTIL_TIMEOUT((Semaphore->count > 0), Timeout);

    Semaphore->count--;

    return AL_OK;
}

static inline AL_S32 AlOsal_Sem_Release(AL_Semaphore_t Semaphore)
{
    Semaphore->count++;

    return AL_OK;
}

/*----------------------------------------------*
 * MUTEX API.*
 *----------------------------------------------*/

typedef AL_Semaphore AL_Lock;
typedef AL_Semaphore_t AL_Lock_t;

static inline AL_S32 AlOsal_Lock_Init(AL_Lock_t Lock, const char* Name)
{
    AL_UNUSED(Name);

    AL_UNUSED(Lock);

    return AL_OK;
}

static inline AL_S32 AlOsal_Lock_Take(AL_Lock_t Lock, AL_U64 Timeout)
{
    AL_UNUSED(Timeout);

    AL_UNUSED(Lock);

    return AL_OK;
}

static inline AL_S32 AlOsal_Lock_Release(AL_Lock_t Lock)
{
    AL_UNUSED(Lock);

    return AL_OK;
}

/*----------------------------------------------*
 * MailBox API.*
 *----------------------------------------------*/

typedef struct
{
    AL_U64          Msg;
    AL_S32          size;
    volatile AL_S16 entry;                         /**< index of messages in msg_pool */
} AL_MailBox;
typedef volatile AL_MailBox*  AL_MailBox_t;

static inline AL_S32 AlOsal_Mb_Init(AL_MailBox_t MailBox, const char* Name)
{
    AL_UNUSED(Name);

    MailBox->Msg   = 0;
    MailBox->size  = 1;
    MailBox->entry = 0;

    return AL_OK;
}

static inline AL_S32 AlOsal_Mb_Send(AL_MailBox_t MailBox, AL_VOID * Msg)
{
    MailBox->Msg = *(AL_U64 *)Msg;
    __COMPILER_BARRIER();
    MailBox->entry = 1;

    return AL_OK;
}

static inline AL_S32 AlOsal_Mb_Receive(AL_MailBox_t MailBox, AL_VOID* Msg, AL_U64 Timeout)
{
    AL_BOOL flag = AL_WAIT_COND_UNTIL_TIMEOUT((MailBox->entry == 1), Timeout);

    if (flag == AL_TRUE) {
        MailBox->entry = 0;
        __COMPILER_BARRIER();
        *(AL_U64 *)Msg = MailBox->Msg;
        return AL_OK;
    }

    return AL_ERR_UNAVAILABLE;
}
/*----------------------------------------------------------------------
 * Critical API.
 *----------------------------------------------------------*/
// extern AL_S32 AlIntr_SetLocalInterrupt(AL_FUNCTION state);
// extern AL_S32 AlIntr_SetInterrupt(AL_U32 IntrId, AL_FUNCTION state);
static inline AL_U64 ALOsal_EnterCritical(AL_VOID)
{
    return AlIntr_SetLocalInterrupt(AL_FUNC_DISABLE);
}

static inline AL_VOID ALOsal_ExitCritical(AL_U64 flag)
{
    AL_UNUSED(flag);
    (AL_VOID)AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
}

/*----------------------------------------------*
 * Critical API for specific devices*
 *----------------------------------------------*/

static inline AL_VOID AlOsal_EnterDevCtritical(AL_U32 DevIntrId, AL_BOOL Condition)
{
    if (Condition) {
        AlIntr_SetInterrupt(DevIntrId, AL_FUNC_DISABLE);
    }
}


static inline AL_VOID AlOsal_ExitDevCtritical(AL_U32 DevIntrId, AL_BOOL Condition)
{
    if (Condition) {
        AlIntr_SetInterrupt(DevIntrId, AL_FUNC_ENABLE);
    }
}

static inline AL_VOID AlOsal_Sleep(AL_U32 Msec)
{
    AlSys_MDelay(Msec);
}

#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AL_OSAL_H */

