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
#define RECV_SIZE 5

/************************** Variable Definitions *****************************/
AL_SPI_HalStruct *Handle;

AL_SPI_ConfigsStruct SpiInitConfigs =
{
    .Mode               = SPI_SLAVE_MODE,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
};

AL_U8 SendData[RECV_SIZE] = { 0x12,0x34,0x56,0x78,0x9A };
AL_U8 RecvData[RECV_SIZE] = { 0x0 };

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 *
 * This function initializes the SPI hardware using the provided configurations. It sets up the SPI device
 * in slave mode with the specified protocol format, clock mode, and slave toggle settings. It logs an error
 * message if the initialization fails.
 *
 * @param  None.
 *
 * @return None.
 *
 * @note Chip SPI MOSI link Master MOSI when Chip as Slave
 *
 */
void main(void)
{
    AL_U32 i;
    AL_S32 ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_ERROR, "Start FPSoc Spi Slave Test\r\n");

    ret = AlSpi_Hal_Init(&Handle, &SpiInitConfigs, AL_NULL, 0);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi_Hal_Init error, ret:0x%x\r\n", ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
#ifdef RX_ONLY
    Handle->Dev.Configs.Trans.TransMode  = SPI_RX_ONLY;
    ret = AlSpi_Hal_RecvDataBlock(Handle, RecvData, RECV_SIZE, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi_Hal_RecvDataBlock error, ret:0x%x\r\n", ret);
        while(1);
    }
#else
    /* full duplex mode */
    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_RX;
    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, RECV_SIZE, RecvData, RECV_SIZE, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi_Hal_TranferDataBlock error, ret:0x%x\r\n", ret);
        while(1);
    }
#endif

    for (i = 0; i < RECV_SIZE; i++)
        AL_LOG(AL_LOG_LEVEL_ERROR, "RecvData[%d]:%d\r\n", i, RecvData[i]);

    AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi Spi Slave Test Pass\r\n");
}

