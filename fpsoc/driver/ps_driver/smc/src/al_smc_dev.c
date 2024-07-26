/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include "al_smc_dev.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_U32 __attribute__((aligned(4))) NandOob64[12] = {52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};
static AL_U32 __attribute__((aligned(4))) NandOob32[6] = {26, 27, 28, 29, 30, 31};
static AL_U32 __attribute__((aligned(4))) NandOob16[3] = {13, 14, 15};        /* data size 512bytes */

extern AL_SMC_HwConfigStruct AlSmc_HwCfg[AL_SMC_NUM_INSTANCE];
/************************** Function Prototypes ******************************/
static AL_U8 ALSmc_Dev_SetFeature(AL_NAND_InfoStruct *NandInfo, AL_U8 Address, AL_U8 *Data);
static AL_U8 ALSmc_Dev_GetFeature(AL_NAND_InfoStruct *NandInfo, AL_U8 Address, AL_U8 *Data);

/********************************************************/
/**
 *
 * This function searches through the predefined list of SMC hardware configurations (`AlSmc_HwCfg`)
 * to find the configuration matching the specified Device ID. It returns a pointer to the matching
 * configuration structure if found, or NULL if no matching configuration is found.
 *
 * @param DeviceId The unique identifier for the SMC device whose configuration is being requested.
 *
 * @return AL_SMC_HwConfigStruct* A pointer to the matching configuration structure, or NULL if no match is found.
 *
 */
AL_SMC_HwConfigStruct *AlSmc_Dev_LookupConfig(AL_U32 DeviceId)
{
    AL_U32 Index;
    AL_SMC_HwConfigStruct *CfgPtr = AL_NULL;

    for (Index = 0; Index < AL_SMC_NUM_INSTANCE; Index++) {
        if (AlSmc_HwCfg[Index].DeviceId == DeviceId) {
            CfgPtr = &AlSmc_HwCfg[Index];
            break;
        }
    }

    return CfgPtr;
}

/**
 *
 * This function is responsible for sending a specific command to the NAND device. The command to be sent
 * is determined by the parameters within the `NandInfo` structure. This function is a critical part of the
 * NAND operation sequence, facilitating operations such as reading, writing, erasing, etc.
 *
 * @param NandInfo Pointer to an `AL_NAND_InfoStruct` structure that contains information about the NAND device
 *                 and the command to be sent.
 * @return None.
 *       It is used by other higher-level functions that perform read, write, and erase operations on the NAND device.
 *
 */
static AL_VOID ALSmc_Dev_SendCmd(AL_NAND_InfoStruct *NandInfo)
{
    AL_UINTPTR CmdPhaseAddr;
    AL_U32 CmdPhaseData, EndCmdReq;

    if (ONFI_ENDIN_CMD_PHASE == NandInfo->Cmd.EndCmdPhase) {
        EndCmdReq = 1;
    } else {
        EndCmdReq = 0;
    }

    /* command phase address */
    CmdPhaseAddr  = NAND_BASE_ADDR                                  |
            (NandInfo->Cmd.AddrCycles << NAND_ADDR_CYCLES_SHIFT)    |
            (EndCmdReq                << NAND_END_CMD_VALID_SHIFT)  |
            NAND_COMMAND_PHASE_FLAG                                 |
            (NandInfo->Cmd.EndCmd     << NAND_END_CMD_SHIFT)        |
            (NandInfo->Cmd.StartCmd   << NAND_START_CMD_SHIFT);

    /* command phase data */
    if ((NandInfo->Cmd.Column != NAND_COLUMN_NOT_VALID) && (NandInfo->Cmd.Page != NAND_PAGE_NOT_VALID)) {
        if (NAND_16BITS == NandInfo->FlashWidth) {
            NandInfo->Cmd.Column >>= 1;
        }

        /* After Reset Cmd And Read Id Cmd */
        CmdPhaseData = NandInfo->Cmd.Column;
        CmdPhaseData |= NandInfo->Cmd.Page << (2*8);

        if (NandInfo->Cmd.AddrCycles > 4) {
            /* Send lower byte of page address */
            AL_REG32_WRITE(CmdPhaseAddr, CmdPhaseData);
            /* build upper byte */
            CmdPhaseData = NandInfo->Cmd.Page >> (32 - (2*8));
        }
    }
    else if (NandInfo->Cmd.Page != NAND_PAGE_NOT_VALID) {
        CmdPhaseData = NandInfo->Cmd.Page;
    } else {
        CmdPhaseData = NandInfo->Cmd.Column;
    }

    /* Send command phase */
    AL_REG32_WRITE(CmdPhaseAddr, CmdPhaseData);
}


/**
 *
 * This function writes data to the NAND flash device using the provided buffer.
 *
 * @param NandInfo Pointer to the AL_NAND_InfoStruct containing NAND flash device information.
 * @param Buf Pointer to the buffer containing the data to be written.
 * @param Length Length of the data to be written.
 *
 * @return None.
 *
 */
static AL_VOID ALSmc_Dev_WriteData(AL_NAND_InfoStruct *NandInfo, AL_U8 *Buf, AL_U32 Length)
{
    AL_U32 Index, EndCmdReq;
    AL_UINTPTR DataPhaseAddr;

    if (ONFI_ENDIN_DATA_PHASE == NandInfo->Cmd.EndCmdPhase) {
        EndCmdReq = 1;
    } else {
        EndCmdReq = 0;
    }

    /* data phase address */
    DataPhaseAddr  = NAND_BASE_ADDR                     |
            (0             << NAND_ADDR_CYCLES_SHIFT)   |
            (EndCmdReq     << NAND_END_CMD_VALID_SHIFT) |
            NAND_DATA_PHASE_FLAG                        |
            (NandInfo->Cmd.EndCmd << NAND_END_CMD_SHIFT)|
            (0            << NAND_ECC_LAST_SHIFT);

    /* Write Data */
    if (NAND_16BITS == NandInfo->FlashWidth) {
        for (Index = 0; Index < Length; Index++)
            AL_REG16_WRITE(DataPhaseAddr, Buf[Index]);
    } else {
        for (Index = 0; Index < Length; Index++)
            AL_REG8_WRITE(DataPhaseAddr, Buf[Index]);
    }
}


/**
 *
 * This function reads data from the NAND flash device and stores it in the provided buffer.
 *
 * @param NandInfo Pointer to the AL_NAND_InfoStruct containing information about the NAND flash device.
 * @param Buf Pointer to the buffer where the read data will be stored.
 * @param Length The length of the data to be read.
 *
 * @return None.
 *
 */
