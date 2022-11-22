/****************************************************************************/
/**
* @file non-secure_m2m test
*
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "nuclei_sdk_soc.h"
#include "xdmaps.h"
#include "al9000_ipc.h"
#include "al9000_aximon.h"

#define TUBE_ADDRESS ((volatile char *) 0xF8800010u)
#define xil_printf(arg...) do {} while(0)

#define DMA_DEVICE_ID 			0
#define INTC_DEVICE_ID			XPAR_SCUGIC_SINGLE_DEVICE_ID

#define TEST_ROUNDS	1	/* Number of loops that the Dma transfers run.*/
#define DMA_LENGTH	4096	/* Length of the Dma Transfers */
#define TIMEOUT_LIMIT 	0x2000	/* Loop count for timeout */
#define XDMAPS_CHANNELS_PER_DEV 8 /* Numbel of DMA Channel*/
#define TEST_MAX_NUM 30
#define TIMEOUT_THRE 0x20
#define XMON_DDR_S1
#ifdef XMON_DDR_S1
#define base0 0x1a100000
#define base1 0x1a200000
#endif
#define DMAX_TEST_CHANNELS 1
#define vfwp printf
#define REG_WRITE(reg_address, reg_wdata)  *(unsigned int*)(reg_address) = reg_wdata
#define REG_READ(reg_address)  *(unsigned int*)reg_address
u32 dma_test_num=0;
void XNullHandler(void *NullParameter)
{
	(void) NullParameter;
}
/* form demosoc.h, interrupt ID*/
IRQn_Type DMA_FAULT_INTR  = SOC_INT56_IRQn;
IRQn_Type DMA_DONE_INTR_0 = SOC_INT57_IRQn;

/**************************** Type Definitions *******************************/

typedef void (*Xil_InterruptHandler)(void* data);
int XDmaPs_Example_W_Intr(u16 DeviceId);
int SetupInterruptSystem(XDmaPs *DmaPtr);
void DmaDoneHandler(unsigned int Channel, XDmaPs_Cmd *DmaCmd, void *CallbackRef);
int DmaCheckHandler(int *src, int * Dst);

XDmaPs DmaInstance;
volatile uint16_t count = 0;
int main(void)
{
	uint32_t midr;
	uint64_t cpunum = 0;
	uint64_t mpidr;
    volatile uint32_t buffer[DMA_LENGTH];
	for (volatile uint32_t i = 0 ; i < DMA_LENGTH ; i++)
	{
		buffer[i] = i;
	}
	config_monitor(AL_DMACX, XID_MAX, 0, XID_MAX, 0, XQOS_MAX, 0x20);
	config_monitor(AL_OCM_S2, XID_MAX, 0, XID_MAX, 0, XQOS_MAX - 4, 0x20);
	config_monitor(AL_DDR_S1, XID_MAX, 0, XID_MAX, 0, XQOS_MAX, 0x20);
	write_To_OCM((uint32_t*)buffer,DMA_LENGTH,(uint32_t*)(base0));
	#ifdef _AARCH_64
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
		int Status;
	#ifdef _AARCH_64
		asm volatile("mrs %0, sctlr_el3":"=r"(sctlr_el3)::"memory");
		vfwp("ctlr_el3 ** %x", (unsigned int)(sctlr_el3 & 0xffffffff));
	#endif
	REG_WRITE(0xF88060A0, 0x001FFFFF);
	REG_WRITE(0xF8801074, 0X00003270);
	REG_WRITE(0xF8801074, 0X00003370);
	REG_WRITE(0xf8801078,0x00001133);
	REG_WRITE(0xf8801074,0x000073f0);
	REG_WRITE(0xf8800080,0x0);
	REG_WRITE(0xf840e004,0x0);
	REG_WRITE(0xf840f004,0x0);
	REG_WRITE(0xf8412004,0x0);
	REG_WRITE(0xf8413004,0x0);
	do{
		Status = XDmaPs_Example_W_Intr(DMA_DEVICE_ID);
	}while (dma_test_num < TEST_MAX_NUM);

	if (Status != XST_SUCCESS) {
		vfwp("Error: XDMaPs_Example_W_Intr failed Status=%d 0x%x\r\n", Status, Status);
		return XST_FAILURE;
	}
//
//	uint32_t ocm_num=0;
//	config_monitor(AL_DDR_S1, XID_MAX, 0, XID_MAX, 0, XQOS_MAX - 1, 0x1f);
//	while(1)
//	{
//		if(ocm_num < 10)
//			printf_monitor(AL_DDR_S1);
//		else
//			break;
//		ocm_num ++;
//	}
//	config_monitor(AL_DDR_S1, XID_MAX, 0, XID_MAX, 0, XQOS_MAX - 1, 0x20);
//	printf_monitor(AL_DDR_S1);
//	printf_monitor(AL_DDR_S1);
//	printf_monitor(AL_DDR_S1);
	uint32_t ocm_num=0;
	config_monitor(AL_OCM_S2, XID_MAX, 0, XID_MAX, 0, XQOS_MAX - 1, 0x1f);
	while(1)
	{
		if(ocm_num < 10)
			printf_monitor(AL_OCM_S2);
		else
			break;
		ocm_num ++;
	}
	config_monitor(AL_OCM_S2, XID_MAX, 0, XID_MAX, 0, XQOS_MAX - 1, 0x20);
	printf_monitor(AL_OCM_S2);
	printf_monitor(AL_OCM_S2);
	printf_monitor(AL_OCM_S2);


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
int XDmaPs_Example_W_Intr(u16 DeviceId )
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

	DmaCmd.ChanCtrl.SrcBurstSize = 8;
	DmaCmd.ChanCtrl.SrcBurstLen = 16;
	DmaCmd.ChanCtrl.SrcInc = 1;
	DmaCmd.ChanCtrl.DstBurstSize = 8;
	DmaCmd.ChanCtrl.DstBurstLen = 16;
	DmaCmd.ChanCtrl.DstInc = 1;
	DmaCmd.BD.SrcAddr = base0;
	DmaCmd.BD.DstAddr = base1;
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
		     Channel < DMAX_TEST_CHANNELS /*XDMAPS_CHANNELS_PER_DEV*/;
		     Channel++) {
			// Set the Done interrupt handler
			XDmaPs_SetDoneHandler(DmaInst, Channel, DmaDoneHandler, (void *)Checked);

			vfwp("set Channel thead non-secure\n");
			Status = XDmaPs_Start(DmaInst, Channel, &DmaCmd, 0, 1);

			TimeOutCnt = 0;
			/* Now the DMA is done */
			while (!Checked[Channel]
			       && TimeOutCnt < TIMEOUT_LIMIT) {
				TimeOutCnt++;
			}
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
	vfwp("setup interrupt system status:%d \n", Status);
	__enable_irq();
	return XST_SUCCESS;

}

void DmaDoneHandler(unsigned int Channel, XDmaPs_Cmd *DmaCmd, void *CallbackRef)
{

	/* done handler */
	volatile int *Checked = (volatile int *)CallbackRef;
	int Index;
	int Status = 1;
	int *Src;
	int *Dst;
	Src = (int *)DmaCmd->BD.SrcAddr;
	Dst = (int *)DmaCmd->BD.DstAddr;
	Checked[Channel] = Status;
	dma_test_num++;
//	printf_monitor(AL_DMACX);
	printf_monitor(AL_OCM_S2);
//	printf_monitor(AL_APU);
//	printf_monitor(AL_DDR_S1);
}

int DmaCheckHandler(int *src, int * dst)
{
	int Index;
	int Status = XST_SUCCESS;
	return Status;
}

