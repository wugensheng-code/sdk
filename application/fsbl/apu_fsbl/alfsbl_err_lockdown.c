/*
 * alfsbl_err_lockdown.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#include "demosoc.h"
#include "alfsbl_hw.h"
#include "alfsbl_err_lockdown.h"

void AlFsbl_ErrorLockDown(uint32_t ErrorStatus)
{
	/**
	 * 1, update the error status register and fsbl instance structure with fsbl error code
	 * 2, get boot mode and check wheather support multi-boot or not
	 * 3.1 support multi-boot:
	 *     stop wdt, hook before fallback(user code), update multiboot register, make sure everything completes, trigger a system reset
	 * 3.2 not support multi-boot:
	 *     jump to a wfe/wfi loop
	 **/
	REG32(SYSCTRL_S_FSBL_ERR_CODE) = ErrorStatus;
	return;
}
