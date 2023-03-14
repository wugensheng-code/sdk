/*
 * main.c
 *
 *  Created on: 2023/2/27
 *  Author: cao.tengfei
 *  Function": demo_pmu
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mtimer.h"
#include "al9000_wdog.h"
#include "string.h"
#include "al9000_register_pmu.h"
#include "al9000_aximon.h"
#ifdef __riscv
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"
#include "demosoc.h"
#else
#include "config.h"
#include "gic.h"
#include "io.h"
#include "type.h"
#include "timer.h"
#include "gic_v3.h"
#include "gic_v3_addr.h"
#include "mm.h"
#endif

#define TIMEOUT_THRE 0x1f
#define TIMEOUT_THRE_SAFE 0x1fff

volatile CFG_CTRL_WDT_T			CfgCtrlWdt = {.reg = 0};
volatile CFG_CTRL_OCM_T			CfgCtrlOcm = {.reg = 0};
volatile TOP_NS_PLS_PROT_T		TopNsPlsPort = {.reg = 0};
volatile CRP_TGT_HTHD_CPUPLL_T	CrpTgtHthdCpupll = {.reg = 0};

volatile ERR_HW_EN0_SET_T	ErrHwEn0Set = {.reg = 0};
volatile ERR_HW_EN0_CLR_T	ErrHwEn0Clr = {.reg = 0};
volatile INT_EN0_SET_T		IntEn0Set = {.reg = 0};
volatile INT_EN0_CLR_T		IntEn0Clr = {.reg = 0};
volatile INT_MSK0_SET_T		IntMsk0Set = {.reg = 0};
volatile INT_MSK0_CLR_T		IntMsk0Clr = {.reg = 0};
volatile INT_CLR0_T			IntClr0 = {.reg = 0};
volatile INT_STATE0_T		IntState0 = {.reg = 0};
volatile RAW_HIS0_T			RawHis0 = {.reg = 0};
volatile PMU_NMI_EN_T		PmuNmiEn = {.reg = 0};
volatile SW_PMU_SHACK_T		SwPmuShack = {.reg = 0};
volatile PMU_STAT_T			PmuStat = {.reg = 0};

volatile ERR_HW_EN0_SET_T	IntTouchFlag;

static void wdt_init(WDT_TypeDef *Wdt, IRQn_Type Irq);
uint32_t wdt_stop(WDT_TypeDef *Wdt);

static inline uint32_t Pmu_Reg32Read(uint32_t *Reg)
{
	return *(volatile uint32_t *)Reg;
}

static inline void Pmu_Reg32Write(uint32_t *Reg, uint32_t Val)
{
	*(volatile uint32_t *)Reg = Val;
}

static void TouchOcmEccDbitErrorIntEn(void)
{
	/*ocm ecc and dbit intr enable*/
	CfgCtrlOcm.reg = 0x0;
	CfgCtrlOcm.ecc_en = 0x1;
	CfgCtrlOcm.dbit_intr_en = 0x1;
	Pmu_Reg32Write(CFG_CTRL_OCM_ADDR, CfgCtrlOcm.reg);
	printf("CFG_CTRL_OCM_ADDR 0x%x\r\n",Pmu_Reg32Read(CFG_CTRL_OCM_ADDR));
	/*write ocm*/
	Pmu_Reg32Write(OCM_START_ADDR, 0X11);
	printf("OCM_START_ADDR 0x%x\r\n",Pmu_Reg32Read(OCM_START_ADDR));
	/*ocm ecc disable*/
	CfgCtrlOcm.ecc_en = 0x0;
	Pmu_Reg32Write(CFG_CTRL_OCM_ADDR, CfgCtrlOcm.reg);
	printf("CFG_CTRL_OCM_ADDR 0x%x\r\n",Pmu_Reg32Read(CFG_CTRL_OCM_ADDR));
	/*write ocm*/
	Pmu_Reg32Write(OCM_START_ADDR, 0X1111);
	printf("OCM_START_ADDR 0x%x\r\n",Pmu_Reg32Read(OCM_START_ADDR));
	/*delay for ecc calc*/
	/*ocm ecc enable, generate error*/
	// CfgCtrlOcm.reg = Pmu_Reg32Read(CFG_CTRL_OCM_ADDR);
	CfgCtrlOcm.ecc_en = 0x1;
	Pmu_Reg32Write(CFG_CTRL_OCM_ADDR, CfgCtrlOcm.reg);
	printf("CFG_CTRL_OCM_ADDR 0x%x\r\n",Pmu_Reg32Read(CFG_CTRL_OCM_ADDR));
	printf("OCM_START_ADDR 0x%x\r\n",Pmu_Reg32Read(OCM_START_ADDR));	//touch a bus error
}

uint32_t AlPmu_OcmEccIntTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t count = 0;

	/*int disable*/
	Pmu_Reg32Write(INT_EN0_CLR_ADDR, ERR_OCM_ECC_MASK);
	/*hw disable*/
	Pmu_Reg32Write(ERR_HW_EN0_CLR_ADDR, ERR_OCM_ECC_MASK);
	/*read clear his*/
	// Pmu_Reg32Read(RAW_HIS0_ADDR);

	printf("-----TouchOcmEccDbitErrorIntEn-----\r\n");
	TouchOcmEccDbitErrorIntEn();
	/*delay for status update*/
	RawHis0.reg = Pmu_Reg32Read(RAW_HIS0_ADDR);
	while(RawHis0.err_ocm_ecc == 0 && count < 20){
		Mtimer_Delay(1000000);
		printf("wait for ocm ecc flag 5s!\r\n");
		// RawHis0.reg = Pmu_Reg32Read(RAW_HIS0_ADDR);
		count++;
	}
	count = 0;
	/*check his*/
	// RawHis0.reg = Pmu_Reg32Read(RAW_HIS0_ADDR);
	if (RawHis0.err_ocm_ecc == 0) {
		printf("Ocm Dbit Ecc Error not touch\r\n");
	} else {
		printf("Ocm Dbit Ecc Error touch\r\n");
		// break;
	}
	printf("-----End of TouchOcmEccDbitErrorIntEn-----\r\n");

	return Status;
}

