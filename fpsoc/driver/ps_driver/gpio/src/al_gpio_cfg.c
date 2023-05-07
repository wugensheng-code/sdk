#include "al_gpio_hw.h"


AL_GPIO_HwConfigStruct AlGpio_HwCfg[AL_GPIO_NUM_INSTANCE] =
{
	{
        .DeviceId           = 0,
        .BaseAddress        = GPIO_BASE_ADDR,
    }
};