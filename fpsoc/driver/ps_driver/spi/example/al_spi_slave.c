/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_spi_hal.h"
#include "al_spinor.h"

#define RECV_SIZE 5

AL_SPI_HalStruct *Handle;

AL_SPI_ConfigsStruct SpiInitConfigs =
{
    .Mode               = SPI_SLAVE_MODE,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
    .ClkDiv             = 40
};

AL_U8 SendData[RECV_SIZE] = { 0x12,0x34,0x56,0x78,0x9A };
AL_U8 RecvData[RECV_SIZE] = { 0x0 };

/**
 * @note Chip SPI MOSI link Master MOSI when Chip as Slave
*/
void main(void)
{
    AL_U32 i;
    AL_S32 ret = AL_OK;

    printf("Start FPSoc Spi Slave Test\r\n");

    ret = AlSpi_Hal_Init(&Handle, &SpiInitConfigs, AL_NULL, 0);
    if (AL_OK != ret) {
        printf("AlSpi_Hal_Init error, ret:0x%x\r\n", ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
#ifdef RX_ONLY
    Handle->Dev.Configs.Trans.TransMode  = SPI_RX_ONLY;
    ret = AlSpi_Hal_RecvDataBlock(Handle, RecvData, RECV_SIZE, 100000);
    if (AL_OK != ret) {
        printf("AlSpi_Hal_RecvDataBlock error, ret:0x%x\r\n", ret);
        while(1);
    }
#else
    /* full duplex mode */
    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_RX;
    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, RECV_SIZE, RecvData, RECV_SIZE, 100000);
    if (AL_OK != ret) {
        printf("AlSpi_Hal_TranferDataBlock error, ret:0x%x\r\n", ret);
        while(1);
    }
#endif

    for (i = 0; i < RECV_SIZE; i++)
        printf("RecvData[%d]:%d\r\n", i, RecvData[i]);

    printf("AlSpi Spi Slave Test Pass\r\n");
}