static AL_VOID ALSmc_Dev_ReadData(AL_NAND_InfoStruct *NandInfo, AL_U8 *Buf, AL_U32 Length)
{
    AL_UINTPTR DataPhaseAddr;
    AL_U32 Index, EndCmdReq;

    if (ONFI_ENDIN_DATA_PHASE == NandInfo->Cmd.EndCmdPhase) {
        EndCmdReq = 1;
    } else {
        EndCmdReq = 0;
    }

    /* data phase address */
    DataPhaseAddr  = NAND_BASE_ADDR                     |
            (0             << NAND_ADDR_CYCLES_SHIFT)   |
            (EndCmdReq     << NAND_END_CMD_VALID_SHIFT) |
            NAND_DATA_PHASE_FLAG                        |
            (NandInfo->Cmd.EndCmd << NAND_END_CMD_SHIFT)|
            (0             << NAND_ECC_LAST_SHIFT);

    /* Read Data */
    if (NAND_16BITS == NandInfo->FlashWidth) {
        for (Index = 0; Index < Length; Index++)
            Buf[Index] = (AL_U8)AL_REG16_READ(DataPhaseAddr);
    } else {
        for (Index = 0; Index < Length; Index++)
            Buf[Index] = AL_REG8_READ(DataPhaseAddr);
    }
}


/**
 *
 * This function writes a buffer of data to the NAND device. It takes a pointer to the NAND information structure,
 * a pointer to the buffer containing the data to be written, and the length of the buffer in bytes.
 *
 * @param NandInfo Pointer to the AL_NAND_InfoStruct structure containing information about the NAND device.
 * @param Buf Pointer to the buffer containing the data to be written.
 * @param Length Length of the buffer in bytes.
 *
 * @return None.
 *
 */
static AL_VOID ALSmc_Dev_WriteBuf(AL_NAND_InfoStruct *NandInfo, AL_U8 *Buf, AL_U32 Length)
{
    AL_U32 *tempBuff = (AL_U32 *)Buf;
    AL_U32 tempLength = Length >> 2;
    AL_U32 Index, EndCmdReq;
    AL_UINTPTR DataPhaseAddr;

    if (ONFI_ENDIN_DATA_PHASE == NandInfo->Cmd.EndCmdPhase) {
        EndCmdReq = 1;
    } else {
        EndCmdReq = 0;
    }

    /* data phase address */
    DataPhaseAddr  = NAND_BASE_ADDR                             |
            (NandInfo->Cmd.ClearCs << NAND_ADDR_CYCLES_SHIFT)   |
            (EndCmdReq             << NAND_END_CMD_VALID_SHIFT) |
            NAND_DATA_PHASE_FLAG                                |
            (NandInfo->Cmd.EndCmd  << NAND_END_CMD_SHIFT)       |
            (NandInfo->Cmd.EccLast << NAND_ECC_LAST_SHIFT);
    /* Write Data */
    for (Index = 0; Index < tempLength; Index++) {
        AL_REG32_WRITE(DataPhaseAddr, tempBuff[Index]);
    }
}


/**
 *
 * This function reads data from the NAND device into a buffer. It supports both ONFI and non-ONFI NAND devices.
 *
 * @param NandInfo Pointer to the AL_NAND_InfoStruct containing information about the NAND device.
 * @param Buf Pointer to the buffer where the read data will be stored.
 * @param Length The length of the data to be read, in bytes.
 *
 * @return None.
 *
 */
static AL_VOID ALSmc_Dev_ReadBuf(AL_NAND_InfoStruct *NandInfo, AL_U8 *Buf, AL_U32 Length)
{
    AL_U32 Index, EndCmdReq;
    AL_UINTPTR DataPhaseAddr = 0;
    AL_U32 *tempBuf = (AL_U32 *)Buf;
    AL_U32 tempLength = Length >> 2;

    if (ONFI_ENDIN_DATA_PHASE == NandInfo->Cmd.EndCmdPhase) {
        EndCmdReq = 1;
    } else {
        EndCmdReq = 0;
    }

    /* data phase address */
    DataPhaseAddr  = NAND_BASE_ADDR                             |
            (NandInfo->Cmd.ClearCs << NAND_ADDR_CYCLES_SHIFT)   |
            (EndCmdReq             << NAND_END_CMD_VALID_SHIFT) |
            NAND_DATA_PHASE_FLAG                                |
            (NandInfo->Cmd.EndCmd  << NAND_END_CMD_SHIFT)       |
            (NandInfo->Cmd.EccLast << NAND_ECC_LAST_SHIFT);

    /* Read Data */
    for (Index = 0; Index < tempLength; Index++) {
        tempBuf[Index] = AL_REG32_READ(DataPhaseAddr);
        EndCmdReq++;
    }
}


/**
 *
 * This function resets the NAND device by sending the appropriate commands and
 * checking the status until the device is ready.
 *
 * @param NandInfo Pointer to the AL_NAND_InfoStruct containing the NAND device information.
 *
 * @return AL_OK if the reset was successful.
 *
 */
AL_U32 ALSmc_Dev_Reset(AL_NAND_InfoStruct *NandInfo)
{
    AL_U8 Status;

    NandInfo->Cmd.StartCmd = ONFI_CMD_RESET1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_RESET2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_RESET_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_RESET_END_TIMING;

    NandInfo->Cmd.Page = ONFI_PAGE_NOT_VALID;
    NandInfo->Cmd.Column = ONFI_COLUMN_NOT_VALID;

    ALSmc_Dev_SendCmd(NandInfo);

    /* Check Nand Status */
    do {
        Status = ALSmc_Dev_ReadStatus(NandInfo);
    } while (!(Status & ONFI_STATUS_RDY));

    return AL_OK;
}


/**
 *
 * This function sets the necessary command parameters to read the ID of the NAND device,
 * and then sends the command and reads the data.
 *
 * @param NandInfo Pointer to the AL_NAND_InfoStruct structure containing the NAND device information.
 *
 * @return None.
 *
 */
AL_VOID ALSmc_Dev_ReadId(AL_NAND_InfoStruct *NandInfo)
{
    NandInfo->Cmd.StartCmd = ONFI_CMD_READ_ID1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_READ_ID2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_READ_ID_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_READ_ID_END_TIMING;

    NandInfo->Cmd.Page = ONFI_PAGE_NOT_VALID;
    NandInfo->Cmd.Column = 0;

    ALSmc_Dev_SendCmd(NandInfo);
    ALSmc_Dev_ReadData(NandInfo, NandInfo->Size.DeviceId, 5);
}


/**
 * This function calculates the CRC-16 checksum for the given buffer.
 *
 * @param Buf The buffer containing the data to calculate the checksum for.
 *
 * @return The calculated CRC-16 checksum.
 *
 */
