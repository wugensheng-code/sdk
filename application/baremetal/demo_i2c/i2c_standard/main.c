/*
 * main.c
 *
 *  Created on: Feb.11.2022
 *  Author: haitao.liu
 *  Function": I2C-100Kb/s, I2C0(master) transmit data to I2C1(slave);
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"

#ifdef BIT_ADDR_10
	#define BIT_ADDR_10
#else
	#define BIT_ADDR_7
#endif

/****************function definition******************/
/*
* master transmitter, slave receiver
*/
int i2c_master_tx(I2C_TypeDef *master,I2C_TypeDef *slave)
{
	int i;
	uint8_t datum;
	printf("* Example 1: Master Tx/Slave Rx\n");
#ifdef BIT_ADDR_7
	// MASTER DEVICE SETUP
	i2c_disable(master);
	i2c_clock_setup(master, I2C_CLOCK); // scl setup
	i2c_setSpeedMode(master, I2c_speed_standard); // 100Kb/s
	i2c_setMasterAddressMode(master, I2c_7bit_address); // 7-bit
	i2c_enableMaster(master); //master
	i2c_setTargetAddress(master,SLAVE_ADDRESS);
	i2c_enable(master);
	// SLAVE DEVICE SETUP
	i2c_disable(slave);
//	i2c_clock_setup(slave, I2C_CLOCK);
	i2c_setSpeedMode(slave, I2c_speed_standard); // 100Kb/s
	i2c_setSlaveAddressMode(slave, I2c_7bit_address); // 7-bit
	i2c_enableSlave(slave); //slave
	i2c_setSlaveAddress(slave,SLAVE_ADDRESS);
	i2c_enable(slave);
#endif
#ifdef BIT_ADDR_10
	// MASTER DEVICE SETUP
	i2c_disable(master);
	i2c_clock_setup(master, I2C_CLOCK); // scl setup
	i2c_setSpeedMode(master, I2c_speed_standard); // 100Kb/s
	i2c_setMasterAddressMode(master, I2c_10bit_address); // 10-bit
	i2c_enableMaster(master); //master
	i2c_setTargetAddress(master,SLAVE_ADDRESS);
	i2c_enable(master);
	// SLAVE DEVICE SETUP
	i2c_disable(slave);
//	i2c_clock_setup(slave, I2C_CLOCK);
	i2c_setSpeedMode(slave, I2c_speed_standard); // 100Kb/s
	i2c_setSlaveAddressMode(slave, I2c_10bit_address); // 10-bit
	i2c_enableSlave(slave); //slave
	i2c_setSlaveAddress(slave,SLAVE_ADDRESS);
	i2c_enable(slave);
#endif
	// Transfer Characteristics
	//
	// This test performs a single master-tx/slave-rx transfer.  In this
	// transfer, seven bytes are sent from the master device to the
	// slave device, from 1 to 7.  The transfer is initiated by
	// writing to the master Tx FIFO.
	printf("Master sending 7 bytes...\n");
	for(i = 1; i <= 7; i++)
	{
		if(i == 7)
		{
			i2c_write(master, stop, (uint8_t) i);
		}
		else
		{
			i2c_write(master, normal, (uint8_t) i);
		}
	}
	// On the slave side, the Rx FIFO is continuously polled for data.
	// As each byte is received, it is compared to the expected value, a
	// sequence from 1 to 7.  If an unexpected byte is received, an
	// error message is printed.
	//
    i = 0;
    printf("Slave received bytes: ");
    while(i++ < 7) {
        while(i2c_isRxFifoEmpty(slave) == true);
        datum = i2c_read(slave);
        if(datum != i)
        {
        	printf("\n*** ERROR: expected %d, received %d ***\n", i, datum);
        	return -1;
        }
        else
        {
        	printf("0x%02x ", datum);
        }
    }
    printf("\n");
    printf("\n");
	return 0;
}

#if 0
/*
* master receiver, slave transmitter
*/
int i2c_master_rx(I2C_TypeDef *master,I2C_TypeDef *slave)
{
	int i;
	uint8_t datum;
	printf("* Example 2: Master Rx/Slave Tx\n");
	// MASTER DEVICE SETUP
	i2c_disable(master);
	i2c_clock_setup(master, I2C_CLOCK); // scl setup
	i2c_setSpeedMode(master, I2c_speed_standard); // 100Kb/s
	i2c_setMasterAddressMode(master, I2c_10bit_address); // 10-bit
	i2c_enableMaster(master); //master
	// Use the start byte protocol with the target address when
	// initiating transfer.
//	i2c_setTxMode(master, I2c_tx_start_byte);
	i2c_setTargetAddress(master,SLAVE_ADDRESS);
	i2c_enable(master);
	// SLAVE DEVICE SETUP
	i2c_disable(slave);
	i2c_setSpeedMode(slave, I2c_speed_standard); // 100Kb/s
	i2c_setMasterAddressMode(slave, I2c_10bit_address); // 10-bit
	i2c_enableSlave(slave); //slave
	i2c_setSlaveAddress(slave,SLAVE_ADDRESS);
	i2c_enable(slave);

	// Transfer Characteristics
	//
	// This test performs a single master-rx/slave-tx transfer.  In this
	// transfer, seven bytes are sent from the slave device to the
	// master device, from 1 to 7.  The transfer is initiated by
	// writing seven read requests to the the master Tx FIFO.  The
	// master device, in turn, starts a transfer with the specified
	// target address and transfer protocols and indicates it wishes to
	// read from the slave device.  It will continue to poll the slave
	// until seven bytes are received.
	//
	printf("Master issuing 7 read requests...\n");
	for(i = 1; i <= 7; i++)
		i2c_issueRead(master, normal);
    // On the slave side, a byte is written to the Tx FIFO each time a
    // read request is received, numbered 1 through 7.  When the master
    // has finished requesting data, the slave device indicates this by
    // triggering the rx done interrupt.
    //
    printf("Write to slave Tx FIFO...\n");
    printf("Writing... ");
    for(i = 1; i <= 7; i++) {
//        while(dw_i2c_isRawIrqActive(slave, I2c_irq_rd_req) == false);
        printf("%d ", i);
        i2c_write(slave, normal, i);
//        // clear the read request interrupt
//        dw_i2c_clearIrq(slave, I2c_irq_rd_req);
    }
    printf("\n");
//    // wait for the rx done interrupt
//    while(dw_i2c_isRawIrqActive(slave, I2c_irq_rx_done) == false);
//    // clear the rx done interrupt
//    dw_i2c_clearIrq(slave, I2c_irq_rx_done);

    printf("Read from master Rx FIFO...\n");

    // The master Rx FIFO should now contain seven bytes which are read
    // and compared to the expected values.  If an unexpected byte is
    // received, an error message is printed.
    i = 0;
    printf("Master read bytes: ");
    while(i++ < 7)
    {
        while(i2c_isRxFifoEmpty(master) == true);
        datum = i2c_read(master);
        if(datum != i)
        {
        	printf("\n*** ERROR: expected %d, received %d ***\n", i, datum);
        	return -1;
        }
        else
        {
        	printf("0x%02x ", datum);
        }
    }
    printf("\n");
    printf("\n");
	return 0;
}
#endif

/*
 * main
 */
int main(void)
{
	int retval = 0;
    printf("I2C test start\n");
    // poll-driven master-transmitter example
    retval |= i2c_master_tx(I2C0, I2C1);
    // poll-driven master-receiver example
//    retval |= i2c_master_rx(I2C0, I2C1);
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
