/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-12-8      Bernard      add file header
 *                             export bsd socket symbol for RT-Thread Application Module
 * 2013-05-25     Bernard      port to v1.4.1
 * 2017-03-26     HuangXiHans  port to v2.0.2
 * 2017-11-15     Bernard      add lock for init_done callback
 * 2018-11-02     MurphyZhao   port to v2.1.0
 * 2020-06-20     liuxianliang port to v2.1.2
 * 2021-06-25     liuxianliang port to v2.0.3
 * 2022-01-18     Meco Man     remove v2.0.2
 * 2022-02-20     Meco Man     integrate v1.4.1 v2.0.3 and v2.1.2 porting layer
 */

/* lwIP includes. */
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"

#if !NO_SYS

#if defined(RTOS_RTTHREAD)
#include <rtthread.h>
#include <rthw.h>

#include <arch/sys_arch.h>
#include <lwip/sys.h>
#include <lwip/opt.h>
#include <lwip/stats.h>
#include <lwip/err.h>
#include <lwip/debug.h>
#include <lwip/netif.h>
#include <lwip/netifapi.h>
#include <lwip/tcpip.h>
#include <lwip/sio.h>
#include <lwip/init.h>
#include <lwip/dhcp.h>
#include <lwip/inet.h>
#include <netif/etharp.h>
#include <netif/al_gbe_ethernetif.h>

void sys_init(void)
{
    /* nothing on RT-Thread porting */
}

/*
 * Create a new semaphore
 *
 * @return the operation status, ERR_OK on OK; others on error
 */
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
    static unsigned short counter = 0;
    char tname[RT_NAME_MAX];
    sys_sem_t tmpsem;

    RT_DEBUG_NOT_IN_INTERRUPT;

    rt_snprintf(tname, RT_NAME_MAX, "%s%d", SYS_LWIP_SEM_NAME, counter);
    counter ++;

    tmpsem = rt_sem_create(tname, count, RT_IPC_FLAG_FIFO);
    if (tmpsem == RT_NULL)
    {
        return ERR_MEM;
    }
    else
    {
        *sem = tmpsem;

        return ERR_OK;
    }
}

/*
 * Deallocate a semaphore
 */
void sys_sem_free(sys_sem_t *sem)
{
    RT_DEBUG_NOT_IN_INTERRUPT;
    rt_sem_delete(*sem);
}

/*
 * Signal a semaphore
 */
void sys_sem_signal(sys_sem_t *sem)
{
    rt_sem_release(*sem);
}

/*
 * Block the thread while waiting for the semaphore to be signaled
 *
 * @return If the timeout argument is non-zero, it will return the number of milliseconds
 *         spent waiting for the semaphore to be signaled; If the semaphore isn't signaled
 *         within the specified time, it will return SYS_ARCH_TIMEOUT; If the thread doesn't
 *         wait for the semaphore, it will return zero
 */
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    rt_err_t ret;
    s32_t t;
    u32_t tick;

    RT_DEBUG_NOT_IN_INTERRUPT;

    /* get the begin tick */
    tick = rt_tick_get();
    if (timeout == 0)
    {
        t = RT_WAITING_FOREVER;
    }
    else
    {
        /* convert msecond to os tick */
        if (timeout < (1000 / RT_TICK_PER_SECOND))
            t = 1;
        else
            t = timeout / (1000 / RT_TICK_PER_SECOND);
    }

    ret = rt_sem_take(*sem, t);

    if (ret == -RT_ETIMEOUT)
    {
        return SYS_ARCH_TIMEOUT;
    }
    else
    {
        if (ret == RT_EOK)
            ret = 1;
    }

    /* get elapse msecond */
    tick = rt_tick_get() - tick;

    /* convert tick to msecond */
    tick = tick * (1000 / RT_TICK_PER_SECOND);
    if (tick == 0)
        tick = 1;

    return tick;
}

#ifndef sys_sem_valid
/** Check if a semaphore is valid/allocated:
 *  return 1 for valid, 0 for invalid
 */
int sys_sem_valid(sys_sem_t *sem)
{
    int ret = 0;

    if (*sem) ret = 1;

    return ret;
}
#endif

#ifndef sys_sem_set_invalid
/** Set a semaphore invalid so that sys_sem_valid returns 0
 */
void sys_sem_set_invalid(sys_sem_t *sem)
{
    *sem = RT_NULL;
}
#endif

/* ====================== Mutex ====================== */

/** Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex
 */
