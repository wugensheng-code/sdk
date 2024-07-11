/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_qspi_dev.h"
#include "al_nor.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define QSPI_IN_TX_FE_INTR(Status)  (Status & QSPI_TXEIS)
#define QSPI_IN_TX_FO_INTR(Status)  (Status & QSPI_TXOIS)
#define QSPI_IN_RX_FU_INTR(Status)  (Status & QSPI_RXUIS)
#define QSPI_IN_RX_FO_INTR(Status)  (Status & QSPI_RXOIS)
#define QSPI_IN_RX_FF_INTR(Status)  (Status & QSPI_RXFIS)

#define QSPI_IN_XIP_RX_FO_INTR(Status)  (Status & QSPI_XRXOIS)
#define QSPI_IN_TX_FU_INTR(Status)      (Status & QSPI_TXUIS)
#define QSPI_IN_AXI_ERR_INTR(Status)    (Status & QSPI_AXIES)
#define QSPI_IN_TFER_ERR_INTR(Status)   (Status & QSPI_SPITES)
#define QSPI_IN_DONE_INTR(Status)       (Status & QSPI_DONES)

#define QSPI_IN_STATUS_ERROR(Status)  0

/************************** Variable Definitions *****************************/
/* Default init config */

/**
 *_____________________________________________________________________________________________________________________________
 *          |                  x1 instruction/address         |                  x2/x4 instruction/address                    |
 *   DFS    |          24-bit address        | 32-bit address |        24-bit address       |           32-bit address        |
 *__________|________________________________|________________|_____________________________|_________________________________|
 *          |         {24’b0,i[7:0]}         |{24’b0,i[7:0]}  |      {24’b0,i[7:0]}         |           {24’b0,i[7:0]}        |
 *          |         {24’b0,a[23:16]}       |{24’b0,a[31:24]}|      {8’b0,a[23:0]}         |           {a[31:0]}             |
 *  8-bit   |         {24’b0,a[15:8]}        |{24’b0,a[23:16]}|                             |                                 |
 *          |         {24’b0,a[7:0]}         |{24’b0,a[15:8]} |                             |                                 |
 *          |                                |{24’b0,a[7:0]}  |                             |                                 |
 *__________|________________________________|________________|_____________________________|_________________________________|
 *          |    {16’b0,a[23:16],i[7:0]}     |                |     {16’b0,i[7:0],8’b0}     |       {16’b0,i[7:0],8’b0}       |
 *  16-bit  |    {16’b0,a[7:0],a[15:8]}      |   No Support   |{a[23:16],8’b0,a[7:0],[15:8]}|{a[23:16],a[31:24],a[7:0],[15:8]}|
 * _________|________________________________|________________|_____________________________|_________________________________|
 *          |                                |                |       {i[7:0],24’b0}        |          {i[7:0],24’b0}         |
 *  32-bit  |{a[7:0],a[15:8],a[23:16],i[7:0]}|   No Support   |{a[7:0],[15:8],a[23:16],8’b0}|{a[7:0],[15:8],a[23:16],a[31:24]}|
 * _________|________________________________|________________|_____________________________|_________________________________|
 *
*/
/* When SpiFrameFormat is Dual Quad,TransMode can only use QSPI_TX_ONLY,QSPI_RX_ONLY mode */
static AL_QSPI_ConfigsStruct QSPIDefInitConfigs =
{
    .Trans.TransMode          = QSPI_TX_ONLY,
    .SpiFrameFormat     = SPI_STANDARD_FORMAT,
    .Trans.DataFrameSize      = QSPI_DFS_8BITS,
    .Trans.EnSpiCfg.AddrLength    = QSPI_ADDR_L32,
    .Trans.EnSpiCfg.InstLength    = QSPI_INST_L8,
    .Trans.EnSpiCfg.TransType     = QSPI_TT0,
    .Trans.EnSpiCfg.WaitCycles    = 8,
    .ClockStretch  = QSPI_EnableClockStretch,
    .SamplDelay         = 0,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .Trans.SlvSelEnum         = QSPI_SER_SS0_EN
};

extern AL_QSPI_HwConfigStruct AlQSPI_HwCfg[AL_QSPI_NUM_INSTANCE];
/************************** Function Prototypes ******************************/

/********************************************************/
/**
 * This function iterates through an array of hardware configuration structures
 * to find the one that matches the given device ID.
 *
 * @param DeviceId The hardware module ID for which the configuration is sought.
 * @return A pointer to the matching AL_QSPI_HwConfigStruct if found, otherwise NULL.
 */
AL_QSPI_HwConfigStruct *AlQspi_Dev_LookupConfig(AL_U32 DeviceId)
{
    AL_U32 Index;
    AL_QSPI_HwConfigStruct *CfgPtr = AL_NULL;

    for (Index = 0; Index < AL_QSPI_NUM_INSTANCE; Index++) {
        if (AlQSPI_HwCfg[Index].DeviceId == DeviceId) {
            CfgPtr = &AlQSPI_HwCfg[Index];
            break;
        }
    }

    return CfgPtr;
}

/**
 * This function examines the state of the QSPI device to determine if a transmit operation is in progress.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @return AL_TRUE if the TX is busy, otherwise AL_FALSE.
 */
AL_BOOL AlQspi_Dev_IsTxBusy(AL_QSPI_DevStruct *Qspi)
{
    return (AL_BOOL)(Qspi->State & AL_QSPI_STATE_TX_BUSY);
}

/**
 * Checks if the QSPI device's receive (RX) is currently busy.
 * This function examines the state of the QSPI device to determine if a receive operation is in progress.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @return AL_TRUE if the RX is busy, otherwise AL_FALSE.
 */
AL_BOOL AlQspi_Dev_IsRxBusy(AL_QSPI_DevStruct *Qspi)
{
    return (AL_BOOL)(Qspi->State & AL_QSPI_STATE_RX_BUSY);
}

/**
 * Sets the QSPI device's transmit (TX) state to busy.
 * This function modifies the state of the QSPI device to indicate that a transmit operation is in progress.
 *
 * @param Qspi Pointer to the QSPI device structure.
 */
static AL_VOID AlQspi_Dev_SetTxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State |= AL_QSPI_STATE_TX_BUSY;
}

/**
 * Sets the QSPI device's receive (RX) state to busy.
 * This function modifies the state of the QSPI device to indicate that a receive operation is in progress.
 *
 * @param Qspi Pointer to the QSPI device structure.
 */
static AL_VOID AlQspi_Dev_SetRxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State |= AL_QSPI_STATE_RX_BUSY;
}

/**
 * Clears the QSPI device's transmit (TX) busy state.
 * This function modifies the state of the QSPI device to indicate that no transmit operation is in progress.
 *
 * @param Qspi Pointer to the QSPI device structure.
 */
static AL_VOID AlQspi_Dev_ClrTxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State &= (~AL_QSPI_STATE_TX_BUSY);
}

/**
 * Clears the QSPI device's receive (RX) busy state.
 * This function modifies the state of the QSPI device to indicate that no receive operation is in progress.
 *
 * @param Qspi Pointer to the QSPI device structure.
 */
static AL_VOID AlQspi_Dev_ClrRxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State &= (~AL_QSPI_STATE_RX_BUSY);
}

/**
 * Prints the values of all registers in the QSPI device.
 * This function reads and logs the current value of each register within the QSPI device.
 *
 * @param QSPIBaseAddr The base address of the QSPI device's registers.
 */