static AL_U32 ALSmc_Dev_CrcCheck(AL_U8 *Buf)
{
    const AL_U32 Polynom = CRC16_POLYNOM;
    AL_U32 Crc = CRC16_INIT;
    AL_U32 Index, j, Bit, Data;
    AL_U32 CrcMask = 0xFFFF;
    AL_U32 CrcHighBit = 0x8000;

    /*
    * ONFI 1.0 Spec 5.1.4.36
    * The Integrity CRC (Cyclic Redundancy Check) field is used to verify
    * that the contents of the parameters page were
    * transferred correctly to the host.
    * CRC-16/IBM: G(X) = X16 + X15 + X2 + 1
    */

    for (Index = 0; Index < CRC16_LEN; Index++) {
        Data = Buf[Index];
        for (j = 0x80; j; j >>= 1) {
            Bit = Crc & CrcHighBit;
            Crc <<= 1;
            if (Data & j) {
                Bit ^= CrcHighBit;
            }
            if (Bit) {
                Crc ^= Polynom;
            }
        }
        Crc &= CrcMask;
    }

    return Crc;
}


/**
 *
 * This function reads the NAND device parameters by sending the appropriate commands
 * and retrieves the necessary information such as data bytes per page, spare bytes per page,
 * pages per block, blocks per unit, total units, and ECC number. It also performs a CRC check
 * on the read data to ensure data integrity.
 *
 * @param NandInfo Pointer to the AL_NAND_InfoStruct to be updated with the device parameters.
 *
 * @return AL_OK if successful, otherwise an error code indicating the failure reason.
 *
 */
AL_U32 ALSmc_Dev_ReadParam(AL_NAND_InfoStruct *NandInfo)
{
    AL_U8 Temp[ONFI_PARAM_LEN], Status, Index;
    AL_U32 Crc;
    AL_REG CmdPhaseAddr;

    AL_U32 TimingSupport = 0x0U;

    NandInfo->Cmd.StartCmd = ONFI_CMD_READ_PARAMETER1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_READ_PARAMETER2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_READ_PARAMETER_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_READ_PARAMETER_END_TIMING;

    NandInfo->Cmd.Page = ONFI_PAGE_NOT_VALID;
    NandInfo->Cmd.Column = 0x00;

    ALSmc_Dev_SendCmd(NandInfo);
    AlSys_MDelay(200);

    for (Index = 0; Index < 3; Index++) {
        ALSmc_Dev_ReadData(NandInfo, Temp, ONFI_PARAM_LEN);
        Crc = ALSmc_Dev_CrcCheck(Temp);
        if (((Crc & 0xff) == Temp[CRC16_LEN]) && (((Crc >> 8) & 0xff) == Temp[CRC16_LEN+1]))
            break;
    }

    if(Index == 3)
        return AL_SMC_EVENTS_TO_ERRS(SmcCrcErr);

    NandInfo->Size.DataBytesPerPage  =     *((AL_U32 *)(&Temp[DATA_PER_PAGE_POS]));
    NandInfo->Size.SpareBytesPerPage =     *((AL_U16 *)(&Temp[SPARE_PER_PAGE_POS]));
    NandInfo->Size.PagesPerBlock      =     *((AL_U32 *)(&Temp[PAGE_PER_BLOCK_POS]));
    NandInfo->Size.BlocksPerUnit      =    *((AL_U32 *)(&Temp[BLOCKS_PER_UINT_POS]));
    NandInfo->Size.TotalUnit          =    Temp[TOTAL_UINT_POS];
    NandInfo->Size.EccNum              =    Temp[ECC_NUM_POS];

    NandInfo->IsSupportFeatureOps = Temp[FEATURE_SUPPORT_POS];
    TimingSupport = (AL_U32)Temp[TIMING_SUPPORT_POS] | ((AL_U32)Temp[TIMING_SUPPORT_POS + 1] << 8);

    for(Index = 5; Index >= 0; Index--) {
        if (TimingSupport & (0x01 << Index)) {
            NandInfo->MaxTimingMode = Index;
            break;
        } else {
            continue;
        }
    }

    return AL_OK;
}

/**
 *
 * Set fixed smc timing for 50M clock.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct structure representing the SMC device.
 *
 * @return AL_OK if successful.
 *
 */
AL_VOID ALSmc_Dev_SetFixedTiming(AL_SMC_DevStruct *Smc)
{
    AL_SMC_ConfigsStruct TimingConfig = { 0 };

    TimingConfig.Cycles.b.Trc = 3;
    TimingConfig.Cycles.b.Twc = 3;
    TimingConfig.Cycles.b.Trea = 1;
    TimingConfig.Cycles.b.Twp = 2;
    TimingConfig.Cycles.b.Tclr = 1;
    TimingConfig.Cycles.b.Tar = 1;
    TimingConfig.Cycles.b.Trr = 1;

    AlSmc_ll_SetCycles(Smc->SmcBaseAddr, TimingConfig.Cycles);
    AlSmc_ll_SetCmdTypeAndChipNum(Smc->SmcBaseAddr, UPDATE_REGS, INTF1_CHIP1);

    return AL_OK;
}

/**
 *
 * This function reads the status of the NAND device using the provided
 * AL_NAND_InfoStruct. It sets the necessary command parameters for reading
 * the status and then sends the command to the device. Finally, it reads
 * the status data from the device and returns it.
 *
 * @param NandInfo Pointer to the AL_NAND_InfoStruct containing the NAND device information.
 *
 * @return The status of the NAND device.
 *
 */
AL_U32 ALSmc_Dev_ReadStatus(AL_NAND_InfoStruct *NandInfo)
{
    AL_U8 Status;

    NandInfo->Cmd.StartCmd = ONFI_CMD_READ_STATUS1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_READ_STATUS2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_READ_STATUS_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_READ_STATUS_END_TIMING;

    NandInfo->Cmd.Page = ONFI_PAGE_NOT_VALID;
    NandInfo->Cmd.Column = ONFI_COLUMN_NOT_VALID;

    ALSmc_Dev_SendCmd(NandInfo);
    ALSmc_Dev_ReadData(NandInfo, &Status, 1);

    return Status;
}


/**
 *
 * This function sets a feature of the NAND device specified by the given address.
 *
 * @param NandInfo Pointer to the AL_NAND_InfoStruct containing information about the NAND device.
 * @param Address The address of the feature to be set.
 * @param Data Pointer to the data to be written for the feature.
 *
 * @return AL_U8 Returns AL_OK if the feature was set successfully, otherwise returns an error code.
 *
 */
AL_U8 ALSmc_Dev_SetFeature(AL_NAND_InfoStruct *NandInfo, AL_U8 Address, AL_U8 *Data)
{
    AL_U8 Status;

    NandInfo->Cmd.StartCmd = ONFI_CMD_SET_FEATURES1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_SET_FEATURES2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_SET_FEATURES_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_SET_FEATURES_END_TIMING;

    NandInfo->Cmd.Page = ONFI_PAGE_NOT_VALID;
    NandInfo->Cmd.Column = Address;

    ALSmc_Dev_SendCmd(NandInfo);
    ALSmc_Dev_WriteData(NandInfo, Data, 4);

    /* Check Nand Status */
    do {
        Status = ALSmc_Dev_ReadStatus(NandInfo);
    } while (!(Status & ONFI_STATUS_RDY));

    return AL_OK;
}


