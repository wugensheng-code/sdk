/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "al_reg_io.h"
#include "alfsbl_hw.h"
#include "alfsbl_init.h"
#include "al_core.h"
#include "al_utils_def.h"

extern uint8_t  DdrAvailable;

static uint32_t AlFsbl_GetResetReason(void);
static uint32_t AlFsbl_SystemInit(AlFsblInfo *FsblInstancePtr);
static uint32_t AlFsbl_ProcessorInit(AlFsblInfo *FsblInstancePtr);
static uint32_t AlFsbl_TcmInit(AlFsblInfo *FsblInstancePtr);
static uint32_t AlFsbl_PmuInit(AlFsblInfo *FsblInstancePtr);
static uint32_t AlFsbl_WdtInit(AlFsblInfo *FsblInstancePtr);
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
	AlIntr_ClearAllPending();

	/// processor init
	/// to get running cpu ID and its running status
	Status = AlFsbl_ProcessorInit(FsblInstancePtr);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	Status = AlFsbl_PmuInit(FsblInstancePtr);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	/// tcm ecc init if required
	Status = AlFsbl_TcmInit(FsblInstancePtr);
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

	if((AL_REG32_READ(SYSCTRL_S_GLOBAL_SRSTN)) & SYSCTRL_S_GLOBAL_SRSTN_MSK_PSONLY) {
		AL_LOG(AL_LOG_LEVEL_INFO, "PS only reset\r\n");
		Ret = FSBL_PS_ONLY_RESET;
	}
	else {
		AL_LOG(AL_LOG_LEVEL_INFO, "System reset\r\n");
		Ret = FSBL_SYSTEM_RESET;
	}

	return Ret;
}




static uint32_t AlFsbl_SystemInit(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = 0;

	// reset pl
	if(FsblInstancePtr->ResetReason == FSBL_SYSTEM_RESET) {
		AL_REG32_SET_BIT(SYSCTRL_S_GLOBAL_SRSTN, 8, 0);
		AL_REG32_SET_BIT(SYSCTRL_S_GLOBAL_SRSTN, 8, 1);
	}

	Status = ALFSBL_SUCCESS;

	return Status;
}


static uint32_t AlFsbl_ProcessorInit(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;

	/// a temporary solution
#if __riscv
	FsblInstancePtr->ProcessorID = ALIH_PH_ATTRIB_DEST_CPU_RPU;
#else
	FsblInstancePtr->ProcessorID = ALIH_PH_ATTRIB_DEST_CPU_APU0;	//[MODIFY]:2
#endif
	return Status;
}


static uint32_t AlFsbl_PmuInit(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	AL_LOG(AL_LOG_LEVEL_INFO, "PMU Error Config Init\r\n");
	uint32_t val = 0;

	val = AL_REG32_READ(SYSCTRL_S_ERR_HW_EN0_SET)   |
			      SYSCTRL_S_ERR_HW0_MSK_BUS_TIMEOUT |
				  SYSCTRL_S_ERR_HW0_MSK_WDT0        |
				  SYSCTRL_S_ERR_HW0_MSK_OCM_ECC;
	AL_REG32_WRITE(SYSCTRL_S_ERR_HW_EN0_SET, val);

	val = AL_REG32_READ(SYSCTRL_S_INT_EN0_SET)      |
			      SYSCTRL_S_ERR_HW0_MSK_BUS_TIMEOUT |
				  SYSCTRL_S_ERR_HW0_MSK_WDT0        |
				  SYSCTRL_S_ERR_HW0_MSK_OCM_ECC;
	AL_REG32_WRITE(SYSCTRL_S_INT_EN0_SET, val);


	if((AL_REG32_READ(CRP_CLK_SEL) & CRP_CLK_SEL_MSK_SLOW_SEL) == CRP_CLK_SEL_MSK_SLOW_SEL) {
		/// pll bypassed
	}
	else {
		/// pll enabled
		// temp comment, because baremetal will config pll in board init interface
		// REG32(SYSCTRL_S_ERR_HW_EN1_SET) = REG32(SYSCTRL_S_ERR_HW_EN1_SET)     |
		// 		                          SYSCTRL_S_ERR_HW1_MSK_CSU_PLL1_LOCK |
		// 		                          SYSCTRL_S_ERR_HW1_MSK_CSU_PLL0_LOCK;

		// REG32(SYSCTRL_S_INT_EN1_SET) = REG32(SYSCTRL_S_INT_EN1_SET)        |
		// 		                       SYSCTRL_S_ERR_HW1_MSK_CSU_PLL1_LOCK |
		// 		                       SYSCTRL_S_ERR_HW1_MSK_CSU_PLL0_LOCK;
	}



	return Status;
}



static uint32_t AlFsbl_TcmInit(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	/// todo

	return Status;
}


static uint32_t AlFsbl_WdtInit(AlFsblInfo *FsblInstancePtr)
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
	FsblStatus = AL_REG32_READ(SYSCTRL_S_FSBL_ERR_CODE);

	/// get reset reason
	ResetReasonValue = AL_REG32_READ(CRP_RST_REASON);

	if((ResetReasonValue & CRP_RST_REASON_MSK_SWDT0)   |
	   (ResetReasonValue & CRP_RST_REASON_MSK_SWDT1)   |
	   (ResetReasonValue & CRP_RST_REASON_MSK_SWDT2)) {
		if(FsblStatus == ALFSBL_RUNNING) {
			Status = ALFSBL_ERR_SYS_WDT_RESET;
			goto END;
		}
	}
	else if(ResetReasonValue & CRP_RST_REASON_MSK_PMU_ERR) {
		if(FsblStatus == ALFSBL_RUNNING) {
			Status = ALFSBL_ERR_PMU_ERR_RESET;
			goto END;
		}
	}
	if(FsblStatus != ALFSBL_RUNNING) {
		AL_LOG(AL_LOG_LEVEL_INFO, "mark fsbl is running...\r\n");
		AL_REG32_WRITE(SYSCTRL_S_FSBL_ERR_CODE, ALFSBL_RUNNING);
	}

	/// clear reset reason
	AL_REG32_WRITE(CRP_RST_REASON, ResetReasonValue);
	Status = ALFSBL_SUCCESS;

END:
	return Status;
}



