/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/***************************** Include Files *********************************/
#include "al_usb_hw.h"

/************************** Variable Prototypes ******************************/
AL_USB_HwConfigStruct AlUsb_HwCfg[AL_USB_NUM_INSTANCE] =
{
#ifdef USB0PS_ENABLE
    {
        .DeviceId           = 0,
        .BaseAddress	    = USB0__BASE_ADDR,
        .IntrId             = SOC_USB0_IRQn,
    },
#endif

#ifdef USB1PS_ENABLE
    {
        .DeviceId           = 1,
        .BaseAddress	    = USB1__BASE_ADDR,
        .IntrId             = SOC_USB1_IRQn,
    }
#endif
};
