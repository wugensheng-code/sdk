#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "al_core.h"
#include "al_usb_hal.h"

#ifdef ENABLE_MMU
#include "al_mmu.h"
#endif

#define AL_USB_DEVICE_ID     0

extern volatile uint8_t dtr_enable;
extern void cdc_acm_init(void);
extern void cdc_acm_data_send_with_dtr_test(void);

extern void USBD_IRQHandler(void);
AL_VOID AlUsb_Dev_IntrHandler()
{
    USBD_IRQHandler();
}

AL_VOID main()
{
    AL_U8 *str = "Hello World From Anlogic!";
    AL_U32 Size = 0x1;
    AL_S32 Ret;

    AL_LOG(AL_LOG_LEVEL_INFO, "str = %s \r\n", str);

    AlCache_DisableMmu();

    for (int i = 0; i < 20; i ++)
    {
        AL_U8 *p;

        p = malloc(Size);
        if (p == NULL) {
            AL_LOG(AL_LOG_LEVEL_INFO, "malloc failed Size = %d \r\n", Size);
        } else {
            AL_LOG(AL_LOG_LEVEL_INFO, "malloc succeed %p size=%d \r\n", p, Size);
        }
        Size <<= 1;
        free(p);
    }

    AL_USB_HalStruct *USB0;
    AL_S32 ret = AlUsb_Hal_Init(&USB0, AL_USB_DEVICE_ID, AlUsb_Dev_IntrHandler);
    if (ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlUsb_Hal_Init success");
    }
    else {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlUsb_Hal_Init failed");
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    cdc_acm_init();

    while (1) {
        AlSys_MDelay(2000);
        usbd_cdc_acm_set_dtr(0, 1);
        cdc_acm_data_send_with_dtr_test();
    }
}