#ifndef __riscv
void ApuNormalIntHandler(void)
{
	IntState0.reg = Pmu_Reg32Read(INT_STATE0_ADDR);
	printf("Cur Int state 0x%x\r\n", IntState0.reg);
	if (IntState0.reg == 0x0) {
		printf("err touch pmu intr\r\n");
		return;
	}
	if (IntState0.int_apu_ecc == 0x1) {
		/*mask*/
		Pmu_Reg32Write(INT_MSK0_SET_ADDR, ERR_APU_ECC_MASK);
		printf("apu normal int apu ecc error intr\r\n");
		/*clear int*/
		Pmu_Reg32Write(INT_CLR0_ADDR, ERR_APU_ECC_MASK);
		/*clear mask*/
		Pmu_Reg32Write(INT_MSK0_CLR_ADDR, ERR_APU_ECC_MASK);
		/*proc done*/
		SwPmuShack.reg = 0x0;
		SwPmuShack.proc_done = 0x1;
		Pmu_Reg32Write(SW_PMU_SHACK_ADDR, SwPmuShack.reg);
		IntTouchFlag.err_hw_en_apu_ecc = 0x1;
	} else if (IntState0.int_bus_timeout == 0x1) {
		/*mask*/
		Pmu_Reg32Write(INT_MSK0_SET_ADDR, ERR_BUS_TIMEOUT_MASK);
		printf("apu normal int bus timeout intr\r\n");
		/*clear int*/
		Pmu_Reg32Write(INT_CLR0_ADDR, ERR_BUS_TIMEOUT_MASK);
		/*restore gp protect*/
		TopNsPlsPort.reg = Pmu_Reg32Read(TOP_NS_PLS_PROT_ADDR);
		TopNsPlsPort.gp_proten = 0x1;
		Pmu_Reg32Write(TOP_NS_PLS_PROT_ADDR, TopNsPlsPort.reg);
		/*clear mask*/
		Pmu_Reg32Write(INT_MSK0_CLR_ADDR, ERR_BUS_TIMEOUT_MASK);
		/*proc done*/
		SwPmuShack.reg = 0x0;
		SwPmuShack.proc_done = 0x1;
		Pmu_Reg32Write(SW_PMU_SHACK_ADDR, SwPmuShack.reg);
		IntTouchFlag.err_hw_en_bus_timeout = 0x1;
	} else if (IntState0.int_ocm_ecc == 0x1) {
		/*mask*/
		Pmu_Reg32Write(INT_MSK0_SET_ADDR, ERR_OCM_ECC_MASK);
		printf("apu normal int ocm ecc intr\r\n");
		/*clear int*/
		Pmu_Reg32Write(INT_CLR0_ADDR, ERR_OCM_ECC_MASK);
		/*clear mask*/
		Pmu_Reg32Write(INT_MSK0_CLR_ADDR, ERR_OCM_ECC_MASK);
		/*proc done*/
		SwPmuShack.reg = 0x0;
		SwPmuShack.proc_done = 0x1;
		Pmu_Reg32Write(SW_PMU_SHACK_ADDR, SwPmuShack.reg);
		IntTouchFlag.err_hw_en_ocm_ecc = 0x1;
	} else if (IntState0.int_wdt0_ot == 0x1) {
		/*mask*/
		Pmu_Reg32Write(INT_MSK0_SET_ADDR, ERR_WDT0_OT_MASK);
		printf("apu normal int wdt0 intr\r\n");
		/*clear int*/
		Pmu_Reg32Write(INT_CLR0_ADDR, ERR_WDT0_OT_MASK);
		/*stop wdt*/
		wdt_stop(WDT);
		/*wdt pause*/
		CfgCtrlWdt.reg = Pmu_Reg32Read(CFG_CTRL_WDT_ADDR);
		CfgCtrlWdt.ctrl_wdt_0 = 0x1;
		Pmu_Reg32Write(CFG_CTRL_WDT_ADDR, CfgCtrlWdt.reg);
		/*clear mask*/
		Pmu_Reg32Write(INT_MSK0_CLR_ADDR, ERR_WDT0_OT_MASK);
		/*proc done*/
		SwPmuShack.reg = 0x0;
		SwPmuShack.proc_done = 0x1;
		Pmu_Reg32Write(SW_PMU_SHACK_ADDR, SwPmuShack.reg);
		IntTouchFlag.err_hw_en_wdt0_ot = 0x1;
	} else if (IntState0.int_wdt1_ot == 0x1) {
		/*mask*/
		Pmu_Reg32Write(INT_MSK0_SET_ADDR, ERR_WDT1_OT_MASK);
		printf("apu normal int wdt1 intr\r\n");
		/*clear int*/
		Pmu_Reg32Write(INT_CLR0_ADDR, ERR_WDT1_OT_MASK);
		/*stop wdt*/
		wdt_stop(WDT1);
		/*wdt pause*/
		CfgCtrlWdt.reg = Pmu_Reg32Read(CFG_CTRL_WDT_ADDR);
		CfgCtrlWdt.ctrl_wdt_1 = 0x1;
		Pmu_Reg32Write(CFG_CTRL_WDT_ADDR, CfgCtrlWdt.reg);
		/*clear mask*/
		Pmu_Reg32Write(INT_MSK0_CLR_ADDR, ERR_WDT1_OT_MASK);
		/*proc done*/
		SwPmuShack.reg = 0x0;
		SwPmuShack.proc_done = 0x1;
		Pmu_Reg32Write(SW_PMU_SHACK_ADDR, SwPmuShack.reg);
		IntTouchFlag.err_hw_en_wdt1_ot = 0x1;
	} else if (IntState0.int_wdt2_ot == 0x1) {
		/*mask*/
		Pmu_Reg32Write(INT_MSK0_SET_ADDR, ERR_WDT2_OT_MASK);
		printf("apu normal int wdt2 intr\r\n");
		/*clear int*/
		Pmu_Reg32Write(INT_CLR0_ADDR, ERR_WDT2_OT_MASK);
		/*stop wdt*/
		wdt_stop(WDT2);
		/*wdt pause*/
		CfgCtrlWdt.reg = Pmu_Reg32Read(CFG_CTRL_WDT_ADDR);
		CfgCtrlWdt.ctrl_wdt_2 = 0x1;
		Pmu_Reg32Write(CFG_CTRL_WDT_ADDR, CfgCtrlWdt.reg);
		/*clear mask*/
		Pmu_Reg32Write(INT_MSK0_CLR_ADDR, ERR_WDT2_OT_MASK);
		/*proc done*/
		SwPmuShack.reg = 0x0;
		SwPmuShack.proc_done = 0x1;
		Pmu_Reg32Write(SW_PMU_SHACK_ADDR, SwPmuShack.reg);
		IntTouchFlag.err_hw_en_wdt2_ot = 0x1;
	} else {
		printf("other uncover int:0x%x\r\n", IntState0.reg);
	}

	return;
}

