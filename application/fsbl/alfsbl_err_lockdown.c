/*
 * alfsbl_err_lockdown.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#include "demosoc.h"
#include "alfsbl_hw.h"
#include "alfsbl_err_lockdown.h"
#include "alfsbl_boot.h"

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
	uint32_t LoopAddr = 0x6101fffc;

	/// update error status register
	REG32(SYSCTRL_S_FSBL_ERR_CODE) = ErrorStatus;

	if(BootMode == ALFSBL_BOOTMODE_JTAG) {
		/// JTAG BOOT MODE not support multi-boot, jump to a infinite loop
		REG32(LoopAddr) = 0xa001a001;
		__asm__ __volatile__(
		"jr %[src]"
		:
		:[src]"r"(LoopAddr)
		);
	}
	else {
		/// Multi-boot: update MULTBOOT register
		REG32(SYSCTRL_S_MULTI_BOOT) = REG32(SYSCTRL_S_MULTI_BOOT) + 1;
		/// trigger a system reset
		REG32(SYSCTRL_S_GLOBAL_SRSTN) = REG32(SYSCTRL_S_GLOBAL_SRSTN) & (~(1 << 9));
		REG32(SYSCTRL_S_GLOBAL_SRSTN) = REG32(SYSCTRL_S_GLOBAL_SRSTN) & (~1);
	}



	return;
}
