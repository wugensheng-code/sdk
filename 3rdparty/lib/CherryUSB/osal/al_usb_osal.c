/*
 * Copyright (c) 2022, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "usb_osal.h"
#include "usb_errno.h"
#include "al_core.h"

#ifdef RTOS_RTTHREAD
#include <rtthread.h>
#include <rthw.h>

usb_osal_thread_t usb_osal_thread_create(const char *name, uint32_t stack_size, uint32_t prio, usb_thread_entry_t entry, void *args)
{
    rt_thread_t htask;
    htask = rt_thread_create(name, entry, args, stack_size, prio, 10);
    rt_thread_startup(htask);
    return (usb_osal_thread_t)htask;
}

void usb_osal_thread_delete(usb_osal_thread_t thread)
{
    if (thread == NULL) {
        return;
    }

    rt_thread_delete(thread);
}

usb_osal_sem_t usb_osal_sem_create(uint32_t initial_count)
{
    return (usb_osal_sem_t)rt_sem_create("usbh_sem", initial_count, RT_IPC_FLAG_FIFO);
}

void usb_osal_sem_delete(usb_osal_sem_t sem)
{
    rt_sem_delete((rt_sem_t)sem);
}

int usb_osal_sem_take(usb_osal_sem_t sem, uint32_t timeout)
{
    int ret = 0;
    rt_err_t result = RT_EOK;

    if (timeout == USB_OSAL_WAITING_FOREVER) {
        result = rt_sem_take((rt_sem_t)sem, RT_WAITING_FOREVER);
    } else {
        result = rt_sem_take((rt_sem_t)sem, rt_tick_from_millisecond(timeout));
    }
    if (result == -RT_ETIMEOUT) {
        ret = -USB_ERR_TIMEOUT;
    } else if (result == -RT_ERROR) {
        ret = -USB_ERR_INVAL;
    } else {
        ret = 0;
    }

    return (int)ret;
}

int usb_osal_sem_give(usb_osal_sem_t sem)
{
    return (int)rt_sem_release((rt_sem_t)sem);
}

void usb_osal_sem_reset(usb_osal_sem_t sem)
{
    rt_sem_control((rt_sem_t)sem, RT_IPC_CMD_RESET, (void *)0);
}

usb_osal_mutex_t usb_osal_mutex_create(void)
{
    return (usb_osal_mutex_t)rt_mutex_create("usbh_mutex", RT_IPC_FLAG_FIFO);
}

void usb_osal_mutex_delete(usb_osal_mutex_t mutex)
{
    rt_mutex_delete((rt_mutex_t)mutex);
}

int usb_osal_mutex_take(usb_osal_mutex_t mutex)
{
    return (int)rt_mutex_take((rt_mutex_t)mutex, RT_WAITING_FOREVER);
}

int usb_osal_mutex_give(usb_osal_mutex_t mutex)
{
    return (int)rt_mutex_release((rt_mutex_t)mutex);
}

usb_osal_mq_t usb_osal_mq_create(uint32_t max_msgs)
{
    return (usb_osal_mq_t)rt_mq_create("usbh_mq", sizeof(uintptr_t), max_msgs, RT_IPC_FLAG_FIFO);
}

void usb_osal_mq_delete(usb_osal_mq_t mq)
{
    rt_mq_delete((rt_mq_t)mq);
}

int usb_osal_mq_send(usb_osal_mq_t mq, uintptr_t addr)
{
    return rt_mq_send((rt_mq_t)mq, &addr, sizeof(uintptr_t));
}

int usb_osal_mq_recv(usb_osal_mq_t mq, uintptr_t *addr, uint32_t timeout)
{
    int ret = 0;
    rt_err_t result = RT_EOK;

    if (timeout == USB_OSAL_WAITING_FOREVER) {
        result = rt_mq_recv((rt_mq_t)mq, addr, sizeof(uintptr_t), RT_WAITING_FOREVER);
    } else {
        result = rt_mq_recv((rt_mq_t)mq, addr, sizeof(uintptr_t), rt_tick_from_millisecond(timeout));
    }
    if (result == -RT_ETIMEOUT) {
        ret = -USB_ERR_TIMEOUT;
    } else if (result == -RT_ERROR) {
        ret = -USB_ERR_INVAL;
    } else {
        ret = 0;
    }

    return (int)ret;
}

struct usb_osal_timer *usb_osal_timer_create(const char *name, uint32_t timeout_ms, usb_timer_handler_t handler, void *argument, bool is_period)
{
    struct usb_osal_timer *timer;

    timer = rt_malloc(sizeof(struct usb_osal_timer));
    memset(timer, 0, sizeof(struct usb_osal_timer));

    timer->timer = (void *)rt_timer_create("usb_tim", handler, argument, timeout_ms, is_period ? (RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER) : (RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER));
    if (timer->timer == NULL) {
        return NULL;
    }
    return timer;
}

void usb_osal_timer_delete(struct usb_osal_timer *timer)
{
    rt_timer_stop(timer->timer);
    rt_timer_delete(timer->timer);
    rt_free(timer);
}

void usb_osal_timer_start(struct usb_osal_timer *timer)
{
    rt_timer_start(timer->timer);
}

void usb_osal_timer_stop(struct usb_osal_timer *timer)
{
    rt_timer_stop(timer->timer);
}

size_t usb_osal_enter_critical_section(void)
{
    return rt_hw_interrupt_disable();
}

void usb_osal_leave_critical_section(size_t flag)
{
    rt_hw_interrupt_enable(flag);
}

void usb_osal_msleep(uint32_t delay)
{
    rt_thread_mdelay(delay);
}
#elif RTOS_FREERTOS
#include <FreeRTOS.h>
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"

usb_osal_thread_t usb_osal_thread_create(const char *name, uint32_t stack_size, uint32_t prio, usb_thread_entry_t entry, void *args)
{
    TaskHandle_t htask = NULL;
    stack_size /= sizeof(StackType_t);
    xTaskCreate(entry, name, stack_size, args, configMAX_PRIORITIES - 1 - prio, &htask);
    return (usb_osal_thread_t)htask;
}

void usb_osal_thread_delete(usb_osal_thread_t thread)
{
    vTaskDelete(thread);
}

usb_osal_sem_t usb_osal_sem_create(uint32_t initial_count)
{
    return (usb_osal_sem_t)xSemaphoreCreateCounting(1, initial_count);
}

void usb_osal_sem_delete(usb_osal_sem_t sem)
{
    vSemaphoreDelete((SemaphoreHandle_t)sem);
}

int usb_osal_sem_take(usb_osal_sem_t sem, uint32_t timeout)
{
    if (timeout == USB_OSAL_WAITING_FOREVER) {
        return (xSemaphoreTake((SemaphoreHandle_t)sem, portMAX_DELAY) == pdPASS) ? 0 : -USB_ERR_TIMEOUT;
    } else {
        return (xSemaphoreTake((SemaphoreHandle_t)sem, pdMS_TO_TICKS(timeout)) == pdPASS) ? 0 : -USB_ERR_TIMEOUT;
    }
}

#if 1
int usb_osal_sem_give(usb_osal_sem_t sem)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int ret;

    if (AlOsal_In_Intr()) {
        ret = xSemaphoreGiveFromISR((SemaphoreHandle_t)sem, &xHigherPriorityTaskWoken);
        if (ret == pdPASS) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    } else {
        ret = xSemaphoreGive((SemaphoreHandle_t)sem);
    }

    return (ret == pdPASS) ? 0 : -USB_ERR_TIMEOUT;
}
#endif

//extern volatile uint64_t ullPortInterruptNesting;
#if 0
int usb_osal_sem_give(usb_osal_sem_t sem)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int ret;

    if (ullPortInterruptNesting == 0) {
        ret = xSemaphoreGive((SemaphoreHandle_t)sem);
    } else {
        ret = xSemaphoreGiveFromISR((SemaphoreHandle_t)sem, &xHigherPriorityTaskWoken);
        if (ret == pdPASS) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }

    return (ret == pdPASS) ? 0 : -USB_ERR_TIMEOUT;
}
#endif

void usb_osal_sem_reset(usb_osal_sem_t sem)
{
    xQueueReset((QueueHandle_t)sem);
}

usb_osal_mutex_t usb_osal_mutex_create(void)
{
    return (usb_osal_mutex_t)xSemaphoreCreateMutex();
}

void usb_osal_mutex_delete(usb_osal_mutex_t mutex)
{
    vSemaphoreDelete((SemaphoreHandle_t)mutex);
}

int usb_osal_mutex_take(usb_osal_mutex_t mutex)
{
    return (xSemaphoreTake((SemaphoreHandle_t)mutex, portMAX_DELAY) == pdPASS) ? 0 : -USB_ERR_TIMEOUT;
}

int usb_osal_mutex_give(usb_osal_mutex_t mutex)
{
    return (xSemaphoreGive((SemaphoreHandle_t)mutex) == pdPASS) ? 0 : -USB_ERR_TIMEOUT;
}

usb_osal_mq_t usb_osal_mq_create(uint32_t max_msgs)
{
    return (usb_osal_mq_t)xQueueCreate(max_msgs, sizeof(uintptr_t));
}

void usb_osal_mq_delete(usb_osal_mq_t mq)
{
    vQueueDelete((QueueHandle_t)mq);
}

int usb_osal_mq_send(usb_osal_mq_t mq, uintptr_t addr)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int ret;

    ret = xQueueSendFromISR((usb_osal_mq_t)mq, &addr, &xHigherPriorityTaskWoken);
    if (ret == pdPASS) {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    return (ret == pdPASS) ? 0 : -USB_ERR_TIMEOUT;
}

int usb_osal_mq_recv(usb_osal_mq_t mq, uintptr_t *addr, uint32_t timeout)
{
    if (timeout == USB_OSAL_WAITING_FOREVER) {
        return (xQueueReceive((usb_osal_mq_t)mq, addr, portMAX_DELAY) == pdPASS) ? 0 : -USB_ERR_TIMEOUT;
    } else {
        return (xQueueReceive((usb_osal_mq_t)mq, addr, pdMS_TO_TICKS(timeout)) == pdPASS) ? 0 : -USB_ERR_TIMEOUT;
    }
}

static void __usb_timeout(TimerHandle_t *handle)
{
    struct usb_osal_timer *timer = (struct usb_osal_timer *)pvTimerGetTimerID((TimerHandle_t)handle);

    timer->handler(timer->argument);
}

struct usb_osal_timer *usb_osal_timer_create(const char *name, uint32_t timeout_ms, usb_timer_handler_t handler, void *argument, bool is_period)
{
    struct usb_osal_timer *timer;

    timer = pvPortMalloc(sizeof(struct usb_osal_timer));

    if (timer == NULL) {
        return NULL;
    }
    memset(timer, 0, sizeof(struct usb_osal_timer));

    timer->handler = handler;
    timer->argument = argument;

    timer->timer = (void *)xTimerCreate("usb_tim", pdMS_TO_TICKS(timeout_ms), is_period, timer, (TimerCallbackFunction_t)__usb_timeout);
    if (timer->timer == NULL) {
        return NULL;
    }
    return timer;
}

void usb_osal_timer_delete(struct usb_osal_timer *timer)
{
    xTimerStop(timer->timer, 0);
    xTimerDelete(timer->timer, 0);
    vPortFree(timer);
}

#if 1
void usb_osal_timer_start(struct usb_osal_timer *timer)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int ret;

    if (AlOsal_In_Intr()) {
        ret = xTimerStartFromISR(timer->timer, &xHigherPriorityTaskWoken);
        if (ret == pdPASS) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    } else {
        xTimerStart(timer->timer, 0);
    }
}
#endif
#if 0
void usb_osal_timer_start(struct usb_osal_timer *timer)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int ret;

    if (ullPortInterruptNesting == 0) {
        xTimerStart(timer->timer, 0);
    }else {
        ret = xTimerStartFromISR(timer->timer, &xHigherPriorityTaskWoken);
        if (ret == pdPASS) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}
#endif

void usb_osal_timer_stop(struct usb_osal_timer *timer)
{
    xTimerStop(timer->timer, 0);
}

#if 1
size_t usb_osal_enter_critical_section(void)
{
    size_t ret;

    if (AlOsal_In_Intr()) {
        ret = taskENTER_CRITICAL_FROM_ISR();
    } else {
        taskENTER_CRITICAL();
        ret = 1;
    }

    return ret;
}
#endif

#if 0
size_t usb_osal_enter_critical_section(void)
{
    size_t ret;

    if (ullPortInterruptNesting == 0) {
        taskENTER_CRITICAL();
        ret = 1;
    } else {
        ret = taskENTER_CRITICAL_FROM_ISR();
    }

    return ret;
}
#endif



#if 1
void usb_osal_leave_critical_section(size_t flag)
{
    if (AlOsal_In_Intr()) {
        taskEXIT_CRITICAL_FROM_ISR(flag);
    } else {
        taskEXIT_CRITICAL();
    }
}
#endif

#if 0
void usb_osal_leave_critical_section(size_t flag)
{
    if (ullPortInterruptNesting == 0) {
        taskEXIT_CRITICAL();
    } else {
        taskEXIT_CRITICAL_FROM_ISR(flag);
    }
}
#endif

void usb_osal_msleep(uint32_t delay)
{
    vTaskDelay(pdMS_TO_TICKS(delay));
}
#else
usb_osal_thread_t usb_osal_thread_create(const char *name, uint32_t stack_size, uint32_t prio, usb_thread_entry_t entry, void *args)
{
    return (usb_osal_thread_t)0;
}

void usb_osal_thread_delete(usb_osal_thread_t thread)
{

}

usb_osal_sem_t usb_osal_sem_create(uint32_t initial_count)
{
    return (usb_osal_sem_t)0;
}

void usb_osal_sem_delete(usb_osal_sem_t sem)
{

}

int usb_osal_sem_take(usb_osal_sem_t sem, uint32_t timeout)
{
    return (int)0;
}

int usb_osal_sem_give(usb_osal_sem_t sem)
{
    return (int)0;
}

usb_osal_mutex_t usb_osal_mutex_create(void)
{
    return (usb_osal_mutex_t)0;
}

void usb_osal_mutex_delete(usb_osal_mutex_t mutex)
{

}

int usb_osal_mutex_take(usb_osal_mutex_t mutex)
{
    return (int)0;
}

int usb_osal_mutex_give(usb_osal_mutex_t mutex)
{
    return (int)0;
}

usb_osal_mq_t usb_osal_mq_create(uint32_t max_msgs)
{
    return (usb_osal_mq_t)0;
}

int usb_osal_mq_send(usb_osal_mq_t mq, uintptr_t addr)
{
    return (int)0;
}

int usb_osal_mq_recv(usb_osal_mq_t mq, uintptr_t *addr, uint32_t timeout)
{
    return (int)0;
}

size_t usb_osal_enter_critical_section(void)
{
    return (size_t)0;
}

void usb_osal_leave_critical_section(size_t flag)
{

}

void usb_osal_msleep(uint32_t delay)
{

}

struct usb_osal_timer *usb_osal_timer_create(const char *name, uint32_t timeout_ms, usb_timer_handler_t handler, void *argument, bool is_period)
{
    struct usb_osal_timer *timer;
    return timer;
}

void usb_osal_timer_delete(struct usb_osal_timer *timer)
{

}

void usb_osal_timer_start(struct usb_osal_timer *timer)
{

}

void usb_osal_timer_stop(struct usb_osal_timer *timer)
{

}
#endif