uint32_t ApuNormalInt_OcmEccIntTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;

	/*clear touch flag*/
	IntTouchFlag.err_hw_en_ocm_ecc = 0;
	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_OCM_ECC_MASK);
	/*hw disable*/
	Pmu_Reg32Write(ERR_HW_EN0_CLR_ADDR, ERR_OCM_ECC_MASK);
	/*generate dbit error*/
	TouchOcmEccDbitErrorIntEn();
	/*wait for ocm ecc intr touch*/
	while(!IntTouchFlag.err_hw_en_ocm_ecc);
	/*int disable*/
	Pmu_Reg32Write(INT_EN0_CLR_ADDR, ERR_OCM_ECC_MASK);

	return Status;
}

void Xmonitor_handler()
{
	AlAxiMon_IntrTimeOutMask(AL_APU);
	AlAxiMon_IntrClear(AL_APU);
	AlAxiMon_IntrDisable(AL_APU);
	printf("Xmonitor_handler:0x%x\r\n", Pmu_Reg32Read(0xf84450f0));
}
void SetupIntr()
{
	AlAxiMon_IntrRespUnMask(AL_APU);
	AlAxiMon_IntrTimeOutUnMask(AL_APU);
	AlAxiMon_IntrRespMask(AL_APU);
	// AlAxiMon_IntrTimeOutMask(AL_APU);
	AlAxiMon_IntrEnable(AL_APU);
	ECLIC_Register_IRQ(SOC_INT129_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,Xmonitor_handler);
	__enable_irq();
}
void config_monitor(AXIMON_TypeDef *AXIMON, uint16_t id_max, uint16_t id_min, uint16_t gid_max, uint16_t gid_min, uint8_t qos_max, uint32_t thre)
{
	AlAxiMon_ResptimeRecordClean(AXIMON);
	AlAxiMon_ResptimeRecordEnble(AXIMON);
	AlAxiMon_QosValue(AXIMON,qos_max);
	AlAxiMon_QosValueRange(AXIMON,qos_max,0);
	AlAxiMon_QosCfgEnable(AXIMON,QosInternal);
	AlAxiMon_SbuparaCountClean(AXIMON);
	AlAxiMon_SubparaAwidRange(AXIMON,id_max,id_min);
	AlAxiMon_SubparaAridRange(AXIMON,id_max,id_min);
	AlAxiMon_SubparaAwgidRange(AXIMON,gid_max,gid_min);
	AlAxiMon_SubparaArgidRange(AXIMON,gid_max,gid_min);
	AlAxiMon_SubparaAwbtypeRange(AXIMON,XTYPE_MAX,0);//incr
	AlAxiMon_SubparaAwbsizeRange(AXIMON,XSIZE_MAX,0);//2^2
	AlAxiMon_SubparaAwblenRange(AXIMON,XLEN_MAX,0);
	AlAxiMon_SubparaArbtypeRange(AXIMON,XTYPE_MAX,0);
	AlAxiMon_SubparaArbsizeRange(AXIMON,XSIZE_MAX,0);
	AlAxiMon_SubparaArblenRange(AXIMON,XLEN_MAX,0);
	AlAxiMon_SbuparaCountEnable(AXIMON);
	AlAxiMon_SetTimeOutValue(AXIMON,thre);
}

uint32_t ApuNormalInt_BusTimeoutTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Value = 0;

	/*clear touch flag*/
	IntTouchFlag.err_hw_en_bus_timeout = 0;
	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_BUS_TIMEOUT_MASK);
	/*hw disable*/
	Pmu_Reg32Write(ERR_HW_EN0_CLR_ADDR, ERR_BUS_TIMEOUT_MASK);
	/*xmon config*/
	SetupIntr();
	config_monitor(AL_APU,XID_MAX,0, 5,5,XQOS_MAX, TIMEOUT_THRE);
	/*disable gp protect en*/
	// TopNsPlsPort.reg = Pmu_Reg32Read(TOP_NS_PLS_PROT_ADDR);
	// TopNsPlsPort.gp_proten = 0x0;
	// Pmu_Reg32Write(TOP_NS_PLS_PROT_ADDR, TopNsPlsPort.reg);
	// // Value = Pmu_Reg32Read(0x80008100);
	// printf("gp value is 0x%x\r\n", Pmu_Reg32Read(0x80004100));
	// Value = Pmu_Reg32Read(0xf8800168);
	// Value |= (0x1 << 9);
	// Pmu_Reg32Write(0xf8800168, Value);
	// Pmu_Reg32Write(0x640007f8, 0x0);
	// Value = Pmu_Reg32Read(0x64080000);
	/*write data to GP0, case bus timeout for none pl partition*/
	// Pmu_Reg32Write(0xf04b000, 0x1);
	/*wait for ocm ecc intr touch*/
	while(!IntTouchFlag.err_hw_en_bus_timeout);

	return Status;
}

uint32_t ApuNormalInt_Wdt0Test(void)
{
	uint32_t Status = AL_PMU_SUCCESS;

	/*clear touch flag*/
	IntTouchFlag.err_hw_en_wdt0_ot = 0;
	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_WDT0_OT_MASK);
	/*hw disable*/
	Pmu_Reg32Write(ERR_HW_EN0_CLR_ADDR, ERR_WDT0_OT_MASK);
	/*wdt pause release*/
	CfgCtrlWdt.reg = Pmu_Reg32Read(CFG_CTRL_WDT_ADDR);
	CfgCtrlWdt.ctrl_wdt_0 = 0x0;
	Pmu_Reg32Write(CFG_CTRL_WDT_ADDR, CfgCtrlWdt.reg);
	/*set and enable wdt*/
	wdt_init(WDT, WDT_IRQn);
	/*wait for ocm ecc intr touch*/
	while(!IntTouchFlag.err_hw_en_wdt0_ot);

	return Status;
}

