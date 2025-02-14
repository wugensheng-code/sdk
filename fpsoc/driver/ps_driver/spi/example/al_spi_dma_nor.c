/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_spi_hal.h"
#include "al_spinor.h"
#include "al_dmacahb_hal.h"

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

/* The Buffer size must be aligned with the cache line when enable MMU */
#define FLAH_ID_SIZE                64
#define BUFFER_SIZE                 512

#ifdef ENABLE_MMU

AL_U8 CACHE_LINE_ALIGN FlashId[FLAH_ID_SIZE] = { 0x0 };

AL_U8 CACHE_LINE_ALIGN DmaSendData[BUFFER_SIZE] = { 0x0 };
AL_U8 CACHE_LINE_ALIGN DmaRecvData[BUFFER_SIZE] = { 0x0 };

#else

AL_U8 FlashId[FLAH_ID_SIZE] = { 0x0 };

AL_U8 DmaSendData[BUFFER_SIZE] = { 0x0 };
AL_U8 DmaRecvData[BUFFER_SIZE] = { 0x0 };

#endif

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 *
 * This function sends a reset command to the NOR flash memory using DMA.
 * It sets the transmission mode to SPI_TX_ONLY and sends the reset command.
 * If the operation fails, it logs an error message.
 *
 * @param  None.
 *
 * @return None.
 *
 */
AL_VOID AlNorDma_Reset(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_INFINEON_SRST;

#ifdef ENABLE_MMU
    AlCache_FlushDcacheRange((AL_UINTPTR)DmaSendData, (AL_UINTPTR)(DmaSendData + BUFFER_SIZE));
#endif

    Ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 1, 100000);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_Reset error, Ret:0x%x\r\n", Ret);
    }
}

/**
 *
 * This function sends a write enable command to the NOR flash memory using DMA.
 * It sets the transmission mode to SPI_TX_ONLY and sends the write enable command.
 * If the operation fails, it logs an error message.
 *
 * @param  None.
 *
 * @return None.
 *
 */
AL_VOID AlNorDma_Wren(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_WREN;

#ifdef ENABLE_MMU
    AlCache_FlushDcacheRange((AL_UINTPTR)DmaSendData, (AL_UINTPTR)(DmaSendData + BUFFER_SIZE));
#endif

    Ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 1, 100000);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_Wren error, Ret:0x%x\r\n", Ret);
    }
}

/**
 *
 * This function repeatedly sends a read status register command to the NOR flash memory
 * using DMA until the write-in-progress bit is cleared, indicating that the previous
 * write or erase operation is complete. If the operation fails, it logs an error message.
 *
 * @param  None.
 *
 * @return None.
 *
 */
AL_VOID AlNorDma_WaitWip(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_RDSR;

    do {
#ifdef ENABLE_MMU
        AlCache_FlushDcacheRange((AL_UINTPTR)DmaSendData, (AL_UINTPTR)(DmaSendData + BUFFER_SIZE));
        AlCache_InvalidateDcacheRange((AL_UINTPTR)DmaRecvData, (AL_UINTPTR)(DmaRecvData + BUFFER_SIZE));
#endif

        Ret = AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, DmaRecvData, 1, 100000);
        if (AL_OK != Ret) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_WaitWip error, Ret:0x%x\r\n", Ret);
        }
#ifdef SPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_ERROR, "WAITWIP Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
    } while (DmaRecvData[0] & SR_WIP);
}

/**
 *
 * This function sends a sector erase command to the NOR flash memory using DMA.
 * It sets the transmission mode to SPI_TX_ONLY and sends the command along with the address
 * of the sector to erase. If the operation fails, it logs an error message.
 *
 * @param  None.
 *
 * @return None.
 *
 */
AL_VOID AlNorDma_ReadStatus(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_RDSR;

#ifdef ENABLE_MMU
    AlCache_FlushDcacheRange((AL_UINTPTR)DmaSendData, (AL_UINTPTR)(DmaSendData + BUFFER_SIZE));
    AlCache_InvalidateDcacheRange((AL_UINTPTR)DmaRecvData, (AL_UINTPTR)(DmaRecvData + BUFFER_SIZE));
#endif

    Ret = AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, DmaRecvData, 1, 100000);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_ReadStatus error, Ret:0x%x\r\n", Ret);
    }
#ifdef SPI_DEBUG
    AL_LOG(AL_LOG_LEVEL_ERROR, "Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
}

/**
 *
 * This function sends a sector erase command to the NOR flash memory using DMA.
 * It sets the transmission mode to SPI_TX_ONLY and sends the command along with the address
 * of the sector to erase. If the operation fails, it logs an error message.
 *
 * @param  None.
 *
 * @return None.
 *
 */