/**
 *
 * This function retrieves a feature from the NAND device at the specified address.
 *
 * @param NandInfo Pointer to the AL_NAND_InfoStruct containing information about the NAND device.
 * @param Address The address of the feature to retrieve.
 * @param Data Pointer to the buffer where the retrieved feature data will be stored.
 *
 * @return AL_U8 The status of the operation. Returns AL_OK if successful.
 *
 */
AL_U8 ALSmc_Dev_GetFeature(AL_NAND_InfoStruct *NandInfo, AL_U8 Address, AL_U8 *Data)
{
    AL_U8 Status;
    AL_REG CmdPhaseAddr;

    NandInfo->Cmd.StartCmd = ONFI_CMD_GET_FEATURES1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_GET_FEATURES2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_GET_FEATURES_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_GET_FEATURES_END_TIMING;

    NandInfo->Cmd.Page = ONFI_PAGE_NOT_VALID;
    NandInfo->Cmd.Column = Address;

    ALSmc_Dev_SendCmd(NandInfo);
    /* Check Nand Status */
    do {
        Status = ALSmc_Dev_ReadStatus(NandInfo);
    } while (!(Status & ONFI_STATUS_RDY));

    CmdPhaseAddr  = NAND_BASE_ADDR          |
            (0 << NAND_ADDR_CYCLES_SHIFT)   |
            (0 << NAND_END_CMD_VALID_SHIFT) |
            NAND_COMMAND_PHASE_FLAG         |
            (0 << NAND_END_CMD_SHIFT)       |
            (0 << NAND_ECC_LAST_SHIFT);

    AL_REG32_WRITE(CmdPhaseAddr, 0);

    ALSmc_Dev_ReadData(NandInfo, Data, 4);

    return AL_OK;
}


/**
 *
 * This function erases a block in the NAND flash memory using the specified page number.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct structure.
 * @param NandInfo Pointer to the AL_NAND_InfoStruct structure.
 * @param Page The page number of the block to be erased.
 *
 * @return The status of the erase operation. Returns AL_OK if successful, or an error code
 * if the erase operation fails.
 *
 */
AL_U32 ALSmc_Dev_EraseBlock(AL_SMC_DevStruct *Smc, AL_NAND_InfoStruct *NandInfo, AL_U8 Page)
{
    AL_U32 Status;

    Status = ALSmc_Dev_ReadStatus(NandInfo);
    if (!(Status & ONFI_STATUS_WP)) {
        return AL_SMC_EVENTS_TO_ERRS(SmcNandWriteProtectErr);
    }

    NandInfo->Cmd.StartCmd = ONFI_CMD_ERASE_BLOCK1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_ERASE_BLOCK2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_ERASE_BLOCK_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_ERASE_BLOCK_END_TIMING;

    NandInfo->Cmd.Page = Page;
    NandInfo->Cmd.Column = ONFI_COLUMN_NOT_VALID;

    ALSmc_Dev_SendCmd(NandInfo);

    /* Wait smc ready */
    while(SMC_BUSY == AlSmc_ll_IsBusy(Smc->SmcBaseAddr));

    /* Check Nand Status */
    Status = ALSmc_Dev_ReadStatus(NandInfo);
    if (Status & ONFI_STATUS_FAIL) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Smc Nand EraseBlock ONFI_STATUS_FAIL error\r\n");
        return AL_SMC_EVENTS_TO_ERRS(SmcNandEraseBlockFailErr);
    }

    return AL_OK;
}


/**
 *
 * This function writes a page to the NAND flash device using hardware ECC calculation.
 * It sets the necessary command parameters for programming a page, sends the command,
 * writes the data buffer to the NAND flash, calculates the ECC, and writes the ECC
 * to the spare buffer. Finally, it checks the NAND status and returns the result.
 *
 * @param Smc The AL_SMC_DevStruct pointer representing the SMC device.
 * @param NandInfo The AL_NAND_InfoStruct pointer representing the NAND flash device information.
 * @param Page The page number to write.
 * @param Buf The data buffer to write to the NAND flash.
 *
 * @return The result of the operation. Returns AL_OK if successful, or an error code if there was an error.
 *
 */
AL_U32 ALSmc_Dev_HwEccWritePage(AL_SMC_DevStruct *Smc, AL_NAND_InfoStruct *NandInfo, AL_U32 Page, AL_U8 *Buf)
{
    AL_U8 EccDataNums, Index;
    AL_U32 *DataOffsetPtr, Status;

    NandInfo->Cmd.StartCmd = ONFI_CMD_PROGRAM_PAGE1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_PROGRAM_PAGE2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_PROGRAM_PAGE_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_PROGRAM_PAGE_END_TIMING;

    NandInfo->Cmd.Page = Page;
    NandInfo->Cmd.Column = 0;

    ALSmc_Dev_SendCmd(NandInfo);

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = 0;
    ALSmc_Dev_WriteBuf(NandInfo, Buf, NandInfo->Size.DataBytesPerPage - ONFI_AXI_DATA_WIDTH);
    Buf += (NandInfo->Size.DataBytesPerPage - ONFI_AXI_DATA_WIDTH);

    NandInfo->Cmd.EccLast = ECC_LAST;
    NandInfo->Cmd.ClearCs = 0;
    ALSmc_Dev_WriteBuf(NandInfo, Buf, ONFI_AXI_DATA_WIDTH);

    switch (NandInfo->Size.SpareBytesPerPage) {
    case 64:
        EccDataNums = 12;
        DataOffsetPtr = NandOob64;
        break;
    case 32:
        EccDataNums = 6;
        DataOffsetPtr = NandOob32;
        break;
    case 16:
        EccDataNums = 3;
        DataOffsetPtr = NandOob16;
        break;
    default:
        /* Page size 256 bytes & 4096 bytes not supported by ECC block*/
        return AL_SMC_EVENTS_TO_ERRS(SmcHwReadSizeErr);
        break;
    }

    Status = AlSmc_Dev_HwCalculateEcc(Smc, NandInfo->EccCalc, EccDataNums);
    if (Status != AL_OK) {
        return Status;
    }

    /* Full spare arrry */
    for (Index = 0; Index < EccDataNums; Index++) {
        NandInfo->SpareBuf[DataOffsetPtr[Index]] = ~NandInfo->EccCalc[Index];
    }

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = 0;
    ALSmc_Dev_WriteBuf(NandInfo, NandInfo->SpareBuf, NandInfo->Size.SpareBytesPerPage - ONFI_AXI_DATA_WIDTH);

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = CLEAR_CS;
    ALSmc_Dev_WriteBuf(NandInfo, NandInfo->SpareBuf + NandInfo->Size.SpareBytesPerPage - ONFI_AXI_DATA_WIDTH, ONFI_AXI_DATA_WIDTH);

    /* Wait smc ready */
    while(SMC_BUSY == AlSmc_ll_IsBusy(Smc->SmcBaseAddr));
    /* Clear intrrupt */
    AlSmc_ll_ClrIntr1(Smc->SmcBaseAddr);

    /* Check Nand Status */
    Status = ALSmc_Dev_ReadStatus(NandInfo);
    if (Status & ONFI_STATUS_FAIL) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Smc Nand HwEccWritePage ONFI_STATUS_FAIL error\r\n");
        return AL_SMC_EVENTS_TO_ERRS(SmcOnfiStatusFailErr);
    }

    return AL_OK;
}