uint32_t ApuNormalInt_Wdt1Test(void)
{
	uint32_t Status = AL_PMU_SUCCESS;

	/*clear touch flag*/
	IntTouchFlag.err_hw_en_wdt1_ot = 0;
	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_WDT1_OT_MASK);
	/*hw disable*/
	Pmu_Reg32Write(ERR_HW_EN0_CLR_ADDR, ERR_WDT1_OT_MASK);
	/*wdt pause release*/
	CfgCtrlWdt.reg = Pmu_Reg32Read(CFG_CTRL_WDT_ADDR);
	CfgCtrlWdt.ctrl_wdt_1 = 0x0;
	Pmu_Reg32Write(CFG_CTRL_WDT_ADDR, CfgCtrlWdt.reg);
	/*set and enable wdt*/
	wdt_init(WDT1, WDT1_IRQn);
	/*wait for ocm ecc intr touch*/
	while(!IntTouchFlag.err_hw_en_wdt1_ot);

	return Status;
}

uint32_t ApuNormalInt_Wdt2Test(void)
{
	uint32_t Status = AL_PMU_SUCCESS;

	/*clear touch flag*/
	IntTouchFlag.err_hw_en_wdt2_ot = 0;
	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_WDT2_OT_MASK);
	/*hw disable*/
	Pmu_Reg32Write(ERR_HW_EN0_CLR_ADDR, ERR_WDT2_OT_MASK);
	/*wdt pause release*/
	CfgCtrlWdt.reg = Pmu_Reg32Read(CFG_CTRL_WDT_ADDR);
	CfgCtrlWdt.ctrl_wdt_2 = 0x0;
	Pmu_Reg32Write(CFG_CTRL_WDT_ADDR, CfgCtrlWdt.reg);
	/*set and enable wdt*/
	wdt_init(WDT2, WDT2_IRQn);
	/*wait for ocm ecc intr touch*/
	while(!IntTouchFlag.err_hw_en_wdt2_ot);

	return Status;
}

void ninterrupt_handler(void)
{
	printf("ninterrupt_handler happened \r\n");
	printf("ninterrupt test pass\r\n");
}

static unsigned long parse_par_el1(unsigned long val)
{
	extern char _text_start[], _text_end[], _rodata_end[];
	extern char _data_start[], _sp[];

	int ret = val & 1;
	printf("Address Translation instruction %s \r\n", (ret == 0) ? "successfully" : "failed");
	if (ret)
		return -1;

	unsigned long pa = (val & PAGE_MASK) & 0xffffffffff;
	printf("par_el1:0x%lx, PA: 0x%lx\r\n", val, pa);

	printf("  NS=%u\r\n", (val >> 9) & 1);
	printf("  SH=%u\r\n", (val >> 7) & 3);
	printf("  ATTR=0x%x\r\n", val >> 56);

	return pa;
}

uint32_t ApuNormalInt_ApuEccTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;

	/*clear touch flag*/
	IntTouchFlag.err_hw_en_apu_ecc = 0;
	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_APU_ECC_MASK);
	/*hw enable*/
	Pmu_Reg32Write(ERR_HW_EN0_CLR_ADDR, ERR_APU_ECC_MASK);
	/*touch apu ecc error*/
	ECLIC_Register_IRQ(SOC_INT23_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, ninterrupt_handler);
	__enable_irq();
	paging_init();
	unsigned long v = mmu_at_test((unsigned long)0x38300240);
	unsigned long pa = parse_par_el1(v);
	ninterrupt_trigger(0x38300240);
	/*wait for ocm ecc intr touch*/
	while(!IntTouchFlag.err_hw_en_apu_ecc);

	return Status;
}

uint32_t AlPmu_ApuNormalIntTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Value = 0;

	/*register pmu intr*/
	ECLIC_Register_IRQ(SOC_INT40_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, ApuNormalIntHandler);
	__enable_irq();

/*----------ApuNormalInt_OcmEccIntTest*/
	// printf("-----ApuNormalInt_OcmEccIntTest-----\r\n");
	// ApuNormalInt_OcmEccIntTest();
	// printf("-----End of ApuNormalInt_OcmEccIntTest-----\r\n");
/*----------ApuNormalInt_BusTimeoutTest*/
	printf("-----ApuNormalInt_BusTimeoutTest-----\r\n");
	ApuNormalInt_BusTimeoutTest();
	printf("-----End of ApuNormalInt_BusTimeoutTest-----\r\n");
/*----------ApuNormalInt_Wdt0Test*/
	// printf("-----ApuNormalInt_Wdt0Test-----\r\n");
	// ApuNormalInt_Wdt0Test();
	// printf("-----End of ApuNormalInt_Wdt0Test-----\r\n");
/*----------ApuNormalInt_Wdt1Test*/
	// printf("-----ApuNormalInt_Wdt1Test-----\r\n");
	// ApuNormalInt_Wdt1Test();
	// printf("-----End of ApuNormalInt_Wdt1Test-----\r\n");
/*----------ApuNormalInt_Wdt2Test*/
	// printf("-----ApuNormalInt_Wdt2Test-----\r\n");
	// ApuNormalInt_Wdt2Test();
	// printf("-----End of ApuNormalInt_Wdt2Test-----\r\n");
/*----------ApuNormalInt_ApuEccTest*/
	// printf("-----ApuNormalInt_ApuEccTest-----\r\n");
	// ApuNormalInt_ApuEccTest();
	// printf("-----End of ApuNormalInt_ApuEccTest-----\r\n");

	return Status;
}

void ApuFatalIntHandler(void)
{
	IntState0.reg = Pmu_Reg32Read(INT_STATE0_ADDR);
	if (IntState0.reg == 0x0) {
		printf("err touch pmu intr\r\n");
		return;
	}
	if (IntState0.int_bus_timeout == 0x1) {
		printf("[fatal] int bus timeout error intr\r\n");
	} else if (IntState0.int_wdt0_ot == 0x1) {
		printf("[fatal] int wdt0 error intr\r\n");
	} else if (IntState0.int_wdt1_ot == 0x1) {
		printf("[fatal] int wdt1 error intr\r\n");
	} else if (IntState0.int_wdt2_ot == 0x1) {
		printf("[fatal] int wdt2 error intr\r\n");
	} else if (IntState0.int_apu_ecc == 0x1) {
		printf("[fatal] int apu ecc error intr\r\n");
	} else if (IntState0.int_ocm_ecc == 0x1) {
		printf("[fatal] int apu ecc error intr\r\n");
	} else {
		printf("other uncover int\r\n");
	}
}