AL_VOID AlQspi_Dev_DumpReg(AL_REG QSPIBaseAddr)
{
    AL_LOG(AL_LOG_LEVEL_DEBUG, "CTRLR0:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_CTRLR0_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "CTRLR1:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_CTRLR1_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SSIENR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_SSIENR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "MWCR:0x%x\r\n",    AL_REG32_READ(QSPIBaseAddr + QSPI_MWCR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SER:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_SER_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "BAUDR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_BAUDR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXFTLR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_TXFTLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXFTLR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_RXFTLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXFLR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_TXFLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXFLR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_RXFLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SR:0x%x\r\n",      AL_REG32_READ(QSPIBaseAddr + QSPI_SR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "IMR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_IMR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "ISR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_ISR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RISR:0x%x\r\n",    AL_REG32_READ(QSPIBaseAddr + QSPI_RISR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXOICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_TXOICR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXOICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_RXOICR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXUICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_RXUICR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "MSTICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_MSTICR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "ICR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_ICR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMACR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_DMACR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMATDLR:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_DMATDLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMARDLR:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_DMARDLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "IDR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_IDR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SSIC VERSION ID:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_SSIC_VERSION_ID_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RX SAMPLE DELAY:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_RX_SAMPLE_DELAY_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SPI CTRLR0:0x%x\r\n",      AL_REG32_READ(QSPIBaseAddr + QSPI_SPI_CTRLR0_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "XIP MODE BITS:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_XIP_MODE_BITS_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "XIP INCR INST:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_XIP_INCR_INST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "XIP WRAP INST:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_XIP_WRAP_INST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "XIP CNT TIME OUT:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_XIP_CNT_TIME_OUT_OFFSET));

}

/**
 * Initializes the QSPI device with specified hardware and configuration parameters.
 * This function sets up the QSPI device according to the provided hardware configuration and optional initialization
 * parameters. If no initialization parameters are provided, default settings are used.
 *
 * @param Qspi Pointer to the QSPI device structure to be initialized.
 * @param HwConfig Pointer to the hardware configuration structure.
 * @param InitConfig Pointer to the initialization configuration structure. If NULL, default settings are used.
 * @return AL_OK if the initialization is successful, otherwise an error code indicating the type of failure.
 */
AL_S32 AlQspi_Dev_Init(AL_QSPI_DevStruct *Qspi, AL_QSPI_HwConfigStruct *HwConfig, AL_QSPI_ConfigsStruct *InitConfig)
{
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    Qspi->HwConfig = *HwConfig;

    Qspi->Configs = (InitConfig == AL_NULL) ? QSPIDefInitConfigs : (*InitConfig);

    AlQspi_ll_Disable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_SetClockStretch(Qspi->HwConfig.BaseAddress, Qspi->Configs.ClockStretch);
    AlQspi_ll_SetClkDiv(Qspi->HwConfig.BaseAddress, (Qspi->HwConfig.InputClockHz / Qspi->HwConfig.IoFreq));
    AlQspi_ll_SetSlvSelToggle(Qspi->HwConfig.BaseAddress, Qspi->Configs.SlvToggleEnum);
    AlQspi_ll_SetRecvDataSamplDelay(Qspi->HwConfig.BaseAddress, Qspi->Configs.SamplDelay);     /* 88M */
    AlQspi_ll_SetQspiFrameFormat(Qspi->HwConfig.BaseAddress, Qspi->Configs.SpiFrameFormat);

    AlQspi_ll_SetXipModeBitEn(Qspi->HwConfig.BaseAddress, QSPI_XipModeBitDisable);
    AlQspi_ll_SetXipDfsFix(Qspi->HwConfig.BaseAddress, QSPI_XipDfsChange);
    AlQspi_ll_SetXipMbl(Qspi->HwConfig.BaseAddress, QSPI_MBL_4);
    AlQspi_ll_SetXipInstPhase(Qspi->HwConfig.BaseAddress, QSPI_XipInstPhaseDisable);
    AlQspi_ll_SetXipPrefetch(Qspi->HwConfig.BaseAddress, QSPI_DisableXipPrefetch);
    AlQspi_ll_SetXipContTrans(Qspi->HwConfig.BaseAddress, QSPI_DisableXipContTrans);
    // AlQspi_ll_SetRecvDataSamplDelay(Qspi->HwConfig.BaseAddress, 2); /* 66M */
    // AlQspi_ll_SetRecvDataSamplDelay(Qspi->HwConfig.BaseAddress, 4);     /* 88M */

    // AlQspi_ll_SetRecvDataSamplEdge(Qspi->HwConfig.BaseAddress, QSPI_NegativeEdgeSampling);
    AlQspi_ll_MaskIntr(Qspi->HwConfig.BaseAddress, QSPI_TXEIM | QSPI_RXFIM);
    AlQspi_ll_Enable(Qspi->HwConfig.BaseAddress);

    Qspi->State |= AL_QSPI_STATE_READY;

    return AL_OK;
}

/**
 * This function configures the QSPI hardware for 32-bit addressing mode, sets up various
 * parameters such as wait cycles, clock stretch, transfer mode, address length, instruction
 * length, transfer type, frame format, XIP mode bit, XIP DFS (Data Frame Size) fix, continuous
 * transfer, prefetch, instruction phase, and incremental instruction. It also disables the
 * QSPI device at the start and enables it at the end after all configurations are done.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @return Returns AL_QSPI_ERR_ILLEGAL_PARAM if the Qspi pointer is NULL, otherwise returns AL_OK.
 */
AL_S32 AlQspi_Dev_XipAddr32Init(AL_QSPI_DevStruct *Qspi)
{
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AlQspi_ll_Disable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_SetWaitCycles(Qspi->HwConfig.BaseAddress, 0x8);
    AlQspi_ll_SetClockStretch(Qspi->HwConfig.BaseAddress, QSPI_DisableClockStretch);
    AlQspi_ll_SetTransfMode(Qspi->HwConfig.BaseAddress, QSPI_RX_ONLY);
    AlQspi_ll_SetAddrLength(Qspi->HwConfig.BaseAddress, QSPI_ADDR_L32);
    AlQspi_ll_SetInstLength(Qspi->HwConfig.BaseAddress, QSPI_INST_L8);
    AlQspi_ll_SetTransType(Qspi->HwConfig.BaseAddress, QSPI_TT0);
    AlQspi_ll_SetQspiFrameFormat(Qspi->HwConfig.BaseAddress, SPI_QUAD_FORMAT);
    AlQspi_ll_SetXipModeBitEn(Qspi->HwConfig.BaseAddress, QSPI_XipModeBitDisable);

    AlQspi_ll_SetXipDfsFix(Qspi->HwConfig.BaseAddress, QSPI_XipDfsChange);
    AlQspi_ll_SetXipContTrans(Qspi->HwConfig.BaseAddress, QSPI_EnableXipContTrans);
    AlQspi_ll_SetXipPrefetch(Qspi->HwConfig.BaseAddress, QSPI_EnableXipPrefetch);
    AlQspi_ll_SetXipInstPhase(Qspi->HwConfig.BaseAddress, QSPI_XipInstPhaseEnable);
    AlQspi_ll_SetXipIncrInst(Qspi->HwConfig.BaseAddress, NOR_OP_READ_1_1_4_4B);
    /* WRAP transfer types is only supported in continuous XIP mode */
    AlQspi_ll_SetXipWrapInst(Qspi->HwConfig.BaseAddress, NOR_OP_READ_1_1_4_4B);
    AlQspi_ll_SetXipCntTimeOut(Qspi->HwConfig.BaseAddress, 0XFF);
    AlQspi_ll_SetXipPort1NorFlashSize(Qspi->HwConfig.BaseAddress, QSPI_XipPort1NorFlash_128MB);

    AlQspi_ll_Enable(Qspi->HwConfig.BaseAddress);

    return AL_OK;
}

/**
 * This function sets up the QSPI device for 24-bit addressing suitable for DMA operations.
 * It configures the device with specific settings for wait cycles, clock stretch, transfer mode,
 * address length, instruction length, transfer type, frame format, XIP mode bit, XIP DFS fix,
 * continuous transfer, prefetch, instruction phase, and incremental instruction. The function
 * disables the QSPI device at the beginning and enables it after all configurations are applied.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @return Returns AL_QSPI_ERR_ILLEGAL_PARAM if the Qspi pointer is NULL, otherwise returns AL_OK.
 */
AL_S32 AlQspi_Dev_XipAddr24InitForDMA(AL_QSPI_DevStruct *Qspi)
{
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AlQspi_ll_Disable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_SetWaitCycles(Qspi->HwConfig.BaseAddress, 0x8);
    AlQspi_ll_SetClockStretch(Qspi->HwConfig.BaseAddress, QSPI_EnableClockStretch);
    AlQspi_ll_SetTransfMode(Qspi->HwConfig.BaseAddress, QSPI_RX_ONLY);
    AlQspi_ll_SetAddrLength(Qspi->HwConfig.BaseAddress, QSPI_ADDR_L24);
    AlQspi_ll_SetInstLength(Qspi->HwConfig.BaseAddress, QSPI_INST_L8);
    AlQspi_ll_SetTransType(Qspi->HwConfig.BaseAddress, QSPI_TT0);
    AlQspi_ll_SetQspiFrameFormat(Qspi->HwConfig.BaseAddress, SPI_QUAD_FORMAT);

    AlQspi_ll_SetXipModeBitEn(Qspi->HwConfig.BaseAddress, QSPI_XipModeBitDisable);

    AlQspi_ll_SetXipDfsFix(Qspi->HwConfig.BaseAddress, QSPI_XipDfsFix);
    AlQspi_ll_SetXipContTrans(Qspi->HwConfig.BaseAddress, QSPI_EnableXipContTrans);
    AlQspi_ll_SetXipPrefetch(Qspi->HwConfig.BaseAddress, QSPI_DisableXipPrefetch);
    AlQspi_ll_SetXipInstPhase(Qspi->HwConfig.BaseAddress, QSPI_XipInstPhaseEnable);
    AlQspi_ll_SetXipIncrInst(Qspi->HwConfig.BaseAddress, NOR_OP_READ_1_1_4);
    /* WRAP transfer types is only supported in continuous XIP mode */
    AlQspi_ll_SetXipWrapInst(Qspi->HwConfig.BaseAddress, NOR_OP_READ_1_1_4);
    AlQspi_ll_SetXipCntTimeOut(Qspi->HwConfig.BaseAddress, 0XFF);
    AlQspi_ll_SetXipPort1NorFlashSize(Qspi->HwConfig.BaseAddress, QSPI_XipPort1NorFlash_16MB);

    AlQspi_ll_Enable(Qspi->HwConfig.BaseAddress);

    return AL_OK;
}

/**
 * This function initializes qspi XIP mode
 * @param   Qspi is structure pointer to qspi device
 * @return
 *          - AL_OK for function success
 *          - Other for function failuregit
 * @note    Enable the qspi xip mode to be non-continuous transfer
 *          and set the instruction to nor flash 1_4_4(0xeb)
*/
AL_S32 AlQspi_Dev_XipAddr24Init(AL_QSPI_DevStruct *Qspi, AL_U8* FlashId)
{
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AlQspi_ll_Disable(Qspi->HwConfig.BaseAddress);

    if (FlashId[1] == 0x40 && FlashId[2] == 0x18) {
        AlQspi_ll_SetWaitCycles(Qspi->HwConfig.BaseAddress, 0x6);
    } else {
        AlQspi_ll_SetWaitCycles(Qspi->HwConfig.BaseAddress, 0xa);
    }

    AlQspi_ll_SetClockStretch(Qspi->HwConfig.BaseAddress, QSPI_EnableClockStretch);
    AlQspi_ll_SetTransfMode(Qspi->HwConfig.BaseAddress, QSPI_RX_ONLY);
    AlQspi_ll_SetAddrLength(Qspi->HwConfig.BaseAddress, QSPI_ADDR_L24);
    AlQspi_ll_SetInstLength(Qspi->HwConfig.BaseAddress, QSPI_INST_L8);
    AlQspi_ll_SetTransType(Qspi->HwConfig.BaseAddress, QSPI_TT1);
    AlQspi_ll_SetQspiFrameFormat(Qspi->HwConfig.BaseAddress, SPI_QUAD_FORMAT);
    AlQspi_ll_SetXipModeBitEn(Qspi->HwConfig.BaseAddress, QSPI_XipModeBitDisable);

    AlQspi_ll_SetXipDfsFix(Qspi->HwConfig.BaseAddress, QSPI_XipDfsChange);
    AlQspi_ll_SetXipContTrans(Qspi->HwConfig.BaseAddress, QSPI_DisableXipContTrans);
    AlQspi_ll_SetXipPrefetch(Qspi->HwConfig.BaseAddress, QSPI_DisableXipPrefetch);
    AlQspi_ll_SetXipInstPhase(Qspi->HwConfig.BaseAddress, QSPI_XipInstPhaseEnable);
    AlQspi_ll_SetXipIncrInst(Qspi->HwConfig.BaseAddress, NOR_OP_READ_1_4_4);
    /* WRAP transfer types is only supported in continuous XIP mode */
    AlQspi_ll_SetXipWrapInst(Qspi->HwConfig.BaseAddress, NOR_OP_READ_1_4_4);
    AlQspi_ll_SetXipCntTimeOut(Qspi->HwConfig.BaseAddress, 0XFF);
    AlQspi_ll_SetXipPort1NorFlashSize(Qspi->HwConfig.BaseAddress, QSPI_XipPort1NorFlash_16MB);

    AlQspi_ll_Enable(Qspi->HwConfig.BaseAddress);

    return AL_OK;
}

/**
 * This function is responsible for sending data through the QSPI device. It checks if the
 * QSPI device is ready and not busy before sending data. The function also configures the
 * data frame size based on the address length and instruction length. It sets up the QSPI
 * device with the appropriate configurations for data frame size, address length, instruction
 * length, transfer type, wait cycles, and frame format. The function calculates the send level
 * and adjusts the TX FIFO start level accordingly. It also handles the clock stretch setting
 * if enabled.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @param SendBuf Pointer to the buffer containing the data to be sent.
 * @param SendSize Size of the data to be sent, in bytes.
 * @return Returns AL_QSPI_ERR_ILLEGAL_PARAM if the Qspi pointer is NULL or SendSize is 0,
 *         AL_QSPI_ERR_NOT_READY if the QSPI device is not ready, AL_QSPI_ERR_NOT_SUPPORT if
 *         the TX FIFO is full, AL_QSPI_ERR_BUSY if the QSPI device is busy, otherwise returns AL_OK.
 */
AL_S32 AlQspi_Dev_SendData(AL_QSPI_DevStruct *Qspi, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_U32 SendLevel, SendValue = 0xFFFFFFFF, TempSendSize;

    if (Qspi == AL_NULL || SendSize == 0) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_TXFIFO_FULL == AlQspi_ll_IsTxFifoFull(Qspi->HwConfig.BaseAddress)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQspi_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQspi_Dev_SetTxBusy(Qspi);

    TempSendSize = SendSize - (Qspi->Configs.Trans.EnSpiCfg.AddrLength + Qspi->Configs.Trans.EnSpiCfg.InstLength) / 2;

    /* Different fifo widths are set depending on the amount of data */
    if ((QSPI_ADDR_L0 != Qspi->Configs.Trans.EnSpiCfg.AddrLength)
        && (Qspi->Configs.SpiFrameFormat != SPI_STANDARD_FORMAT)) {
        if ((0 == (TempSendSize % 4)) && (0 != TempSendSize)) {
            Qspi->Configs.Trans.DataFrameSize = QSPI_DFS_32BITS;
            Qspi->BitsPerWord = 32;
        } else if ((0 == (TempSendSize % 2)) && (0 != TempSendSize)) {
            Qspi->Configs.Trans.DataFrameSize = QSPI_DFS_16BITS;
            Qspi->BitsPerWord = 16;
        } else {
            Qspi->Configs.Trans.DataFrameSize = QSPI_DFS_8BITS;
            Qspi->BitsPerWord = 8;
        }
    } else {
        Qspi->Configs.Trans.DataFrameSize = QSPI_DFS_8BITS;
        Qspi->BitsPerWord = 8;
    }

    Qspi->SendBuffer.BufferPtr    = SendBuf;
    Qspi->SendBuffer.HandledCnt   = 0;
    Qspi->SendBuffer.RequestedCnt = SendSize;

    AlQspi_ll_Disable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_SetDataFrameSize(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.DataFrameSize);
    AlQspi_ll_SetAddrLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->HwConfig.BaseAddress, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetTransfMode(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.TransMode);

    SendLevel = MIN((AL_U32)(Qspi->HwConfig.FifoLen / 2), TempSendSize / (Qspi->BitsPerWord >> 3));
    if (SPI_STANDARD_FORMAT != Qspi->Configs.SpiFrameFormat) {
        SendLevel += 2;
    }
    AlQspi_ll_SetTxStartFifoLevel(Qspi->HwConfig.BaseAddress, SendLevel ? SendLevel - 1 : 0);

    if (Qspi->Configs.ClockStretch == QSPI_EnableClockStretch &&
       Qspi->Configs.SpiFrameFormat != SPI_STANDARD_FORMAT) {
        AlQspi_ll_SetRecvNumOfDataFrames(Qspi->HwConfig.BaseAddress, (TempSendSize / (Qspi->BitsPerWord >> 3))
                                         ? (TempSendSize / (Qspi->BitsPerWord >> 3)- 1) : 0);
    } else
        AlQspi_ll_SetRecvNumOfDataFrames(Qspi->HwConfig.BaseAddress, 0);

    AlQspi_ll_Enable(Qspi->HwConfig.BaseAddress);

    if (Qspi->Configs.SpiFrameFormat == SPI_STANDARD_FORMAT) {
        switch (Qspi->Configs.Trans.DataFrameSize) {
        case QSPI_DFS_32BITS:
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U32 *)(Qspi->SendBuffer.BufferPtr));
            } else {
                AL_LOG(AL_LOG_LEVEL_INFO, "Qspi not support this mode combination\r\n");
            }
            break;
        case QSPI_DFS_16BITS:
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U16 *)(Qspi->SendBuffer.BufferPtr));
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + 2));
            } else {
                AL_LOG(AL_LOG_LEVEL_INFO, "Qspi not support this mode combination\r\n");
            }
            break;
        case QSPI_DFS_8BITS:
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr));
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 1));
            }
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L8) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 2));
            }
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L16) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 3));
            }
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L24) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 4));
            }
            break;
        default:
            AL_LOG(AL_LOG_LEVEL_INFO, "DFS is set error in Qspi\r\n");
            break;
        }
    } else {
        switch (Qspi->Configs.Trans.DataFrameSize) {
        case QSPI_DFS_32BITS:
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[0] << 24);
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 8 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                    Qspi->SendBuffer.BufferPtr[3] << 24);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] | Qspi->SendBuffer.BufferPtr[2] << 8 |
                    Qspi->SendBuffer.BufferPtr[3] << 16 | Qspi->SendBuffer.BufferPtr[4] << 24);
                }
            }
            break;
        case QSPI_DFS_16BITS:
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[0] << 8);
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] |
                    Qspi->SendBuffer.BufferPtr[3] << 8);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 24 |
                    Qspi->SendBuffer.BufferPtr[3] | Qspi->SendBuffer.BufferPtr[4] << 8);
                }
            }
            break;
        case QSPI_DFS_8BITS:
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[0]);
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 8 |
                    Qspi->SendBuffer.BufferPtr[3]);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                    Qspi->SendBuffer.BufferPtr[3] << 8 | Qspi->SendBuffer.BufferPtr[4]);
                }
            }
            break;
        default:
            AL_LOG(AL_LOG_LEVEL_INFO, "DFS is set error in Qspi\r\n");
            break;
        }
    }

    Qspi->SendBuffer.HandledCnt = (Qspi->Configs.Trans.EnSpiCfg.AddrLength + Qspi->Configs.Trans.EnSpiCfg.InstLength) / 2;
    SendLevel = MIN(Qspi->HwConfig.FifoLen - AlQspi_ll_ReadTxFifoLevel(Qspi->HwConfig.BaseAddress), (Qspi->SendBuffer.RequestedCnt -
    Qspi->SendBuffer.HandledCnt) / (Qspi->BitsPerWord >> 3));

    while (SendLevel--) {
        if (Qspi->BitsPerWord == 8) {
            SendValue = *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
        } else if (Qspi->BitsPerWord == 16) {
            SendValue = *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
        } else {
            SendValue = *(AL_U32 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
        }

        AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, SendValue);
        Qspi->SendBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
    }

    SendLevel = MIN((AL_U32)(Qspi->HwConfig.FifoLen / 2), SendSize / (Qspi->BitsPerWord >> 3));
    AlQspi_ll_SetTxFifoThrLevel(Qspi->HwConfig.BaseAddress, SendLevel);

    AlQspi_ll_SetSlvSel(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.SlvSelEnum);
    AlQspi_ll_EnableIntr(Qspi->HwConfig.BaseAddress, QSPI_TXEIM);

    return AL_OK;
}