/**
 *
 * This function reads a page from the NAND flash device and performs hardware ECC calculation.
 * It sets up the necessary command and address cycles for reading the page, sends the command to the device,
 * waits for the SMC to be ready, clears any interrupts, and then reads the data and spare bytes from the page.
 * It calculates the ECC code for the spare bytes and corrects any errors in the data using the ECC code.
 *
 * @param Smc The AL_SMC_DevStruct pointer representing the SMC device.
 * @param NandInfo The AL_NAND_InfoStruct pointer representing the NAND flash device information.
 * @param Page The page number to read from.
 * @param Buf The buffer to store the read data.
 *
 * @return Returns AL_OK if the operation is successful, otherwise returns an error code.
 *
 */
AL_U32 ALSmc_Dev_HwEccReadPage(AL_SMC_DevStruct *Smc, AL_NAND_InfoStruct *NandInfo, AL_U32 Page, AL_U8 *Buf)
{
    AL_U8 EccDataNums, EccOffset=0, Index;
    AL_U32 *DataOffsetPtr, Status;
    AL_U8 *TempBuf = Buf;

    NandInfo->Cmd.StartCmd = ONFI_CMD_READ_PAGE1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_READ_PAGE2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_READ_PAGE_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_READ_PAGE_END_TIMING;

    NandInfo->Cmd.Page = Page;
    NandInfo->Cmd.Column = 0;

    ALSmc_Dev_SendCmd(NandInfo);
    // AlSys_UDelay(2);
    /* Wait smc ready */
    while(SMC_BUSY == AlSmc_ll_IsBusy(Smc->SmcBaseAddr));
    /* Clear intrrupt */
    AlSmc_ll_ClrIntr1(Smc->SmcBaseAddr);

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = 0;
    ALSmc_Dev_ReadBuf(NandInfo, Buf, NandInfo->Size.DataBytesPerPage -ONFI_AXI_DATA_WIDTH);
    Buf += (NandInfo->Size.DataBytesPerPage - ONFI_AXI_DATA_WIDTH);

    NandInfo->Cmd.EccLast = ECC_LAST;
    NandInfo->Cmd.ClearCs = 0;
    ALSmc_Dev_ReadBuf(NandInfo, Buf, ONFI_AXI_DATA_WIDTH);

    switch (NandInfo->Size.SpareBytesPerPage) {
    case 64:
        EccDataNums = 12;
        DataOffsetPtr = NandOob64;
        break;
    case 32:
        EccDataNums = 6;
        DataOffsetPtr = NandOob32;
        break;
    case 16:
        EccDataNums = 3;
        DataOffsetPtr = NandOob16;
        break;
    default:
        /* Page size 256 bytes & 4096 bytes not supported by ECC block*/
        return AL_SMC_EVENTS_TO_ERRS(SmcHwReadSizeErr);
        break;
    }

    Status = AlSmc_Dev_HwCalculateEcc(Smc, NandInfo->EccCalc, EccDataNums);
    if (Status != AL_OK) {
        return Status;
    }

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = 0;
    ALSmc_Dev_ReadBuf(NandInfo, NandInfo->SpareBuf, NandInfo->Size.SpareBytesPerPage - ONFI_AXI_DATA_WIDTH);

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = CLEAR_CS;
    ALSmc_Dev_ReadBuf(NandInfo, NandInfo->SpareBuf + NandInfo->Size.SpareBytesPerPage - ONFI_AXI_DATA_WIDTH, ONFI_AXI_DATA_WIDTH);

    for (Index = 0; Index < EccDataNums; Index++) {
        NandInfo->EccCode[Index] = ~NandInfo->SpareBuf[DataOffsetPtr[Index]];
    }

    Index = NandInfo->Size.DataBytesPerPage / NAND_ECC_BLOCK_SIZE;
    for (; Index; Index--) {
        Status = AlSmc_Dev_HwCorrectEcc(&NandInfo->EccCode[EccOffset], &NandInfo->EccCalc[EccOffset], TempBuf);
        if (Status != AL_OK)
            return Status;
        EccOffset += NAND_ECC_BYTES;
        TempBuf += NAND_ECC_BLOCK_SIZE;
    }

    return AL_OK;
}


/**
 *
 * This function writes a page of data to the NAND flash device specified by the Smc and NandInfo parameters.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct structure representing the SMC device.
 * @param NandInfo Pointer to the AL_NAND_InfoStruct structure representing the NAND flash device information.
 * @param Page The page number to write the data to.
 * @param Buf Pointer to the buffer containing the data to be written.
 *
 * @return The status of the write operation. Returns AL_OK if the write operation is successful,
 * or an error code if the write operation fails.
 *
 */
AL_U32 ALSmc_Dev_WritePage(AL_SMC_DevStruct *Smc, AL_NAND_InfoStruct *NandInfo, AL_U32 Page, AL_U8 *Buf)
{
    AL_U32 Status;

    NandInfo->Cmd.StartCmd = ONFI_CMD_PROGRAM_PAGE1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_PROGRAM_PAGE2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_PROGRAM_PAGE_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_PROGRAM_PAGE_END_TIMING;

    NandInfo->Cmd.Page = Page;
    NandInfo->Cmd.Column = 0;

    ALSmc_Dev_SendCmd(NandInfo);

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = 0;
    ALSmc_Dev_WriteBuf(NandInfo, Buf, NandInfo->Size.DataBytesPerPage);

    ALSmc_Dev_WriteBuf(NandInfo, NandInfo->SpareBuf, NandInfo->Size.SpareBytesPerPage - ONFI_AXI_DATA_WIDTH);

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = CLEAR_CS;
    ALSmc_Dev_WriteBuf(NandInfo, NandInfo->SpareBuf + NandInfo->Size.SpareBytesPerPage - ONFI_AXI_DATA_WIDTH, ONFI_AXI_DATA_WIDTH);

    /* Wait smc ready */
    while(SMC_BUSY == AlSmc_ll_IsBusy(Smc->SmcBaseAddr));
    /* Clear intrrupt */
    AlSmc_ll_ClrIntr1(Smc->SmcBaseAddr);

    /* Check Nand Status */
    Status = ALSmc_Dev_ReadStatus(NandInfo);
    if (Status & ONFI_STATUS_FAIL) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Smc Nand WritePage ONFI_STATUS_FAIL error\r\n");
        return AL_SMC_EVENTS_TO_ERRS(SmcOnfiStatusFailErr);
    }

    return AL_OK;
}


