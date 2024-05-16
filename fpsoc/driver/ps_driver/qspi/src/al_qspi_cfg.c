/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/********************************* including Files *********************************/
#include "al_qspi_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_QSPI_HwConfigStruct AlQSPI_HwCfg[AL_QSPI_NUM_INSTANCE] =
{
    {
        .IoFreq             = QSPI_IO_FREQ,
        .DeviceId           = 0,
        .BaseAddress        = QSPI_BASE_ADDR,
        .InterrupId         = SOC_QSPI_IRQn,
        .FifoLen            = QSPI_FIFO_LENGTH,
        .CsSel              = QSPI_CS0_EN
        .InputClockHz       = QSPI_CLOCK,
    }
};
