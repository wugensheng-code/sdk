#include <stdio.h>
#include "demosoc.h"
#include "alfsbl_hw.h"
#include "alfsbl_handoff.h"

#define AARCH_64

void __attribute__((noinline)) AlFsbl_HandoffExit(uint64_t HandoffAddress)
{
	//printf("Exit from FSBL\r\n");
#if __riscv
	__asm__ __volatile__(
	"jr %[src]"
	:
	:[src]"r"(HandoffAddress)
	);

#elif (defined AARCH_64 || defined aarch64)
	__asm__ __volatile__("mov x30, %0"::"r"(HandoffAddress):"x30");/* move the destination address into x30 register */
	// __asm__ __volatile__("tlbi ALLE3":::);/* invalidate All E3 translation tables */
	// __asm__ __volatile__("ic IALLU":::);/* invalidate I Cache All to PoU, Inner Shareable */
	// __asm__ __volatile__("dsb sy":::);
	// __asm__ __volatile__("isb" : : : "memory");/* make sure it completes */
	// __asm__ __volatile__("mrs x5, SCTLR_EL3"::: "memory", "x5");/* Read control register */
	// __asm__ __volatile__("mov x6, #0x1005":::"x6");/* D, I , M bits disable */
	// __asm__ __volatile__("bic x5, x5, x6":::"x5", "x6");/* Disable MMU, L1 and L2 I/D cache */
	// __asm__ __volatile__("msr SCTLR_EL3, x5"::: "memory", "x5");
	// __asm__ __volatile__("isb" : : : "memory");
	//__asm__ __volatile__("msr CurrentEL, #0xc":::);
	__asm__ __volatile__("br x30":::"x30");

#else
	__asm__ __volatile__("mov x30, %0"::"r"(HandoffAddress):"x30");/* move the destination address into x30 register */
	__asm__ __volatile__("MSR      SCTLR_EL2, XZR":::"memory");
	__asm__ __volatile__("MSR      SCTLR_EL1, XZR":::"memory");
	__asm__ __volatile__("MSR      SCR_EL3, XZR":::"memory");	/* AArch32 */
	//__asm__ __volatile__("LDR      x1, x30":::"x1","x30");
	__asm__ __volatile__("MSR      ELR_EL3, x30":::"x30", "memory");
	__asm__ __volatile__("MOV      x1, 0x13":::"x1");
	__asm__ __volatile__("msr spsr_el3, x1":::"x1", "memory");
	__asm__ __volatile__("eret":::);
	__asm__ __volatile__("br x30":::"x30");

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

	printf("Mark FSBL is completed...\r\n");
	REG32(SYSCTRL_S_FSBL_ERR_CODE) = ALFSBL_COMPLETED;

	/// remove isolation of ps-pl cross interface
	REG32(CRP_ISO_CTRL) = REG32(CRP_ISO_CTRL) & (~CRP_ISO_CTRL_MSK_PL_OTHER_IN);

	/// disable pcap to restore pcap-pl isolation
	REG32(CSU_PCAP_ENABLE) = 0;

	/// release reset of gp0m, gp1m, hp0, hp1
	REG32(CRP_SRST_CTRL2) = REG32(CRP_SRST_CTRL2) | 0x33;

	/// gp normal access to pl, gp port and fahb port
	REG32(SYSCTRL_NS_PLS_PROT) = REG32(SYSCTRL_NS_PLS_PROT) & (~0x3);

	if(FsblInstancePtr->PrimaryBootDevice == ALFSBL_BOOTMODE_JTAG) {
		printf("jump to a infinite loop\r\n");
		REG32(0x60000000) = 0xa001a001;
		AlFsbl_HandoffExit(0x60000000);
	}

	for(HandoffIdx = 0; HandoffIdx < FsblInstancePtr->HandoffCpuNum; HandoffIdx++) {
		CpuSettings = FsblInstancePtr->HandoffValues[HandoffIdx].CpuSettings;
		HandoffAddress = FsblInstancePtr->HandoffValues[HandoffIdx].HandoffAddress;
		printf("handoff cpu : %08x\r\n", FsblInstancePtr->HandoffValues[HandoffIdx].CpuSettings);
		printf("handoff addr: %08x\r\n", FsblInstancePtr->HandoffValues[HandoffIdx].HandoffAddress);

		if(RunningCpu != CpuSettings) {
			printf("hand off different cpu\r\n");
			/// handoff to a different cpu
			/// update reset vector
			/// soft reset the handoff target cpu, pulse reset
			if(CpuSettings == ALIH_PH_ATTRIB_DEST_CPU_RPU) {
				REG32(SYSCTRL_S_RPU_RESET_VECTOR_H) = HandoffAddress >> 32;
				REG32(SYSCTRL_S_RPU_RESET_VECTOR_L) = HandoffAddress & 0xffffffff;
				REG32(SYSCTRL_S_XPU_SRST) = REG32(SYSCTRL_S_XPU_SRST) | (1UL << 10);  /// release level reset
				REG32(SYSCTRL_S_XPU_SRST) = REG32(SYSCTRL_S_XPU_SRST) & (~(1 << 8));  /// trigger pulse reset
			}
			else if(CpuSettings == ALIH_PH_ATTRIB_DEST_CPU_APU0) {
				REG32(SYSCTRL_S_APU0_RESET_VECTOR_H) = HandoffAddress >> 34;
				REG32(SYSCTRL_S_APU0_RESET_VECTOR_L) = (HandoffAddress >> 2) & 0xffffffff;
				REG32(SYSCTRL_S_XPU_SRST) = REG32(SYSCTRL_S_XPU_SRST) | (1UL << 4);   /// release level reset
				REG32(SYSCTRL_S_XPU_SRST) = REG32(SYSCTRL_S_XPU_SRST) & (~(1 << 0));  /// trigger pulse reset
			}
			else if(CpuSettings == ALIH_PH_ATTRIB_DEST_CPU_APU1) {
				REG32(SYSCTRL_S_APU1_RESET_VECTOR_H) = HandoffAddress >> 34;
				REG32(SYSCTRL_S_APU1_RESET_VECTOR_L) = (HandoffAddress >> 2) & 0xffffffff;
				REG32(SYSCTRL_S_XPU_SRST) = REG32(SYSCTRL_S_XPU_SRST) | (1UL << 5);   /// release level reset
				REG32(SYSCTRL_S_XPU_SRST) = REG32(SYSCTRL_S_XPU_SRST) & (~(1 << 1));  /// trigger pulse reset
			}
		}
		else {
			if(RunningCpu == ALIH_PH_ATTRIB_DEST_CPU_RPU) {
				REG32(SYSCTRL_S_RPU_RESET_VECTOR_H) = HandoffAddress >> 32;
				REG32(SYSCTRL_S_RPU_RESET_VECTOR_L) = HandoffAddress & 0xffffffff;
			}
			else if(RunningCpu == ALIH_PH_ATTRIB_DEST_CPU_APU0) {
				REG32(SYSCTRL_S_APU0_RESET_VECTOR_H) = HandoffAddress >> 34;
				REG32(SYSCTRL_S_APU0_RESET_VECTOR_L) = (HandoffAddress >> 2) & 0xffffffff;
			}
			else if(RunningCpu == ALIH_PH_ATTRIB_DEST_CPU_APU1) {
				REG32(SYSCTRL_S_APU1_RESET_VECTOR_H) = HandoffAddress >> 34;
				REG32(SYSCTRL_S_APU1_RESET_VECTOR_L) = (HandoffAddress >> 2) & 0xffffffff;
			}
			AlFsbl_HandoffExit(HandoffAddress);
		}
	}

END:
	return Status;
}
