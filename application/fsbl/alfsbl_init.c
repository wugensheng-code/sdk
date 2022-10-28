/*
 * alfsbl_init.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#include "alfsbl_hw.h"
#include "alfsbl_init.h"
#include "demosoc.h"
#include <stdio.h>

#if __riscv
#include "core_feature_eclic.h"
#else
#include "gic_v3_addr.h"
#endif

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

//	// set PCAP not enable, to make the signal to config model not change
//	REG32(CSU_PCAP_ENABLE) = 0;
//
//	// reset pl, release reset before pl bitstream config
//	if(FsblInstancePtr->ResetReason == FSBL_SYSTEM_RESET) {
//		REG32(SYSCTRL_S_GLOBAL_SRSTN) = REG32(SYSCTRL_S_GLOBAL_SRSTN) & (~SYSCTRL_S_GLOBAL_SRSTN_MSK_GLB_PL_SRST);
//	}




	Status = ALFSBL_SUCCESS;

	return Status;
}




static void AlFsbl_ClearPendingInterrupt(void)
{
	
#if __riscv
	IRQn_Type IrqNum;
	printf("Clear all pending interrupts of rpu...\r\n");
	for(IrqNum = Reserved0_IRQn; IrqNum < SOC_INT_MAX; IrqNum++) {
		__ECLIC_ClearPendingIRQ(IrqNum);	//risc-v
	}
#else	//[MODIFY]:1
	//uint32_t RegVal = 0;
	printf("Clear all pending interrupts of apu...\r\n");
	//RegVal = (*(volatile uint32_t *)(uint32_t)(+4*i));
	/*for(int i = 0; i < 4; i++){
		(*(volatile uint32_t *)(uint32_t)(GICD_CPENDSGIR+i)) = 0xFFFFFFFF;
	}*/

	REG32(GICD_ICENABLER) = ~0;
	REG32(GICD_ICPENDR) = ~0;
	REG32(GICD_ACTIVE_CLEAR) = ~0;

	REG32(GICD_ICENABLER+4) = ~0;
	REG32(GICD_ICPENDR+4) = ~0;
	REG32(GICD_ACTIVE_CLEAR+4) = ~0;

	REG32(GICD_ICENABLER+8) = ~0;
	REG32(GICD_ICPENDR+8) = ~0;
	REG32(GICD_ACTIVE_CLEAR+8) = ~0;

	REG32(GICD_ICENABLER+12) = ~0;
	REG32(GICD_ICPENDR+12) = ~0;
	REG32(GICD_ACTIVE_CLEAR+12) = ~0;

	REG32(GICD_ICENABLER+16) = ~0;
	REG32(GICD_ICPENDR+16) = ~0;
	REG32(GICD_ACTIVE_CLEAR+16) = ~0;

	REG32(GICD_ICENABLER+20) = ~0;
	REG32(GICD_ICPENDR+20) = ~0;
	REG32(GICD_ACTIVE_CLEAR+20) = ~0;
#endif
	return;
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
		printf("mark fsbl is running...\r\n");
		REG32(SYSCTRL_S_FSBL_ERR_CODE) = ALFSBL_RUNNING;
	}

	Status = ALFSBL_SUCCESS;

END:
	return Status;
}



