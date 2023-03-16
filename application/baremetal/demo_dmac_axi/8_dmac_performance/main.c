/****************************************************************************/
/**
* @file performance test
*
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "nuclei_sdk_soc.h"
#include "xdmaps.h"

#define vfwp printf
#define REG_WRITE(reg_address, reg_wdata)  *(unsigned int*)(reg_address) = reg_wdata
#define REG_READ(reg_address)  *(unsigned int*)reg_address

void XNullHandler(void *NullParameter)
{
	(void) NullParameter;
}


#define TUBE_ADDRESS ((volatile char *) 0xF8800010u)
#define xil_printf(arg...) do {} while(0)

#define DMA_DEVICE_ID 			0
#define INTC_DEVICE_ID			XPAR_SCUGIC_SINGLE_DEVICE_ID

#define TEST_ROUNDS	1	/* Number of loops that the Dma transfers run.*/
#define DMA_LENGTH	16	/* Length of the Dma Transfers */
#define TIMEOUT_LIMIT 	0x2000	/* Loop count for timeout */
#define XDMAPS_CHANNELS_PER_DEV 8 /* Numbel of DMA Channel*/

/* form demosoc.h, interrupt ID*/
IRQn_Type DMA_FAULT_INTR  = SOC_INT56_IRQn; 
IRQn_Type DMA_DONE_INTR_0 = SOC_INT57_IRQn; 
IRQn_Type DMA_DONE_INTR_1 = SOC_INT58_IRQn; 
IRQn_Type DMA_DONE_INTR_2 = SOC_INT59_IRQn; 
IRQn_Type DMA_DONE_INTR_3 = SOC_INT60_IRQn;
IRQn_Type DMA_DONE_INTR_4 = SOC_INT61_IRQn;
IRQn_Type DMA_DONE_INTR_5 = SOC_INT62_IRQn; 
IRQn_Type DMA_DONE_INTR_6 = SOC_INT63_IRQn;   
IRQn_Type DMA_DONE_INTR_7 = SOC_INT64_IRQn; 

/**************************** Type Definitions *******************************/

typedef void (*Xil_InterruptHandler)(void* data);
int XDmaPs_Example_W_Intr(u16 DeviceId);
int SetupInterruptSystem(XDmaPs *DmaPtr);
void DmaDoneHandler(unsigned int Channel, XDmaPs_Cmd *DmaCmd, void *CallbackRef);
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
	uint32_t midr;
	uint64_t cpunum = 0;
	uint64_t mpidr;
	
	#ifdef __aarch64__
	uint64_t sctlr_el3;
	
	asm volatile("mrs %0, midr_el1":"=r"(midr)::"memory");
	asm volatile("mrs %0, mpidr_el1":"=r"(mpidr)::"memory");
	#endif

	#ifdef _AARCH_32
	uint32_t cpunum = 0;
	uint32_t mpidr;
	asm volatile("mrc p15,0,%0,c0,c0,0":"=r"(midr)::"memory");
	asm volatile("mrc p15,0,%0,c0,c0,5":"=r"(mpidr)::"memory");
	#endif

	cpunum = mpidr & 0x00ff;

	if (cpunum == 0) {
		int Status;
		
	#ifdef __aarch64__
		asm volatile("mrs %0, sctlr_el3":"=r"(sctlr_el3)::"memory");
		vfwp("ctlr_el3 ** %x", (unsigned int)(sctlr_el3 & 0xffffffff));
	#endif

		vfwp("set manager non-secure mode\n");
		vfwp("set irq and periph non-secure mode\n");
		REG_WRITE(0xF88060A0, 0x00000000);
		//REG_WRITE(0xF88060A0, 0x00000000);

		vfwp("reset dmac\n");
		REG_WRITE(0xF8801074, 0X00003270);
		REG_WRITE(0xF8801074, 0X00003370);
		
		Status = XDmaPs_Example_W_Intr(DMA_DEVICE_ID);
		if (Status != XST_SUCCESS) {
			vfwp("Error: XDMaPs_Example_W_Intr failed Status=%d 0x%x\r\n", Status, Status);
			return XST_FAILURE;
		}

		vfwp("Successfully ran XDMaPs_Example_W_Intr");
		return XST_SUCCESS;

	} else {
		vfwp("cpu[%d] running", cpunum);
	}

	return 0;
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
int XDmaPs_Example_W_Intr(u16 DeviceId)
{

	int Index;
	unsigned int Channel = 0;
	int Status;
	int TestStatus;
	int TestRound;
	int TimeOutCnt;
	volatile int Checked[XDMAPS_CHANNELS_PER_DEV];
	XDmaPs_Config *DmaCfg;
	XDmaPs *DmaInst = &DmaInstance;
	XDmaPs_Cmd DmaCmd;

	memset(&DmaCmd, 0, sizeof(XDmaPs_Cmd));

	DmaCmd.ChanCtrl.SrcBurstSize = 4;
	DmaCmd.ChanCtrl.SrcBurstLen = 4;
	DmaCmd.ChanCtrl.SrcInc = 1;
	DmaCmd.ChanCtrl.DstBurstSize = 4;
	DmaCmd.ChanCtrl.DstBurstLen = 4;
	DmaCmd.ChanCtrl.DstInc = 1;
	DmaCmd.BD.SrcAddr = (u32) Src;
	DmaCmd.BD.DstAddr = (u32) Dst;
	DmaCmd.BD.Length = DMA_LENGTH * sizeof(int);


	/*
	 * Initialize the DMA Driver
	 */
	DmaCfg = XDmaPs_LookupConfig(DeviceId);
	if (DmaCfg == NULL) {
		xil_printf("%s %d \n", __FUNCTION__, __LINE__);
		return XST_FAILURE;
	} else {
		vfwp("dmacfg addr: ** %x \n", DmaCfg->BaseAddress);
	}

	Status = XDmaPs_CfgInitialize(DmaInst, DmaCfg, DmaCfg->BaseAddress);

	/*
	 * Setup the interrupt system.
	 */
	Status = SetupInterruptSystem(DmaInst);
	if (Status != XST_SUCCESS) {
		vfwp("%s %d \n" , __FUNCTION__, __LINE__);
		return XST_FAILURE;
	}
	TestStatus = XST_SUCCESS;

	for (TestRound = 0; TestRound < TEST_ROUNDS; TestRound++) {
		xil_printf("Test round %d\r\n", TestRound);
		for (Channel = 0;
		     Channel < 8 /*XDMAPS_CHANNELS_PER_DEV*/;
		     Channel++) {

			/* Initialize source */
			for (Index = 0; Index < DMA_LENGTH; Index++)
				Src[Index] = DMA_LENGTH - Index;

			/* Clear destination */
			for (Index = 0; Index < DMA_LENGTH; Index++)
				Dst[Index] = 0;

			Checked[Channel] = 0;
			// Set the Done interrupt handler 
			XDmaPs_SetDoneHandler(DmaInst, Channel, DmaDoneHandler, (void *)Checked);

			vfwp("set Channel thead non-secure\n");
			//vfwp("set Channel thead secure\n");
			Status = XDmaPs_Start(DmaInst, Channel, &DmaCmd, 0, 0);
			printf("start\n");

			TimeOutCnt = 0;

			/* Now the DMA is done */
			while (!Checked[Channel]
			       && TimeOutCnt < TIMEOUT_LIMIT) {
				TimeOutCnt++;
			}

			Checked[Channel] = DmaCheckHandler(Src, Dst);

			if (Checked[Channel] < 0) {
				/* DMA controller failed */
				xil_printf("%s %d \n", __FUNCTION__, __LINE__);
				TestStatus = XST_FAILURE;
			}
		}
	}

	return TestStatus;
}

