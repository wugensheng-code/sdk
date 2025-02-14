#include "rtthread.h"
#include "al_core.h"
#include "al_usb_hal.h"
#include "al_gpio_hal.h"

#ifdef ENABLE_MMU
#include "al_cache.h"
#endif

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       2048
#define THREAD_TIMESLICE        5

#define AL_USB_DEVICE_ID     0

#define AL_GPIO_DEVICE_ID       0
#define AL_USB_PHY_RESET_PIN    7 /* PS_IO7 */

extern int usbh_initialize(uint8_t busid, uint32_t reg_base);
extern void USBH_IRQHandler(uint8_t busid);

AL_VOID AlUsb_Dev_IntrHandler(AL_VOID *Instance)
{
    USBH_IRQHandler(0);
}

AL_VOID AlUsb_PhyHardReset()
{
    AL_GPIO_HalStruct *GPIO;
    AL_U32 RawVal = 0;

    AL_S32 ret = AlGpio_Hal_Init(&GPIO, AL_GPIO_DEVICE_ID, NULL);

    if (ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init success");
    }
    else {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init failed");
    }

    AlGpio_Hal_WritePin(GPIO, AL_USB_PHY_RESET_PIN, 0x1);
    AlGpio_Hal_WritePin(GPIO, AL_USB_PHY_RESET_PIN, 0x0);
    AlSys_MDelay(20);
    AlGpio_Hal_WritePin(GPIO, AL_USB_PHY_RESET_PIN, 0x1);
}

static rt_thread_t tid1 = RT_NULL;

/* 线程 1 的入口函数 */
static void thread1_entry(void *parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* 线程 1 采用低优先级运行，一直打印计数值 */
        rt_kprintf("thread1 count: %d\n", count ++);
        rt_thread_mdelay(500);
    }
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
static struct rt_thread thread2;
/* 线程 2 入口 */
static void thread2_entry(void *param)
{
    rt_uint32_t count = 0;

    /* 线程 2 拥有较高的优先级，以抢占线程 1 而获得执行 */
    for (count = 0; count < 10000 ; count++)
    {
        /* 线程 2 打印计数值 */
        rt_kprintf("thread2 count: %d\n", count);
        rt_thread_mdelay(500);
    }
    rt_kprintf("thread2 exit\n");
    /* 线程 2 运行结束后也将自动被系统脱离 */
}

/* 线程示例 */
int thread_sample(void)
{
    /* 创建线程 1，名称是 thread1，入口是 thread1_entry*/
    tid1 = rt_thread_create("thread1",
                            thread1_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    /* 初始化线程 2，名称是 thread2，入口是 thread2_entry */
    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),
                   THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    rt_thread_startup(&thread2);

    return 0;
}
MSH_CMD_EXPORT(thread_sample, thread sample);

int main()
{
    AL_S8 *str = "Hello World From Anlogic!";

    rt_kprintf("str = %s \r\n", str);
    //AlCache_DisableMmu();

    AlUsb_PhyHardReset();

    AL_USB_HalStruct *USB0;
    AL_S32 ret = AlUsb_Hal_Init(&USB0, AL_USB_DEVICE_ID, AlUsb_Dev_IntrHandler);
    if (ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlUsb_Hal_Init success");
    }
    else {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlUsb_Hal_Init failed");
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    usbh_initialize(0, USB0__BASE_ADDR);
    usbh_class_test();

    while (1) {
        //AlSys_MDelay(2000);
        rt_thread_mdelay(500);
        ;
    }
    return 0;
}