/**
 * This function is responsible for receiving data from a QSPI device. It checks for various conditions
 * such as null pointers, readiness of the QSPI device, and whether the receive FIFO is empty or not.
 * It also sets the data frame size based on the receive size and configures the QSPI hardware for receiving data.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @param ReceiveBuf Pointer to the buffer where received data will be stored.
 * @param ReceiveSize Size of the data to be received.
 * @return Returns AL_OK if successful, otherwise returns an error code indicating the type of error.
 */
AL_S32 AlQspi_Dev_RecvData(AL_QSPI_DevStruct *Qspi, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    AL_U32 Temp;

    if (Qspi == AL_NULL || ReceiveSize == 0 ) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_RXFIFO_NOTEMPTY== AlQspi_ll_IsRxFifoEmpty(Qspi->HwConfig.BaseAddress)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQspi_Dev_IsRxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQspi_Dev_SetRxBusy(Qspi);

    Qspi->RecvBuffer.BufferPtr    = ReceiveBuf;
    Qspi->RecvBuffer.RequestedCnt = ReceiveSize;
    Qspi->RecvBuffer.HandledCnt   = 0;

    if (0 == (ReceiveSize % 4)) {
        Qspi->Configs.Trans.DataFrameSize = QSPI_DFS_32BITS;
        Qspi->BitsPerWord = 32;
    } else if (0 == (ReceiveSize % 2)) {
        Qspi->Configs.Trans.DataFrameSize = QSPI_DFS_16BITS;
        Qspi->BitsPerWord = 16;
    } else {
        Qspi->Configs.Trans.DataFrameSize = QSPI_DFS_8BITS;
        Qspi->BitsPerWord = 8;
    }

    AlQspi_ll_Disable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_SetAddrLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->HwConfig.BaseAddress, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetTransfMode(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.TransMode);
    Temp = ReceiveSize / (Qspi->BitsPerWord >> 3);
    AlQspi_ll_SetRecvNumOfDataFrames(Qspi->HwConfig.BaseAddress, Temp ? Temp - 1 : 0);
    AlQspi_ll_SetSlvSel(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.SlvSelEnum);
    AlQspi_ll_SetDataFrameSize(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.DataFrameSize);
    AlQspi_ll_Enable(Qspi->HwConfig.BaseAddress);
    Temp = MIN(ReceiveSize / (Qspi->BitsPerWord >> 3), Qspi->HwConfig.FifoLen / 2);
    AlQspi_ll_SetRxFifoThrLevel(Qspi->HwConfig.BaseAddress, Temp ? Temp - 1 : 0);
    //  AlQspi_ll_SetRxFifoThrLevel(Qspi->HwConfig.BaseAddress, 0);
    AlQspi_ll_EnableIntr(Qspi->HwConfig.BaseAddress, QSPI_RXFIM);

    return AL_OK;
}

