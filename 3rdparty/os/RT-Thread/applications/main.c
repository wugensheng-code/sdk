/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-02     armink       the first version
 */

#include <stdlib.h>
#include <rtthread.h>

#ifndef ULOG_USING_SYSLOG
#define LOG_TAG              "example"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
#else
#include <syslog.h>
#endif /* ULOG_USING_SYSLOG */

void ulog_example(void)
{
    int count = 0;

#ifdef ULOG_USING_SYSLOG
    openlog("example1", 0, 0);
#endif

    while (count++ < 50)
    {
#ifndef ULOG_USING_SYSLOG
        /* output different level log by LOG_X API */
        LOG_D("LOG_D(%d): RT-Thread is an open source IoT operating system from China.", count);
        LOG_I("LOG_I(%d): RT-Thread is an open source IoT operating system from China.", count);
        LOG_W("LOG_W(%d): RT-Thread is an open source IoT operating system from China.", count);
        LOG_E("LOG_E(%d): RT-Thread is an open source IoT operating system from China.", count);
        ulog_d("test", "ulog_d(%d): RT-Thread is an open source IoT operating system from China.", count);
        ulog_i("test", "ulog_i(%d): RT-Thread is an open source IoT operating system from China.", count);
        ulog_w("test", "ulog_w(%d): RT-Thread is an open source IoT operating system from China.", count);
        ulog_e("test", "ulog_e(%d): RT-Thread is an open source IoT operating system from China.", count);

#ifdef ULOG_USING_FILTER
        if (count == 20)
        {
            /* Set the global filer level is INFO. All of DEBUG log will stop output */
            ulog_global_filter_lvl_set(LOG_LVL_INFO);
            /* Set the test tag's level filter's level is ERROR. The DEBUG, INFO, WARNING log will stop output. */
            ulog_tag_lvl_filter_set("test", LOG_LVL_ERROR);
        }
        else if (count == 30)
        {
            /* Set the example tag's level filter's level is LOG_FILTER_LVL_SILENT, the log enter silent mode. */
            ulog_tag_lvl_filter_set("example", LOG_FILTER_LVL_SILENT);
            /* Set the test tag's level filter's level is WARNING. The DEBUG, INFO log will stop output. */
            ulog_tag_lvl_filter_set("test", LOG_LVL_WARNING);
        }
        else if (count == 40)
        {
            /* Set the test tag's level filter's level is LOG_FILTER_LVL_ALL. All level log will resume output. */
            ulog_tag_lvl_filter_set("test", LOG_FILTER_LVL_ALL);
            /* Set the global filer level is LOG_FILTER_LVL_ALL. All level log will resume output */
            ulog_global_filter_lvl_set(LOG_FILTER_LVL_ALL);
        }
#endif /* ULOG_USING_FILTER */

#else
        /* output different priority log by syslog API */
        syslog(LOG_INFO, "syslog(%d) LOG_INFO: RT-Thread is an open source IoT operating system from China.", count);
        syslog(LOG_DEBUG, "syslog(%d) LOG_DEBUG: RT-Thread is an open source IoT operating system from China.", count);
        syslog(LOG_WARNING, "syslog(%d) LOG_WARNING: RT-Thread is an open source IoT operating system from China.", count);
        syslog(LOG_ERR, "syslog(%d) LOG_ERR: RT-Thread is an open source IoT operating system from China.", count);
        syslog(LOG_INFO | LOG_MAIL, "syslog(%d) LOG_INFO | LOG_MAIL: RT-Thread is an open source IoT operating system from China.", count);
        syslog(LOG_DEBUG | LOG_DAEMON, "syslog(%d) LOG_DEBUG | LOG_DAEMON: RT-Thread is an open source IoT operating system from China.", count);
        syslog(LOG_WARNING | LOG_AUTH, "syslog(%d) LOG_WARNING | LOG_AUTH: RT-Thread is an open source IoT operating system from China.", count);
        syslog(LOG_ERR | LOG_SYSLOG, "syslog(%d) LOG_ERR | LOG_SYSLOG: RT-Thread is an open source IoT operating system from China.", count);

        if (count == 20)
        {
            /* Set log priority mask. Only output ERR and WARNING log. */
            setlogmask(LOG_MASK(LOG_ERR) | LOG_MASK(LOG_WARNING));
        }
        else if (count == 40)
        {
            /* Set log priority mask. The log which level is less than ERROR will stop output. */
            setlogmask(LOG_UPTO(LOG_ERR));
        }
#endif /* ULOG_USING_SYSLOG */

        rt_thread_delay(rt_tick_from_millisecond(rand() % 1000));
    }
}



#define SAMPLE_UART_NAME       "uart0"

/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void serial_thread_entry(void *parameter)
{
    char ch;

    while (1)
    {
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }
        /* 读取到的数据通过串口错位输出 */
        ch = ch + 1;
        rt_device_write(serial, 0, &ch, 1);
    }
}

static int uart_sample()
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
    char str[] = "hello RT-Thread!\r\n";

    rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);

    /* 查找系统中的串口设备 */
    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_STREAM);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);
    /* 发送字符串 */
    rt_device_write(serial, 0, str, (sizeof(str) - 1));

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}


int main()
{
    // uart_sample();

    // ulog_example();
    
    return 0;
}