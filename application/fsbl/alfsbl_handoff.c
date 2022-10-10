/*
 * alfsbl_handoff.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#include <stdio.h>
#include "demosoc.h"
#include "alfsbl_hw.h"
#include "alfsbl_handoff.h"

void AlFsbl_HandoffExit(uint64_t HandoffAddress)
{
	//printf("Exit from FSBL\r\n");
#if __riscv
	__asm__ __volatile__(
	"jr %[src]"
	:
	:[src]"r"(HandoffAddress)
	);
#else
	__asm__ __volatile__("mov x30, %0"::"Jr"(HandoffAddress):);/* move the destination address into x30 register */
	__asm__ __volatile__("tlbi ALLE3":::);/* invalidate All E3 translation tables */
	__asm__ __volatile__("ic IALLU":::);/* invalidate I Cache All to PoU, Inner Shareable */
	__asm__ __volatile__("dsb sy":::);
	__asm__ __volatile__("isb" : : : "memory");/* make sure it completes */
	__asm__ __volatile__("mrs x5, SCTLR_EL3"::: "memory");/* Read control register */
	__asm__ __volatile__("mov x6, #0x1005":::);/* D, I , M bits disable */
	__asm__ __volatile__("bic x5, x5, x6":::);/* Disable MMU, L1 and L2 I/D cache */
	__asm__ __volatile__("msr SCTLR_EL3, x5"::: "memory");
	__asm__ __volatile__("isb" : : : "memory");
	//__asm__ __volatile__("msr CurrentEL, #0xc":::);
	__asm__ __volatile__("br x30":::);
#endif
}


uint32_t AlFsbl_Handoff(const AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	uint32_t HandoffIdx;
	uint32_t RunningCpu;
	uint32_t CpuSettings;
	uint64_t HandoffAddress;

	RunningCpu = FsblInstancePtr->ProcessorID;

	printf("Mark FSBL is completed...");
	REG32(SYSCTRL_S_FSBL_ERR_CODE) = ALFSBL_COMPLETED;

	/// remove isolation of ps-pl cross interface
	REG32(CRP_ISO_CTRL) = REG32(CRP_ISO_CTRL) & (~CRP_ISO_CTRL_MSK_PL_OTHER_IN);

	/// disable pcap to restore pcap-pl isolation
	REG32(CSU_PCAP_ENABLE) = 0;

	for(HandoffIdx = 0; HandoffIdx < FsblInstancePtr->HandoffCpuNum; HandoffIdx++) {
		CpuSettings = FsblInstancePtr->HandoffValues[HandoffIdx].CpuSettings;
		HandoffAddress = FsblInstancePtr->HandoffValues[HandoffIdx].HandoffAddress;
		//printf("handoff cpu : %08x\r\n", FsblInstancePtr->HandoffValues[HandoffIdx].CpuSettings);
		//printf("handoff addr: %08x\r\n", FsblInstancePtr->HandoffValues[HandoffIdx].HandoffAddress);

		printf("handoff cpu : %08x\r\n", CpuSettings);
		printf("handoff addr: %08x\r\n", HandoffAddress);

		if(RunningCpu != CpuSettings) {
			/// handoff to a different cpu
			/// update reset vector
			/// soft reset the handoff target cpu
		}
		else {
			AlFsbl_HandoffExit(HandoffAddress);
		}
	}

END:
	return Status;
}