/**
 *
 * This function reads a page from the NAND flash device specified by the Smc and NandInfo parameters.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct structure representing the SMC device.
 * @param NandInfo Pointer to the AL_NAND_InfoStruct structure representing the NAND flash device information.
 * @param Page The page number to read from.
 * @param Buf Pointer to the buffer where the read data will be stored.
 *
 * @return Returns AL_OK if the page read operation is successful. Otherwise, it returns an error code.
 *
 */
AL_U32 ALSmc_Dev_ReadPage(AL_SMC_DevStruct *Smc, AL_NAND_InfoStruct *NandInfo, AL_U32 Page, AL_U8 *Buf)
{
    AL_U32 Status;
    AL_REG CmdPhaseAddr;

    NandInfo->Cmd.StartCmd = ONFI_CMD_READ_PAGE1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_READ_PAGE2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_READ_PAGE_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_READ_PAGE_END_TIMING;

    NandInfo->Cmd.Page = Page;
    NandInfo->Cmd.Column = 0;

    ALSmc_Dev_SendCmd(NandInfo);

    /* Check Nand Status */
    Status = ALSmc_Dev_ReadStatus(NandInfo);
    if (Status & ONFI_STATUS_FAIL) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Smc Nand ReadPage ONFI_STATUS_FAIL error\r\n");
        return AL_SMC_EVENTS_TO_ERRS(SmcOnfiStatusFailErr);
    }

    CmdPhaseAddr  = NAND_BASE_ADDR                 |
            (0     << NAND_ADDR_CYCLES_SHIFT)        |
            (0     << NAND_END_CMD_VALID_SHIFT)    |
            NAND_COMMAND_PHASE_FLAG                |
            (0     << NAND_END_CMD_SHIFT)            |
            (0     << NAND_ECC_LAST_SHIFT);

    AL_REG32_WRITE(CmdPhaseAddr, 0);

    /* Wait smc ready */
    while(SMC_BUSY == AlSmc_ll_IsBusy(Smc->SmcBaseAddr));
    /* Clear intrrupt */
    AlSmc_ll_ClrIntr1(Smc->SmcBaseAddr);

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = 0;
    ALSmc_Dev_ReadBuf(NandInfo, Buf, NandInfo->Size.DataBytesPerPage);

    ALSmc_Dev_ReadBuf(NandInfo, NandInfo->SpareBuf, NandInfo->Size.SpareBytesPerPage - ONFI_AXI_DATA_WIDTH);

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = CLEAR_CS;
    ALSmc_Dev_ReadBuf(NandInfo, NandInfo->SpareBuf + NandInfo->Size.SpareBytesPerPage - ONFI_AXI_DATA_WIDTH, ONFI_AXI_DATA_WIDTH);

    return AL_OK;
}


/**
 *
 * This function writes spare data to a NAND flash device using the provided parameters.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct structure representing the SMC device.
 * @param NandInfo Pointer to the AL_NAND_InfoStruct structure representing the NAND flash device information.
 * @param Page The page number to write the spare data to.
 *
 * @return The status of the write operation. Returns AL_OK if successful, or an error code if an error occurred.
 *
 */
AL_U32 ALSmc_Dev_WriteSpare(AL_SMC_DevStruct *Smc, AL_NAND_InfoStruct *NandInfo, AL_U32 Page)
{
    AL_U32 Status;

    if (1 == NandInfo->Size.EccNum) {
        AlSmc_Dev_EccHwDisable(Smc);
    }

    NandInfo->Cmd.StartCmd = ONFI_CMD_PROGRAM_PAGE1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_PROGRAM_PAGE2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_PROGRAM_PAGE_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_PROGRAM_PAGE_END_TIMING;

    NandInfo->Cmd.Page = Page;
    NandInfo->Cmd.Column = NandInfo->Size.DataBytesPerPage;

    ALSmc_Dev_SendCmd(NandInfo);

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = 0;
    ALSmc_Dev_WriteBuf(NandInfo, NandInfo->SpareBuf, NandInfo->Size.SpareBytesPerPage - ONFI_AXI_DATA_WIDTH);

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = CLEAR_CS;
    ALSmc_Dev_WriteBuf(NandInfo, NandInfo->SpareBuf + NandInfo->Size.SpareBytesPerPage- ONFI_AXI_DATA_WIDTH, ONFI_AXI_DATA_WIDTH);

    /* Wait smc ready */
    while(SMC_BUSY == AlSmc_ll_IsBusy(Smc->SmcBaseAddr));
    /* Clear intrrupt */
    AlSmc_ll_ClrIntr1(Smc->SmcBaseAddr);

    /* Check Nand Status */
    Status = ALSmc_Dev_ReadStatus(NandInfo);
    if (Status & ONFI_STATUS_FAIL) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Smc Nand WritceSpare ONFI_STATUS_FAIL error\r\n");
        return AL_SMC_EVENTS_TO_ERRS(SmcOnfiStatusFailErr);
    }

    if (1 == NandInfo->Size.EccNum) {
        AlSmc_Dev_EccHwEnable(Smc);
    }

    return AL_OK;
}


/**
 *
 * This function reads spare data from a NAND flash device using the provided parameters.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct structure representing the SMC device.
 * @param NandInfo Pointer to the AL_NAND_InfoStruct structure representing the NAND flash device information.
 * @param Page The page number from which to read the spare data.
 *
 * @return The status of the operation. Returns AL_OK if successful, or an error code if an error occurred.
 *
 */
