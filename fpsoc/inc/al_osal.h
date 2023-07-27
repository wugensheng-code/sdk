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

/*----------------------------------------------*
 * MUTEX API.*
 *----------------------------------------------*/

#define AL_WAITFOREVER           RT_WAITING_FOREVER
#define AL_WAITING_NO            RT_WAITING_NO
typedef volatile struct rt_mutex     AL_Mutex;
typedef volatile struct rt_mutex_t*  AL_Mutex_t;

static inline AL_S32 Al_OSAL_Mutex_Init(AL_Mutex_t mutex, const char* name)
{
    return rt_mutex_init(mutex, name, RT_IPC_FLAG_PRIO);
}

static inline AL_S32 Al_OSAL_Mutex_Take(AL_Mutex_t mutex, AL_S32 Timeout)
{
    return rt_mutex_take(mutex, Timeout);
}

static inline AL_S32 Al_OSAL_Mutex_Release(AL_Mutex_t mutex)
{
    return rt_mutex_release(mutex);
}

/*----------------------------------------------*
 * Semaphore API.*
 *----------------------------------------------*/

typedef volatile struct rt_semaphore  AL_Semaphore;
typedef volatile struct rt_semaphore* AL_Semaphore_t;

static inline AL_S32 Al_OSAL_Sem_Init(AL_Semaphore_t Semaphore, const char* name, AL_S32 value)
{
    return rt_sem_init(Semaphore, name, value, RT_IPC_FLAG_PRIO);
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

typedef volatile struct
{
    AL_Semaphore    Semaphore;
    AL_U64          msg;
    AL_S32          size;
    volatile AL_S16 entry;                         /**< index of messages in msg_pool */
} AL_MailBox;
typedef volatile AL_MailBox*  AL_MailBox_t;

static inline AL_S32 Al_OSAL_Mb_Init(AL_MailBox_t MailBox, const char* name)
{
    AL_S64 ret = AL_OK;

    ret = rt_sem_init(&MailBox->Semaphore, name, 0, RT_IPC_FLAG_PRIO);
    if (ret != AL_OK) {
        return ret;
    }

    MailBox->msg   = 0;
    MailBox->size  = 1;
    MailBox->entry = 0;

}

static inline AL_S32 Al_OSAL_Mb_Send(AL_MailBox_t MailBox, AL_VOID * msg)
{
    AL_BOOL flag = (AL_BOOL)rt_sem_release(&MailBox->Semaphore);

    if (flag == AL_TRUE) {
        AL_U32 *mvalue = (AL_U32 *)msg;
        *mvalue        = (AL_U32)((MailBox->msg >> 32) & 0xFFFF);
        *(mvalue + 1)  =  (AL_U32)(MailBox->msg & 0xFFFF);
        MailBox->entry = 0;

        return AL_OK;
    }
}

static inline AL_S32 Al_OSAL_Mb_Recive(AL_MailBox_t MailBox, AL_VOID* msg, AL_S32 timeout)
{
    AL_BOOL flag = (AL_BOOL)rt_sem_take(&MailBox->Semaphore, AL_WAITFOREVER);

    if (flag == AL_TRUE) {
        AL_U32 *mvalue = (AL_U32 *)msg;
        *mvalue        = (AL_U32)((MailBox->msg >> 32) & 0xFFFF);
        *(mvalue + 1)  =  (AL_U32)(MailBox->msg & 0xFFFF);
        MailBox->entry = 0;

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

static inline AL_OSAL_EnterCritical(AL_VOID)
{
    rt_enter_critical();
}

static inline AL_OSAL_ExitCritical(AL_VOID)
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

#define AL_WAITFOREVER          (-1UL)
#define AL_WAITING_NO           (0)
typedef volatile struct
{
  volatile AL_S32 count;
}AL_Semaphore;
typedef volatile AL_Semaphore* AL_Semaphore_t;

static inline AL_S32 Al_OSAL_Sem_Init(AL_Semaphore_t Semaphore, const char* name, AL_S32 value)
{
    AL_UNUSED(name);

    Semaphore->count = value;

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

typedef AL_Semaphore AL_Mutex;
typedef AL_Semaphore_t AL_Mutex_t;

static inline AL_S32 Al_OSAL_Mutex_Init(AL_Mutex_t mutex, const char* name)
{
    AL_UNUSED(name);

    AL_UNUSED(mutex);

    return AL_OK;
}

static inline AL_S32 Al_OSAL_Mutex_Take(AL_Mutex_t mutex, AL_S32 Timeout)
{
    AL_UNUSED(Timeout);

    AL_UNUSED(mutex);

    return AL_OK;
}

static inline AL_S32 Al_OSAL_Mutex_Release(AL_Mutex_t mutex)
{
    AL_UNUSED(mutex);

    return AL_OK;
}

/*----------------------------------------------*
 * MailBox API.*
 *----------------------------------------------*/

typedef volatile struct
{
    AL_U64          msg;
    AL_S32          size;
    volatile AL_S16 entry;                         /**< index of messages in msg_pool */
} AL_MailBox;
typedef volatile AL_MailBox*  AL_MailBox_t;

static inline AL_S32 Al_OSAL_Mb_Init(AL_MailBox_t MailBox, const char* name)
{
    AL_UNUSED(name);

    MailBox->msg   = 0;
    MailBox->size  = 1;
    MailBox->entry = 0;
}

static inline AL_S32 Al_OSAL_Mb_Send(AL_MailBox_t MailBox, AL_VOID * msg)
{
    AL_U32 *mvalue = (AL_U32 *)msg;
    MailBox->msg = ((AL_U64)(*mvalue) << 32) | (AL_U64)*(mvalue + 1);
    MailBox->entry = 1;

    return AL_OK;
}

static inline AL_S32 Al_OSAL_Mb_Recive(AL_MailBox_t MailBox, AL_VOID* msg, AL_S32 timeout)
{
    AL_BOOL flag = AL_WAIT_COND_UNTIL_TIMEOUT((MailBox->entry == 1), timeout);

    if (flag == AL_TRUE) {
        AL_U32 *mvalue = (AL_U32 *)msg;
        *mvalue        = (AL_U32)((MailBox->msg >> 32) & 0xFFFFFFFF);
        *(mvalue + 1)  =  (AL_U32)(MailBox->msg & 0xFFFFFFFF);
        MailBox->entry = 0;

        return AL_OK;
    }

    return AL_ERR_UNAVAILABLE;
}
/*----------------------------------------------------------------------
 * Critical API.
 *----------------------------------------------------------*/

static inline AL_VOID AL_OSAL_EnterCritical(AL_VOID)
{
    AlIntr_SetGlobalInterrupt(AL_FUNC_DISABLE);
}

static inline AL_VOID AL_OSAL_ExitCritical(AL_VOID)
{
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);
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

