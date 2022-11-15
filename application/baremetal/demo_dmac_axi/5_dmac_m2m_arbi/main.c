/****************************************************************************/
/**
* @file dmac mem2mem arbitrarily
*
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "nuclei_sdk_soc.h"
#include "al9000_registers.h"

#include "xdmaps.h"

#define vfwp printf
#define REG_WRITE(reg_address, reg_wdata)  *(unsigned int*)(reg_address) = reg_wdata
#define REG_READ(reg_address)  *(unsigned int*)reg_address

void XNullHandler(void *NullParameter)
{
	(void) NullParameter;
}

#define TEST_ROUNDS	1	/* Number of loops that the Dma transfers run.*/
#define DMA_LENGTH	174	 /*all number of data*/

/**************************** Type Definitions *******************************/
int XDmaPs_Example_W_Intr(void);
int DmaCheckHandler(int *src, int * Dst);
/************************** Variable Definitions *****************************/
#ifdef __ICCARM__
#pragma data_alignment=32
static int Src[DMA_LENGTH];
static int Dst[DMA_LENGTH];
#pragma data_alignment=4
#else
static int Src[DMA_LENGTH] __attribute__ ((aligned (32)));
static int Dst[DMA_LENGTH] __attribute__ ((aligned (32)));
#endif

XDmaPs DmaInstance;

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
 *
 *  Case to test the DMA.
 *
 * @param	DeviceId is the Device ID of the DMA controller.
 * @return	XST_SUCCESS to indicate success, otherwise XST_FAILURE.
 *
 ****************************************************************************/
volatile unsigned rdata0;
volatile unsigned rdata_cpc0;
int XDmaPs_Example_W_Intr(void)
{

	int Index;

	// OCM 61000000 DV use
	// OCM 61010000 DMA Instruction
	#define OCM_L 0x61000000
	int OCM__BASE1_ADDR = OCM_L+0x00010000;

	char *DmaProgBuf;
	DmaProgBuf = OCM__BASE1_ADDR;
	//vfwp("Start addr:%x \n", DmaProgBuf);
	DmaProgBuf += XDmaPs_Instr_DMAGO(DmaProgBuf, 0, OCM__BASE1_ADDR+6, 0);
	vfwp("Start addr:%x \n", DmaProgBuf);
	DmaProgBuf += XDmaPs_Instr_DMAMOV(DmaProgBuf, 0x0, (u32)Src);
	DmaProgBuf += XDmaPs_Instr_DMAMOV(DmaProgBuf, 0x2, (u32)Dst);
	DmaProgBuf += XDmaPs_Instr_DMAMOV(DmaProgBuf, 0x1, 0x003DC0F7);
	DmaProgBuf += XDmaPs_Instr_DMALP(DmaProgBuf, 0, 5);
		DmaProgBuf += XDmaPs_Instr_DMALD(DmaProgBuf);
		//DmaProgBuf += XDmaPs_Instr_DMARMB(DmaProgBuf);
		DmaProgBuf += XDmaPs_Instr_DMAST(DmaProgBuf);
		//DmaProgBuf += XDmaPs_Instr_DMAWMB(DmaProgBuf);
	DmaProgBuf += XDmaPs_Instr_DMALPEND(DmaProgBuf, DmaProgBuf-2, 0);
	DmaProgBuf += XDmaPs_Instr_DMAMOV(DmaProgBuf, 0x1, 0x0019C067);
		DmaProgBuf += XDmaPs_Instr_DMALD(DmaProgBuf);
		DmaProgBuf += XDmaPs_Instr_DMARMB(DmaProgBuf);
		DmaProgBuf += XDmaPs_Instr_DMAST(DmaProgBuf);
		DmaProgBuf += XDmaPs_Instr_DMAWMB(DmaProgBuf);
	DmaProgBuf += XDmaPs_Instr_DMAMOV(DmaProgBuf, 0x1, 0x00084021);
		DmaProgBuf += XDmaPs_Instr_DMALD(DmaProgBuf);
		DmaProgBuf += XDmaPs_Instr_DMAST(DmaProgBuf);
		DmaProgBuf += XDmaPs_Instr_DMAWMB(DmaProgBuf);
	DmaProgBuf += XDmaPs_Instr_DMASEV(DmaProgBuf, 0);
	DmaProgBuf += XDmaPs_Instr_DMAEND(DmaProgBuf);
	vfwp("End addr:%x \n", DmaProgBuf);

	 // set boot_addr
	 REG_WRITE(TOP_NS__CFG_CTRL1_DMAC_AXI__ADDR, OCM__BASE1_ADDR);
	 //REG_WRITE(TOP_NS__CFG_CTRL1_DMAC_AXI__ADDR, 0);
	 rdata0 = REG_READ(TOP_NS__CFG_CTRL1_DMAC_AXI__ADDR);
	 //vfwp("** CFG_CTRL1_DMAC_AXI__ADDR = %x \n",rdata0);
	 // set boot
	 REG_WRITE(TOP_NS__CFG_CTRL0_DMAC_AXI__ADDR, 0x00000001);
	 rdata0 = REG_READ(TOP_NS__CFG_CTRL0_DMAC_AXI__ADDR);
	 //vfwp("** CFG_CTRL0_DMAC_AXI__ADDR = %x \n",rdata0);


	 /* Initialize source */
	 vfwp("Src addr %x \n",Src);
	 for (Index = 0; Index < DMA_LENGTH; Index++)
		 Src[Index] = DMA_LENGTH - Index;

	 /* Clear destination */
	 vfwp("Dst addr %x \n",Dst);
	 for (Index = 0; Index < DMA_LENGTH; Index++)
	 	 Dst[Index] = 0;


	 // reset dmac
	REG_WRITE(CRP__SRST_CTRL1__ADDR, 0X00003270);
	//rdata0 = REG_READ(0Xf8801074);
	//vfwp("** damc_SOFT_RESET = %x\n",rdata0);
	REG_WRITE(CRP__SRST_CTRL1__ADDR, 0X00003370);
	//rdata0 = REG_READ(0Xf8801074);
    //vfwp("** damc_SOFT_RESET = %x\n",rdata0);


	// wait for dmac process until event status = FFFF
	while(1){
	  rdata0 = REG_READ(DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR);
	  printf("** DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR = %x\n",rdata0);
	   if(rdata0 == 0x00000001) break;
	 }

	// view CPC0
		rdata_cpc0 = REG_READ(0xF8418104);
		vfwp("** CPC0 = %x \n",rdata_cpc0);
		// view DPC
		rdata0 = REG_READ(0xF8418004);
		vfwp("** DPC = %x \n",rdata0);
		// view CSR0
		rdata0 = REG_READ(0xF8418100);
		vfwp("** CSR0 = %x \n",rdata0);
		// view CCR0
		rdata0 = REG_READ(0xF8418408);
		vfwp("** CCR0 = %x \n",rdata0);

	rdata0 = DmaCheckHandler(Src, Dst);
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

	/* DMA successful */
	/* compare the src and dst buffer */
	for (Index = 0; Index < DMA_LENGTH; Index++) {
		if ((src[Index] != dst[Index]) ||
				(dst[Index] != DMA_LENGTH - Index)) {
			vfwp("[%d]: 0x%x 0x%x\n", Index ,src[Index], dst[Index]);
			Status = -XST_FAILURE;
			//break;
		}

		vfwp("[%d]: 0x%x 0x%x\n", Index ,src[Index], dst[Index]);
	}

	vfwp("check channel result=%d \n", Status);
	return Status;
}
