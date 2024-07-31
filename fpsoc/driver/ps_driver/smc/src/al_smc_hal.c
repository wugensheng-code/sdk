/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_smc_hal.h"
#include <string.h>
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_SMC_DevStruct AL_SMC_DevInstance[AL_SMC_NUM_INSTANCE];
static AL_NAND_InfoStruct AL_NAND_InfoInstance[AL_SMC_NUM_INSTANCE];

/********************************************************/
#define AL_SMC_HAL_LOCK(Handle)          do {} while (0)
#define AL_SMC_HAL_UNLOCK(Handle)        do {} while (0)


/**
 *
 * This function configures the SMC peripheral based on the configurations passed through `InitConfig`.
 * It sets up the hardware module identified by `DevId` and updates the `Handle` structure to reflect
 * the new configuration. The function also performs a hardware reset on the NAND device, reads its ID,
 * and configures ECC (Error-Correcting Code) based on the NAND device's requirements.
 *
 * @param Handle Pointer to an `AL_SMC_HalStruct` structure that contains the SMC device instance.
 * @param InitConfig Pointer to an `AL_SMC_ConfigsStruct` structure that contains the configuration
 * information for the specified SMC peripheral.
 * @param DevId Hardware module ID.
 *
 * @return AL_U32 Returns `AL_OK` if the function succeeds, otherwise returns an error code indicating
 * the type of failure.
 *
 */
