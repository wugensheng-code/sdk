/****************************************************************************/
/**
* @file dmac mem2peri2mem  Loop back
*
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "nuclei_sdk_soc.h"
#include "al9000_registers.h"

#include "xdmaps.h"

#define vfwp printf

#define I2S__ADDR 0x80008000
#define I2S_RESET_REG (I2S__ADDR + 0x0) // write 0x7 to reset I2S

void XNullHandler(void *NullParameter)
{
	(void) NullParameter;
}

#define TEST_ROUNDS	1	/* Number of loops that the Dma transfers run.*/
#define DMA_LENGTH	50	 /*all number of data*/

/**************************** Type Definitions *******************************/
int XDmaPs_Example_W_Intr(void);
int DmaCheckHandler(int *src, int * Dst);
/************************** Variable Definitions *****************************/
static int Src[DMA_LENGTH];
static int Dst[DMA_LENGTH];
static char instrctions[200];

int main(void)
{
		int Status;

		Status = XDmaPs_Example_W_Intr();
		if (Status != XST_SUCCESS) {
			vfwp("Error: XDMaPs_Example_W_Intr failed Status=%d 0x%x\r\n", Status, Status);
			return XST_FAILURE;
		}

		vfwp("Successfully ran XDMaPs_Example_W_Intr");
		return XST_SUCCESS;

	return 0;
}

/*****************************************************************************/
/**
 *  Case to test the DMA.
 *
 * @return	XST_SUCCESS to indicate success, otherwise XST_FAILURE.
 ****************************************************************************/
