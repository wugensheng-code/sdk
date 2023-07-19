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
 * Critical API.*
 *----------------------------------------------*/

#define AL_OSAL_ENTER_CRITICAL                rt_enter_critical
#define AL_OSAL_EXIT_CRITICAL                 rt_exit_critical

#elif RTOS_FREERTOS

#endif

#else

/*----------------------------------------------*
 * Semaphore API.*
 *----------------------------------------------*/

#define AL_WAITFOREVER          (~0UL)
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

    mutex->count = 1;

    return AL_OK;
}

static inline AL_S32 Al_OSAL_Mutex_Take(AL_Mutex_t mutex, AL_S32 Timeout)
{
    AL_WAIT_COND_UNTIL_TIMEOUT((mutex->count > 0), Timeout);
    
    Al_OSAL_Sem_Take((AL_Semaphore_t)mutex, Timeout);
}

static inline AL_S32 Al_OSAL_Mutex_Release(AL_Mutex_t mutex)
{
    return Al_OSAL_Sem_Release((AL_Semaphore_t)mutex);
}

/*----------------------------------------------*
 * Critical API.*
 *----------------------------------------------*/

static inline AL_VOID AL_OSAL_ENTER_CRITICAL(AL_VOID)
{
    AlIntr_SetGlobalInterrupt(AL_FUNC_DISABLE);
} 

static inline AL_VOID AL_OSAL_EXIT_CRITICAL(AL_VOID)
{
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);
}

#define OSAL_CHECK(Ret)                \
{                                      \
    if(Ret != AL_OK)                   \
    {                                  \
        return Ret;                    \
    }                                  \
}
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AL_OSAL_H */

