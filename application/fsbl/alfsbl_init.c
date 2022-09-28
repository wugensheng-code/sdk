/*
 * alfsbl_init.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#include "alfsbl_hw.h"
#include "alfsbl_init.h"
#include "demosoc.h"
#include "core_feature_eclic.h"
#include <stdio.h>

static uint32_t AlFsbl_GetResetReason(void);
static uint32_t AlFsbl_SystemInit(AlFsblInfo *FsblInstancePtr);
static void     AlFsbl_ClearPendingInterrupt(void);
static uint32_t AlFsbl_ProcessorInit(AlFsblInfo *FsblInstancePtr);
static uint32_t AlFsbl_TcmInit(AlFsblInfo *FsblInstancePtr);
static uint32_t AlFsbl_DdrInit(void);
static uint32_t AlFsbl_ValidateResetReason(void);


uint32_t AlFsbl_Initialize(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	FsblInstancePtr->ResetReason = AlFsbl_GetResetReason();

	/// system init: peripherals, watch dog
	Status = AlFsbl_SystemInit(FsblInstancePtr);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	/// clear pending interrupt
	printf("Clear all pending interrupts of rpu...\n");
	AlFsbl_ClearPendingInterrupt();

	/// processor init
	/// to get running cpu ID and its running status
	Status = AlFsbl_ProcessorInit(FsblInstancePtr);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	/// tcm ecc init if required
	Status = AlFsbl_TcmInit(FsblInstancePtr);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	/// ddr ecc init if required
	Status = AlFsbl_DdrInit();
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	/// reset reason validation
	Status = AlFsbl_ValidateResetReason();
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	/// fsbl info data init
	FsblInstancePtr->HandoffCpuNum = 0;

END:
	return Status;
}



static uint32_t AlFsbl_GetResetReason(void)
{
	uint32_t Ret;

	if((REG32(SYSCTRL_S_GLOBAL_SRSTN)) & SYSCTRL_S_GLOBAL_SRSTN_MSK_PSONLY) {
		Ret = FSBL_PS_ONLY_RESET;
	}
	else {
		Ret = FSBL_SYSTEM_RESET;
	}

	return Ret;
}




static uint32_t AlFsbl_SystemInit(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = 0;

	// isolate PL-PS cross interface
	// make sure FSBL exits with isolation removed
	REG32(CRP_ISO_CTRL) = REG32(CRP_ISO_CTRL) | CRP_ISO_CTRL_MSK_PL_OTHER_IN;

	// set PCAP not enable, to make the signal to config model not change
	REG32(CSU_PCAP_ENABLE) = 0;

	// reset pl, release reset before pl bitstream config
	if(FsblInstancePtr->ResetReason == FSBL_SYSTEM_RESET) {

		REG32(SYSCTRL_S_GLOBAL_SRSTN) = REG32(SYSCTRL_S_GLOBAL_SRSTN) & (~SYSCTRL_S_GLOBAL_SRSTN_MSK_GLB_PL_SRST);
	}




	Status = ALFSBL_SUCCESS;

	return Status;
}




static void AlFsbl_ClearPendingInterrupt(void)
{
	IRQn_Type IrqNum;
	for(IrqNum = Reserved0_IRQn; IrqNum < SOC_INT_MAX; IrqNum++) {
		__ECLIC_ClearPendingIRQ(IrqNum);
	}


	return;
}


static uint32_t AlFsbl_ProcessorInit(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;

	/// a temporary solution
	FsblInstancePtr->ProcessorID = ALIH_PH_ATTRIB_DEST_CPU_RPU;

	return Status;
}


static uint32_t AlFsbl_TcmInit(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	/// todo

	return Status;
}


static uint32_t AlFsbl_DdrInit(void)
{
	uint32_t Status = ALFSBL_SUCCESS;
	/// todo

	return Status;
}


static uint32_t AlFsbl_ValidateResetReason(void)
{
	uint32_t Status = ALFSBL_SUCCESS;
	uint32_t FsblStatus;
	uint32_t ResetReasonValue;

	/// get fsbl status
	FsblStatus = REG32(SYSCTRL_S_FSBL_ERR_CODE);

	/// get reset reason
	ResetReasonValue = REG32(CRP_RST_REASON);

	if((ResetReasonValue & CRP_RST_REASON_MSK_SWDT0)   |
	   (ResetReasonValue & CRP_RST_REASON_MSK_SWDT1)   |
	   (ResetReasonValue & CRP_RST_REASON_MSK_SWDT2)) {
		if(FsblStatus == ALFSBL_RUNNING) {
			Status = ALFSBL_ERR_SYS_WDT_RESET;
			goto END;
		}
	}
	if(FsblStatus != ALFSBL_RUNNING) {
		printf("mark fsbl is running...\n");
		REG32(SYSCTRL_S_FSBL_ERR_CODE) = ALFSBL_RUNNING;
	}

	Status = ALFSBL_SUCCESS;

END:
	return Status;
}