err_t sys_mutex_new(sys_mutex_t *mutex)
{
    static unsigned short counter = 0;
    char tname[RT_NAME_MAX];
    sys_mutex_t tmpmutex;

    RT_DEBUG_NOT_IN_INTERRUPT;

    rt_snprintf(tname, RT_NAME_MAX, "%s%d", SYS_LWIP_MUTEX_NAME, counter);
    counter ++;

    tmpmutex = rt_mutex_create(tname, RT_IPC_FLAG_PRIO);
    if (tmpmutex == RT_NULL)
    {
        return ERR_MEM;
    }
    else
    {
        *mutex = tmpmutex;
        return ERR_OK;
    }
}

/** Lock a mutex
 * @param mutex the mutex to lock
 */
void sys_mutex_lock(sys_mutex_t *mutex)
{
    RT_DEBUG_NOT_IN_INTERRUPT;
    rt_mutex_take(*mutex, RT_WAITING_FOREVER);
    return;
}

/** Unlock a mutex
 * @param mutex the mutex to unlock
 */
void sys_mutex_unlock(sys_mutex_t *mutex)
{
    rt_mutex_release(*mutex);
}

/** Delete a semaphore
 * @param mutex the mutex to delete
 */
void sys_mutex_free(sys_mutex_t *mutex)
{
    RT_DEBUG_NOT_IN_INTERRUPT;
    rt_mutex_delete(*mutex);
}

#ifndef sys_mutex_valid
/** Check if a mutex is valid/allocated:
 *  return 1 for valid, 0 for invalid
 */
int sys_mutex_valid(sys_mutex_t *mutex)
{
    int ret = 0;

    if (*mutex) ret = 1;

    return ret;
}
#endif

#ifndef sys_mutex_set_invalid
/** Set a mutex invalid so that sys_mutex_valid returns 0
 */
void sys_mutex_set_invalid(sys_mutex_t *mutex)
{
    *mutex = RT_NULL;
}
#endif

/* ====================== Mailbox ====================== */

/*
 * Create an empty mailbox for maximum "size" elements
 *
 * @return the operation status, ERR_OK on OK; others on error
 */
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    static unsigned short counter = 0;
    char tname[RT_NAME_MAX];
    sys_mbox_t tmpmbox;

    RT_DEBUG_NOT_IN_INTERRUPT;

    rt_snprintf(tname, RT_NAME_MAX, "%s%d", SYS_LWIP_MBOX_NAME, counter);
    counter ++;

    tmpmbox = rt_mb_create(tname, size, RT_IPC_FLAG_FIFO);
    if (tmpmbox != RT_NULL)
    {
        *mbox = tmpmbox;

        return ERR_OK;
    }

    return ERR_MEM;
}

/*
 * Deallocate a mailbox
 */
void sys_mbox_free(sys_mbox_t *mbox)
{
    RT_DEBUG_NOT_IN_INTERRUPT;
    rt_mb_delete(*mbox);
    return;
}

/** Post a message to an mbox - may not fail
 * -> blocks if full, only used from tasks not from ISR
 * @param mbox mbox to posts the message
 * @param msg message to post (ATTENTION: can be NULL)
 */
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    RT_DEBUG_NOT_IN_INTERRUPT;
    rt_mb_send_wait(*mbox, (rt_ubase_t)msg, RT_WAITING_FOREVER);
    return;
}

/*
 * Try to post the "msg" to the mailbox
 *
 * @return return ERR_OK if the "msg" is posted, ERR_MEM if the mailbox is full
 */
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    if (rt_mb_send(*mbox, (rt_ubase_t)msg) == RT_EOK)
    {
        return ERR_OK;
    }

    return ERR_MEM;
}

#if (LWIP_VERSION_MAJOR * 100 + LWIP_VERSION_MINOR) >= 201 /* >= v2.1.0 */
err_t sys_mbox_trypost_fromisr(sys_mbox_t *q, void *msg)
{
  return sys_mbox_trypost(q, msg);
}
#endif /* (LWIP_VERSION_MAJOR * 100 + LWIP_VERSION_MINOR) >= 201 */

