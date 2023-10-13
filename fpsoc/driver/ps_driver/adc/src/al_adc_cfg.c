/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/********************************* including Files *********************************/
#include "al_adc_hw.h"

/********************************* Constant Definitions *****************************/

AL_ADC_HwConfigStruct AlAdc_HwConfig[AL_ADC_NUM_INSTANCE] =
{
    {
        .DeviceId           = 0,
        .AdcBaseAddress     = ADC__BASE_ADDR,
        .GpBaseAddress      = GP0_BASE_ADDR,
        .IntrNum            = SOC_ADC_IRQn
    }
};