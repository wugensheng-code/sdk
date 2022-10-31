/****************************************************************************/
/**
* @file dmac abort irq test
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

#define DMA_LENGTH	174	/* Length of the Dma Transfers */ /*all number of data*/

/**************************** Type Definitions *******************************/
static int XDmaPs_Example_W_Intr(void);
int DmaCheckHandler(int *src, int * Dst);
/************************** Variable Definitions *****************************/

#ifdef __ICCARM__
#pragma data_alignment=32
static int Src[DMA_LENGTH];
static int Dst[DMA_LENGTH];
#pragma data_alignment=4
#else
static int DmaProgBuf[DMA_LENGTH] __attribute__ ((aligned (256)));
static int Src[DMA_LENGTH] __attribute__ ((aligned (32)));
static int Dst[DMA_LENGTH] __attribute__ ((aligned (32)));
static int DmaAbortIrq = 0;
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

	return XST_SUCCESS;
}

void XDmaPs_AbortISR(void * InstPtr)
{
	DmaAbortIrq = 1;

	__disable_irq();

	vfwp("Got XDmaPs_AbortISR\r\n", InstPtr);
	vfwp("Test pass\r\n", InstPtr);

	REG_WRITE(CRP__SRST_CTRL1__ADDR, 0x00003270);
}

/*****************************************************************************/
/**
 *
 * Case to test the DMA.
 *
 * @param	DeviceId is the Device ID of the DMA controller.
 * @return	XST_SUCCESS to indicate success, otherwise XST_FAILURE.
 *
 ****************************************************************************/

static int XDmaPs_Example_W_Intr(void)
{
	int Status;
	volatile unsigned rdata0;

	vfwp("starting test dma abort, insert undefined instruction\r\n");

	//insert undefined instruction 0xff, 0xff, 0xff to DmaProgBuf;
	DmaProgBuf[0] = 0xff;
	DmaProgBuf[1] = 0xff;
	DmaProgBuf[2] = 0xff;
	DmaProgBuf[3] = 0xff;

	Status = ECLIC_Register_IRQ(SOC_INT56_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, XDmaPs_AbortISR);
	__enable_irq();

	 // set boot_addr
	 REG_WRITE(TOP_NS__CFG_CTRL1_DMAC_AXI__ADDR, (unsigned int)DmaProgBuf);
	 rdata0 = REG_READ(TOP_NS__CFG_CTRL1_DMAC_AXI__ADDR);

	 // set boot
	 REG_WRITE(TOP_NS__CFG_CTRL0_DMAC_AXI__ADDR, 0x00000001);
	 rdata0 = REG_READ(TOP_NS__CFG_CTRL0_DMAC_AXI__ADDR);

	 // reset dmac
	REG_WRITE(CRP__SRST_CTRL1__ADDR, 0X00003270);
	REG_WRITE(CRP__SRST_CTRL1__ADDR, 0X00003370);

	while (!DmaAbortIrq);

	return Status;
}