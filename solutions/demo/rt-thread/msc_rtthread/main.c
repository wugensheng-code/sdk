#include "rtthread.h"
#include "al_core.h"

#ifdef ENABLE_MMU
#include "al_mmu.h"
#endif

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       2048
#define THREAD_TIMESLICE        5


typedef enum IRQn {
	SOC_INT75_IRQn           = 88, //APU:88  RPU:75
	SOC_INT76_IRQn           = 89, //APU:89  RPU:76
    SOC_INT_MAX_IRQn,
} IRQn_Type;

typedef unsigned int            AL_U32;

#define AL_USB_NUM_INSTANCE    (2)

#define USB0__BASE_ADDR        (0xF8180000UL)
#define USB1__BASE_ADDR        (0xF8180000UL)

#define USB_CLOCK              (40000000UL)

typedef struct
{
    AL_U32                  DeviceId;
    AL_U32                  BaseAddress;
    AL_U32                  ClockHz;
    AL_U32                  IntrNum;
} AL_USB_HwConfigStruct;

AL_USB_HwConfigStruct AlUsb_HwConfig[AL_USB_NUM_INSTANCE] =
{
    {
        .DeviceId       = 0,
        .BaseAddress    = USB0__BASE_ADDR,
        .ClockHz        = USB_CLOCK,
        .IntrNum        = SOC_USB0_IRQn,
    },
    {
        .DeviceId       = 1,
        .BaseAddress    = USB1__BASE_ADDR,
        .ClockHz        = USB_CLOCK,
        .IntrNum        = SOC_USB1_IRQn
    },
};

AL_USB_HwConfigStruct *AlUsb_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AL_USB_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_USB_NUM_INSTANCE; Index++) {
        if (AlUsb_HwConfig[Index].DeviceId == DevId) {
            ConfigPtr = &AlUsb_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

typedef struct
{
    AL_U32                  BaudRate;
    AL_U32                  WordLength;
    AL_U32                  StopBits;
    AL_U32                  Mode;
    AL_U32                  HwFlowCtl;
} AL_USB_InitStruct;

typedef struct
{
    AL_REG                        BaseAddr;
    AL_USB_InitStruct             Configs;
    AL_U32                        IntrNum;
    AL_U32                        DevId;
    AL_U32                        InputClockHz;
} AL_USB_DevStruct;

typedef struct
{
    AL_USB_DevStruct            *Dev;

} AL_USB_HalStruct;

extern void USBH_IRQHandler(void);
AL_VOID AlUsb_Dev_IntrHandler(AL_VOID *Instance)
{
    USBH_IRQHandler();
}

static AL_USB_DevStruct AL_USB_DevInstance[AL_USB_NUM_INSTANCE];

AL_S32 AlUsb_Hal_Init(AL_USB_HalStruct *Handle, AL_U32 DevId, AL_USB_InitStruct *InitConfig)
{
    AL_USB_DevStruct *Dev;
    Dev = &AL_USB_DevInstance[DevId];

	Dev->DevId        = DevId;
    Dev->BaseAddr     = AlUsb_Dev_LookupConfig(DevId)->BaseAddress;
    Dev->IntrNum      = AlUsb_Dev_LookupConfig(DevId)->IntrNum;
    Dev->InputClockHz = AlUsb_Dev_LookupConfig(DevId)->ClockHz;

	rt_kprintf("Dev->IntrNum : %d \r\n", Dev->IntrNum);
	(AL_VOID)AlIntr_RegHandler(Dev->IntrNum, AL_NULL, AlUsb_Dev_IntrHandler, Dev);
}


extern void usbh_initialize(void);
extern void usbh_class_test(void);

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

    AlCache_DisableMmu();

    #ifdef ENABLE_MMU
    extern AL_U32 _no_cache_section_start;
    AL_UINTPTR gbe_buffer_addr = (AL_UINTPTR) &(_no_cache_section_start);

    mmu_settlb(gbe_buffer_addr, NORM_NONCACHE);
    #endif

    AL_USB_HalStruct usb0_hal;
    AL_S32 ret = AlUsb_Hal_Init(&usb0_hal, 0, NULL);
    if (ret != AL_OK) {
        rt_kprintf("AlUsb_Hal_Init error\r\n");
        return ;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_U32 u = 0;
    usbh_initialize();
    usbh_class_test();

    while (1) {
        //AlSys_MDelay(2000);
        rt_thread_mdelay(500);
        ;
    }
    return 0;
}