volatile unsigned rdata0;
volatile unsigned rdata_cpc0;
int XDmaPs_Example_W_Intr(void)
{
	// config I2S
	WriteReg(0xF8800080, 0x00000001); //  write 0 to <gp_proten> field of PLS_PROT
	WriteReg(0xF8801078, 0x00001133);
	WriteReg(I2S_RESET_REG, 0x7);
	WriteReg(I2S_RESET_REG, 0x0);
	WriteReg(0x80008008, 0x0);
	WriteReg(0x80008018, 0x0);

	int Index;

	#define I2S_DATA_RX_REG (I2S__ADDR + 0x28) // read
	#define I2S_DATA_TX_REG (I2S__ADDR + 0x2c) // write
	#define I2S_CTRL_REG (I2S__ADDR + 0x4) // write 0x3 to send single request

	char *DmaProgBuf;
	DmaProgBuf = instrctions;
	DmaProgBuf += XDmaPs_Instr_DMAGO(DmaProgBuf, 0, (&instrctions[0])+6, 0);
	vfwp("Start Instruction addr:%x \n", DmaProgBuf);
	// 5 singe transfers
	DmaProgBuf += XDmaPs_Instr_DMAMOV(DmaProgBuf, 0x0, (u32)Src);           // SAR
	DmaProgBuf += XDmaPs_Instr_DMAMOV(DmaProgBuf, 0x2, I2S_DATA_TX_REG);    // DAR
	DmaProgBuf += XDmaPs_Instr_DMAMOV(DmaProgBuf, 0x1, 0x00410205);         // CCR SS32 SB1 Sinc DS32 DB1 Dfix
	DmaProgBuf += XDmaPs_Instr_DMAFLUSHP(DmaProgBuf, 0); // init dma0 periph request interface
	DmaProgBuf += XDmaPs_Instr_DMALP(DmaProgBuf, 0, DMA_LENGTH);
		DmaProgBuf += XDmaPs_Instr_DMALDP(DmaProgBuf,0,0);
		DmaProgBuf += XDmaPs_Instr_DMAWFP(DmaProgBuf,1,0);
		DmaProgBuf += XDmaPs_Instr_DMASTP(DmaProgBuf,0,0);
	DmaProgBuf += XDmaPs_Instr_DMALPEND(DmaProgBuf, DmaProgBuf-6, 0); // LDP、WFP、STP共6bytes
	DmaProgBuf += XDmaPs_Instr_DMAMOV(DmaProgBuf, 0x0, I2S_DATA_RX_REG);    // SAR
	DmaProgBuf += XDmaPs_Instr_DMAMOV(DmaProgBuf, 0x2, (u32)Dst);           // DAR
	DmaProgBuf += XDmaPs_Instr_DMAMOV(DmaProgBuf, 0x1, 0x00414204);         // CCR  SS 32 SB 1 Sfix DS 32 DB 1 Dinc  4bytes (0x00414204)
	DmaProgBuf += XDmaPs_Instr_DMAFLUSHP(DmaProgBuf, 3); // init dma3 periph request interface
	DmaProgBuf += XDmaPs_Instr_DMALP(DmaProgBuf, 0, DMA_LENGTH);
		DmaProgBuf += XDmaPs_Instr_DMAWFP(DmaProgBuf,1,3);
		DmaProgBuf += XDmaPs_Instr_DMALDP(DmaProgBuf,0,3);
		DmaProgBuf += XDmaPs_Instr_DMASTP(DmaProgBuf,0,3);
	DmaProgBuf += XDmaPs_Instr_DMALPEND(DmaProgBuf, DmaProgBuf-6, 0);
	DmaProgBuf += XDmaPs_Instr_DMASEV(DmaProgBuf, 0);
	DmaProgBuf += XDmaPs_Instr_DMAEND(DmaProgBuf);
	vfwp("End addr:%x \n", DmaProgBuf);

	 // set boot_addr
	 WriteReg(TOP_NS__CFG_CTRL1_DMAC_AXI__ADDR, instrctions);
	 rdata0 = ReadReg(TOP_NS__CFG_CTRL1_DMAC_AXI__ADDR);
	 vfwp("** CFG_CTRL1_DMAC_AXI__ADDR = %x \n",rdata0);
	 // set boot
	 WriteReg(TOP_NS__CFG_CTRL0_DMAC_AXI__ADDR, 0x00000001);
	 rdata0 = ReadReg(TOP_NS__CFG_CTRL0_DMAC_AXI__ADDR);
	 vfwp("** CFG_CTRL0_DMAC_AXI__ADDR = %x \n",rdata0);

	 /* Initialize source */
	 vfwp("Src addr %x \n",Src);
	 for (Index = 0; Index < DMA_LENGTH; Index++)
		 Src[Index] = (DMA_LENGTH - Index)<< 8;

	 /* Clear destination */
	 vfwp("Dst addr %x \n",Dst);
	 for (Index = 0; Index < DMA_LENGTH; Index++)
	 	 Dst[Index] = 0;

	 // reset dmac
	WriteReg(CRP__SRST_CTRL1__ADDR, 0X00003270);
	//rdata0 = ReadReg(0Xf8801074);
	//vfwp("** damc_SOFT_RESET = %x\n",rdata0);
	WriteReg(CRP__SRST_CTRL1__ADDR, 0X00003370);
	//rdata0 = ReadReg(0Xf8801074);
    //vfwp("** damc_SOFT_RESET = %x\n",rdata0);

	// send request
	WriteReg(I2S_CTRL_REG, 0x3);
	// wait for event0
	while(1){
	  rdata0 = ReadReg(DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR);
	  printf("** DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR = %x\n",rdata0);
	   if(rdata0 == 0x00000001) break;
	 }
	 // cancel request
	WriteReg(I2S_CTRL_REG, 0x0);
	WriteReg(I2S_RESET_REG, 0x7);
	WriteReg(I2S_RESET_REG, 0x0);

		// view CPC0
		rdata_cpc0 = ReadReg(0xF8418104);
		vfwp("** CPC0 = %x \n",rdata_cpc0);
		// view DPC
		rdata0 = ReadReg(0xF8418004);
		vfwp("** DPC = %x \n",rdata0);
		// view CSR0
		rdata0 = ReadReg(0xF8418100);
		vfwp("** CSR0 = %x \n",rdata0);
		// view CCR0
		rdata0 = ReadReg(0xF8418408);
		vfwp("** CCR0 = %x \n",rdata0);

	rdata0 = DmaCheckHandler(Src, Dst);
	vfwp(rdata0);
	if (rdata_cpc0 == DmaProgBuf ) {
		return XST_SUCCESS;
	}
	else
		return XST_FAILURE;

}

int DmaCheckHandler(int *src, int * dst)
{
	int Index;
	int Status = XST_SUCCESS;
	int srch;
	int dsth;
	int data;

	/* DMA successful */
	/* compare the src and dst buffer high 24bits*/
	for (Index = 0; Index < DMA_LENGTH; Index++) {
		srch = src[Index] >> 8;
		dsth = dst[Index] >> 8;
		data = (DMA_LENGTH - Index);
		if ((srch != dsth) ||
			(dsth != data)) {
			vfwp("Error [%d]: srch 0x%x dsth 0x%x  data 0x%x\n", Index ,srch, dsth, data);
			Status = -XST_FAILURE;
			//break;
		}
		else{
			vfwp("Right [%d]: srch 0x%x dsth 0x%x  data 0x%x\n", Index ,srch, dsth, data);
		}
	}
	vfwp("check channel result=%d \n", Status);
	return Status;
}
