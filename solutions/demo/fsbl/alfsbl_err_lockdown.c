/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "alfsbl_hw.h"
#include "alfsbl_err_lockdown.h"
#include "alfsbl_boot.h"
#include "al_reg_io.h"
#include "al_utils_def.h"

/**
 * 1, update the error status register and fsbl instance structure with fsbl error code
 * 2, get boot mode and check wheather support multi-boot or not
 * 3.1 support multi-boot:
 *     stop wdt, hook before fallback(user code), update multiboot register, make sure everything completes, trigger a system reset
 * 3.2 not support multi-boot:
 *     jump to a wfe/wfi loop
 **/

void AlFsbl_ErrorLockDown(AlFsblInfo *FsblInstancePtr, uint32_t ErrorStatus)
{

	uint32_t BootMode;
	BootMode = FsblInstancePtr->PrimaryBootDevice;
	uint32_t LoopAddr = 0;

	/// update error status register
	AL_REG32_WRITE(SYSCTRL_S_FSBL_ERR_CODE, ErrorStatus);

	if(BootMode == ALFSBL_BOOTMODE_JTAG) {
		/// JTAG BOOT MODE not support multi-boot, jump to a infinite loop
#if __riscv
		LoopAddr = RPU_LOOP_ADDR;
		AL_REG32_WRITE(LoopAddr, RPU_LOOP_INSTRUCTION);
		__asm__ __volatile__(
		"jr %[src]"
		:
		:[src]"r"(LoopAddr)
		);
#else
		LoopAddr = APU_LOOP_ADDR;
		AL_REG32_WRITE(LoopAddr, APU_LOOP_INSTRUCTION);
		__asm__ __volatile__("mov x30, %0"::"r"(LoopAddr):"x30");
#endif
	}
	else {
		/// Multi-boot: update MULTBOOT register

		AL_REG32_WRITE(SYSCTRL_S_MULTI_BOOT, AL_REG32_READ(SYSCTRL_S_MULTI_BOOT) + 1);
		
		/// trigger a system reset
		//AL_REG32_SET_BIT(reg_addr, shift, value)
		AL_REG32_SET_BIT(SYSCTRL_S_GLOBAL_SRSTN, 9, 0);
		AL_REG32_SET_BIT(SYSCTRL_S_GLOBAL_SRSTN, 0, 0);
	}



	return;
}
