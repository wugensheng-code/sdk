/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_spi_hal.h"
#include "al_spinor.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define RECV_SIZE 32

/************************** Variable Definitions *****************************/
AL_SPI_HalStruct *Handle;

AL_SPI_ConfigsStruct SpiInitConfigs =
{
    .Mode               = SPI_MASTER_MODE,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
};

AL_U8 SendData[RECV_SIZE] =
{
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48
};

AL_U8 RecvData[RECV_SIZE] = { 0x0 };

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 * @note Chip SPI MOSI link Master MOSI when Chip as Slave
*/
void main(void)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_ERROR, "Start FPSoc Spi Master Test\r\n");

    Ret = AlSpi_Hal_Init(&Handle, &SpiInitConfigs, AL_NULL, 0);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi_Hal_Init error, Ret:0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    /* full duplex mode */
     Handle->Dev.Configs.Trans.TransMode  = SPI_TX_RX;
    Ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, RECV_SIZE, RecvData, RECV_SIZE, 100000);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi_Hal_TranferDataBlock error, Ret:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "AlSpi_Hal_TranferDataBlock Recv Data:\r\n");
    for (AL_U32 Index = 0; Index < RECV_SIZE; Index++)
    {
        printf("0x%x ", RecvData[Index]);
        if ((Index+1) % 16 == 0) {
            printf("\r\n");
        }
    }

    return AL_OK;
}