uint32_t AlPmu_ApuFatalBusTimeoutTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Value = 0;

	/*register pmu intr*/
	ECLIC_Register_IRQ(SOC_INT39_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, ApuFatalIntHandler);
	__enable_irq();

	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_BUS_TIMEOUT_MASK);
	/*hw enable*/
	Pmu_Reg32Write(ERR_HW_EN0_SET_ADDR, ERR_BUS_TIMEOUT_MASK);
	/*disable gp protect en*/
	// TopNsPlsPort.reg = Pmu_Reg32Read(TOP_NS_PLS_PROT_ADDR);
	// TopNsPlsPort.gp_proten = 0x0;
	// Pmu_Reg32Write(TOP_NS_PLS_PROT_ADDR, TopNsPlsPort.reg);
	/*write data to GP0, case bus timeout for none pl partition*/
	// Pmu_Reg32Write(GP0_START_ADDR, 0x1234);
	/*xmon config*/
	SetupIntr();
	config_monitor(AL_APU,XID_MAX,0, 5,5,XQOS_MAX, TIMEOUT_THRE);

	return Status;
}

uint32_t AlPmu_ApuFatalWdt0Test(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Value = 0;

	/*register pmu intr*/
	ECLIC_Register_IRQ(SOC_INT39_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, ApuFatalIntHandler);
	__enable_irq();

	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_WDT0_OT_MASK);
	/*hw enable*/
	Pmu_Reg32Write(ERR_HW_EN0_SET_ADDR, ERR_WDT0_OT_MASK);
	/*wdt pause release*/
	CfgCtrlWdt.reg = Pmu_Reg32Read(CFG_CTRL_WDT_ADDR);
	CfgCtrlWdt.ctrl_wdt_0 = 0x0;
	Pmu_Reg32Write(CFG_CTRL_WDT_ADDR, CfgCtrlWdt.reg);
	/*set and enable wdt*/
	wdt_init(WDT, WDT_IRQn);

	return Status;
}

uint32_t AlPmu_ApuFatalWdt1Test(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Value = 0;

	/*register pmu intr*/
	ECLIC_Register_IRQ(SOC_INT39_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, ApuFatalIntHandler);
	__enable_irq();

	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_WDT1_OT_MASK);
	/*hw enable*/
	Pmu_Reg32Write(ERR_HW_EN0_SET_ADDR, ERR_WDT1_OT_MASK);
	/*wdt pause release*/
	CfgCtrlWdt.reg = Pmu_Reg32Read(CFG_CTRL_WDT_ADDR);
	CfgCtrlWdt.ctrl_wdt_1 = 0x0;
	Pmu_Reg32Write(CFG_CTRL_WDT_ADDR, CfgCtrlWdt.reg);
	/*set and enable wdt*/
	wdt_init(WDT1, WDT1_IRQn);

	return Status;
}

uint32_t AlPmu_ApuFatalWdt2Test(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Value = 0;

	/*register pmu intr*/
	ECLIC_Register_IRQ(SOC_INT39_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, ApuFatalIntHandler);
	__enable_irq();

	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_WDT2_OT_MASK);
	/*hw enable*/
	Pmu_Reg32Write(ERR_HW_EN0_SET_ADDR, ERR_WDT2_OT_MASK);
	/*wdt pause release*/
	CfgCtrlWdt.reg = Pmu_Reg32Read(CFG_CTRL_WDT_ADDR);
	CfgCtrlWdt.ctrl_wdt_2 = 0x0;
	Pmu_Reg32Write(CFG_CTRL_WDT_ADDR, CfgCtrlWdt.reg);
	/*set and enable wdt*/
	wdt_init(WDT2, WDT2_IRQn);

	return Status;
}

uint32_t AlPmu_ApuFatalApuEccTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Value = 0;

	/*register pmu intr*/
	ECLIC_Register_IRQ(SOC_INT39_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, ApuFatalIntHandler);
	__enable_irq();

	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_APU_ECC_MASK);
	/*hw enable*/
	Pmu_Reg32Write(ERR_HW_EN0_SET_ADDR, ERR_APU_ECC_MASK);
	/*touch apu ecc error*/
	ECLIC_Register_IRQ(SOC_INT23_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, ninterrupt_handler);
	__enable_irq();
	paging_init();
	unsigned long v = mmu_at_test((unsigned long)0x38300240);
	unsigned long pa = parse_par_el1(v);
	ninterrupt_trigger(0x38300240);

	return Status;
}

uint32_t AlPmu_ApuFatalOcmEccTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Value = 0;

	/*register pmu intr*/
	ECLIC_Register_IRQ(SOC_INT39_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, ApuFatalIntHandler);
	__enable_irq();

	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_OCM_ECC_MASK);
	/*hw enable*/
	Pmu_Reg32Write(ERR_HW_EN0_SET_ADDR, ERR_OCM_ECC_MASK);
	/*generate dbit error*/
	TouchOcmEccDbitErrorIntEn();

	return Status;
}
#endif /*apu*/

#ifdef __riscv
uint32_t RpuEcc_Enable2BitEcc(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Rdata = 0;
	uint32_t Wdata = 0;

	printf("-----RpuEcc_Enable2BitEcc-----\r\n");
	Rdata = __RV_CSR_READ(0x7DD);
	printf("Rdata_0x7DD = 0x%x\r\n", Rdata);
	Wdata = Rdata | 0xF;
	__RV_CSR_WRITE(0x7DD, Wdata);
	printf("Wdata_0x7DD = 0x%x\r\n", Wdata);
	Rdata = __RV_CSR_READ(0x7DD);
	printf("Rdata_0x7DD = 0x%x\r\n", Rdata);
	Mtimer_Delay(5);

	Rdata = Pmu_Reg32Read(STATE_RPU_ADDR);
	printf("State rpu is 0x%x\r\n", Rdata);

	return Status;
}

