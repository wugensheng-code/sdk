/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "alfsbl_hw.h"
#include "alfsbl_handoff.h"
#include "alfsbl_config.h"
#include "al_reg_io.h"
#include "al_utils_def.h"
#include "soc_plat.h"
#include "al_wdt_hal.h"

volatile uint32_t RegVal_RAW_HIS0;
volatile uint32_t RegVal_RAW_HIS1;


#ifndef ALFSBL_WDT_EXCLUDE
extern AL_WDT_HalStruct *alfsbl_wdt0;
#endif

void __attribute__((noinline)) AlFsbl_HandoffExit(uint64_t HandoffAddress)
{
#ifdef __riscv
	__asm__ __volatile__("fence");
	__asm__ __volatile__("jr %[src]"::[src]"r"(HandoffAddress));
#elif defined __aarch64__
	__asm__ __volatile__("mov x30, %0"::"r"(HandoffAddress):"x30");/* move the destination address into x30 register */
	__asm__ __volatile__("msr daifset, 0xF"::: "memory");
	__asm__ __volatile__("isb" : : : "memory");/* make sure it completes */
	__asm__ __volatile__("tlbi ALLE3":::);/* invalidate All E3 translation tables */
	__asm__ __volatile__("ic IALLU":::);/* invalidate I Cache All to PoU, Inner Shareable */
	__asm__ __volatile__("dsb sy":::);
	__asm__ __volatile__("isb" : : : "memory");/* make sure it completes */
	__asm__ __volatile__("mrs x5, SCTLR_EL3"::: "memory", "x5");/* Read control register */
	__asm__ __volatile__("mov x6, #0x1005":::"x6");/* D, I , M bits disable */
	__asm__ __volatile__("bic x5, x5, x6":::"x5", "x6");/* Disable MMU, L1 and L2 I/D cache */
	__asm__ __volatile__("msr SCTLR_EL3, x5"::: "memory", "x5");
	__asm__ __volatile__("isb" : : : "memory");
	__asm__ __volatile__("br x30":::"x30");
#else
	__asm__ __volatile__("mov x30, %0"::"r"(HandoffAddress):"x30");/* move the destination address into x30 register */
	__asm__ __volatile__("MSR      SCTLR_EL2, XZR":::"memory");
	__asm__ __volatile__("MSR      SCTLR_EL1, XZR":::"memory");
	__asm__ __volatile__("MSR      SCR_EL3, XZR":::"memory");	/* AArch32 */
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
	uint32_t ResetReasonValue = 0;

	RunningCpu = FsblInstancePtr->ProcessorID;

	AL_LOG(AL_LOG_LEVEL_INFO, "Mark FSBL is completed...\r\n");
	AL_REG32_WRITE(SYSCTRL_S_FSBL_ERR_CODE, ALFSBL_COMPLETED);

	/// disable pcap to restore pcap-pl isolation
	AL_REG32_WRITE(CSU_PCAP_ENABLE, 0);

#ifndef ALFSBL_WDT_EXCLUDE
	/// disable wdt
	AL_LOG(AL_LOG_LEVEL_INFO, "wdt disable\r\n");
	AlWdt_ll_Pause(alfsbl_wdt0->BaseAddr, AL_FUNC_ENABLE);
	AlWdt_ll_Enable(alfsbl_wdt0->BaseAddr, AL_FUNC_DISABLE);
#endif

	/// init gp, hp, fahb, apu-acp ports between ps and pl
	Soc_PsPlInit();

	/// clear reset reason
	ResetReasonValue = AL_REG32_READ(CRP_RST_REASON);
	AL_REG32_WRITE(CRP_RST_REASON, ResetReasonValue);  /// write 1 clear

	RegVal_RAW_HIS0 = AL_REG32_READ(SYSCTRL_S_RAW_HIS0);
	AL_REG32_WRITE(SYSCTRL_S_RAW_HIS0, RegVal_RAW_HIS0);
	RegVal_RAW_HIS1 = AL_REG32_READ(SYSCTRL_S_RAW_HIS1);
	AL_REG32_WRITE(SYSCTRL_S_RAW_HIS1, RegVal_RAW_HIS1);

	if(FsblInstancePtr->PrimaryBootDevice == ALFSBL_BOOTMODE_JTAG) {
		while(1) {
			__asm__ __volatile__("wfi");
		}

	}

	for(HandoffIdx = 0; HandoffIdx < FsblInstancePtr->HandoffCpuNum; HandoffIdx++) {
		CpuSettings = FsblInstancePtr->HandoffValues[HandoffIdx].CpuSettings;
		HandoffAddress = FsblInstancePtr->HandoffValues[HandoffIdx].HandoffAddress;

		if(RunningCpu != CpuSettings) {
			/// handoff to a different cpu
			/// update reset vector
			/// soft reset the handoff target cpu, pulse reset
			if(CpuSettings == ALIH_PH_ATTRIB_DEST_CPU_RPU) {
				AL_REG32_WRITE(SYSCTRL_S_RPU_RESET_VECTOR_H, HandoffAddress >> 32);
				AL_REG32_WRITE(SYSCTRL_S_RPU_RESET_VECTOR_L, HandoffAddress & 0xffffffff);
				AL_REG32_SET_BIT(SYSCTRL_S_XPU_SRST, 10, 1);  /// release level reset
				AL_REG32_SET_BIT(SYSCTRL_S_XPU_SRST, 8, 0);   /// trigger pulse reset
			}
			else if(CpuSettings == ALIH_PH_ATTRIB_DEST_CPU_APU0) {
				AL_REG32_WRITE(SYSCTRL_S_APU0_RESET_VECTOR_H, HandoffAddress >> 34);
				AL_REG32_WRITE(SYSCTRL_S_APU0_RESET_VECTOR_L, (HandoffAddress >> 2) & 0xffffffff);
				AL_REG32_SET_BIT(SYSCTRL_S_XPU_SRST, 4, 1);   /// release level reset
				AL_REG32_SET_BIT(SYSCTRL_S_XPU_SRST, 0, 0);   /// trigger pulse reset
			}
			else if(CpuSettings == ALIH_PH_ATTRIB_DEST_CPU_APU1) {
				AL_REG32_WRITE(SYSCTRL_S_APU1_RESET_VECTOR_H, HandoffAddress >> 34);
				AL_REG32_WRITE(SYSCTRL_S_APU1_RESET_VECTOR_L, (HandoffAddress >> 2) & 0xffffffff);
				AL_REG32_SET_BIT(SYSCTRL_S_XPU_SRST, 5, 1);   /// release level reset
				AL_REG32_SET_BIT(SYSCTRL_S_XPU_SRST, 1, 0);	  /// tringger pulse reset
			}
		}
		else {
			if(RunningCpu == ALIH_PH_ATTRIB_DEST_CPU_RPU) {
				AL_REG32_WRITE(SYSCTRL_S_RPU_RESET_VECTOR_H, HandoffAddress >> 32);
				AL_REG32_WRITE(SYSCTRL_S_RPU_RESET_VECTOR_L, HandoffAddress & 0xffffffff);
			}
			else if(RunningCpu == ALIH_PH_ATTRIB_DEST_CPU_APU0) {
				AL_REG32_WRITE(SYSCTRL_S_APU0_RESET_VECTOR_H, HandoffAddress >> 34);
				AL_REG32_WRITE(SYSCTRL_S_APU0_RESET_VECTOR_L, (HandoffAddress >> 2) & 0xffffffff);
			}
			else if(RunningCpu == ALIH_PH_ATTRIB_DEST_CPU_APU1) {
				AL_REG32_WRITE(SYSCTRL_S_APU1_RESET_VECTOR_H, HandoffAddress >> 34);
				AL_REG32_WRITE(SYSCTRL_S_APU1_RESET_VECTOR_L, (HandoffAddress >> 2) & 0xffffffff);

			}
			AlFsbl_HandoffExit(HandoffAddress);
		}
	}

	return Status;
}
