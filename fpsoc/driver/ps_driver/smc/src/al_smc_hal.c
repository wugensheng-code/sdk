/***************************** Include Files *********************************/
#include "al_smc_hal.h"
#include "al_intr.h"
#include <string.h>
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_SMC_DevStruct AL_SMC_DevInstance[AL_SMC_NUM_INSTANCE];
static AL_NAND_InfoStruct AL_NAND_InfoInstance[AL_SMC_NUM_INSTANCE];

/********************************************************/
#ifdef USE_RTOS
static AL_S32 AlSmc_Hal_WaitTxDoneOrTimeout(AL_SMC_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

static AL_S32 AlSmc_Hal_WaitRxDoneOrTimeout(AL_SMC_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO:wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

static AlSmc_Hal_IntrHandler(AL_SMC_HalStruct *Handle, AL_U32 Event, AL_U32 EventData)
{
    /*
     * TODO:send event
    */
    (void) Handle;
    (void) Event;
    (void) EventData;
}

#define AL_SMC_HAL_LOCK(Handle)        do {} while (0)

#define AL_SMC_HAL_UNLOCK(Handle)      do {} while (0)

#else


#define AL_SMC_HAL_LOCK(Handle)          do {} while (0)

#define AL_SMC_HAL_UNLOCK(Handle)        do {} while (0)

#endif

AL_U32 AlSmc_Hal_Init(AL_SMC_HalStruct *Handle, AL_SMC_ConfigsStruct *InitConfig, AL_U32 DevId)
{
    AL_S32 ret = AL_OK;
    AL_SMC_HwConfigStruct *CfgPtr = AL_NULL;

    if (Handle == AL_NULL) {
        return AL_DEF_ERR(AL_SMC, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    AL_SMC_HAL_LOCK(Handle);

    CfgPtr = AlSmc_Dev_LookupConfig(DevId);
    Handle->Dev = &AL_SMC_DevInstance[CfgPtr->DeviceId];
    Handle->Dev->SmcBaseAddr = CfgPtr->SmcBaseAddr;
    Handle->Dev->NandBaseAddr = CfgPtr->NandBaseAddr;
	Handle->NandInfo = &AL_NAND_InfoInstance[CfgPtr->DeviceId];

	// AlSmc_Dev_InitCyclesAndMemWidth(Handle->Dev);
	printf("ALSmc_Dev_Reset\r\n");
	ret = ALSmc_Dev_Reset(Handle->NandInfo);
	if(AL_OK != ret) {
		goto HAL_INIT_END;
	}
	printf("ALSmc_Dev_ReadId\r\n");
	ALSmc_Dev_ReadId(Handle->NandInfo);

	printf("NandFlash ID:0x%x", Handle->NandInfo->Size.DeviceId[0]);
	printf(" 0x%x", Handle->NandInfo->Size.DeviceId[1]);
	printf(" 0x%x", Handle->NandInfo->Size.DeviceId[2]);
	printf(" 0x%x", Handle->NandInfo->Size.DeviceId[3]);
	printf(" 0x%x\r\n", Handle->NandInfo->Size.DeviceId[4]);

	printf("ALSmc_Dev_ReadParam\r\n");
	ret = ALSmc_Dev_ReadParam(Handle->NandInfo);
	if(AL_OK != ret) {
		goto HAL_INIT_END;
	}

	printf("SpareBytesPerPage: %d\r\n", Handle->NandInfo->Size.SpareBytesPerPage);
	printf("DataBytesPerPage: %d\r\n", Handle->NandInfo->Size.DataBytesPerPage);
	printf("PagesPerBlock: %d\r\n", Handle->NandInfo->Size.PagesPerBlock);
	printf("BlocksPerUnit: %d\r\n", Handle->NandInfo->Size.BlocksPerUnit);
	printf("TotalUnit: %d\r\n", Handle->NandInfo->Size.TotalUnit);
	printf("EccNum: %d\r\n", Handle->NandInfo->Size.EccNum);

	if (Handle->NandInfo->Size.EccNum == 1) {
		ret = AlSmc_Dev_EccHwInit(Handle->Dev, Handle->NandInfo);
		if(AL_OK != ret) {
			goto HAL_INIT_END;
		}
	} else {
		ret = AlSmc_Dev_EccHwDisable(Handle->Dev);
		if(AL_OK != ret) {
			goto HAL_INIT_END;
		}

		ret = AlSmc_Dev_EnableOnDieEcc(Handle->NandInfo);
		if(AL_OK != ret) {
			goto HAL_INIT_END;
		}
	}

HAL_INIT_END:
	AL_SMC_HAL_UNLOCK(Handle);

    return ret;

}


AL_U32 AlSmc_Hal_ReadPage(AL_SMC_HalStruct *Handle, AL_U32 Offset, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_U8 Status;
	AL_U32 NumOfBytes, PartialBytes, CopyOffset, PartialPageRead, Page, Col;
    AL_U8 *Ptr = Data;
    AL_U8 *BufPtr = AL_NULL;

    Page = (Offset / Handle->NandInfo->Size.DataBytesPerPage);
    Col = Offset & (Handle->NandInfo->Size.DataBytesPerPage - 1);
	PartialBytes = Handle->NandInfo->Size.DataBytesPerPage - Col;
	NumOfBytes = (PartialBytes < Size) ? PartialBytes:Size;
	CopyOffset = Handle->NandInfo->Size.DataBytesPerPage - PartialBytes;

	/* Set SMC_REG_ECC1_MEMCMD0 Reg*/
	AlSmc_ll_SetEccCmd0(Handle->Dev->SmcBaseAddr, SMC_ECC_CMD0);

	/* Set SMC_REG_ECC1_MEMCMD1 Reg*/
	AlSmc_ll_SetEccCmd1(Handle->Dev->SmcBaseAddr, SMC_ECC_CMD1);

    while(Size) {
		// if ((Page%Handle->NandInfo->Size.PagesPerBlock)==0) {

		// 	// Status = Nand_ReadSpareBytes(Page, 0, nand);
		// 	if(Status != AL_OK)
		// 	{
		// 		return Status;
		// 	}
		// }

		/* Check if partial read */
		if (NumOfBytes < Handle->NandInfo->Size.DataBytesPerPage) {
			BufPtr = Handle->NandInfo->DataBuf;
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

		if(Status != AL_OK) {
			return Status;
		}

		if(PartialPageRead) {
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


AL_U32 AlSmc_Hal_WritePage(AL_SMC_HalStruct *Handle, AL_U32 Offset, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_U8 Status;
	AL_U32 NumOfBytes, PartialBytes, CopyOffset, Page, Col;
    AL_U8 *Ptr = Data;
    AL_U8 *BufPtr = AL_NULL;

	/* Check Nand Status */
	Status = ALSmc_Dev_ReadStatus(Handle->NandInfo);
	if (!(Status & ONFI_STATUS_WP)) {
		// return FAILED;
		while (1);
	}

	memset(Handle->NandInfo->SpareBuf, 0xff, Handle->NandInfo->Size.SpareBytesPerPage);

    Page = (Offset / Handle->NandInfo->Size.DataBytesPerPage);
    Col = Offset & (Handle->NandInfo->Size.DataBytesPerPage - 1);
	PartialBytes = Handle->NandInfo->Size.DataBytesPerPage - Col;
	NumOfBytes = (PartialBytes < Size) ? PartialBytes:Size;
	CopyOffset = Handle->NandInfo->Size.DataBytesPerPage - PartialBytes;

    while(Size) {
		// if ((Page%Handle->NandInfo->Size.PagesPerBlock)==0) {
		// 	// Status = Nand_ReadSpareBytes(Page, 0, nand);
		// 	if(Status != AL_OK) {
		// 		return Status;
		// 	}
		// }

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

		if(Status != AL_OK) {
			return Status;
		}

		Ptr += NumOfBytes;
		Size -= NumOfBytes;
		Page++;
		NumOfBytes = (Size > Handle->NandInfo->Size.DataBytesPerPage) ? Handle->NandInfo->Size.DataBytesPerPage : Size;
		CopyOffset = 0;
	}

}


AL_U32 AlSmc_Hal_ReadSpare(AL_SMC_HalStruct *Handle, AL_U32 Page, AL_U8 *Data, AL_U32 Timeout)
{
    AL_U8 Status;
    Status = ALSmc_Dev_ReadSpare(Handle->Dev, Handle->NandInfo, Page);
    return Status;
}


AL_U32 AlSmc_Hal_WriteSpare(AL_SMC_HalStruct *Handle, AL_U32 Page, AL_U8 *Data, AL_U32 Timeout)
{
   	AL_U8 Status;
    Status = ALSmc_Dev_WriteSpare(Handle->Dev, Handle->NandInfo, Page);
    return Status;
}


AL_U32 AlSmc_Hal_EraseBlock(AL_SMC_HalStruct *Handle, AL_U32 Page, AL_U32 Timeout)
{
	AL_U8 Status;
    Status = ALSmc_Dev_EraseBlock(Handle->Dev, Handle->NandInfo, Page);
	return Status;
}