/**
 * This function handles both sending and receiving data through the QSPI interface. It performs checks
 * for null pointers, readiness of the device, and whether the device is busy. It sets the data frame size
 * based on the send size and configures the QSPI hardware for data transfer. The function also handles
 * the configuration of various parameters required for the data transfer such as address length, instruction
 * length, transfer type, wait cycles, QSPI frame format, and transfer mode.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @param SendBuf Pointer to the buffer containing the data to be sent.
 * @param SendSize Size of the data to be sent.
 * @param ReceiveBuf Pointer to the buffer where received data will be stored.
 * @param ReceiveSize Size of the data to be received.
 * @return Returns AL_OK if successful, otherwise returns an error code indicating the type of error.
 */
AL_S32 AlQspi_Dev_TranferData(AL_QSPI_DevStruct *Qspi, AL_U8 *SendBuf, AL_U32 SendSize, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    AL_U32 SendLevel, SendValue = 0xffffffff, Temp, TempSendSize;

    if (Qspi == AL_NULL || SendSize == 0 || ReceiveSize == 0) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (AlQspi_Dev_IsRxBusy(Qspi) ||AlQspi_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQspi_Dev_SetRxBusy(Qspi);
    AlQspi_Dev_SetTxBusy(Qspi);

    TempSendSize = SendSize - (Qspi->Configs.Trans.EnSpiCfg.AddrLength + Qspi->Configs.Trans.EnSpiCfg.InstLength) / 2;

    if (QSPI_ADDR_L0 != Qspi->Configs.Trans.EnSpiCfg.AddrLength) {
        if ((0 == (TempSendSize % 4)) && (0 != TempSendSize)) {
            Qspi->Configs.Trans.DataFrameSize = QSPI_DFS_32BITS;
            Qspi->BitsPerWord = 32;
        } else if ((0 == (TempSendSize % 2)) && (0 != TempSendSize)) {
            Qspi->Configs.Trans.DataFrameSize = QSPI_DFS_16BITS;
            Qspi->BitsPerWord = 16;
        } else {
            Qspi->Configs.Trans.DataFrameSize = QSPI_DFS_8BITS;
            Qspi->BitsPerWord = 8;
        }
    } else {
        Qspi->Configs.Trans.DataFrameSize = QSPI_DFS_8BITS;
        Qspi->BitsPerWord = 8;
    }

    Qspi->SendBuffer.BufferPtr    = SendBuf;
    Qspi->SendBuffer.RequestedCnt = SendSize;
    Qspi->SendBuffer.HandledCnt   = 0;
    Qspi->RecvBuffer.BufferPtr    = ReceiveBuf;
    Qspi->RecvBuffer.RequestedCnt = ReceiveSize;
    Qspi->RecvBuffer.HandledCnt   = 0;

    AlQspi_ll_Disable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_SetAddrLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->HwConfig.BaseAddress, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetTransfMode(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.TransMode);
    Temp = ReceiveSize / (Qspi->BitsPerWord >> 3);
    AlQspi_ll_SetRecvNumOfDataFrames(Qspi->HwConfig.BaseAddress, Temp ? Temp - 1 : 0);
    AlQspi_ll_SetDataFrameSize(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.DataFrameSize);
    SendLevel = MIN((AL_U32)(Qspi->HwConfig.FifoLen / 2), TempSendSize / (Qspi->BitsPerWord >> 3));
    if (SPI_STANDARD_FORMAT != Qspi->Configs.SpiFrameFormat) {
        SendLevel += 2;
    }
    AlQspi_ll_SetTxStartFifoLevel(Qspi->HwConfig.BaseAddress, SendLevel ? SendLevel - 1 : 0);

    AlQspi_ll_Enable(Qspi->HwConfig.BaseAddress);

    if (Qspi->Configs.SpiFrameFormat == SPI_STANDARD_FORMAT) {
        switch (Qspi->Configs.Trans.DataFrameSize) {
        case QSPI_DFS_32BITS:
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U32 *)(Qspi->SendBuffer.BufferPtr));
            } else {
                AL_LOG(AL_LOG_LEVEL_INFO, "Qspi not support this mode combination\r\n");
            }
            break;
        case QSPI_DFS_16BITS:
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U16 *)(Qspi->SendBuffer.BufferPtr));
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + 2));
            } else {
                AL_LOG(AL_LOG_LEVEL_INFO, "Qspi not support this mode combination\r\n");
            }
            break;
        case QSPI_DFS_8BITS:
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr));
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 1));
            }
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L8) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 2));
            }
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L16) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 3));
            }
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L24) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 4));
            }
            break;
        default:
            AL_LOG(AL_LOG_LEVEL_INFO, "DFS is set error in Qspi\r\n");
            break;
        }
    } else {
        switch (Qspi->Configs.Trans.DataFrameSize) {
        case QSPI_DFS_32BITS:
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[0] << 24);
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 8 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                    Qspi->SendBuffer.BufferPtr[3] << 24);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] | Qspi->SendBuffer.BufferPtr[2] << 8 |
                    Qspi->SendBuffer.BufferPtr[3] << 16 | Qspi->SendBuffer.BufferPtr[4] << 24);
                }
            }
            break;
        case QSPI_DFS_16BITS:
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[0] << 8);
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] |
                    Qspi->SendBuffer.BufferPtr[3] << 8);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 24 |
                    Qspi->SendBuffer.BufferPtr[3] | Qspi->SendBuffer.BufferPtr[4] << 8);
                }
            }
            break;
        case QSPI_DFS_8BITS:
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[0]);
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 8 |
                    Qspi->SendBuffer.BufferPtr[3]);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                    Qspi->SendBuffer.BufferPtr[3] << 8 | Qspi->SendBuffer.BufferPtr[4]);
                }
            }
            break;
        default:
            AL_LOG(AL_LOG_LEVEL_INFO, "DFS is set error in Qspi\r\n");
            break;
        }
    }

    Qspi->SendBuffer.HandledCnt = (Qspi->Configs.Trans.EnSpiCfg.AddrLength + Qspi->Configs.Trans.EnSpiCfg.InstLength) / 2;

    SendLevel = MIN3(Qspi->HwConfig.FifoLen - AlQspi_ll_ReadTxFifoLevel(Qspi->HwConfig.BaseAddress), (Qspi->SendBuffer.RequestedCnt -
    Qspi->SendBuffer.HandledCnt) / (Qspi->BitsPerWord >> 3) , Qspi->HwConfig.FifoLen -
    (Qspi->RecvBuffer.RequestedCnt - Qspi->SendBuffer.RequestedCnt)/ (Qspi->BitsPerWord >> 3));

    while (SendLevel--) {

        /* qspi send data can only be to adjust the byte order by software */
        if (Qspi->BitsPerWord == 8) {
            SendValue = *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
        } else if (Qspi->BitsPerWord == 16) {
            SendValue = *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            // SendValue = QSPI_ENDIAN_SWAP16(SendValue);
        } else {
            SendValue = *(AL_U32 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            // SendValue = QSPI_ENDIAN_SWAP32(SendValue);
        }

        AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, SendValue);
        Qspi->SendBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
    }

    SendLevel = MIN((AL_U32)(Qspi->HwConfig.FifoLen / 2), SendSize / (Qspi->BitsPerWord >> 3));
    Temp =  MIN(ReceiveSize / (Qspi->BitsPerWord >> 3), Qspi->HwConfig.FifoLen / 2);
    AlQspi_ll_SetRxFifoThrLevel(Qspi->HwConfig.BaseAddress, Temp ? Temp - 1 : 0);
    AlQspi_ll_SetTxFifoThrLevel(Qspi->HwConfig.BaseAddress, SendLevel);

    /* Set cs to start transfer */
    AlQspi_ll_SetSlvSel(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.SlvSelEnum);
    AlQspi_ll_EnableIntr(Qspi->HwConfig.BaseAddress, QSPI_RXFIM | QSPI_TXEIM);

    return AL_OK;
}