AL_VOID AlNorDma_Erase(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_SE;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0;
    DmaSendData[3] = 0;

#ifdef ENABLE_MMU
    AlCache_FlushDcacheRange((AL_UINTPTR)DmaSendData, (AL_UINTPTR)(DmaSendData + BUFFER_SIZE));
#endif

    Ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 4, 100000);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_Erase error, Ret:0x%x\r\n", Ret);
    }
}

/**
 *
 * This function sends a page read command to the NOR flash memory using DMA.
 * It sets the transmission mode to SPI_EEPROM and sends the command along with the address
 * of the page to read. If the operation fails, it logs an error message.
 *
 * @param  None.
 *
 * @return None.
 *
 */
AL_VOID AlNorDma_ReadPage(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_READ;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0;
    DmaSendData[3] = 0;

#ifdef ENABLE_MMU
    AlCache_FlushDcacheRange((AL_UINTPTR)DmaSendData, (AL_UINTPTR)(DmaSendData + BUFFER_SIZE));
    AlCache_InvalidateDcacheRange((AL_UINTPTR)DmaRecvData, (AL_UINTPTR)(DmaRecvData + BUFFER_SIZE));
#endif

    Ret = AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 4, DmaRecvData, 240, 100000);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_ReadPage error, Ret:0x%x\r\n", Ret);
    }
}

/**
 *
 * This function sends a page program command to the NOR flash memory using DMA.
 * It sets the transmission mode to SPI_TX_ONLY and sends the command along with the address
 * of the page to write and the data to be written. If the operation fails, it logs an error message.
 *
 * @param  None.
 *
 * @return None.
 *
 */
AL_VOID AlNorDma_WritePage(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_PP;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0;
    DmaSendData[3] = 0;

    AL_U32 Index = 0;
    for (Index = 0; Index < 400; Index++) {
        DmaSendData[Index + 4] = Index % 255;
    }

#ifdef ENABLE_MMU
    AlCache_FlushDcacheRange((AL_UINTPTR)DmaSendData, (AL_UINTPTR)(DmaSendData + BUFFER_SIZE));
#endif

    Ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 240, 100000);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_WritePage error, Ret:0x%x\r\n", Ret);
    }
}

/**
 *
 * This function sends a read ID command to the NOR flash memory using DMA.
 * It sets the transmission mode to SPI_EEPROM and sends the command. The ID read from the
 * NOR flash memory is stored in a predefined array. If the operation fails, it logs an error message.
 *
 * @param  None.
 *
 * @return None.
 *
 */
AL_VOID AlNorDma_ReadId(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_RDID;;

#ifdef ENABLE_MMU
    AlCache_FlushDcacheRange((AL_UINTPTR)DmaSendData, (AL_UINTPTR)(DmaSendData + BUFFER_SIZE));
    AlCache_InvalidateDcacheRange((AL_UINTPTR)DmaRecvData, (AL_UINTPTR)(DmaRecvData + BUFFER_SIZE));
#endif

    Ret = AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, FlashId, 3, 100000);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_ReadId error, Ret:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA Read Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}

/**
 *
 * This function initializes the SPI and DMA, performs a series of NOR flash memory operations
 * including reset, read ID, write enable, erase, write, and read. It checks the results of erase
 * and write operations and logs the outcome. If any operation fails, it logs an error message.
 *
 * @param  None.
 *
 * @return None.
 *
 */
AL_VOID main(AL_VOID)
{
    AL_U32 Index;
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_ERROR, "Start FPSoc Spi AL_SPI_RUN_DMA Test\r\n");

    Ret = AlSpi_Hal_Init(&Handle, &SpiInitConfigs, AL_NULL, AL_SPI_DEVICE_ID);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi_Hal_Init error, Ret:0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    // AlNorDma_Reset();
    /**/
    AlNorDma_ReadId();

    /**/
    AlNorDma_Wren();
    AlNorDma_ReadStatus();
    AlNorDma_Erase();
    AlNorDma_WaitWip();

    /**/
    AlNorDma_ReadPage();
    for (Index = 0; Index < 240; Index++) {
        if(0xff != DmaRecvData[Index]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi test erase norflash error, Ret:0x%x\r\n", Ret);
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error DmaRecvData[%d]:%d\r\n", Index, DmaRecvData[Index]);
            while (1);
        }
    }
    memset(DmaRecvData, 0x0, sizeof(DmaRecvData));
    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlSpi test erase norflash success\r\n");

    /**/
    AlNorDma_Wren();
    AlNorDma_WritePage();
    AlNorDma_WaitWip();

    AlNorDma_ReadPage();
    for (Index = 0; Index < 230; Index++) {
        if(Index != DmaRecvData[Index]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlSpi data write norflash test error, Ret:0x%x\r\n", Ret);
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error DmaRecvData[%d]:%d\r\n", Index, DmaRecvData[Index]);
            while (1);
        }
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlSpi test write norflash success\r\n");

    return 0;
}

