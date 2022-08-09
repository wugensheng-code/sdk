/*
 *
 */

#include <stdio.h>
#include "config.h"
#include "gic.h"
#include "delay.h"
#include "io.h"
#include "type.h"
#include "timer.h"
#include "gic_v3.h"
#include "gic_v3_addr.h"


static int pmu_irq_happened = 0;

void pmu_irq_handler(void)
{
	pmu_irq_happened = 1;

	int PMOVSSET = read_sysreg_s(SYS_PMOVSSET_EL0);
	write_sysreg_s(PMOVSSET ,SYS_PMOVSCLR_EL0);
	write_sysreg_s(PMOVSSET ,SYS_PMCNTENCLR_EL0);
	printf("pmu_irq_handler happened \r\n");
}


/**
 * @desc  : main function call by startup64.S
 * boot flow: setup_vector --> clear bss --> lowlevel_init --> set sp --> main
 * @param {*}
 * @return {*}
 */
int main()
{
	unsigned long regvalue;

	printf("test pmu_irq\r\n");

	request_irq(23, pmu_irq_handler);

	irq_enable();

	// 1. PMCR_EL0 （Performance Monitors Control Register）
	// E:1: 写入0时关闭PMU硬件，写入1时使能PMU硬件
	write_sysreg_s(0x01, SYS_PMCR_EL0);

	//PMINTENSET_EL1: enable counter
	//C, bit [31]: overflow interrupt request enable bit
	//P<n>, bit [n], for n = 0 to 30
	write_sysreg_s((0x01 << 31), SYS_PMINTENSET_EL1);

	//Performance Monitors Count Enable Set Register
	write_sysreg_s((0x01 << 31), SYS_PMCNTENSET_EL0);

	while (pmu_irq_happened == 0) {
	}

	printf("pmu irq test pass\r\n");

	return 0;
}
