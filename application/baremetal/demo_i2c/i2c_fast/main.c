/*
 * main.c
 *
 *  Created on: Feb.11.2022
 *  Author: haitao.liu
 *  Function": I2C-400Kb/s, I2C0(master) transmit data to I2C1(slave);
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"
/*
 * main
 */
int main(void)
{
	int retval = 0;
    printf("I2C test start\n");
    // poll-driven master-transmitter example
    // retval |= i2c_master_tx(I2C0, I2C1);
    // poll-driven master-receiver example
	retval |= i2c_master_rx(I2C0, I2C1,I2c_speed_fast,I2c_7bit_address);
    // interrupt-driven master-transmitter example
//    dwe_i2c_irq_master_gen_call(master, slave);
    // interrupt-driven master back-to-back example
//    dwe_i2c_irq_master_back2back(master, slave);

    if(retval == -1)
    {
    	printf("FAIL\n");
    	return -1;
    }
    else
    {
    	printf("PASS\n");
    	while(1);
    }

}
