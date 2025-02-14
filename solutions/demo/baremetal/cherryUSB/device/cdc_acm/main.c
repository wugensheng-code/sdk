#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "al_core.h"
#include "al_usb_hal.h"
#include "al_gpio_hal.h"

#ifdef ENABLE_MMU
#include "al_cache.h"
#endif

#define AL_USB_DEVICE_ID        0

#define AL_GPIO_DEVICE_ID       0
#define AL_USB_PHY_RESET_PIN    7 /* PS_IO7 */

extern void cdc_acm_init(uint8_t busid, uint32_t reg_base);
extern void cdc_acm_data_send_with_dtr_test(uint8_t busid);
extern void USBD_IRQHandler(uint8_t busid);

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

AL_VOID AlUsb_Dev_IntrHandler()
{
    USBD_IRQHandler(0);
}

AL_VOID main()
{
    AL_U8 *str = "Hello World From Anlogic!";
    AL_U32 Size = 0x1;
    AL_S32 Ret;

    AL_LOG(AL_LOG_LEVEL_INFO, "str = %s \r\n", str);

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

    cdc_acm_init(0, USB0__BASE_ADDR);

    while (1) {
        AlSys_MDelay(2000);
        cdc_acm_data_send_with_dtr_test(0);
    }
}