uint32_t RpuEcc_EnableIlmEcc(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Rdata = 0;
	uint32_t Wdata = 0;

	printf("-----RpuEcc_EnableIlmEcc-----\r\n");
	Rdata = __RV_CSR_READ(0x7C0);
	printf("Rdata_0x7C0 = 0x%x\r\n", Rdata);
	Wdata = Rdata | 0xF;
	__RV_CSR_WRITE(0x7C0, Wdata);
	printf("Wdata_0x7C0 = 0x%x\r\n", Wdata);
	Rdata = __RV_CSR_READ(0x7C0);
	Mtimer_Delay(10);
	printf("Rdata_0x7C0 = 0x%x\r\n", Rdata);

	return Status;
}

uint32_t RpuEcc_IlmEccTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Rdata = 0;
	uint32_t Wdata = 0;

	printf("-----RpuEcc_IlmEccTest-----\r\n");
	RpuEcc_EnableIlmEcc();
	__SD(ILM_START_ADDR, 0x5F3C0A55AA33CCFF);
	Mtimer_Delay(20);
	__LD(ILM_START_ADDR);
	Mtimer_Delay(50);

	Rdata = Pmu_Reg32Read(STATE_RPU_ADDR);
	printf("State rpu is 0x%x\r\n", Rdata);

	return Status;
}

uint32_t RpuEcc_EnableDlmEcc(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Rdata = 0;
	uint32_t Wdata = 0;

	printf("-----RpuEcc_EnableDlmEcc-----\r\n");
	Rdata = __RV_CSR_READ(0x7C1);
	printf("Rdata_0x7C1 = 0x%x\r\n", Rdata);
	Wdata = Rdata | 0xF;
	__RV_CSR_WRITE(0x7C1, Wdata);
	printf("Wdata_0x7C1 = 0x%x\r\n", Wdata);
	Rdata = __RV_CSR_READ(0x7C1);
	printf("Rdata_0x7C1 = 0x%x\r\n", Rdata);
	Mtimer_Delay(10);

	return Status;
}

uint32_t RpuEcc_DlmEccTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Rdata = 0;
	uint32_t Wdata = 0;

	printf("-----RpuEcc_DlmEccTest-----\r\n");
	RpuEcc_EnableDlmEcc();
	__SD(DLM_START_ADDR, 0x5F3C0A55AA33CCFF);
	Mtimer_Delay(20);
	__LD(DLM_START_ADDR);
	Mtimer_Delay(50);

	Rdata = Pmu_Reg32Read(STATE_RPU_ADDR);
	printf("State rpu is 0x%x\r\n", Rdata);

	return Status;
}

uint32_t RpuEcc_EnableICacheEcc(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Rdata = 0;
	uint32_t Wdata = 0;

	printf("-----RpuEcc_EnableICacheEcc-----\r\n");
	Rdata = __RV_CSR_READ(0x7CA);
	printf("Rdata_0x7CA = 0x%x\r\n", Rdata);
	Wdata = Rdata | 0x3F;
	__RV_CSR_WRITE(0x7CA, Wdata);
	printf("Wdata_0x7CA = 0x%x\r\n", Wdata);
	Rdata = __RV_CSR_READ(0x7CA);
	printf("Rdata_0x7CA = 0x%x\r\n", Rdata);
	Mtimer_Delay(10);

	Rdata = __RV_CSR_READ(0x7CB);
	printf("Rdata_0x7CB = 0x%x\r\n", Rdata);
	Mtimer_Delay(10);
	__RV_CSR_WRITE(0x7CB, ILM_START_ADDR);
	Mtimer_Delay(10);

	Rdata = __RV_CSR_READ(0x7CC);
	printf("Rdata_0x7CC = 0x%x\r\n", Rdata);
	Mtimer_Delay(10);
	Wdata = Rdata | 0xB;
	__RV_CSR_WRITE(0x7CC, ILM_START_ADDR);
	Rdata = __RV_CSR_READ(0x7CC);
	printf("Rdata_0x7CC = 0x%x\r\n", Rdata);
	Mtimer_Delay(10);

	Rdata = __RV_CSR_READ(0x7CD);
	printf("Rdata_0x7CD = 0x%x\r\n", Rdata);
	Mtimer_Delay(50);

	return Status;
}

uint32_t RpuEcc_ICacheEccTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Rdata = 0;
	uint32_t Wdata = 0;

	printf("-----RpuEcc_ICacheEccTest-----\r\n");
	RpuEcc_EnableICacheEcc();
	Rdata = Pmu_Reg32Read(STATE_RPU_ADDR);
	printf("State rpu is 0x%x\r\n", Rdata);

	RpuEcc_EnableICacheEcc();
	Rdata = Pmu_Reg32Read(STATE_RPU_ADDR);
	printf("State rpu is 0x%x\r\n", Rdata);

	return Status;
}

uint32_t RpuEcc_EnableDCacheEcc(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Rdata = 0;
	uint32_t Wdata = 0;

	printf("-----RpuEcc_EnableDCacheEcc-----\r\n");
	Rdata = __RV_CSR_READ(0x7CA);
	printf("Rdata_0x7CA = 0x%x\r\n", Rdata);
	Wdata = Rdata | (0x3F << 16);
	__RV_CSR_WRITE(0x7CA, Wdata);
	printf("Wdata_0x7CA = 0x%x\r\n", Wdata);
	Rdata = __RV_CSR_READ(0x7CA);
	printf("Rdata_0x7CA = 0x%x\r\n", Rdata);
	Mtimer_Delay(10);

	Rdata = __RV_CSR_READ(0x7CB);
	printf("Rdata_0x7CB = 0x%x\r\n", Rdata);
	Mtimer_Delay(10);
	__RV_CSR_WRITE(0x7CB, DLM_START_ADDR);
	Mtimer_Delay(10);

	Rdata = __RV_CSR_READ(0x7CC);
	printf("Rdata_0x7CC = 0x%x\r\n", Rdata);
	Mtimer_Delay(10);
	Wdata = Rdata | 0x3;
	__RV_CSR_WRITE(0x7CC, DLM_START_ADDR);
	Rdata = __RV_CSR_READ(0x7CC);
	printf("Rdata_0x7CC = 0x%x\r\n", Rdata);
	Mtimer_Delay(10);

	Rdata = __RV_CSR_READ(0x7CD);
	printf("Rdata_0x7CD = 0x%x\r\n", Rdata);
	Mtimer_Delay(50);

	return Status;
}

