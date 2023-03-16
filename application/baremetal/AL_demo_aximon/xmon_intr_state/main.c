/****************************************************************************/
/**
* @file non-secure_m2m test
*
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "nuclei_sdk_soc.h"
#include "al9000_aximon.h"
#include "al9000_registers.h"

#define TIMEOUT_THRE 0x1f

#define vfwp printf
#define REG_WRITE(reg_address, reg_wdata)  *(volatile unsigned int *)(reg_address) = reg_wdata
#define REG_READ(reg_address)  *(volatile unsigned int *)reg_address


/**************************** Type Definitions *******************************/

AXIMON_TypeDef *xmon_list[] = 	{AL_DDR_S0,	AL_DDR_S1,	AL_DDR_S2,	AL_DDR_S3,	AL_SX2X_M0,
								AL_SX2X_M1,	AL_OCM_S2,	AL_SH_M2,	AL_SMC,		AL_DMACX,
								AL_GP_S0,	AL_GP_S1,	AL_HP_S0,	AL_HP_S1,	AL_APU,
								AL_JPU,		AL_MAIN_S6,	AL_MAIN_M6,	AL_MAIN_S1,	AL_MAIN_M0,
								AL_ACP
								};



uint32_t xmon_irq_state[] = 	{0x2,		0x4,		0x20000,	0x40000,	0x8,
								0x10,		0x40,		0x100,		0x200,		0x400,
								0x800,		0x1000,		0x2000,		0x4000,		0x1,
								0x8000,		0x10000,	0x20,		0x80000,	0x80,
								};

static uint32_t intr_flag = 0;
static uint32_t which = 0;

void Xmonitor_handler()
{
	intr_flag = REG_READ(TOP_NS__INTR_XMON_STATE__ADDR);

	AlAxiMon_IntrClear(xmon_list[which]);

	AlAxiMon_IntrDisable(xmon_list[which]);

	printf("XMON_STATE: 0x%x		xmon base=%p \r\n", intr_flag, xmon_list[which]);
}


void Xmon_SetupIntr(AXIMON_TypeDef *xmon)
{
	AlAxiMon_IntrTimeOutUnMask(xmon);
	AlAxiMon_IntrEnable(xmon);
	ECLIC_Register_IRQ(SOC_INT129_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, Xmonitor_handler);
	__enable_irq();
}


int main(void)
{
	uint32_t midr	= 0;
	uint64_t cpunum	= 0;
	uint64_t mpidr	= 0;

	printf("check INTR_XMON_STATE \r\n");

	#ifdef __aarch64__
	uint64_t sctlr_el3	= 0;

	asm volatile("mrs %0, midr_el1":"=r"(midr)::"memory");
	asm volatile("mrs %0, mpidr_el1":"=r"(mpidr)::"memory");
	#endif

	#ifdef _AARCH_32
	uint64_t cpunum	= 0;
	uint64_t mpidr	= 0;
	asm volatile("mrc p15,0,%0,c0,c0,0":"=r"(midr)::"memory");
	asm volatile("mrc p15,0,%0,c0,c0,5":"=r"(mpidr)::"memory");
	#endif

	cpunum = mpidr & 0x00ff;

	if (cpunum == 0) {
		printf("cpunum == 0 \r\n");

	#ifdef __aarch64__
		asm volatile("mrs %0, sctlr_el3":"=r"(sctlr_el3)::"memory");
		vfwp("ctlr_el3 ** %x", (unsigned int)(sctlr_el3 & 0xffffffff));
	#endif

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],  XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],  XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],  XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],  XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which], XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which], XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],  XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],   XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],     XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],   XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],   XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],   XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],   XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],   XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],     XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],     XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which], XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which], XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which], XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);
		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which], XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		which ++;
		intr_flag = 0x0;

		printf("config xmon: %p \r\n", xmon_list[which]);

		//release acp_srst
		uint32_t r = REG_READ(CRP__SRST_CTRL0__ADDR);
		REG_WRITE(CRP__SRST_CTRL0__ADDR, (r | (0x01 << CRP__SRST_CTRL0__APU_ACP_SRST_N__SHIFT)));

		Xmon_SetupIntr(xmon_list[which]);
		config_monitor(xmon_list[which],     XID_MAX, 0, 5, 5, XQOS_MAX, TIMEOUT_THRE);
		while (intr_flag == 0x0);
		intr_flag = 0x0;
		printf("[AUTOTEST]:[XMON]:[xmon_intr_state]:[PASS]]\r\n");

		return 0;

	} else {
		vfwp("cpu[%d] running", cpunum);
		while (1);
	}

	return 0;
}