/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/********************************* including Files *********************************/
#include "al_hal.h"
#include "al_spi_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_SPI_HwConfigStruct AlSpi_HwCfg[AL_SPI_NUM_INSTANCE] =
{
#ifdef SPI0PS_ENABLE
    {
        .DeviceId           = 0,
        .BaseAddress        = SPI0_BASE_ADDR,
        .InputClockHz       = SPI_CLOCK,
        .IoClockHz          = SPI0_IO_FREQ,
        .IntrId             = SOC_SPI0_MST_IRQn,
        .FifoLen            = SPI_FIFO_LENGTH,
        .CsSel              = SPI_CS0_EN
    },
#endif
#ifdef SPI1PS_ENABLE
    {
        .DeviceId           = 1,
        .BaseAddress        = SPI1_BASE_ADDR,
        .InputClockHz       = SPI_CLOCK,
        .IoClockHz          = SPI1_IO_FREQ,
        .IntrId             = SOC_SPI1_MST_IRQn,
        .FifoLen            = SPI_FIFO_LENGTH,
        .CsSel              = SPI_CS0_EN
    }
#endif
};
