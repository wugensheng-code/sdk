#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "al_core.h"

#ifdef ENABLE_MMU
#include "al_mmu.h"
#endif




#define AL_USB_NUM_INSTANCE    (2)

#define USB0__BASE_ADDR        (0xF8180000UL)
#define USB1__BASE_ADDR        (0xF8180000UL)

typedef struct
{
    AL_U32                  DeviceId;
    AL_U32                  BaseAddress;
    AL_U32                  IntrNum;
} AL_USB_HwConfigStruct;

AL_USB_HwConfigStruct AlUsb_HwConfig[AL_USB_NUM_INSTANCE] =
{
    {
        .DeviceId       = 0,
        .BaseAddress    = USB0__BASE_ADDR,
        .IntrNum        = SOC_USB0_IRQn,
    },
    {
        .DeviceId       = 1,
        .BaseAddress    = USB1__BASE_ADDR,
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

extern void USBD_IRQHandler(void);
AL_VOID AlUsb_Dev_IntrHandler(AL_VOID *Instance)
{
    USBD_IRQHandler();
}

static AL_USB_DevStruct AL_USB_DevInstance[AL_USB_NUM_INSTANCE];

AL_S32 AlUsb_Hal_Init(AL_USB_HalStruct *Handle, AL_U32 DevId, AL_USB_InitStruct *InitConfig)
{
    AL_USB_DevStruct *Dev;
    Dev = &AL_USB_DevInstance[DevId];

	Dev->DevId        = DevId;
    Dev->BaseAddr     = AlUsb_Dev_LookupConfig(DevId)->BaseAddress;
    Dev->IntrNum      = AlUsb_Dev_LookupConfig(DevId)->IntrNum;

	printf("Dev->IntrNum : %d \r\n", Dev->IntrNum);
	(AL_VOID)AlIntr_RegHandler(Dev->IntrNum, AL_NULL, AlUsb_Dev_IntrHandler, Dev);
}

extern volatile uint8_t dtr_enable;
extern void cdc_acm_init(void);
extern void cdc_acm_data_send_with_dtr_test(void);

AL_VOID main()
{
    AL_U8 *str = "Hello World From Anlogic!";
    AL_U32 Size = 0x1;

    printf("str = %s \r\n", str);

    AlCache_DisableMmu();

    #if 0
    extern AL_U32 _no_cache_section_start;
    mmu_settlb((AL_UINTPTR) &(_no_cache_section_start), NORM_NONCACHE);
    #endif

    for (int i = 0; i < 20; i ++)
    {
        AL_U8 *p;

        p = malloc(Size);
        if (p == NULL) {
            printf("malloc failed Size = %d \r\n", Size);
        } else {
            printf("malloc succeed %p size=%d \r\n", p, Size);
        }
        Size <<= 1;
        free(p);
    }

    AL_USB_HalStruct usb0_hal;
    AL_S32 ret = AlUsb_Hal_Init(&usb0_hal, 0, NULL);
    if (ret != AL_OK) {
        printf("AlUsb_Hal_Init error\r\n");
        return ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);


    AL_U32 u = 0;
    cdc_acm_init();

    while (1) {
        AlSys_MDelay(2000);
        usbd_cdc_acm_set_dtr(0, 1);
        cdc_acm_data_send_with_dtr_test();
    }

    while (u < 2) {
        if (dtr_enable) {
            u++;
            cdc_acm_data_send_with_dtr_test();
        }
    }

    while (1) {
        ;
    }
}