/**
 * This function configures the QSPI device for transmission, checks for various error conditions before sending,
 * and then initiates the DMA transfer. It sets up the QSPI device with the specified configurations such as
 * data frame size, address length, instruction length, transfer type, wait cycles, SPI frame format, and transfer mode.
 * It calculates the send level based on the FIFO length and the send data size, adjusting for non-standard SPI frame formats.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @param SendDataSize Size of the data to be sent in bytes.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlQspi_Dev_DmaSendData(AL_QSPI_DevStruct *Qspi, AL_U32 SendDataSize)
{
    AL_U32 SendLevel;

    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_TXFIFO_FULL == AlQspi_ll_IsTxFifoFull(Qspi->HwConfig.BaseAddress)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQspi_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQspi_ll_Disable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_SetDataFrameSize(Qspi->HwConfig.BaseAddress, QSPI_DFS_8BITS);
    AlQspi_ll_SetAddrLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->HwConfig.BaseAddress, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetTransfMode(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.TransMode);
    SendLevel = MIN((AL_U32)(Qspi->HwConfig.FifoLen / 2), SendDataSize);
    if (SPI_STANDARD_FORMAT != Qspi->Configs.SpiFrameFormat) {
        SendLevel += 2;
    }
    AlQspi_ll_SetTxStartFifoLevel(Qspi->HwConfig.BaseAddress, SendLevel ? SendLevel - 1 : 0);
    // AlQspi_ll_SetTxStartFifoLevel(Qspi->HwConfig.BaseAddress, 0);
    if (Qspi->Configs.ClockStretch == QSPI_EnableClockStretch &&
       Qspi->Configs.SpiFrameFormat != SPI_STANDARD_FORMAT)
    {
        AlQspi_ll_SetRecvNumOfDataFrames(Qspi->HwConfig.BaseAddress, SendDataSize ? SendDataSize - 1 : 0);
    }
    AlQspi_ll_SetTxFifoThrLevel(Qspi->HwConfig.BaseAddress, Qspi->HwConfig.FifoLen / 2);
    AlQspi_ll_TxDmaEnable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_SetDmaTransLevel(Qspi->HwConfig.BaseAddress, Qspi->HwConfig.FifoLen / 2 + 2);
    AlQspi_ll_Enable(Qspi->HwConfig.BaseAddress);

    if (Qspi->Configs.SpiFrameFormat == SPI_STANDARD_FORMAT) {
        AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr));
        if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 1));
        }
        if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L8) {
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 2));
        }
        if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L16) {
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 3));
        }
        if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L24) {
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 4));
        }
    } else {
        AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[0]);
        if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 8 |
                Qspi->SendBuffer.BufferPtr[3]);
            } else {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                Qspi->SendBuffer.BufferPtr[3] << 8 | Qspi->SendBuffer.BufferPtr[4]);
            }
        }
    }

    return AL_OK;
}

/**
 * This function prepares the QSPI device for data reception, checks for various error conditions before receiving,
 * and then enables DMA reception. It configures the QSPI device with the necessary settings such as data frame size,
 * RX FIFO threshold level, address length, instruction length, transfer type, wait cycles, and SPI frame format.
 * It calculates the number of data frames to receive based on the receive size and the bits per word, adjusting for the actual data size.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @param RecvSize Size of the data to be received in bytes.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlQspi_Dev_DmaRecvData(AL_QSPI_DevStruct *Qspi, AL_U16 RecvSize)
{
    AL_U32 TempRecvSize;
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_RXFIFO_NOTEMPTY== AlQspi_ll_IsRxFifoEmpty(Qspi->HwConfig.BaseAddress)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQspi_Dev_IsRxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQspi_ll_Disable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_SetDataFrameSize(Qspi->HwConfig.BaseAddress, QSPI_DFS_8BITS);
    AlQspi_ll_SetRxFifoThrLevel(Qspi->HwConfig.BaseAddress, Qspi->HwConfig.FifoLen / 2);
    AlQspi_ll_RxDmaEnable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_SetDmaRecevLevel(Qspi->HwConfig.BaseAddress, 0);
    AlQspi_ll_SetAddrLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->HwConfig.BaseAddress, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetTransfMode(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.TransMode);
    TempRecvSize = RecvSize / (Qspi->BitsPerWord >> 3);
    AlQspi_ll_SetRecvNumOfDataFrames(Qspi->HwConfig.BaseAddress, TempRecvSize ? TempRecvSize - 1 : 0);
    AlQspi_ll_Enable(Qspi->HwConfig.BaseAddress);

    return AL_OK;
}

/**
 * This function checks for readiness and busyness of the QSPI device, calculates the actual send size by adjusting for address and instruction lengths,
 * and then configures both the TX and RX DMA for the transfer. It sets up the QSPI device with the necessary configurations for both sending and receiving data.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @param SendSize Size of the data to be sent in bytes.
 * @param RecvSize Size of the data to be received in bytes.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlQspi_Dev_DmaTranferData(AL_QSPI_DevStruct *Qspi, AL_U32 SendSize, AL_U16 RecvSize)
{
    AL_U32 SendLevel, TempSendSize;
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (AlQspi_Dev_IsRxBusy(Qspi) || AlQspi_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    TempSendSize = SendSize - (Qspi->Configs.Trans.EnSpiCfg.AddrLength + Qspi->Configs.Trans.EnSpiCfg.InstLength) / 2;

    AlQspi_ll_Disable(Qspi->HwConfig.BaseAddress);

    AlQspi_ll_SetRxFifoThrLevel(Qspi->HwConfig.BaseAddress, Qspi->HwConfig.FifoLen / 2);
    AlQspi_ll_SetTxFifoThrLevel(Qspi->HwConfig.BaseAddress, Qspi->HwConfig.FifoLen / 2);
    AlQspi_ll_TxDmaEnable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_RxDmaEnable(Qspi->HwConfig.BaseAddress);
    AlQspi_ll_SetDmaRecevLevel(Qspi->HwConfig.BaseAddress, 0);

    AlQspi_ll_SetAddrLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->HwConfig.BaseAddress, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetDataFrameSize(Qspi->HwConfig.BaseAddress, QSPI_DFS_8BITS);
    AlQspi_ll_SetTransfMode(Qspi->HwConfig.BaseAddress, Qspi->Configs.Trans.TransMode);
    AlQspi_ll_SetRecvNumOfDataFrames(Qspi->HwConfig.BaseAddress, RecvSize ? RecvSize - 1 : 0);

    SendLevel = MIN((AL_U32)(Qspi->HwConfig.FifoLen / 2), TempSendSize);
    if (SPI_STANDARD_FORMAT != Qspi->Configs.SpiFrameFormat) {
        SendLevel += 2;
    }
    AlQspi_ll_SetDmaTransLevel(Qspi->HwConfig.BaseAddress, Qspi->HwConfig.FifoLen / 2 + 2);
    AlQspi_ll_SetTxStartFifoLevel(Qspi->HwConfig.BaseAddress, SendLevel ? SendLevel - 1 : 0);
    AlQspi_ll_Enable(Qspi->HwConfig.BaseAddress);

    if (Qspi->Configs.SpiFrameFormat == SPI_STANDARD_FORMAT) {
        AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr));
        if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 1));
        }
        if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L8) {
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 2));
        }
        if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L16) {
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 3));
        }
        if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L24) {
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 4));
        }
    } else {
        AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[0]);
        if (Qspi->Configs.Trans.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
            if (Qspi->Configs.Trans.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 8 |
                Qspi->SendBuffer.BufferPtr[3]);
            } else {
                AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                Qspi->SendBuffer.BufferPtr[3] << 8 | Qspi->SendBuffer.BufferPtr[4]);
            }
        }
    }

    return AL_OK;
}

/**
 * This function controls the QSPI device by executing various control commands.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @param Cmd The command to execute.
 * @param Data Pointer to the data required for the command.
 * @return Returns AL_OK on success, AL_QSPI_ERR_ILLEGAL_PARAM if the Qspi pointer is NULL,
 *         or AL_QSPI_ERR_NOT_SUPPORT if the command is not supported.
 */