/** Wait for a new message to arrive in the mbox
 * @param mbox mbox to get a message from
 * @param msg pointer where the message is stored
 * @param timeout maximum time (in milliseconds) to wait for a message
 * @return time (in milliseconds) waited for a message, may be 0 if not waited
           or SYS_ARCH_TIMEOUT on timeout
 *         The returned time has to be accurate to prevent timer jitter!
 */
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
    rt_err_t ret;
    s32_t t;
    u32_t tick;

    RT_DEBUG_NOT_IN_INTERRUPT;

    /* get the begin tick */
    tick = rt_tick_get();

    if(timeout == 0)
    {
        t = RT_WAITING_FOREVER;
    }
    else
    {
        /* convirt msecond to os tick */
        if (timeout < (1000 / RT_TICK_PER_SECOND))
            t = 1;
        else
            t = timeout / (1000 / RT_TICK_PER_SECOND);
    }

    ret = rt_mb_recv(*mbox, (rt_ubase_t *)msg, t);
    if(ret != RT_EOK)
    {
        return SYS_ARCH_TIMEOUT;
    }

    /* get elapse msecond */
    tick = rt_tick_get() - tick;

    /* convert tick to msecond */
    tick = tick * (1000 / RT_TICK_PER_SECOND);
    if (tick == 0)
        tick = 1;

    return tick;
}

/**
 * @ingroup sys_mbox
 * This is similar to sys_arch_mbox_fetch, however if a message is not
 * present in the mailbox, it immediately returns with the code
 * SYS_MBOX_EMPTY. On success 0 is returned.
 * To allow for efficient implementations, this can be defined as a
 * function-like macro in sys_arch.h instead of a normal function. For
 * example, a naive implementation could be:
 * \#define sys_arch_mbox_tryfetch(mbox,msg) sys_arch_mbox_fetch(mbox,msg,1)
 * although this would introduce unnecessary delays.
 *
 * @param mbox mbox to get a message from
 * @param msg pointer where the message is stored
 * @return 0 (milliseconds) if a message has been received
 *         or SYS_MBOX_EMPTY if the mailbox is empty
 */
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
    int ret;

    ret = rt_mb_recv(*mbox, (rt_ubase_t *)msg, 0);
    if(ret == -RT_ETIMEOUT)
    {
        return SYS_ARCH_TIMEOUT;
    }
    else
    {
        if (ret == RT_EOK)
            ret = 0;
    }

    return ret;
}

#ifndef sys_mbox_valid
/** Check if an mbox is valid/allocated:
 *  return 1 for valid, 0 for invalid
 */
int sys_mbox_valid(sys_mbox_t *mbox)
{
    int ret = 0;

    if (*mbox) ret = 1;

    return ret;
}
#endif

#ifndef sys_mbox_set_invalid
/** Set an mbox invalid so that sys_mbox_valid returns 0
 */
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    *mbox = RT_NULL;
}
#endif

/* ====================== System ====================== */

/*
 * Start a new thread named "name" with priority "prio" that will begin
 * its execution in the function "thread()". The "arg" argument will be
 * passed as an argument to the thread() function
 */
sys_thread_t sys_thread_new(const char    *name,
                            lwip_thread_fn thread,
                            void          *arg,
                            int            stacksize,
                            int            prio)
{
    rt_thread_t t;

    RT_DEBUG_NOT_IN_INTERRUPT;

    /* create thread */
    t = rt_thread_create(name, thread, arg, stacksize, prio, 0);
    RT_ASSERT(t != RT_NULL);

    /* startup thread */
    rt_thread_startup(t);

    return t;
}

sys_prot_t sys_arch_protect(void)
{
    rt_base_t level;
    level = rt_hw_interrupt_disable(); /* disable interrupt */
    return level;
}

void sys_arch_unprotect(sys_prot_t pval)
{
    rt_hw_interrupt_enable(pval); /* enable interrupt */
}

void sys_arch_assert(const char *file, int line)
{
    rt_kprintf("\nAssertion: %d in %s\n",
               line, file);
    RT_ASSERT(0);
}

u32_t sys_jiffies(void)
{
    return rt_tick_get();
}

u32_t sys_now(void)
{
    return rt_tick_get_millisecond();
}

#elif defined(RTOS_FREERTOS)

/* ------------------------ System architecture includes ----------------------------- */
#include "arch/sys_arch.h"

/* ------------------------ lwIP includes --------------------------------- */
#include "lwip/opt.h"

/* Very crude mechanism used to determine if the critical section handling
functions are being called from an interrupt context or not.  This relies on
the interrupt handler setting this variable manually. */
volatile portBASE_TYPE xInsideISR = pdFALSE;

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates a new mailbox
 * Inputs:
 *      int size                -- Size of elements in the mailbox
 * Outputs:
 *      sys_mbox_t              -- Handle to new mailbox
 *---------------------------------------------------------------------------*/
