/*****************************************************************************
*****************************************************************************/

#ifndef AL_OSAL_H
#define AL_OSAL_H

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
#include <rthw.h>

/*----------------------------------------------*
 * MUTEX API.*
 *----------------------------------------------*/

#define AL_WAITFOREVER           RT_WAITING_FOREVER
#define AL_WAITING_NO            RT_WAITING_NO
typedef struct
{
    struct rt_mutex     Thread_Lock;
    AL_S64              Isr_Lock;
} AL_Lock;
typedef AL_Lock* AL_Lock_t;

static inline AL_S32 Al_OSAL_Lock_Init(AL_Lock_t Lock, const char* Name)
{
    return rt_mutex_init(&Lock->Thread_Lock, Name, RT_IPC_FLAG_PRIO);
}

static inline AL_S32 Al_OSAL_Lock_Take(AL_Lock_t Lock, AL_S32 Timeout)
{
    /* If the scheduler is started and in thread context */
    if (rt_interrupt_get_nest() == 0 && rt_thread_self() != RT_NULL)
    {
        return rt_mutex_take(&Lock->Thread_Lock, Timeout);
    }
    else
    {
        Lock->Isr_Lock = rt_hw_interrupt_disable();
    }
}

static inline AL_S32 Al_OSAL_Lock_Release(AL_Lock_t Lock)
{
    /* If the scheduler is started and in thread context */
    if (rt_interrupt_get_nest() == 0 && rt_thread_self() != RT_NULL)
    {
        return rt_mutex_release(&Lock->Thread_Lock);
    }
    else
    {
        rt_hw_interrupt_enable(Lock->Isr_Lock);
    }
}

/*----------------------------------------------*
 * Semaphore API.*
 *----------------------------------------------*/

typedef struct rt_semaphore  AL_Semaphore;
typedef struct rt_semaphore* AL_Semaphore_t;

static inline AL_S32 Al_OSAL_Sem_Init(AL_Semaphore_t Semaphore, const char* Name, AL_S32 Value)
{
    return rt_sem_init(Semaphore, Name, Value, RT_IPC_FLAG_PRIO);
}

static inline AL_S32 Al_OSAL_Sem_Take(AL_Semaphore_t Semaphore, AL_S32 Timeout)
{
    return rt_sem_take(Semaphore, Timeout);
}

static inline AL_S32 Al_OSAL_Sem_Release(AL_Semaphore_t Semaphore)
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

static inline AL_S32 Al_OSAL_Mb_Init(AL_MailBox_t MailBox, const char* Name)
{
    AL_S64 ret = AL_OK;

    ret = rt_sem_init(&MailBox->Semaphore, Name, 0, RT_IPC_FLAG_PRIO);
    if (ret != AL_OK) {
        return ret;
    }

    MailBox->Msg   = 0;
    MailBox->size  = 1;
    MailBox->entry = 0;

}

static inline AL_S32 Al_OSAL_Mb_Send(AL_MailBox_t MailBox, AL_VOID * Msg)
{
    MailBox->Msg = *(AL_U64 *)Msg;
    __COMPILER_BARRIER();
    MailBox->entry = 1;

    rt_sem_release(&MailBox->Semaphore);
}

static inline AL_S32 Al_OSAL_Mb_Recive(AL_MailBox_t MailBox, AL_VOID* Msg, AL_S32 Timeout)
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

static inline AL_VOID Al_OSAL_EnterDevCtritical(AL_U32 DevIntrId, AL_BOOL Condition)
{
    (AL_VOID)DevIntrId;
    (AL_VOID)Condition;
    rt_enter_critical();
}


static inline AL_VOID Al_OSAL_ExitDevCtritical(AL_U32 DevIntrId, AL_BOOL Condition)
{
    (AL_VOID)DevIntrId;
    (AL_VOID)Condition;
    rt_exit_critical();
}


/*----------------------------------------------*
 * Critical API.*
 *----------------------------------------------*/

static inline AL_VOID AL_OSAL_EnterCritical(AL_VOID)
{
    rt_enter_critical();
}

static inline AL_VOID AL_OSAL_ExitCritical(AL_VOID)
{
    rt_exit_critical();
}

