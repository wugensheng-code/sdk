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
#define DMA_LENGTH	128	/* Length of the Dma Transfers */
#define TIMEOUT_LIMIT 	0x2000	/* Loop count for timeout */
#define XDMAPS_CHANNELS_PER_DEV 8 /* Numbel of DMA Channel*/

#define TIMEOUT_THRE 0x20
#define XMON_SX2X_M0
#ifdef XMON_DDR_S1
#define base0 0x01210000
#define base1 0x01211000
#endif
#ifdef XMON_SX2X_M0
#define base0 0x80000000
#define base1 0x80001000
#endif
#ifdef XMON_GP_S1
#define base0 0x00000000
#define base1 0x00001000
#endif
#define DMAX_TEST_CHANNELS 1
#define vfwp printf
#define REG_WRITE(reg_address, reg_wdata)  *(unsigned int*)(reg_address) = reg_wdata
#define REG_READ(reg_address)  *(unsigned int*)reg_address
void Debug_Printf(AXIMON_TypeDef *AXIMON)
{
	printf("AXIMON debug_access_type	= %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_access_type	));
	printf("AXIMON debug_burst_size	    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_burst_size		));
	printf("AXIMON debug_burst_length   = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_burst_length	));
	printf("AXIMON debug_burst_type	    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_burst_type		));
	printf("AXIMON debug_addr_h		    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_addr_h			));
	printf("AXIMON debug_addr_l		    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_addr_l			));
	printf("AXIMON debug_id			    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_id				));
//	printf("AXIMON Timeoutflag			= %x\r\n",AlAxiMon_GetMonTimeOutValue(AXIMON					));
}
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
uint32_t num;
void Xmonitor_handler()
{
	printf("**********************************Intr******************************\r\n");
	AlAxiMon_IntrClear(AL_DDR_S1);
	AlAxiMon_IntrClear(AL_DMACX);
	AlAxiMon_IntrClear(AL_OCM_S2);
}
void SetupIntr()
{
	AlAxiMon_IntrMask  (AL_DDR_S1);
	AlAxiMon_IntrEnable(AL_DDR_S1);
	AlAxiMon_IntrMask  (AL_DMACX);
	AlAxiMon_IntrEnable(AL_DMACX);
	AlAxiMon_IntrMask  (AL_OCM_S2);
	AlAxiMon_IntrEnable(AL_OCM_S2);
	ECLIC_Register_IRQ(SOC_INT129_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,Xmonitor_handler);
	__enable_irq();
}
void config_monitor(AXIMON_TypeDef *AXIMON, uint16_t id_max, uint16_t id_min, uint16_t gid_max, uint16_t gid_min, uint16_t thre)
{
	AlAxiMon_ResptimeRecordClean(AXIMON);
	AlAxiMon_ResptimeRecordEnble(AXIMON);
	AlAxiMon_QosValue(AXIMON,XQOS_MAX);
	AlAxiMon_QosValueRange(AXIMON,XQOS_MAX,0);
	AlAxiMon_QosCfgEnable(AXIMON,QosInternal);
	AlAxiMon_SbuparaCountClean(AXIMON);
	AlAxiMon_SubparaAwidRange(AXIMON,id_max,id_min);
	AlAxiMon_SubparaAridRange(AXIMON,id_max,id_min);
	AlAxiMon_SubparaAwgidRange(AXIMON,gid_max,gid_min);
	AlAxiMon_SubparaArgidRange(AXIMON,gid_max,gid_min);
	AlAxiMon_SubparaAwbtypeRange(AXIMON,1,1);//incr
	AlAxiMon_SubparaAwbsizeRange(AXIMON,2,2);//2^2
	AlAxiMon_SubparaAwblenRange(AXIMON,15,15);
	AlAxiMon_SubparaArbtypeRange(AXIMON,1,1);
	AlAxiMon_SubparaArbsizeRange(AXIMON,XSIZE_MAX,0);
	AlAxiMon_SubparaArblenRange(AXIMON,XLEN_MAX,0);
	AlAxiMon_SbuparaCountEnable(AXIMON);
	AXIMON->DEBUG_TIMEOUT_THD = thre;
}
void printf_monitor(AXIMON_TypeDef *AXIMON)
{
	printf("CASE NUM = %d\r\n",num);
//  	printf("AXIMON awqos_record         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awqos_record      ));
//  	printf("AXIMON arqos_record         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arqos_record      ));
  	printf("AXIMON awresptime_max       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_max    ));
//  	printf("AXIMON awresptime_acc_h     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_acc_h  ));
//  	printf("AXIMON awresptime_acc_l     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_acc_l  ));
//  	printf("AXIMON awresptime_cnt       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_cnt    ));
  	printf("AXIMON arresptime_max       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_max    ));
//  	printf("AXIMON arresptime_acc_h     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_acc_h  ));
//  	printf("AXIMON arresptime_acc_l     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_acc_l  ));
//  	printf("AXIMON arresptime_cnt       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_cnt    ));
//  	printf("AXIMON bandwidth_wr         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,bandwidth_wr      ));
//  	printf("AXIMON bandwidth_rd         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,bandwidth_rd      ));

 	printf("AXIMON cnt_awcmd  	        = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awcmd    ));
	printf("AXIMON cnt_awbtype	        = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awbtype  ));
	printf("AXIMON cnt_awblen	        = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awblen   ));
	printf("AXIMON cnt_awbsize          = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awbsize  ));
	printf("AXIMON cnt_awid             = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awid     ));
	printf("AXIMON cnt_awgid            = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awgid    ));
	printf("AXIMON cnt_arcmd            = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arcmd    ));
	printf("AXIMON cnt_arbtype          = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arbtype  ));
	printf("AXIMON cnt_arblen           = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arblen   ));
	printf("AXIMON cnt_arbsize          = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arbsize  ));
	printf("AXIMON cnt_arid             = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arid     ));
	printf("AXIMON cnt_argid            = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_argid    ));

//	printf("AXIMON subpara_awid         = %x\r\n",AlAxiMon_GetSubpara(AXIMON,subpara_awid       ));
//	printf("AXIMON subpara_arid         = %x\r\n",AlAxiMon_GetSubpara(AXIMON,subpara_arid       ));
//	printf("AXIMON subpara_grpid        = %x\r\n",AlAxiMon_GetSubpara(AXIMON,subpara_grpid      ));
//	Debug_Printf(AXIMON);
//	printf("AXIMON Intr                 = %x\r\n",AXIMON->INTR);
 	num ++;
}
void config_case()
{
	config_monitor(AL_DMACX,XID_MAX,0, 5,5, TIMEOUT_THRE);//0
	config_monitor(AL_DDR_S1 ,XID_MAX,0,5,5, TIMEOUT_THRE);//3 pass
	config_monitor(AL_SX2X_M0 ,XID_MAX,0,5,5, TIMEOUT_THRE);//3 pass
	config_monitor(AL_SX2X_M1 ,XID_MAX,0,5,5, TIMEOUT_THRE);//3 pass
	config_monitor(AL_OCM_S2 ,XID_MAX,0,5,5, TIMEOUT_THRE);//3 pass
	config_monitor(AL_GP_S0 ,XID_MAX,0,5,5, TIMEOUT_THRE);//3 pass
	config_monitor(AL_GP_S1 ,XID_MAX,0,5,5, TIMEOUT_THRE);//3 pass
}
void print_case()
{
	printf_monitor(AL_DMACX);
	printf_monitor(AL_DDR_S1);
	printf_monitor(AL_SX2X_M0);
	printf_monitor(AL_SX2X_M1);
	printf_monitor(AL_OCM_S2);
	printf_monitor(AL_GP_S0);//3 pass
	printf_monitor(AL_GP_S1);//3 pass
}
int main(void)
{
	uint32_t midr;
	uint64_t cpunum = 0;
	uint64_t mpidr;
    volatile uint32_t buffer[128];
	for (volatile uint32_t i = 0 ; i < 100 ; i++)
	{
		buffer[i] = i;
	}
	write_To_OCM((uint32_t*)buffer,128,(uint32_t*)0x80000000);
	config_case();
	SetupIntr();
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

	if (cpunum == 0) {
		int Status;

	#ifdef _AARCH_64
		asm volatile("mrs %0, sctlr_el3":"=r"(sctlr_el3)::"memory");
		vfwp("ctlr_el3 ** %x", (unsigned int)(sctlr_el3 & 0xffffffff));
	#endif

		//vfwp("set manager secure mode\n");
		vfwp("set manager non-secure mode\n");
		vfwp("set irq and periph non-secure mode\n");
		REG_WRITE(0xF88060A0, 0x001FFFFF);
		//REG_WRITE(0xF88060A0, 0x00100000);

		vfwp("reset dmac\n");
		REG_WRITE(0xF8801074, 0X00003270);
		REG_WRITE(0xF8801074, 0X00003370);
    	REG32_WRITE(0xf8801078,0x00001133);
    	REG32_WRITE(0xf8801074,0x000073f0);
    	REG32_WRITE(0xf8800080,0x0);
    	REG32_WRITE(0xf840e004,0x0);
    	REG32_WRITE(0xf840f004,0x0);
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
	DmaCmd.ChanCtrl.SrcBurstLen = 32;
	DmaCmd.ChanCtrl.SrcInc = 1;
	DmaCmd.ChanCtrl.DstBurstSize = 4;
	DmaCmd.ChanCtrl.DstBurstLen = 32;
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
	print_case();
	config_case();
}

int DmaCheckHandler(int *src, int * dst)
{
	int Index;
	int Status = XST_SUCCESS;
	return Status;
}