AL_U32 AlSmc_Hal_Init(AL_SMC_HalStruct *Handle, AL_SMC_ConfigsStruct *InitConfig, AL_U32 DevId)
{
    AL_S32 Ret = AL_OK;
    AL_SMC_HwConfigStruct *CfgPtr = AL_NULL;

    AL_ASSERT(Handle != AL_NULL, AL_SMC_ERR_ILLEGAL_PARAM);

    AL_SMC_HAL_LOCK(Handle);

    CfgPtr = AlSmc_Dev_LookupConfig(DevId);
    Handle->Dev = &AL_SMC_DevInstance[CfgPtr->DeviceId];
    Handle->Dev->SmcBaseAddr = CfgPtr->SmcBaseAddr;
    Handle->Dev->NandBaseAddr = CfgPtr->NandBaseAddr;
    Handle->Dev->InputClkFreq = CfgPtr->InputClkFreq;
    Handle->NandInfo = &AL_NAND_InfoInstance[CfgPtr->DeviceId];

    AlSmc_Dev_EccHwDisable(Handle->Dev);

    Ret = ALSmc_Dev_Reset(Handle->NandInfo);
    if (AL_OK != Ret) {
        goto HAL_INIT_END;
    }

    ALSmc_Dev_ReadId(Handle->NandInfo);

    AL_LOG(AL_LOG_LEVEL_INFO, "NandFlash ID:0x%x, 0x%x, 0x%x, 0x%x, 0x%x", Handle->NandInfo->Size.DeviceId[0],
    Handle->NandInfo->Size.DeviceId[1], Handle->NandInfo->Size.DeviceId[2],
    Handle->NandInfo->Size.DeviceId[3], Handle->NandInfo->Size.DeviceId[4]);

    Ret = ALSmc_Dev_ReadParam(Handle->NandInfo);
    if (AL_OK != Ret) {
        goto HAL_INIT_END;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "SpareBytesPerPage: %d\r\n", Handle->NandInfo->Size.SpareBytesPerPage);
    AL_LOG(AL_LOG_LEVEL_INFO, "DataBytesPerPage: %d\r\n", Handle->NandInfo->Size.DataBytesPerPage);
    AL_LOG(AL_LOG_LEVEL_INFO, "PagesPerBlock: %d\r\n", Handle->NandInfo->Size.PagesPerBlock);
    AL_LOG(AL_LOG_LEVEL_INFO, "BlocksPerUnit: %d\r\n", Handle->NandInfo->Size.BlocksPerUnit);
    AL_LOG(AL_LOG_LEVEL_INFO, "TotalUnit: %d\r\n", Handle->NandInfo->Size.TotalUnit);
    AL_LOG(AL_LOG_LEVEL_INFO, "EccNum: %d\r\n", Handle->NandInfo->Size.EccNum);

    if (Handle->NandInfo->Size.EccNum == 1) {
        Ret = AlSmc_Dev_EccHwInit(Handle->Dev, Handle->NandInfo);
        if (AL_OK != Ret) {
            goto HAL_INIT_END;
        }
    } else {
        Ret = AlSmc_Dev_EccHwDisable(Handle->Dev);
        if (AL_OK != Ret) {
            goto HAL_INIT_END;
        }

        Ret = AlSmc_Dev_EnableOnDieEcc(Handle->NandInfo);
        if (AL_OK != Ret) {
            goto HAL_INIT_END;
        }
    }

    ALSmc_Dev_SetFixedTiming(Handle->Dev);

    HAL_INIT_END:
    AL_SMC_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 *
 * This function reads a specified amount of data starting from a given offset in the NAND flash memory.
 * It handles partial page reads, ECC (if enabled), and updates the provided buffer with the read data.
 *
 * @param Handle Pointer to an `AL_SMC_HalStruct` structure that contains the SMC device instance.
 * @param Offset The offset from which to start reading data in the NAND flash.
 * @param Data Pointer to the buffer where the read data will be stored.
 * @param Size The number of bytes to read from the NAND flash.
 *
 * @return AL_U32 Returns `AL_OK` if the function succeeds in reading the data, otherwise returns
 * an error code indicating the type of failure.
 *
 */
AL_U32 AlSmc_Hal_ReadPage(AL_SMC_HalStruct *Handle, AL_U64 Offset, AL_U8 *Data, AL_U32 Size)
{
    AL_U8 Status;
    AL_U32 NumOfBytes, PartialBytes, CopyOffset, PartialPageRead, Page, Col;
    AL_U8 *Ptr = Data;
    AL_U8 *BufPtr = AL_NULL;

    AL_ASSERT(Handle != AL_NULL, AL_SMC_ERR_ILLEGAL_PARAM);

    Page = (AL_U32) (Offset / Handle->NandInfo->Size.DataBytesPerPage);
    Col = (AL_U32) (Offset & (Handle->NandInfo->Size.DataBytesPerPage - 1));
    PartialBytes = Handle->NandInfo->Size.DataBytesPerPage - Col;
    NumOfBytes = (PartialBytes < Size) ? PartialBytes:Size;
    CopyOffset = Handle->NandInfo->Size.DataBytesPerPage - PartialBytes;

    /* Set SMC_REG_ECC1_MEMCMD0 Reg*/
    AlSmc_ll_SetEccCmd0(Handle->Dev->SmcBaseAddr, SMC_ECC_CMD0);

    /* Set SMC_REG_ECC1_MEMCMD1 Reg*/
    AlSmc_ll_SetEccCmd1(Handle->Dev->SmcBaseAddr, SMC_ECC_CMD1);

    while(Size) {
        /* Check if partial read */
        if (NumOfBytes < Handle->NandInfo->Size.DataBytesPerPage) {
            BufPtr = &Handle->NandInfo->DataBuf[0];
            PartialPageRead = 1;
        } else {
            BufPtr = Ptr;
            PartialPageRead = 0;
        }

        /* ecc Num = 1.Use SMC IP ECC */
        if (Handle->NandInfo->Size.EccNum == 1) {
            Status = ALSmc_Dev_HwEccReadPage(Handle->Dev, Handle->NandInfo, Page, BufPtr);
        } else {
            Status = ALSmc_Dev_ReadPage(Handle->Dev, Handle->NandInfo, Page, BufPtr);
        }

        if (Status != AL_OK) {
            return Status;
        }

        if (PartialPageRead) {
            memcpy(Ptr, (BufPtr + CopyOffset), NumOfBytes);
        }

        Ptr += NumOfBytes;
        Size -= NumOfBytes;
        Page++;
        NumOfBytes = (Size > Handle->NandInfo->Size.DataBytesPerPage) ?
        Handle->NandInfo->Size.DataBytesPerPage : Size;
        CopyOffset = 0;
    }

    return AL_OK;
}

/**
 *
 * This function writes a specified amount of data to the NAND flash memory starting from a given offset.
 * It handles partial page writes, ECC (if enabled), and ensures that the write protection status is
 * checked before attempting to write to the NAND flash.
 *
 * @param Handle Pointer to an `AL_SMC_HalStruct` structure that contains the SMC device instance.
 * @param Offset The offset at which to start writing data to the NAND flash.
 * @param Data Pointer to the buffer containing the data to be written.
 * @param Size The number of bytes to write to the NAND flash.
 *
 * @return AL_U32 Returns `AL_OK` if the function succeeds in writing the data, otherwise returns
 * an error code indicating the type of failure.
 *
 */
AL_U32 AlSmc_Hal_WritePage(AL_SMC_HalStruct *Handle, AL_U64 Offset, AL_U8 *Data, AL_U32 Size)
{
    AL_U8 Status;
    AL_U32 NumOfBytes, PartialBytes, CopyOffset, Page, Col;
    AL_U8 *Ptr = Data;
    AL_U8 *BufPtr = AL_NULL;

    AL_ASSERT(Handle != AL_NULL, AL_SMC_ERR_ILLEGAL_PARAM);

    /* Check Nand Status */
    Status = ALSmc_Dev_ReadStatus(Handle->NandInfo);
    if (!(Status & ONFI_STATUS_WP)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Smc Nand ONFI_STATUS_WP error\r\n");
        return AL_SMC_ERR_NOT_SUPPORT;
    }

    memset(Handle->NandInfo->SpareBuf, 0xff, Handle->NandInfo->Size.SpareBytesPerPage);

    Page = (AL_U32) (Offset / Handle->NandInfo->Size.DataBytesPerPage);
    Col = (AL_U32) (Offset & (Handle->NandInfo->Size.DataBytesPerPage - 1));
    PartialBytes = Handle->NandInfo->Size.DataBytesPerPage - Col;
    NumOfBytes = (PartialBytes < Size) ? PartialBytes:Size;
    CopyOffset = Handle->NandInfo->Size.DataBytesPerPage - PartialBytes;

    while(Size) {
        /* Check if partial read */
        if (NumOfBytes < Handle->NandInfo->Size.DataBytesPerPage) {
            BufPtr = Handle->NandInfo->DataBuf;
            memset(BufPtr, 0xff, Handle->NandInfo->Size.DataBytesPerPage);
            memcpy(BufPtr + CopyOffset, Ptr, NumOfBytes);
        } else {
            BufPtr = Ptr;
        }

        /* ecc Num = 1.Use SMC IP ECC */
        if (Handle->NandInfo->Size.EccNum == 1) {
            Status = ALSmc_Dev_HwEccWritePage(Handle->Dev, Handle->NandInfo, Page, BufPtr);
        } else {
            Status = ALSmc_Dev_WritePage(Handle->Dev, Handle->NandInfo, Page, BufPtr);
        }

        if (Status != AL_OK) {
            return Status;
        }

        Ptr += NumOfBytes;
        Size -= NumOfBytes;
        Page++;
        NumOfBytes = (Size > Handle->NandInfo->Size.DataBytesPerPage) ? Handle->NandInfo->Size.DataBytesPerPage : Size;
        CopyOffset = 0;
    }
    return AL_OK;
}


/**
 *
 * This function reads the spare data from the specified page in the SMC device.
 *
 * @param Handle Pointer to the AL_SMC_HalStruct structure.
 * @param Page The page number from which to read the spare data.
 * @param Data Pointer to the buffer where the spare data will be stored.
 *
 * @return The status of the read operation.
 *
 */
AL_U32 AlSmc_Hal_ReadSpare(AL_SMC_HalStruct *Handle, AL_U32 Page, AL_U8 *Data)
{
    AL_U8 Status;
    AL_ASSERT(Handle != AL_NULL, AL_SMC_ERR_ILLEGAL_PARAM);

    Status = ALSmc_Dev_ReadSpare(Handle->Dev, Handle->NandInfo, Page);
    return Status;
}

/**
 *
 * This function writes spare data to the specified page in the NAND flash memory.
 *
 * @param Handle Pointer to the AL_SMC_HalStruct structure.
 * @param Page The page number to write the spare data to.
 * @param Data Pointer to the data to be written.
 *
 * @return The status of the write operation.
 *
 */
AL_U32 AlSmc_Hal_WriteSpare(AL_SMC_HalStruct *Handle, AL_U32 Page, AL_U8 *Data)
{
   	AL_U8 Status;
    AL_ASSERT(Handle != AL_NULL, AL_SMC_ERR_ILLEGAL_PARAM);

    Status = ALSmc_Dev_WriteSpare(Handle->Dev, Handle->NandInfo, Page);
    return Status;
}

/**
 *
 * This function erases a block in the SMC HAL using the specified handle and page number.
 *
 * @param Handle The SMC HAL handle.
 * @param Page The page number of the block to be erased.
 *
 * @return The status of the erase operation.
 *
 */
AL_U32 AlSmc_Hal_EraseBlock(AL_SMC_HalStruct *Handle, AL_U32 Page)
{
    AL_U8 Status;
    AL_ASSERT(Handle != AL_NULL, AL_SMC_ERR_ILLEGAL_PARAM);

    Status = ALSmc_Dev_EraseBlock(Handle->Dev, Handle->NandInfo, Page);
    return Status;
}