err_t sys_mbox_new( sys_mbox_t *pxMailBox, int iSize )
{
err_t xReturn = ERR_MEM;

    *pxMailBox = xQueueCreate( iSize, sizeof( void * ) );

    if( *pxMailBox != NULL )
    {
        xReturn = ERR_OK;
        SYS_STATS_INC_USED( mbox );
    }

    return xReturn;
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a mailbox. If there are messages still present in the
 *      mailbox when the mailbox is deallocated, it is an indication of a
 *      programming error in lwIP and the developer should be notified.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 * Outputs:
 *      sys_mbox_t              -- Handle to new mailbox
 *---------------------------------------------------------------------------*/
void sys_mbox_free( sys_mbox_t *pxMailBox )
{
unsigned long ulMessagesWaiting;

    ulMessagesWaiting = uxQueueMessagesWaiting( *pxMailBox );
    configASSERT( ( ulMessagesWaiting == 0 ) );

    #if SYS_STATS
    {
        if( ulMessagesWaiting != 0UL )
        {
            SYS_STATS_INC( mbox.err );
        }

        SYS_STATS_DEC( mbox.used );
    }
    #endif /* SYS_STATS */

    vQueueDelete( *pxMailBox );
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_post
 *---------------------------------------------------------------------------*
 * Description:
 *      Post the "msg" to the mailbox.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *data              -- Pointer to data to post
 *---------------------------------------------------------------------------*/
void sys_mbox_post( sys_mbox_t *pxMailBox, void *pxMessageToPost )
{
    while( xQueueSendToBack( *pxMailBox, &pxMessageToPost, portMAX_DELAY ) != pdTRUE );
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_trypost
 *---------------------------------------------------------------------------*
 * Description:
 *      Try to post the "msg" to the mailbox.  Returns immediately with
 *      error if cannot.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *msg               -- Pointer to data to post
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *                                  if not.
 *---------------------------------------------------------------------------*/
err_t sys_mbox_trypost( sys_mbox_t *pxMailBox, void *pxMessageToPost )
{
err_t xReturn;
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if( xInsideISR != pdFALSE )
    {
        xReturn = xQueueSendFromISR( *pxMailBox, &pxMessageToPost, &xHigherPriorityTaskWoken );
    }
    else
    {
        xReturn = xQueueSend( *pxMailBox, &pxMessageToPost, ( TickType_t ) 0 );
    }

    if( xReturn == pdPASS )
    {
        xReturn = ERR_OK;
    }
    else
    {
        /* The queue was already full. */
        xReturn = ERR_MEM;
        SYS_STATS_INC( mbox.err );
    }

    return xReturn;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_fetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread until a message arrives in the mailbox, but does
 *      not block the thread longer than "timeout" milliseconds (similar to
 *      the sys_arch_sem_wait() function). The "msg" argument is a result
 *      parameter that is set by the function (i.e., by doing "*msg =
 *      ptr"). The "msg" parameter maybe NULL to indicate that the message
 *      should be dropped.
 *
 *      The return values are the same as for the sys_arch_sem_wait() function:
 *      Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 *      timeout.
 *
 *      Note that a function with a similar name, sys_mbox_fetch(), is
 *      implemented by lwIP.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- SYS_ARCH_TIMEOUT if timeout, else number
 *                                  of milliseconds until received.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_fetch( sys_mbox_t *pxMailBox, void **ppvBuffer, u32_t ulTimeOut )
{
void *pvDummy;
TickType_t xStartTime, xEndTime, xElapsed;
unsigned long ulReturn;

    xStartTime = xTaskGetTickCount();

    if( NULL == ppvBuffer )
    {
        ppvBuffer = &pvDummy;
    }

    if( ulTimeOut != 0UL )
    {
        configASSERT( xInsideISR == ( portBASE_TYPE ) 0 );

        if( pdTRUE == xQueueReceive( *pxMailBox, &( *ppvBuffer ), ulTimeOut/ portTICK_PERIOD_MS ) )
        {
            xEndTime = xTaskGetTickCount();
            xElapsed = ( xEndTime - xStartTime ) * portTICK_PERIOD_MS;

            ulReturn = xElapsed;
        }
        else
        {
            /* Timed out. */
            *ppvBuffer = NULL;
            ulReturn = SYS_ARCH_TIMEOUT;
        }
    }
    else
    {
        while( pdTRUE != xQueueReceive( *pxMailBox, &( *ppvBuffer ), portMAX_DELAY ) );
        xEndTime = xTaskGetTickCount();
        xElapsed = ( xEndTime - xStartTime ) * portTICK_PERIOD_MS;

        if( xElapsed == 0UL )
        {
            xElapsed = 1UL;
        }

        ulReturn = xElapsed;
    }

    return ulReturn;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_tryfetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Similar to sys_arch_mbox_fetch, but if message is not ready
 *      immediately, we'll return with SYS_MBOX_EMPTY.  On success, 0 is
 *      returned.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 * Outputs:
 *      u32_t                   -- SYS_MBOX_EMPTY if no messages.  Otherwise,
 *                                  return ERR_OK.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_tryfetch( sys_mbox_t *pxMailBox, void **ppvBuffer )
{
void *pvDummy;
unsigned long ulReturn;
long lResult;
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if( ppvBuffer== NULL )
    {
        ppvBuffer = &pvDummy;
    }

    if( xInsideISR != pdFALSE )
    {
        lResult = xQueueReceiveFromISR( *pxMailBox, &( *ppvBuffer ), &xHigherPriorityTaskWoken );
    }
    else
    {
        lResult = xQueueReceive( *pxMailBox, &( *ppvBuffer ), 0UL );
    }

    if( lResult == pdPASS )
    {
        ulReturn = ERR_OK;
    }
    else
    {
        ulReturn = SYS_MBOX_EMPTY;
    }

    return ulReturn;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates and returns a new semaphore. The "ucCount" argument specifies
 *      the initial state of the semaphore.
 *      NOTE: Currently this routine only creates counts of 1 or 0
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      u8_t ucCount              -- Initial ucCount of semaphore (1 or 0)
 * Outputs:
 *      sys_sem_t               -- Created semaphore or 0 if could not create.
 *---------------------------------------------------------------------------*/
err_t sys_sem_new( sys_sem_t *pxSemaphore, u8_t ucCount )
{
err_t xReturn = ERR_MEM;

    vSemaphoreCreateBinary( ( *pxSemaphore ) );

    if( *pxSemaphore != NULL )
    {
        if( ucCount == 0U )
        {
            xSemaphoreTake( *pxSemaphore, 1UL );
        }

        xReturn = ERR_OK;
        SYS_STATS_INC_USED( sem );
    }
    else
    {
        SYS_STATS_INC( sem.err );
    }

    return xReturn;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_sem_wait
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread while waiting for the semaphore to be
 *      signaled. If the "timeout" argument is non-zero, the thread should
 *      only be blocked for the specified time (measured in
 *      milliseconds).
 *
 *      If the timeout argument is non-zero, the return value is the number of
 *      milliseconds spent waiting for the semaphore to be signaled. If the
 *      semaphore wasn't signaled within the specified time, the return value is
 *      SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 *      (i.e., it was already signaled), the function may return zero.
 *
 *      Notice that lwIP implements a function with a similar name,
 *      sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to wait on
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- Time elapsed or SYS_ARCH_TIMEOUT.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_sem_wait( sys_sem_t *pxSemaphore, u32_t ulTimeout )
{
TickType_t xStartTime, xEndTime, xElapsed;
unsigned long ulReturn;

    xStartTime = xTaskGetTickCount();

    if( ulTimeout != 0UL )
    {
        if( xSemaphoreTake( *pxSemaphore, ulTimeout / portTICK_PERIOD_MS ) == pdTRUE )
        {
            xEndTime = xTaskGetTickCount();
            xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;
            ulReturn = xElapsed;
        }
        else
        {
            ulReturn = SYS_ARCH_TIMEOUT;
        }
    }
    else
    {
        while( xSemaphoreTake( *pxSemaphore, portMAX_DELAY ) != pdTRUE );
        xEndTime = xTaskGetTickCount();
        xElapsed = ( xEndTime - xStartTime ) * portTICK_PERIOD_MS;

        if( xElapsed == 0UL )
        {
            xElapsed = 1UL;
        }

        ulReturn = xElapsed;
    }

    return ulReturn;
}

/** Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex */
err_t sys_mutex_new( sys_mutex_t *pxMutex )
{
err_t xReturn = ERR_MEM;

    *pxMutex = xSemaphoreCreateMutex();

    if( *pxMutex != NULL )
    {
        xReturn = ERR_OK;
        SYS_STATS_INC_USED( mutex );
    }
    else
    {
        SYS_STATS_INC( mutex.err );
    }

    return xReturn;
}

/** Lock a mutex
 * @param mutex the mutex to lock */
void sys_mutex_lock( sys_mutex_t *pxMutex )
{
    while( xSemaphoreTake( *pxMutex, portMAX_DELAY ) != pdPASS );
}

/** Unlock a mutex
 * @param mutex the mutex to unlock */
void sys_mutex_unlock(sys_mutex_t *pxMutex )
{
    xSemaphoreGive( *pxMutex );
}


/** Delete a semaphore
 * @param mutex the mutex to delete */
void sys_mutex_free( sys_mutex_t *pxMutex )
{
    SYS_STATS_DEC( mutex.used );
    vQueueDelete( *pxMutex );
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_signal
 *---------------------------------------------------------------------------*
 * Description:
 *      Signals (releases) a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to signal
 *---------------------------------------------------------------------------*/
void sys_sem_signal( sys_sem_t *pxSemaphore )
{
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if( xInsideISR != pdFALSE )
    {
        xSemaphoreGiveFromISR( *pxSemaphore, &xHigherPriorityTaskWoken );
    }
    else
    {
        xSemaphoreGive( *pxSemaphore );
    }
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to free
 *---------------------------------------------------------------------------*/
void sys_sem_free( sys_sem_t *pxSemaphore )
{
    SYS_STATS_DEC(sem.used);
    vQueueDelete( *pxSemaphore );
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_init
 *---------------------------------------------------------------------------*
 * Description:
 *      Initialize sys arch
 *---------------------------------------------------------------------------*/
void sys_init(void)
{
}

u32_t sys_now(void)
{
    return xTaskGetTickCount();
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_thread_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Starts a new thread with priority "prio" that will begin its
 *      execution in the function "thread()". The "arg" argument will be
 *      passed as an argument to the thread() function. The id of the new
 *      thread is returned. Both the id and the priority are system
 *      dependent.
 * Inputs:
 *      char *name              -- Name of thread
 *      void (* thread)(void *arg) -- Pointer to function to run.
 *      void *arg               -- Argument passed into function
 *      int stacksize           -- Required stack amount in bytes
 *      int prio                -- Thread priority
 * Outputs:
 *      sys_thread_t            -- Pointer to per-thread timeouts.
 *---------------------------------------------------------------------------*/
sys_thread_t sys_thread_new( const char *pcName, void( *pxThread )( void *pvParameters ), void *pvArg, int iStackSize, int iPriority )
{
TaskHandle_t xCreatedTask;
portBASE_TYPE xResult;
sys_thread_t xReturn;

    xResult = xTaskCreate( pxThread, pcName, iStackSize, pvArg, iPriority, &xCreatedTask );

    if( xResult == pdPASS )
    {
        xReturn = xCreatedTask;
    }
    else
    {
        xReturn = NULL;
    }

    return xReturn;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_protect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" critical region protection and
 *      returns the previous protection level. This function is only called
 *      during very short critical regions. An embedded system which supports
 *      ISR-based drivers might want to implement this function by disabling
 *      interrupts. Task-based systems might want to implement this by using
 *      a mutex or disabling tasking. This function should support recursive
 *      calls from the same task or interrupt. In other words,
 *      sys_arch_protect() could be called while already protected. In
 *      that case the return value indicates that it is already protected.
 *
 *      sys_arch_protect() is only required if your port is supporting an
 *      operating system.
 * Outputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
sys_prot_t sys_arch_protect( void )
{
    if( xInsideISR == pdFALSE )
    {
        taskENTER_CRITICAL();
    }
    return ( sys_prot_t ) 1;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_unprotect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" set of critical region
 *      protection to the value specified by pval. See the documentation for
 *      sys_arch_protect() for more information. This function is only
 *      required if your port is supporting an operating system.
 * Inputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
void sys_arch_unprotect( sys_prot_t xValue )
{
    (void) xValue;
    if( xInsideISR == pdFALSE )
    {
        taskEXIT_CRITICAL();
    }
}

/*
 * Prints an assertion messages and aborts execution.
 */
void sys_assert( const char *pcMessage )
{
    (void) pcMessage;

    for (;;)
    {
    }
}

#endif

#else

#include "al_core.h"

u32_t sys_now(void)
{
    u64_t tick = AlSys_GetTimerTickCount();
    return (tick / 50 /1000);
}

#endif /* ！NO_SYS */
