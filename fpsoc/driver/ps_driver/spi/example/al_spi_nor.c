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
#define AL_SPI_DEVICE_ID            1


/************************** Variable Definitions *****************************/
AL_SPI_HalStruct *Handle;

AL_SPI_ConfigsStruct SpiInitConfigs =
{
    .Mode               = SPI_MASTER_MODE,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
};

AL_U8 __attribute__((aligned(4))) SendData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) RecvData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) FlashId[10] = { 0x0 };

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 *
 * This function sends a reset command to the NOR flash device using SPI communication.
 * It sets the transmission mode to SPI_TX_ONLY and sends the reset command defined by NOR_OP_INFINEON_SRST.
 * If the command fails to send, it logs an error with the return value.
 *
 * @param  None.
 *
 * @return None.
 *
 */
void AlNor_Reset(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_INFINEON_SRST;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Reset error, ret:0x%x\r\n", ret);
    }
}


/**
 *
 * This function sends a write enable command to the NOR flash device to allow modifications.
 * It sets the transmission mode to SPI_TX_ONLY and sends the write enable command defined by NOR_OP_WREN.
 * If the command fails to send, it logs an error with the return value.
 *
 * @param  None.
 *
 * @return None.
 *
 */
void AlNor_Wren(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_WREN;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Wren error, ret:0x%x\r\n", ret);
    };
}

/**
 *
 *
 * This function allows setting the status register of the NOR flash device to a specific value.
 * It sets the transmission mode to SPI_TX_ONLY, prepares the command and data, and sends them.
 * If the command fails to send, it logs an error with the return value.
 *
 * @param data The value to set the status register to.
 *
 * @return None.
 *
 */
void AlNor_SetStatus(AL_U8 data)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_WRSR;
    SendData[1] = data;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetStatus error, ret:0x%x\r\n", ret);
    }
}

/**
 *
 * This function continuously reads the status register of the NOR flash device until the WIP bit is cleared,
 * indicating that the write or erase operation is complete. It uses SPI_EEPROM mode for communication.
 * If reading the status register fails, it logs an error.
 *
 * @param  None.
 *
 * @return None.
 *
 */
void AlNor_WriteWip(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDSR;

    do {
        ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
        if (AL_OK != ret) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WriteWip error, ret:0x%x\r\n");
        }
#ifdef SPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_ERROR, "WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}

/**
 *
 * This function reads the status register of the NOR flash device and logs its value.
 * It sets the transmission mode to SPI_EEPROM and sends the read status register command.
 * If the command fails to send, it logs an error with the return value.
 *
 * @param  None.
 *
 * @return None.
 *
 */
void AlNor_ReadStatus(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDSR;

    AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadStatus error, ret:0x%x\r\n", ret);
    }
#ifdef SPI_DEBUG
    AL_LOG(AL_LOG_LEVEL_ERROR, "Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
}

/**
 *
 * This function sends a sector erase command to the NOR flash device.
 * It sets the transmission mode to SPI_TX_ONLY and sends the sector erase command along with the address to erase.
 * If the command fails to send, it logs an error with the return value.
 *
 * @param  None.
 *
 * @return None.
 *
 */
void AlNor_Erase(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 4, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Erase error, ret:0x%x\r\n", ret);
    }
}

/**
 *
 * This function sends a chip erase command to the NOR flash device, removing all data.
 * It sets the transmission mode to SPI_TX_ONLY and sends the chip erase command.
 * If the command fails to send, it logs an error with the return value.
 *
 * @param  None.
 *
 * @return None.
 *
 */
void AlNor_EraseChip(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_CE;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_EraseChip error, ret:0x%x\r\n", ret);
    }
}

/**
 *
 * This function reads data from a specific page of the NOR flash device.
 * It sets the transmission mode to SPI_EEPROM, sends the read command with the page address, and stores the received data.
 * If the command fails, it logs an error with the return value.
 *
 * @param  None.
 *
 * @return None.
 *
 */
void AlNor_ReadPage(void)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadPage error, ret:0x%x\r\n", ret);
    }
}

/**
 *
 * Similar to AlNor_ReadPage but reads from a specific page with an additional offset.
 * It sets the transmission mode to SPI_EEPROM and sends the read command with the page address and offset.
 * If the command fails, it logs an error with the return value.
 *
 * @param  None.
 *
 * @return None.
 *
 */
void AlNor_ReadPage1(void)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;

    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0x10;
    SendData[3] = 0;

    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (AL_OK !=ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadPage error, ret:0x%x\r\n", ret);
    }
}

/**
 *
 * This function writes data to a specific page of the NOR flash device.
 * It sets the transmission mode to SPI_TX_ONLY, prepares the data to be written, and sends the write command with the data.
 * If the command fails to send, it logs an error with the return value.
 *
 * @param  None.
 *
 * @return None.
 *
 */
void AlNor_WritePage(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (AL_OK !=ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WritePage error ret:0x%x\r\n", ret);
    }
}

/**
 *
 * Similar to AlNor_WritePage but writes to a specific page with an additional offset.
 * It sets the transmission mode to SPI_TX_ONLY, prepares the data to be written, and sends the write command with the data and offset.
 * If the command fails to send, it logs an error with the return value.
 *
 * @param  None.
 *
 * @return None.
 *
 */
void AlNor_WritePage1(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0x10;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
       SendData[i + 4] = i % 255;
    }
    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (AL_OK !=ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WritePage error, ret:0x%x\r\n", ret);
    }
}

/**
 *
 * This function reads the manufacturer and device identification of the NOR flash device.
 * It sets the transmission mode to SPI_EEPROM and sends the read ID command.
 * The received ID is logged. If the command fails, it logs an error with the return value.
 *
 * @param  None.
 *
 * @return None.
 *
 */
AL_VOID AlNor_ReadId(AL_VOID)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDID;
    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, FlashId, 3, 100000);
    if (AL_OK !=ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadId error, ret:0x%x\r\n", ret);
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}


/**
 *
 * This function initializes the SPI driver, performs various SPI NOR flash operations,
 * and checks the results for errors.
 *
 * @param  None.
 *
 * @return None.
 *
 */
void main(void)
{
    AL_U32 i;
    AL_S32 ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_ERROR, "Start FPSoc Spi Test\r\n");

    ret = AlSpi_Hal_Init(&Handle, &SpiInitConfigs, AL_NULL, AL_SPI_DEVICE_ID);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi_Hal_Init error, ret:0x%x\r\n", ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlNor_ReadId();

    AlNor_Wren();
    AlNor_ReadStatus();
    AlNor_Erase();
    AlNor_WriteWip();

    AlNor_ReadPage();
    for (i = 0; i < 240; i++) {
        if(0xff != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi test erase norflash error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
    memset(RecvData, 0x0, sizeof(RecvData));
    AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi test erase norflash success\r\n");

    AlNor_Wren();
    AlNor_WritePage();
    AlNor_WriteWip();

    AlNor_Wren();
    AlNor_WritePage1();
    AlNor_WriteWip();

    AlNor_ReadPage();
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi data write norflash test error, ret:0x%x\r\n", ret);
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
    memset(RecvData, 0x0, sizeof(RecvData));

    AlNor_ReadPage1();
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi data write1 norflash test error, ret:0x%x\r\n", ret);
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi test write norflash success\r\n");
}

