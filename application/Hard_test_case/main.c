/*
 * main.c
 *
 *  Created on: 2022/3/20
 *  Author: pang.wei
 *  Function": demo_per
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"
#include "../../../SoC/demosoc/Common/Include/demosoc_xec.h"
#include "al9000_test.h"
uint32_t main(){
	SD_Test();
	iic_test();
	qspi_test();
	spi_test();
	dma_test();
	gbe_test();
	can_test();
	timer_test();
	gpio_test();

#if __riscv
	benchmark_test();
#endif

	plps_test();
	usb_test();
}