AL_U32 ALSmc_Dev_ReadSpare(AL_SMC_DevStruct *Smc, AL_NAND_InfoStruct *NandInfo, AL_U32 Page)
{
    // Function variables
    AL_U32 Status;
    AL_REG CmdPhaseAddr;

    // Disable ECC hardware if ECCNum is 1
    if (1 == NandInfo->Size.EccNum) {
        AlSmc_Dev_EccHwDisable(Smc);
    }

    // Set command parameters for reading page
    NandInfo->Cmd.StartCmd = ONFI_CMD_READ_PAGE1;
    NandInfo->Cmd.EndCmd = ONFI_CMD_READ_PAGE2;
    NandInfo->Cmd.AddrCycles = ONFI_CMD_READ_PAGE_CYCLES;
    NandInfo->Cmd.EndCmdPhase = ONFI_CMD_READ_PAGE_END_TIMING;

    NandInfo->Cmd.Page = Page;
    NandInfo->Cmd.Column = NandInfo->Size.DataBytesPerPage;

    // Send command to the NAND flash device
    ALSmc_Dev_SendCmd(NandInfo);

    /* Wait smc ready */
    while(SMC_BUSY == AlSmc_ll_IsBusy(Smc->SmcBaseAddr));
    /* Clear interrupt */
    AlSmc_ll_ClrIntr1(Smc->SmcBaseAddr);

    /* Check NAND status */
    Status = ALSmc_Dev_ReadStatus(NandInfo);
    if (Status & ONFI_STATUS_FAIL) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Smc Nand ReadSpare ONFI_STATUS_FAIL error\r\n");
        return AL_SMC_EVENTS_TO_ERRS(SmcOnfiStatusFailErr);
    }

    // Set command phase address
    CmdPhaseAddr  = NAND_BASE_ADDR                 |
            (0     << NAND_ADDR_CYCLES_SHIFT)      |
            (0     << NAND_END_CMD_VALID_SHIFT)    |
            NAND_COMMAND_PHASE_FLAG                |
            (0     << NAND_END_CMD_SHIFT)          |
            (0     << NAND_ECC_LAST_SHIFT);

    AL_REG32_WRITE(CmdPhaseAddr, 0);

    // Read spare data from the NAND flash device
    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = 0;
    ALSmc_Dev_ReadBuf(NandInfo, NandInfo->SpareBuf, NandInfo->Size.SpareBytesPerPage - ONFI_AXI_DATA_WIDTH);

    NandInfo->Cmd.EccLast = 0;
    NandInfo->Cmd.ClearCs = CLEAR_CS;
    ALSmc_Dev_ReadBuf(NandInfo, NandInfo->SpareBuf + NandInfo->Size.SpareBytesPerPage - ONFI_AXI_DATA_WIDTH, ONFI_AXI_DATA_WIDTH);

    // Enable ECC hardware if ECCNum is 1
    if (1 == NandInfo->Size.EccNum) {
        AlSmc_Dev_EccHwEnable(Smc);
    }

    return AL_OK;
}


#define BAD_BLOCK 0xff


/**
 *
 * This function reads the spare area of a NAND flash page and checks if the first byte is 0xFF.
 * If the first byte is not 0xFF, it indicates that the block is bad.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct structure representing the SMC device.
 * @param NandInfo Pointer to the AL_NAND_InfoStruct structure representing the NAND flash device information.
 * @param Page The page number to check.
 *
 * @return AL_OK if the block is not bad, otherwise returns the error code AL_SMC_EVENTS_TO_ERRS(BAD_BLOCK).
 *
 */
AL_U32 ALSmc_Dev_CheckIsBadBlock(AL_SMC_DevStruct *Smc, AL_NAND_InfoStruct *NandInfo, AL_U32 Page)
{
    ALSmc_Dev_ReadSpare(Smc, NandInfo, Page);

    if (NandInfo->SpareBuf[0] != 0xff) {
        return AL_SMC_EVENTS_TO_ERRS(BAD_BLOCK);
    }

    return AL_OK;
}


/**
 *
 * This function calculates the ECC value for the specified data and retrieves it from the hardware.
 * It checks the busy signal to ensure that the ECC calculation is complete before proceeding.
 * The ECC value is read from the hardware and stored in the provided data buffer.
 * If the ECC value is not valid, an error is returned.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct structure.
 * @param Data Pointer to the data buffer where the ECC value will be stored.
 * @param EccDataNums Number of ECC data values to calculate and retrieve.
 *
 * @return AL_OK if the ECC calculation and retrieval is successful.
 *         AL_SMC_EVENTS_TO_ERRS(SmcEccDataInvalidErr) if the ECC value is not valid.
 *
 */
AL_U32 AlSmc_Dev_HwCalculateEcc(AL_SMC_DevStruct *Smc, AL_U8 *Data, AL_U8 EccDataNums)
{
    AL_U8 Count, EccReg;
    AL_U32 EccValue;

    /* Check busy signal if it is busy to poll*/
    while(ECC_BUSY == AlSmc_ll_IsEccBusy(Smc->SmcBaseAddr));

    for(EccReg=0; EccReg < EccDataNums/3; EccReg++) {
        EccValue = AlSmc_ll_ReadEccValue(Smc->SmcBaseAddr, EccReg);
        if (EccValue & (1 << SMC_ECC1_BLOCK0_ECC1_0_VALID_SHIFT)) {
            for (Count = 0; Count < 3; Count++) {
                *Data = EccValue & 0xFF;
                EccValue = EccValue >> 8;
                Data++;
            }
        } else {
            /* error  */
            return AL_SMC_EVENTS_TO_ERRS(SmcEccDataInvalidErr);
        }
    }

    return AL_OK;
}


/**
 *
 * This function takes in the ECC code read from memory, the ECC code calculated for the buffer,
 * and the buffer itself. It performs ECC correction if errors are detected in the ECC codes.
 *
 * @param eccCode Pointer to the ECC code read from memory.
 * @param eccCalc Pointer to the ECC code calculated for the buffer.
 * @param buf Pointer to the buffer.
 *
 * @return AL_OK if no errors are detected after ECC correction.
 *         AL_SMC_EVENTS_TO_ERRS(SmcTwoBitsErr) if two bits error is detected.
 *         AL_SMC_EVENTS_TO_ERRS(SmcMultipleBitsErr) if multiple bits errors are detected.
 *
 */