AL_S32 AlQspi_Dev_IoCtl(AL_QSPI_DevStruct *Qspi, AL_Qspi_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    switch (Cmd)
    {
    case AL_QSPI_IOCTL_SET_SLAVE_SELECT: {
        AL_QSPI_SlvSelEnum SlvSel = *(AL_QSPI_SlvSelEnum *)Data;
        Qspi->Configs.Trans.SlvSelEnum = SlvSel;
        break;
    }

    case AL_QSPI_IOCTL_GET_SLAVE_SELECT: {
        AL_QSPI_SlvSelEnum *SlvSel = (AL_QSPI_SlvSelEnum *)Data;
        *SlvSel = Qspi->Configs.Trans.SlvSelEnum;
        break;
    }

    case AL_QSPI_IOCTL_SET_IO_FREQ: {
        AL_U16 *IoFreq = (AL_U16 *)Data;
        Qspi->HwConfig.IoFreq = *IoFreq;
        AlQspi_ll_SetClkDiv(Qspi->HwConfig.BaseAddress, (Qspi->HwConfig.InputClockHz / Qspi->HwConfig.IoFreq));
    }

    case AL_QSPI_IOCTL_GET_IO_FREQ: {
        AL_U16 *IoFreq = (AL_U16 *)Data;
        *IoFreq = Qspi->HwConfig.IoFreq;
    }

    case AL_QSPI_IOCTL_SET_SLV_TOGGLE: {
        AL_QSPI_SlvSelToggleEnum *SlvToggle = (AL_QSPI_SlvSelToggleEnum *)Data;
        Qspi->Configs.SlvToggleEnum = *SlvToggle;
        AlQspi_ll_SetSlvSelToggle(Qspi->HwConfig.BaseAddress, Qspi->Configs.SlvToggleEnum);
    }

    case AL_QSPI_IOCTL_GET_SLV_TOGGLE: {
        AL_QSPI_SlvSelToggleEnum *SlvToggle = (AL_QSPI_SlvSelToggleEnum *)Data;
        *SlvToggle = Qspi->Configs.SlvToggleEnum;
    }

    case AL_QSPI_IOCTL_SET_SAMPLE_DELAY: {
        AL_U16 *SamplDelay = (AL_U16 *)Data;
        Qspi->Configs.SamplDelay = *SamplDelay;
        AlQspi_ll_SetRecvDataSamplDelay(Qspi->HwConfig.BaseAddress, Qspi->Configs.SamplDelay);
    }

    case AL_QSPI_IOCTL_GET_SAMPLE_DELAY: {
        AL_U16 *SamplDelay = (AL_U16 *)Data;
        *SamplDelay = Qspi->Configs.SamplDelay;
    }

    default:
        return AL_QSPI_ERR_NOT_SUPPORT;
        break;
    }

    return Ret;
}