/******************************************************************************/
/**
 *
 * This function connects the interrupt handler of the interrupt controller to
 * the processor.  This function is seperate to allow it to be customized for
 * each application. Each processor or RTOS may require unique processing to
 * connect the interrupt handler.
 *
 * @param	DmaPtr is the DMA instance pointer.
 *
 * @return	None.
 *
 ****************************************************************************/
int SetupInterruptSystem(XDmaPs *DmaPtr)
{
	int Status;
	/*
	 * Connect the Fault ISR
	 */
	Status = ECLIC_Register_IRQ(SOC_INT56_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, XDmaPs_FaultISR);

	/*
	 * Connect the Done ISR for all 8 channels of DMA 0
	 */
	Status = ECLIC_Register_IRQ(SOC_INT57_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, XDmaPs_DoneISR_0);
	Status = ECLIC_Register_IRQ(SOC_INT58_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, XDmaPs_DoneISR_1);
	Status = ECLIC_Register_IRQ(SOC_INT59_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, XDmaPs_DoneISR_2);
	Status = ECLIC_Register_IRQ(SOC_INT60_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, XDmaPs_DoneISR_3);
	Status = ECLIC_Register_IRQ(SOC_INT61_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, XDmaPs_DoneISR_4);
	Status = ECLIC_Register_IRQ(SOC_INT62_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, XDmaPs_DoneISR_5);
	Status = ECLIC_Register_IRQ(SOC_INT63_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, XDmaPs_DoneISR_6);
	Status = ECLIC_Register_IRQ(SOC_INT64_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, XDmaPs_DoneISR_7);
	
	vfwp("setup interrupt system status:%d \n", Status);

#if 0
	if (Status != XST_SUCCESS)
		return XST_FAILURE;
#endif
	__enable_irq();

	return XST_SUCCESS;

}

void DmaDoneHandler(unsigned int Channel, XDmaPs_Cmd *DmaCmd, void *CallbackRef)
{
	printf("End\n");

	/* done handler */
	volatile int *Checked = (volatile int *)CallbackRef;
	int Index;
	int Status = 1;
	int *Src;
	int *Dst;

	Src = (int *)DmaCmd->BD.SrcAddr;
	Dst = (int *)DmaCmd->BD.DstAddr;

	/* DMA successful */
	/* compare the src and dst buffer */
	for (Index = 0; Index < DMA_LENGTH; Index++) {
		if ((Src[Index] != Dst[Index]) ||
				(Dst[Index] != DMA_LENGTH - Index)) {
			Status = -XST_FAILURE;
		}
	}

	Checked[Channel] = Status;
	vfwp("Got channel[%d] irq, result=%d \n", Channel, Status);
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
			Status = -XST_FAILURE;
			break;
		}

		xil_printf("[%d]: 0x%x 0x%x\n", Index ,src[Index], dst[Index]);
	}

	vfwp("check channel result=%d \n", Status);
	return Status;
}
