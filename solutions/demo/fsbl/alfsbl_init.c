/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "al_reg_io.h"
#include "alfsbl_hw.h"
#include "alfsbl_init.h"
#include "alfsbl_config.h"
#include "al_core.h"
#include "al_utils_def.h"
#include "al_wdt_hal.h"


static uint32_t AlFsbl_GetResetReason(void);
static uint32_t AlFsbl_SystemInit(AlFsblInfo *FsblInstancePtr);
static uint32_t AlFsbl_ProcessorInit(AlFsblInfo *FsblInstancePtr);
static uint32_t AlFsbl_TcmInit(AlFsblInfo *FsblInstancePtr);
static uint32_t AlFsbl_PmuReset(AlFsblInfo *FsblInstancePtr);
#ifndef ALFSBL_PMU_EXCLUDE
static uint32_t AlFsbl_PmuInit(AlFsblInfo *FsblInstancePtr);
#endif
static uint32_t AlFsbl_ValidateResetReason(void);

#ifndef ALFSBL_WDT_EXCLUDE
void AlWdt_Hal_FsblEventHandler();
static uint32_t AlFsbl_WdtInit(void);
static AL_WDT_InitStruct FSBL_WDT_Init = {
	.ResetPuaseLength = WDT_RPL_PCLK_CYCLES_8,
	.ResponseMode     = WDT_INTR_MODE,
	.TimeOutValue     = WDT_TIMEOUT_PERIOD_2G_CLOCKS,
};
AL_WDT_HalStruct *alfsbl_wdt0;
#endif


uint32_t AlFsbl_Initialize(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	FsblInstancePtr->ResetReason = AlFsbl_GetResetReason();

	/// system init: peripherals, watch dog
	Status = AlFsbl_SystemInit(FsblInstancePtr);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	AlFsbl_PmuReset(FsblInstancePtr);

	/// clear pending interrupt
	AlIntr_ClearAllPending();

	/// processor init
	/// to get running cpu ID and its running status
	Status = AlFsbl_ProcessorInit(FsblInstancePtr);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

#ifndef ALFSBL_PMU_EXCLUDE
	Status = AlFsbl_PmuInit(FsblInstancePtr);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}
#endif

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

#ifndef ALFSBL_WDT_EXCLUDE
	/// init wdt
	Status = AlFsbl_WdtInit();
#endif

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

	/// close pl-ps bus connections, hp and gpm bus
	AL_REG32_SET_BITS(CRP_SRST_CTRL2, 0, 2, 0);
	AL_REG32_SET_BITS(CRP_SRST_CTRL2, 4, 2, 0);

	/// close pl-ps bus connections, fahb and gps bus
	AL_REG32_SET_BITS(SYSCTRL_NS_PLS_PROT, 0, 2, 3);

	/// close apu acp bus connections
	AL_REG32_SET_BITS(CRP_SRST_CTRL0, 8, 1, 0);
	AL_REG32_SET_BITS(APU_CTRL_AINACTS, 0, 1, 1);

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


static uint32_t AlFsbl_PmuReset(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	AL_LOG(AL_LOG_LEVEL_INFO, "PMU Error Config Init\r\n");
	uint32_t val = 0;

	val = AL_REG32_READ(SYSCTRL_S_ERR_HW_EN0_SET);
	AL_REG32_WRITE(SYSCTRL_S_ERR_HW_EN0_CLR, val);

	val = AL_REG32_READ(SYSCTRL_S_INT_EN0_SET);
	AL_REG32_WRITE(SYSCTRL_S_INT_EN0_CLR, val);

	val = AL_REG32_READ(SYSCTRL_S_ERR_HW_EN1_SET);
	AL_REG32_WRITE(SYSCTRL_S_ERR_HW_EN1_CLR, val);

	val = AL_REG32_READ(SYSCTRL_S_INT_EN1_SET);
	AL_REG32_WRITE(SYSCTRL_S_INT_EN1_CLR, val);

	val = AL_REG32_READ(SYSCTRL_S_RAW_HIS1);
	AL_REG32_WRITE(SYSCTRL_S_RAW_HIS1, val);

	return Status;
}

#ifndef ALFSBL_PMU_EXCLUDE
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
#if 0
		val = AL_REG32_READ(SYSCTRL_S_ERR_HW_EN1_SET) |
				SYSCTRL_S_ERR_HW1_MSK_PLL2_LOCK |
				SYSCTRL_S_ERR_HW1_MSK_PLL1_LOCK |
				SYSCTRL_S_ERR_HW1_MSK_PLL0_LOCK;
		AL_REG32_WRITE(SYSCTRL_S_ERR_HW_EN1_SET, val);

		val = AL_REG32_READ(SYSCTRL_S_INT_EN1_SET) |
				SYSCTRL_S_ERR_HW1_MSK_PLL2_LOCK |
				SYSCTRL_S_ERR_HW1_MSK_PLL1_LOCK |
				SYSCTRL_S_ERR_HW1_MSK_PLL0_LOCK;
		AL_REG32_WRITE(SYSCTRL_S_INT_EN1_SET, val);
#endif
	}

	return Status;
}
#endif

static uint32_t AlFsbl_TcmInit(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	/// todo

	return Status;
}


#ifndef ALFSBL_WDT_EXCLUDE
void AlWdt_Hal_FsblEventHandler()
{
	AL_LOG(AL_LOG_LEVEL_DEBUG, "wdt int\r\n");
	AlWdt_ll_ClearIntr(alfsbl_wdt0->BaseAddr);
	return;
}

static uint32_t AlFsbl_WdtInit(void)
{
	uint32_t Status = ALFSBL_SUCCESS;
	Status = AlWdt_Hal_Init(&alfsbl_wdt0, 0, &FSBL_WDT_Init, AlWdt_Hal_FsblEventHandler);
	if(Status != ALFSBL_SUCCESS) {
		AL_LOG(AL_LOG_LEVEL_ERROR, "WDT initialize error： 0x%08x\r\n", Status);
		return ALFSBL_ERROR_WDT_INIT_ERR;
	}	

	return ALFSBL_SUCCESS;
}
#endif


static uint32_t AlFsbl_ValidateResetReason(void)
{
	uint32_t Status = ALFSBL_SUCCESS;
	uint32_t FsblStatus;
	uint32_t ResetReasonValue;
	uint32_t RawHis0Value;
	uint32_t RawHis1Value;


	/// get fsbl status
	FsblStatus = AL_REG32_READ(SYSCTRL_S_FSBL_ERR_CODE);

	/// get reset reason
	ResetReasonValue = AL_REG32_READ(CRP_RST_REASON);
	RawHis0Value     = AL_REG32_READ(SYSCTRL_S_RAW_HIS0);
	RawHis1Value     = AL_REG32_READ(SYSCTRL_S_RAW_HIS1);

	/// print history record of reset reason and pmu status
	AL_LOG(AL_LOG_LEVEL_INFO, "history reset reason: %08x\r\n", ResetReasonValue);
	AL_LOG(AL_LOG_LEVEL_INFO, "history pmu status 0: %08x\r\n", RawHis0Value);
	AL_LOG(AL_LOG_LEVEL_INFO, "history pmu status 1: %08x\r\n", RawHis1Value);


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



