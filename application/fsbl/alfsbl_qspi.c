/*
 * alfsbl_qspi.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#include <stdio.h>

#include "demosoc.h"

#include "alfsbl_qspi.h"
#include "alfsbl_misc.h"
#include "alfsbl_sec.h"
#include "qspi_drv.h"
#include "qspi_flash_drv.h"

extern QSPI_CORE_HANDLE    *g_pdev;


//-----------------------------------------------------------
//QSPI_FLASH_SR_BIT_SET: set flash status-register certain bit
//-----------------------------------------------------------
void QSPI_FLASH_SR_BIT_SET (unsigned bit_num, unsigned bit_val)
{
    unsigned data_r_7to0;   //read data
    unsigned data_r_15to8;  //read data

    //-----------------------------------------------------------
    //read flash status-register
    //-----------------------------------------------------------
    //config qspi
    Qspi_Disable(g_pdev);  //disable
	Qspi_Width(g_pdev,QSPI_WIDTH_X1);//QSPI_WIDTH_X4
	Qspi_Mode(g_pdev,TMOD_EEPROM);//TMOD_TX_ONLY
    Qspi_Dfs(g_pdev,DFS_BYTE);

	Qspi_TxfifoStartLevelSet(g_pdev,0); //transfer will start on serial line

    //REG_WRITE(QSPI__CTRLR1__ADDR,0x0);  //rx need receive 1 data-frame
    //QSPI_TXFIFO_START_LEVEL_SET(0x0);  //tx need transmit 1 data-frame
	Qspi_Ctrl1Ndf(g_pdev,0);

	Qspi_Enable(g_pdev);

    //sent read flash status-register[7:0] instruction 0x05
    //QSPI_DATA_WRITE(0x05);  //send inst
	Qspi_DataTransmit(g_pdev,0x5);
    //wait qspi is not busy
    //QSPI_WAIT_ACT_COMPLETE();  //until tx-fifo is empty, and qspi is not busy
    //read flash status-register[7:0] from rx-fifo
	//
    while( !Qspi_TxfifoEmpty(g_pdev) );
	while(Qspi_Busy(g_pdev));
    data_r_7to0 = Qspi_DataRead(g_pdev);  //read data from rx-fifo

    //sent read flash status-register[15:8] instruction 0x35
    //QSPI_DATA_WRITE(0x35);  //send inst
	Qspi_DataTransmit(g_pdev,0x35);
    //wait qspi is not busy
	while( !Qspi_TxfifoEmpty(g_pdev) );
	while(Qspi_Busy(g_pdev));  //until tx-fifo is empty, and qspi is not busy
    //read flash status-register[15:8] from rx-fifo
    data_r_15to8 = Qspi_DataRead(g_pdev);  //read data from rx-fifo

    //-----------------------------------------------------------
    //modified the bit needed to be set
    //-----------------------------------------------------------
    //set if bit[7:0]
    if (bit_num < 8) {
        data_r_7to0 = (~(0x1 << bit_num) & data_r_7to0) | (bit_val << bit_num);  //only set the needed bit
    //set if bit[15:8]
    } else {
        data_r_15to8 = (~(0x1 << (bit_num-8)) & data_r_15to8) | (bit_val << (bit_num-8));  //only set the needed bit
    }

    //-----------------------------------------------------------
    //set flash status-register certain bit
    //-----------------------------------------------------------
    //QSPI_FLASH_SEND_WREN();  //send flash WREN
	Qspi_Disable(g_pdev);  //disable
	Qspi_Width(g_pdev,QSPI_WIDTH_X1);
	Qspi_Mode(g_pdev,TMOD_TX_ONLY);
	Qspi_TxfifoStartLevelSet(g_pdev,0); //transfer will start on serial line
	Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev,0x6);
    while( !Qspi_TxfifoEmpty(g_pdev) );
	while(Qspi_Busy(g_pdev));  //until tx-fifo is empty, and qspi is not busy

    //config qspi
    Qspi_Disable(g_pdev);  //disable
    Qspi_TxfifoStartLevelSet(g_pdev,2); //transfer will start on serial line
    Qspi_Enable(g_pdev);  //enable
    //sent write flash status-register instruction 0x01, and set value[15:0]
    Qspi_DataTransmit(g_pdev,0x01);  //send inst
    Qspi_DataTransmit(g_pdev,data_r_7to0);  //send data
    Qspi_DataTransmit(g_pdev,data_r_15to8);  //send data
    //wait qspi is not busy
    while( !Qspi_TxfifoEmpty(g_pdev) );
	while(Qspi_Busy(g_pdev));  //until tx-fifo is empty, and qspi is not busy
    //wait flash WIP returns to 0
    //QSPI_FLASH_WAIT_WIP_COMPLETE();  //until WIP returns to 0
    Qspi_Disable(g_pdev);  //disable
}  //QSPI_FLASH_SR_BIT_SE



uint32_t AlFsbl_Qspi24Init(void)
{
	QspiParams qspi_params;
	Csu_QspiInit(&qspi_params);

	QSPI_FLASH_SR_BIT_SET(9, 1);
    Csu_QspiSetMode(QSPI_WIDTH_X2, QSPI_ADDR_24);

    Qspi_Disable(g_pdev);
	Qspi_SckdivCfg(g_pdev,0x2); // ahb: 200M, spi: 200 / 4 = 50M

	g_pdev->regs->SPI_CTRLR0 = (g_pdev->regs->SPI_CTRLR0) | (1 << 30);

    Qspi_Enable(g_pdev);

	return 0;
}


uint32_t AlFsbl_Qspi24Copy(PTRSIZE SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo)
{
	uint32_t ret;
#ifdef QSPI_XIP_THROUTH_CSU_DMA
	if(pSecureInfo != NULL) {
		pSecureInfo->InputAddr  = SrcAddress + QSPI_XIP_BASEADDR;
		pSecureInfo->OutputAddr = DestAddress;
		pSecureInfo->DataLength = Length;

		ret = AlFsbl_DecHash_1(pSecureInfo);
	}
	else {
		ret = AlFsbl_CsuDmaCopy(
				SrcAddress + QSPI_XIP_BASEADDR,
				DestAddress,
				Length,
				CSUDMA_DST_INCR | CSUDMA_SRC_INCR);
	}
#else
	ret = Csu_QspiRead(SrcAddress, (uint8_t *)(DestAddress), Length);
#endif
	return ret;
}


uint32_t AlFsbl_Qspi24Release(void)
{
	return 0;
}