uint32_t RpuEcc_DCacheEccTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Rdata = 0;
	uint32_t Wdata = 0;

	printf("-----RpuEcc_EnableIlmEcc-----\r\n");
	RpuEcc_EnableDCacheEcc();
	Rdata = Pmu_Reg32Read(STATE_RPU_ADDR);
	printf("State rpu is 0x%x\r\n", Rdata);

	RpuEcc_EnableDCacheEcc();
	Rdata = Pmu_Reg32Read(STATE_RPU_ADDR);
	printf("State rpu is 0x%x\r\n", Rdata);

	return Status;
}

uint32_t RpuNormalInt_RpuEccTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;

	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_RPU_FATAL_MASK);
	/*hw disable*/
	Pmu_Reg32Write(ERR_HW_EN0_CLR_ADDR, ERR_RPU_FATAL_MASK);
	/*enable rpu 2 bit ecc*/
	RpuEcc_Enable2BitEcc();
	/*rpu ilm ecc error test*/
	IntTouchFlag.err_hw_en_rpu_fatal = 0;
	RpuEcc_IlmEccTest();
	while(!IntTouchFlag.err_hw_en_rpu_fatal);

	/*rpu ilm ecc error test*/
	IntTouchFlag.err_hw_en_rpu_fatal = 0;
	RpuEcc_DlmEccTest();
	while(!IntTouchFlag.err_hw_en_rpu_fatal);

	/*rpu ilm ecc error test*/
	IntTouchFlag.err_hw_en_rpu_fatal = 0;
	RpuEcc_ICacheEccTest();
	while(!IntTouchFlag.err_hw_en_rpu_fatal);

	/*rpu ilm ecc error test*/
	IntTouchFlag.err_hw_en_rpu_fatal = 0;
	RpuEcc_DCacheEccTest();
	while(!IntTouchFlag.err_hw_en_rpu_fatal);

	return Status;
}

void RpuNormalIntHandler(void)
{
	IntState0.reg = Pmu_Reg32Read(INT_STATE0_ADDR);
	if (IntState0.reg == 0x0) {
		printf("err touch pmu intr\r\n");
		return;
	}
	if (IntState0.int_rpu_fatal == 0x1) {
		/*mask*/
		Pmu_Reg32Write(INT_MSK0_SET_ADDR, ERR_RPU_FATAL_MASK);
		printf("rpu normal int rpu ecc error intr\r\n");
		/*clear int*/
		Pmu_Reg32Write(INT_CLR0_ADDR, ERR_RPU_FATAL_MASK);
		/*clear mask*/
		Pmu_Reg32Write(INT_MSK0_CLR_ADDR, ERR_RPU_FATAL_MASK);
		/*proc done*/
		SwPmuShack.reg = 0x0;
		SwPmuShack.proc_done = 0x1;
		Pmu_Reg32Write(SW_PMU_SHACK_ADDR, SwPmuShack.reg);
		IntTouchFlag.err_hw_en_rpu_fatal = 0x1;
	} else {
		printf("other uncover int\r\n");
	}
}

uint32_t AlPmu_RpuNormalIntTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Value = 0;
	printf("-----AlPmu_RpuNormalIntTest-----\r\n");

	/*register pmu intr*/
	ECLIC_Register_IRQ(SOC_INT40_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, RpuNormalIntHandler);
	__enable_irq();

	RpuNormalInt_RpuEccTest();

	return Status;
}

void RpuFatalIntHandler(void)
{
	IntState0.reg = Pmu_Reg32Read(INT_STATE0_ADDR);
	if (IntState0.reg == 0x0) {
		printf("err touch pmu intr\r\n");
		return;
	}
	if (IntState0.int_rpu_fatal == 0x1) {
		printf("rpu fatal int rpu ecc error intr\r\n");
	} else if (IntState0.int_wdt0_ot == 0x1) {
		// printf("rpu fatal wdt0 intr\r\n");
		/*stop wdt*/
		wdt_stop(WDT);
	} else {
		printf("other uncover intr\r\n");
	}
}

uint32_t AlPmu_RpuFatalRpuEccTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Value = 0;

	/*register pmu intr*/
	ECLIC_Register_IRQ(SOC_INT39_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, RpuFatalIntHandler);
	__enable_irq();

	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_RPU_FATAL_MASK);
	/*hw enable*/
	Pmu_Reg32Write(ERR_HW_EN0_SET_ADDR, ERR_RPU_FATAL_MASK);
	/*enable rpu 2 bit ecc*/
	RpuEcc_Enable2BitEcc();
	/*rpu ilm ecc error test*/
	RpuEcc_IlmEccTest();

	return Status;
}

void RpuNmiHandler(unsigned long mcause, unsigned long sp)
{
	printf("NMI\r\n");
    while(1);
}

uint32_t AlPmu_RpuNmiOcmEccTest(void)
{
	uint32_t Status = AL_PMU_SUCCESS;
	uint32_t Value = 0;

	/*register pmu intr*/
	ECLIC_Register_IRQ(SOC_INT39_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, RpuFatalIntHandler);
	__enable_irq();
	Exception_Register_EXC(NMI_EXCn, RpuNmiHandler);

	printf("MMISC_CTL : 0x%x\r\n", __RV_CSR_READ(CSR_MMISC_CTL));
	printf("MTVEC     : 0x%x\r\n", __RV_CSR_READ(CSR_MTVEC));
	printf("MNVEC     : 0x%x\r\n", __RV_CSR_READ(CSR_MNVEC));

	/*int enable*/
	Pmu_Reg32Write(INT_EN0_SET_ADDR, ERR_WDT0_OT_MASK);
	/*hw enable*/
	Pmu_Reg32Write(ERR_HW_EN0_SET_ADDR, ERR_WDT0_OT_MASK);
	/*set nmi*/
	Pmu_Reg32Write(PMU_NMI_EN_ADDR, PMU_NMI_EN_RPU_MASK);
	// /*enable rpu 2 bit ecc*/
	// RpuEcc_Enable2BitEcc();
	// /*rpu ilm ecc error test*/
	// RpuEcc_IlmEccTest();
	/*wdt pause release*/
	CfgCtrlWdt.reg = Pmu_Reg32Read(CFG_CTRL_WDT_ADDR);
	CfgCtrlWdt.ctrl_wdt_0 = 0x0;
	Pmu_Reg32Write(CFG_CTRL_WDT_ADDR, CfgCtrlWdt.reg);
	/*set and enable wdt*/
	wdt_init(WDT, WDT_IRQn);

	return 0;
}