/**
 * This function registers a callback function for QSPI interrupt events.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @param Callback The callback function to be registered.
 * @param CallbackRef User-defined reference that will be passed back in the callback.
 * @return Returns AL_OK on success, AL_QSPI_ERR_ILLEGAL_PARAM if either Qspi or Callback is NULL,
 *         or AL_QSPI_ERR_NOT_SUPPORT if a callback is already registered.
 */
AL_S32 AlQspi_Dev_RegisterIntrCallBack(AL_QSPI_DevStruct *Qspi, QSPI_EventCallBack Callback, AL_VOID *CallbackRef)
{
    if (Qspi == AL_NULL || Callback == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if (Qspi->EventCallBack != AL_NULL) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    Qspi->EventCallBack        = Callback;
    Qspi->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

/**
 * This function unregisters the interrupt callback function for the QSPI device.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @return Returns AL_OK on success or AL_QSPI_ERR_ILLEGAL_PARAM if the Qspi pointer is NULL.
 */
AL_S32 AlQspi_Dev_UnRegisterIntrCallBack(AL_QSPI_DevStruct *Qspi)
{
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    Qspi->EventCallBack = (QSPI_EventCallBack)AL_NULL;

    return AL_OK;
}

/**
 * This function handles received data for the QSPI device. It reads data from the RX FIFO, updates the receive buffer,
 * and manages the RX FIFO threshold level. It also triggers the receive done event when all data is received.
 *
 * @param Qspi Pointer to the QSPI device structure.
 */
static AL_VOID AlQspi_Dev_RecvDataHandler(AL_QSPI_DevStruct *Qspi)
{
    AL_U32 Length, RxFifoLevel, Status, Temp;

    Length = Qspi->RecvBuffer.RequestedCnt - Qspi->RecvBuffer.HandledCnt;

    if (Length) {
        RxFifoLevel = AlQspi_ll_ReadRxFifoLevel(Qspi->HwConfig.BaseAddress);
        if (!RxFifoLevel) {
            Status = AlQspi_ll_ReadRawIntrStatus(Qspi->HwConfig.BaseAddress);
            if (Status & QSPI_RXOIS) {
                /* FIFO overflow on Rx */
                AL_LOG(AL_LOG_LEVEL_INFO, "Error FIFO overflow on Rx\r\n");
                Qspi->State |= AL_QSPI_STATE_ERROR;
                AL_QSPI_EventStruct QspiEvent = {
                    .Events       = AL_QSPI_RX_FIFO_FULL,
                    .EventData    = Qspi->SendBuffer.HandledCnt
                };
                (*Qspi->EventCallBack)(QspiEvent, Qspi->EventCallBackRef);
            }
        } else if ((Qspi->HwConfig.FifoLen == RxFifoLevel) && (Qspi->Configs.Trans.TransMode == QSPI_EEPROM)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Error FIFO Full on Rx, cs high\r\n");
            AL_QSPI_EventStruct QspiEvent = {
                .Events       = AL_QSPI_RX_FIFO_FULL,
                .EventData    = Qspi->SendBuffer.HandledCnt
            };
            (*Qspi->EventCallBack)(QspiEvent, Qspi->EventCallBackRef);
            Qspi->State |= AL_QSPI_STATE_ERROR;
        }

        RxFifoLevel = MIN(RxFifoLevel, Length / (Qspi->BitsPerWord >> 3));
        for (; RxFifoLevel; --RxFifoLevel) {
            if (Qspi->BitsPerWord == 8) {
                *(AL_U8 *)(Qspi->RecvBuffer.BufferPtr + Qspi->RecvBuffer.HandledCnt) = (AL_U8)AlQspi_ll_DataReceive(Qspi->HwConfig.BaseAddress);
            } else if (Qspi->BitsPerWord == 16) {
                *(AL_U16 *)(Qspi->RecvBuffer.BufferPtr + Qspi->RecvBuffer.HandledCnt) = (AL_U16)AlQspi_ll_DataReceive(Qspi->HwConfig.BaseAddress);
            } else if (Qspi->BitsPerWord == 32) {
                *(AL_U32 *)(Qspi->RecvBuffer.BufferPtr + Qspi->RecvBuffer.HandledCnt) = AlQspi_ll_DataReceive(Qspi->HwConfig.BaseAddress);
            }

            Qspi->RecvBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
            Length = Qspi->RecvBuffer.RequestedCnt - Qspi->RecvBuffer.HandledCnt;
        }
    }

    Temp =  MIN(Length / (Qspi->BitsPerWord >> 3), (AL_U32)(Qspi->HwConfig.FifoLen / 2));
    AlQspi_ll_SetRxFifoThrLevel(Qspi->HwConfig.BaseAddress, Temp ? Temp - 1 : 0);

    if (Qspi->RecvBuffer.HandledCnt == Qspi->RecvBuffer.RequestedCnt) {
        AlQspi_ll_MaskIntr(Qspi->HwConfig.BaseAddress, QSPI_RXFIM);

        if (Qspi->EventCallBack) {
            AL_QSPI_EventStruct QspiEvent = {
                .Events        = AL_QSPI_RECEIVE_DONE,
                .EventData    = Qspi->SendBuffer.HandledCnt
            };
            AlQspi_Dev_ClrRxBusy(Qspi);
            (*Qspi->EventCallBack)(QspiEvent, Qspi->EventCallBackRef);
        }
    }
}

/**
 * This function handles sending data for the QSPI device. It writes data to the TX FIFO from the send buffer,
 * manages the TX FIFO threshold level, and triggers the send done event when all data is sent.
 *
 * @param Qspi Pointer to the QSPI device structure.
 */
static AL_VOID AlQspi_Dev_SendDataHandler(AL_QSPI_DevStruct *Qspi)
{
    AL_U32 TxFifoLevel, Length, Room, SendValue = 0xffffffff;

    Length = Qspi->SendBuffer.RequestedCnt - Qspi->SendBuffer.HandledCnt;

    if (Length) {
        TxFifoLevel = AlQspi_ll_ReadTxFifoLevel(Qspi->HwConfig.BaseAddress);

        if (!TxFifoLevel) {
            /* return error fifo entries */
            AL_LOG(AL_LOG_LEVEL_INFO, "Error Tx fifo enteries, cs high\r\n");
            Qspi->State |= AL_QSPI_STATE_ERROR;
            AL_QSPI_EventStruct QspiEvent = {
                .Events       = AL_QSPI_TX_FIFO_EMPTY,
                .EventData    = Qspi->SendBuffer.HandledCnt
            };
            (*Qspi->EventCallBack)(QspiEvent, Qspi->EventCallBackRef);
        }

        Room = MIN((Qspi->HwConfig.FifoLen - TxFifoLevel), Length / (Qspi->BitsPerWord >> 3));
        AlQspi_ll_SetTxStartFifoLevel(Qspi->HwConfig.BaseAddress, Room ? Room -1 : 0);
        for (; Room; --Room) {
            if (Qspi->BitsPerWord == 8) {
                SendValue = *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            } else if (Qspi->BitsPerWord == 16) {
                SendValue = *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            } else {
                SendValue = *(AL_U32 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            }
            AlQspi_ll_DataTransmit(Qspi->HwConfig.BaseAddress, SendValue);
            Qspi->SendBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
            Length = Qspi->SendBuffer.RequestedCnt - Qspi->SendBuffer.HandledCnt;
        }
    }

    if (Qspi->SendBuffer.HandledCnt == Qspi->SendBuffer.RequestedCnt) {
        AlQspi_ll_MaskIntr(Qspi->HwConfig.BaseAddress, QSPI_TXEIM);
        if (Qspi->EventCallBack) {
            AL_QSPI_EventStruct QspiEvent = {
                .Events         = AL_QSPI_SEND_DONE,
                .EventData    = Qspi->SendBuffer.HandledCnt
            };
            AlQspi_Dev_ClrTxBusy(Qspi);
            (*Qspi->EventCallBack)(QspiEvent, Qspi->EventCallBackRef);
        }
    }
}

/**
 * This function triggers an event callback with a specified event ID. This function is used internally
 * to notify about different QSPI events.
 *
 * @param Qspi Pointer to the QSPI device structure.
 * @param EventId The ID of the event to trigger.
 */
static AL_VOID AlQspi_Dev_EventHandler(AL_QSPI_DevStruct *Qspi, AL_QSPI_EventIdEnum EventId)
{
    if (Qspi->EventCallBack) {
        AL_QSPI_EventStruct QspiEvent = {
            .Events         = EventId,
            .EventData      = 0,
        };
        (*Qspi->EventCallBack)(QspiEvent, Qspi->EventCallBackRef);
    }
}

/**
 * This function handles interrupts for the QSPI device. This function checks the interrupt status
 * and calls the appropriate handler based on the type of interrupt that occurred.
 * It handles full and empty conditions for both transmit and receive FIFOs, as well
 * as overflow and underflow conditions. It also handles specific conditions for
 * Execute in Place (XIP) mode.
 *
 * @param instance Pointer to the QSPI device instance. This should be cast to
 *                 (AL_QSPI_DevStruct *) inside the function.
 */
AL_VOID AlQspi_Dev_IntrHandler(AL_VOID *instance)
{
    AL_QSPI_DevStruct *Qspi = (AL_QSPI_DevStruct *) instance;
    AL_QSPI_IntrStatusEnum IntrStatus = AlQspi_ll_GetIntrStatus(Qspi->HwConfig.BaseAddress);

    if (QSPI_IN_RX_FF_INTR(IntrStatus)) {
        AlQspi_Dev_RecvDataHandler(Qspi);
    }

    if (QSPI_IN_TX_FE_INTR(IntrStatus)) {
        AlQspi_Dev_SendDataHandler(Qspi);
    }

    if (QSPI_IN_TX_FO_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error qspi tx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "Tx Fifo Level:0x%x\r\n", AlQspi_ll_ReadTxFifoLevel(Qspi->HwConfig.BaseAddress));
        AlQspi_Dev_EventHandler(Qspi, AL_QSPI_TX_FO);
        Qspi->State |= AL_QSPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlQspi_ll_ClearAllIntr(Qspi->HwConfig.BaseAddress);
    }

    if (QSPI_IN_RX_FO_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error qspi rx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "Rx Fifo Level:0x%x\r\n", AlQspi_ll_ReadRxFifoLevel(Qspi->HwConfig.BaseAddress));
        AlQspi_Dev_EventHandler(Qspi, AL_QSPI_RX_FO);
        Qspi->State |= AL_QSPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlQspi_ll_ClearAllIntr(Qspi->HwConfig.BaseAddress);
    }

    if (QSPI_IN_RX_FU_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error qspi rx fifo underflow intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "Rx Fifo Level:0x%x\r\n", AlQspi_ll_ReadRxFifoLevel(Qspi->HwConfig.BaseAddress));
        AlQspi_Dev_EventHandler(Qspi, AL_QSPI_RX_FU);
        Qspi->State |= AL_QSPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlQspi_ll_ClearAllIntr(Qspi->HwConfig.BaseAddress);
    }

    if (QSPI_IN_XIP_RX_FO_INTR(IntrStatus)){
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error qspi XIP rx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "Rx Fifo Level:0x%x\r\n", AlQspi_ll_ReadRxFifoLevel(Qspi->HwConfig.BaseAddress));
        AlQspi_Dev_EventHandler(Qspi, AL_QSPI_RX_FO);
        Qspi->State |= AL_QSPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlQspi_ll_ClearAllIntr(Qspi->HwConfig.BaseAddress);
    }
}