/*----------------------------------------------*
 * Critical API.*
 *----------------------------------------------*/

static inline AL_VOID Al_OSAL_Sleep(AL_U32 Time)
{
    rt_thread_mdelay(Time);
}

#elif RTOS_FREERTOS

#endif

#else

/*----------------------------------------------*
 * Semaphore API.*
 *----------------------------------------------*/

#define AL_WAITFOREVER          (-1)
#define AL_WAITING_NO           (0)
typedef volatile struct
{
  volatile AL_S32 count;
}AL_Semaphore;
typedef volatile AL_Semaphore* AL_Semaphore_t;

static inline AL_S32 Al_OSAL_Sem_Init(AL_Semaphore_t Semaphore, const char* Name, AL_S32 Value)
{
    AL_UNUSED(Name);

    Semaphore->count = Value;

    return AL_OK;
}

static inline AL_S32 Al_OSAL_Sem_Take(AL_Semaphore_t Semaphore, AL_S32 Timeout)
{
    AL_WAIT_COND_UNTIL_TIMEOUT((Semaphore->count > 0), Timeout);

    Semaphore->count--;

    return AL_OK;
}

static inline AL_S32 Al_OSAL_Sem_Release(AL_Semaphore_t Semaphore)
{
    Semaphore->count++;

    return AL_OK;
}

/*----------------------------------------------*
 * MUTEX API.*
 *----------------------------------------------*/

typedef AL_Semaphore AL_Lock;
typedef AL_Semaphore_t AL_Lock_t;

static inline AL_S32 Al_OSAL_Lock_Init(AL_Lock_t Lock, const char* Name)
{
    AL_UNUSED(Name);

    AL_UNUSED(Lock);

    return AL_OK;
}

static inline AL_S32 Al_OSAL_Lock_Take(AL_Lock_t Lock, AL_S32 Timeout)
{
    AL_UNUSED(Timeout);

    AL_UNUSED(Lock);

    return AL_OK;
}

static inline AL_S32 Al_OSAL_Lock_Release(AL_Lock_t Lock)
{
    AL_UNUSED(Lock);

    return AL_OK;
}

/*----------------------------------------------*
 * MailBox API.*
 *----------------------------------------------*/

typedef volatile struct
{
    AL_U64          Msg;
    AL_S32          size;
    volatile AL_S16 entry;                         /**< index of messages in msg_pool */
} AL_MailBox;
typedef volatile AL_MailBox*  AL_MailBox_t;

static inline AL_S32 Al_OSAL_Mb_Init(AL_MailBox_t MailBox, const char* Name)
{
    AL_UNUSED(Name);

    MailBox->Msg   = 0;
    MailBox->size  = 1;
    MailBox->entry = 0;
}

static inline AL_S32 Al_OSAL_Mb_Send(AL_MailBox_t MailBox, AL_VOID * Msg)
{
    MailBox->Msg = *(AL_U64 *)Msg;
    __COMPILER_BARRIER();
    MailBox->entry = 1;

    return AL_OK;
}

static inline AL_S32 Al_OSAL_Mb_Recive(AL_MailBox_t MailBox, AL_VOID* Msg, AL_S32 Timeout)
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

static inline AL_VOID AL_OSAL_EnterCritical(AL_VOID)
{
    (AL_VOID)AlIntr_SetGlobalInterrupt(AL_FUNC_DISABLE);
}

static inline AL_VOID AL_OSAL_ExitCritical(AL_VOID)
{
    (AL_VOID)AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);
}

/*----------------------------------------------*
 * Critical API for specific devices*
 *----------------------------------------------*/

static inline AL_VOID Al_OSAL_EnterDevCtritical(AL_U32 DevIntrId, AL_BOOL Condition)
{
    if (Condition) {
        AlIntr_SetInterrupt(DevIntrId, AL_FUNC_DISABLE);
    }
}


static inline AL_VOID Al_OSAL_ExitDevCtritical(AL_U32 DevIntrId, AL_BOOL Condition)
{
    if (Condition) {
        AlIntr_SetInterrupt(DevIntrId, AL_FUNC_ENABLE);
    }
}

static inline AL_VOID Al_OSAL_Sleep(AL_U32 Time)
{

}

#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AL_OSAL_H */