#endif /*rpu*/


/*available test int*/
//bus_timeout
//wdt2_ot
//wdt1_ot
//wdt0_ot
//apu_ecc wait for check 
//rpu_fatal wait for check
//ocm_ecc
uint32_t main()
{
	uint32_t Status = AL_PMU_SUCCESS;

	/*register pmu0 handler*/
	// ECLIC_Register_IRQ(39, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, pmu_handler);
#ifdef _OCM_ECC_INT_TEST
	printf("-----ocm ecc int test-----\r\n");
	AlPmu_OcmEccIntTest();
	printf("-----end of ocm ecc int test-----\r\n");
#endif

#ifdef _APU_NORMAL_INT_TEST
	printf("-----apu normal int func test-----\r\n");
	AlPmu_ApuNormalIntTest();
	printf("-----end of apu normal int func test-----\r\n");
#endif

#ifdef _RPU_NORMAL_INT_TEST
	printf("-----rpu normal int func test-----\r\n");
	AlPmu_RpuNormalIntTest();
	printf("-----end of rpu normal int func test-----\r\n");
#endif

#ifdef _APU_FATAL_BUS_TIMEOUT_TEST
	printf("-----apu fatal bus timeout func test-----\r\n");
	AlPmu_ApuFatalBusTimeoutTest();
	printf("-----end of apu fatal bus timeout func test-----\r\n");
#endif

#ifdef _APU_FATAL_WDT0_TEST
printf("-----apu fatal wdt0 func test-----\r\n");
	AlPmu_ApuFatalWdt0Test();
	printf("-----end of apu fatal wdt0 func test-----\r\n");
#endif

#ifdef _APU_FATAL_WDT1_TEST
printf("-----apu fatal wdt0 func test-----\r\n");
	AlPmu_ApuFatalWdt1Test();
	printf("-----end of apu fatal wdt0 func test-----\r\n");
#endif

#ifdef _APU_FATAL_WDT2_TEST
printf("-----apu fatal wdt2 func test-----\r\n");
	AlPmu_ApuFatalWdt2Test();
	printf("-----end of apu fatal wdt2 func test-----\r\n");
#endif

#ifdef _APU_FATAL_APU_ECC_TEST
printf("-----apu fatal apu ecc func test-----\r\n");
	AlPmu_ApuFatalApuEccTest();
	printf("-----end of apu fatal apu ecc func test-----\r\n");
#endif

#ifdef _APU_FATAL_OCM_ECC_TEST
printf("-----apu fatal ocm ecc func test-----\r\n");
	AlPmu_ApuFatalOcmEccTest();
	printf("-----end of apu fatal ocm ecc func test-----\r\n");
#endif

#ifdef _RPU_FATAL_RPU_MULTIBIT_ECC_TEST
printf("-----rpu fatal rpu ecc func test-----\r\n");
	AlPmu_RpuFatalRpuEccTest();
	printf("-----end of rpu fatal rpu ecc func test-----\r\n");
#endif

#ifdef _RPU_NMI_OCM_ECC_TEST
	printf("-----rpu nmi ocm ecc func test-----\r\n");
	AlPmu_RpuNmiOcmEccTest();
	printf("-----end of rpu nmi ocm ecc func test-----\r\n");
#endif

	while(1);
}

/* case 1.2 */
volatile uint32_t interrupt_count=0;
volatile uint32_t int_num = 3;

void wdt_handler(void)
{
	// printf("i\r\n");
}

/**
 * \brief  This function initialize wdt
 * \param  wdt: wdt regs struct
 * \param  WDT_InitTypeDef: contains the configuration for the specified WDT peripheral
 * \retval if wdt==null or WDT_InitStruct==NULL ,return -1 ; otherwise 0;
 */
int AL9000_wdt_init(WDT_TypeDef *wdt,WDT_InitTypeDef *WDT_InitStruct)
{
	if((__RARELY(wdt == NULL))||(__RARELY(WDT_InitStruct == NULL)))
	{
		printf("[wdog]:e1");
		return -1;
	}
	//printf("i");

	volatile unsigned int read_temp = WDT->EOI;

	wdt->TORR &= TORR_CLR_MASK;

	wdt->TORR |= WDT_InitStruct->WDT_TimeOutValue;

	wdt->CR	&= CR_CLR_MASK;

	wdt->CR |= WDT_InitStruct->WDT_PuaseLength;
    wdt->CR |= WDT_InitStruct->WDT_Mode;
    wdt->CR |= WDT_CR_WDT_EN;

	wdt->CRR = WDT_CCVR_VALUE;
}

uint32_t wdt_stop(WDT_TypeDef *Wdt)
{
	uint32_t Value = 0;
	// printf("[wdog]:Stop\r\n");

	Value = Wdt->EOI;
	Value = Wdt->CR;
	Value &= ~(WDT_CR_WDT_EN);
	Wdt->CR = Value;

	return 0;
}

static void wdt_init(WDT_TypeDef *Wdt, IRQn_Type Irq)
{
	WDT_InitTypeDef WDT_InitStruct;

	volatile unsigned int read_temp = Wdt->EOI;
	ECLIC_Register_IRQ(Irq, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, wdt_handler);
	__enable_irq();

	WDT_InitStruct.WDT_PuaseLength	= WDT_CR_RPL_2;
	WDT_InitStruct.WDT_Mode		= WDT_CR_RMOD_INTERRUPT;
	WDT_InitStruct.WDT_TimeOutValue	= WDT_TORR_TOP_167M;
	AL9000_wdt_init(Wdt,&WDT_InitStruct);

	printf("[wdog]:Init\r\n");

	return ;
}