AL_U32 AlSmc_Dev_HwCorrectEcc(AL_U8 *eccCode, AL_U8 *eccCalc, AL_U8 *buf)
{
    AL_U8 bitPos = 0;
    AL_U32 bytePos = 0;
    AL_U16 eccOdd = 0, eccEven = 0;
    AL_U16 readEccLow = 0, readEccHigh = 0;
    AL_U16 calcEccLow = 0, calcEccHigh = 0;

    /* Lower 12 bits of ECC Read */
    readEccLow = (eccCode[0] | (eccCode[1] << 8)) & 0xfff;
    /* Upper 12 bits of ECC Read */
    readEccHigh = ((eccCode[1] >> 4) | (eccCode[2] << 4)) & 0xfff;

    /* Lower 12 bits of ECC calculated */
    calcEccLow = (eccCalc[0] | (eccCalc[1] << 8)) & 0xfff;
    /* Upper 12 bits of ECC Calculated */
    calcEccHigh = ((eccCalc[1] >> 4) | (eccCalc[2] << 4)) & 0xfff;

    eccOdd = readEccLow ^ calcEccLow;
    eccEven = readEccHigh ^ calcEccHigh;

    /* No Error */
    if ((eccOdd == 0) && (eccEven == 0)) {
        return AL_OK;
    }

    /* One bit Error */
    if (eccOdd == (~eccEven & 0xfff)) {
        bytePos = (eccOdd >> 3) & 0x1ff;
        bitPos = eccOdd & 0x07;
        /* Toggling error bit */
        buf[bytePos] ^= (1 << bitPos);
        AL_LOG(AL_LOG_LEVEL_WARNING, "Smc Nand Read Page happen One bit Error:bitPos:%d, buf[%d]:%d\r\n",bitPos, bytePos, buf[bytePos]);
        return AL_OK;
    }

    /* Two bits Error */
    if (OneHot((eccOdd | eccEven)) == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Smc Nand Read Page Find Two bits Error\r\n");
        return AL_SMC_EVENTS_TO_ERRS(SmcTwoBitsErr);
    }

    /* Multiple bits error */
    return AL_SMC_EVENTS_TO_ERRS(SmcMultipleBitsErr);
}


/**
 *
 * This function initializes the ECC hardware for the SMC device by setting the necessary registers
 * and configurations based on the NAND flash information provided.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct representing the SMC device.
 * @param NandInfo Pointer to the AL_NAND_InfoStruct representing the NAND flash information.
 *
 * @return AL_U32 Returns AL_OK if the ECC hardware initialization is successful.
 *                Returns an error code if the page size is not supported by the ECC block.
 *
 */
AL_U32 AlSmc_Dev_EccHwInit(AL_SMC_DevStruct *Smc, AL_NAND_InfoStruct *NandInfo)
{
    AL_U32 Ecc1Config;

    /* Set SMC_REG_ECC1_MEMCMD0 Reg*/
    AlSmc_ll_SetEccCmd0(Smc->SmcBaseAddr, SMC_ECC_CMD0);

    /* Set SMC_REG_ECC1_MEMCMD1 Reg*/
    AlSmc_ll_SetEccCmd1(Smc->SmcBaseAddr, SMC_ECC_CMD1);

    switch (NandInfo->Size.DataBytesPerPage) {
    case 512:
        Ecc1Config = ONE_512BYTE_SIZE;
        break;
    case 1024:
        Ecc1Config = TWO_512BYTE_SIZE;
        break;
    case 2048 :
        Ecc1Config = FOUR_512BYTE_SIZE;
        break;
    default:
        /* Page size 256 bytes & 4096 bytes not supported by ECC block */
        return AL_SMC_EVENTS_TO_ERRS(SmcHwInitSizeErr);
        break;
    }

    /* Check Ecc Busy */
    while(ECC_BUSY == AlSmc_ll_IsEccBusy(Smc->SmcBaseAddr));

    AlSmc_ll_SetEccMode(Smc->SmcBaseAddr, ECC_MODE_APB);
    AlSmc_ll_SetEccReadEnd(Smc->SmcBaseAddr, InPageEnd);
    AlSmc_ll_SetEccJump(Smc->SmcBaseAddr, ECC_NOJUMP);
    AlSmc_ll_SetEccPageSize(Smc->SmcBaseAddr, Ecc1Config);

    return AL_OK;
}


/**
 *
 * This function disables the ECC hardware for the specified AL_SMC_DevStruct.
 * It first checks if the ECC is busy and waits until it becomes idle.
 * Then, it sets the ECC mode to bypass.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct.
 *
 * @return AL_OK if the ECC hardware is successfully disabled.
 *
 */
AL_U32 AlSmc_Dev_EccHwDisable(AL_SMC_DevStruct *Smc)
{
    /* Check Ecc Busy */
    while(ECC_BUSY == AlSmc_ll_IsEccBusy(Smc->SmcBaseAddr));

    /* Set Ecc Model is bypass */
    AlSmc_ll_SetEccMode(Smc->SmcBaseAddr, ECC_BYPASSS);

    return AL_OK;
}


/**
 *
 * This function enables the ECC hardware for the specified AL_SMC_DevStruct.
 * It first checks if the ECC is busy and waits until it becomes idle.
 * Then, it sets the ECC mode to bypass using the AlSmc_ll_SetEccMode function.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct.
 *
 * @return AL_OK if the ECC hardware is successfully enabled.
 *
 */
AL_U32 AlSmc_Dev_EccHwEnable(AL_SMC_DevStruct *Smc)
{
    /* Check Ecc Busy */
    while(ECC_BUSY == AlSmc_ll_IsEccBusy(Smc->SmcBaseAddr));

    /* Set Ecc Model is bypass */
    AlSmc_ll_SetEccMode(Smc->SmcBaseAddr, ECC_MODE_APB);

    return AL_OK;
}


/**
 * This function enables on-die ECC for the NAND flash device.
 *
 * @param NandInfo Pointer to the AL_NAND_InfoStruct containing NAND flash device information.
 *
 * @return AL_U32 Returns AL_OK if on-die ECC is enabled successfully, otherwise returns an error code.
 *
 */
AL_U32 AlSmc_Dev_EnableOnDieEcc(AL_NAND_InfoStruct *NandInfo)
{
    AL_U8 EccGetFeature[4];
    AL_U8 EccSetFeature[4] = {0x08, 0, 0, 0};

    if (NandInfo->Size.DeviceId[0] == NAND_MFR_AMD) {
        EccSetFeature[0] = 0x10;
    }

    ALSmc_Dev_GetFeature(NandInfo, 0x90, &EccGetFeature[0]);
    ALSmc_Dev_SetFeature(NandInfo, 0x90, &EccSetFeature[0]);

    if (EccGetFeature[0] != EccSetFeature[0]) {
        return AL_SMC_EVENTS_TO_ERRS(SmcWriteEccFeatErr);
    }

    return AL_OK;
}

/**
 *
 * This function initializes the cycles and memory width for the AL_SMC_DevStruct.
 * It sets the cycles if the cycles value is not zero, sets the memory width,
 * and sets the command type and chip number.
 *
 * @param Smc Pointer to the AL_SMC_DevStruct.
 *
 * @return None.
 *
 */
AL_VOID AlSmc_Dev_InitCyclesAndMemWidth(AL_SMC_DevStruct *Smc)
{
    if (Smc->Configs.Cycles.d32 != 0) {
        AlSmc_ll_SetCycles(Smc->SmcBaseAddr, Smc->Configs.Cycles);
    }

    AlSmc_ll_SetMemWidth(Smc->SmcBaseAddr, Smc->Configs.SmcWidth);
    AlSmc_ll_SetCmdTypeAndChipNum(Smc->SmcBaseAddr, UPDATE_REGS, INTF1_CHIP1);
}

