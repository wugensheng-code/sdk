#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_ttc.h"
#define TEST_TTC0
#ifdef TEST_TTC0
	#define AL9000_TTC AL9000_TTC0
#endif
#ifdef TEST_TTC1
	#define AL9000_TTC AL9000_TTC1
#endif
void SOC_TTC0_HANDLER(void)
{
	uint8_t intr_value;
	intr_value = TTC_clear_interrput(AL9000_TTC,clk1);
   	TTC_Counter_Disable(AL9000_TTC,clk1);
	// printf("TTC Value=%d\r\n",AL9000_TTC->CNTR_VAL_REG_1);
	if(intr_value == 0x1)
		printf("[AUTOTEST]:[TTC0]:[Iv_INTR]:[PASS]\r\n");
	else if(intr_value == 0x2)
		printf("[AUTOTEST]:[TTC0]:[M1_INTR]:[PASS]\r\n");
	else if(intr_value == 0x4)
		printf("[AUTOTEST]:[TTC0]:[M2_INTR]:[PASS]\r\n");
	else if(intr_value == 0x8)
		printf("[AUTOTEST]:[TTC0]:[M3_INTR]:[PASS]\r\n");
	else if(intr_value == 0x10)
		printf("[AUTOTEST]:[TTC0]:[Ov_INTR]:[PASS]\r\n");
	else if(intr_value == 0x20)
		printf("[AUTOTEST]:[TTC0]:[Ev_INTR]:[PASS]\r\n");
	else
		printf("[AUTOTEST]:[TTC0]:[INTR]:[Fail]\r\n");
}
void SOC_TTC1_HANDLER(void)
{
	uint8_t intr_value;
	intr_value = TTC_clear_interrput(AL9000_TTC,clk1);
   	TTC_Counter_Disable(AL9000_TTC,clk1);
	// printf("TTC Value=%d\r\n",AL9000_TTC->CNTR_VAL_REG_1);
	if(intr_value == 0x1)
		printf("[AUTOTEST]:[TTC1]:[Iv_INTR]:[PASS]\r\n");
	else if(intr_value == 0x2)
		printf("[AUTOTEST]:[TTC1]:[M1_INTR]:[PASS]\r\n");
	else if(intr_value == 0x4)
		printf("[AUTOTEST]:[TTC1]:[M2_INTR]:[PASS]\r\n");
	else if(intr_value == 0x8)
		printf("[AUTOTEST]:[TTC1]:[M3_INTR]:[PASS]\r\n");
	else if(intr_value == 0x10)
		printf("[AUTOTEST]:[TTC1]:[Ov_INTR]:[PASS]\r\n");
	else if(intr_value == 0x20)
		printf("[AUTOTEST]:[TTC1]:[Ev_INTR]:[PASS]\r\n");
	else
		printf("[AUTOTEST]:[TTC1]:[INTR]:[Fail]\r\n");
}
int main(void){
	// __RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
	ECLIC_Register_IRQ(SOC_INT48_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_TTC0_HANDLER);
	ECLIC_Register_IRQ(SOC_INT49_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_TTC0_HANDLER);
	ECLIC_Register_IRQ(SOC_INT50_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_TTC0_HANDLER);
	ECLIC_Register_IRQ(SOC_INT51_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_TTC1_HANDLER);
	ECLIC_Register_IRQ(SOC_INT52_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_TTC1_HANDLER);
	ECLIC_Register_IRQ(SOC_INT53_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_TTC1_HANDLER);
	__enable_irq();

#ifdef M1_intr
	TTC_Counter_Disable(AL9000_TTC,clk1);
	TTC_Presacle_Enable(AL9000_TTC,clk1);
	TTC_PS_V(AL9000_TTC,clk1,2);
	Set_TTC_wave_pol(AL9000_TTC,clk1);
	TTC_Match_Mode(AL9000_TTC,clk1);
	Set_Match_counter_level(AL9000_TTC,match1,250);//32768/65535 50%
	enable_interrupt_type(AL9000_TTC,M1,clk1);
	TTC_wave_Enable(AL9000_TTC,clk1);
	TTC_Counter_Enable(AL9000_TTC,clk1);
#endif
#ifdef M2_intr
	TTC_Counter_Disable(AL9000_TTC,clk1);
	TTC_Presacle_Enable(AL9000_TTC,clk1);
	TTC_PS_V(AL9000_TTC,clk1,2);
	Set_TTC_wave_pol(AL9000_TTC,clk1);
	TTC_Match_Mode(AL9000_TTC,clk1);
	Set_Match_counter_level(AL9000_TTC,match2,250);//32768/65535 50%
	enable_interrupt_type(AL9000_TTC,M2,clk1);
	TTC_wave_Enable(AL9000_TTC,clk1);
	TTC_Counter_Enable(AL9000_TTC,clk1);
#endif
#ifdef M3_intr
	TTC_Counter_Disable(AL9000_TTC,clk1);
	TTC_Presacle_Enable(AL9000_TTC,clk1);
	TTC_PS_V(AL9000_TTC,clk1,2);
	Set_TTC_wave_pol(AL9000_TTC,clk1);
	TTC_Match_Mode(AL9000_TTC,clk1);
	Set_Match_counter_level(AL9000_TTC,match3,250);//32768/65535 50%
	enable_interrupt_type(AL9000_TTC,M3,clk1);
	TTC_wave_Enable(AL9000_TTC,clk1);
	TTC_Counter_Enable(AL9000_TTC,clk1);
#endif
#ifdef Iv_intr
	TTC_Counter_Disable(AL9000_TTC,clk1);
	TTC_Presacle_Enable(AL9000_TTC,clk1);
	TTC_PS_V(AL9000_TTC,clk1,2);
	Set_TTC_wave_pol(AL9000_TTC,clk1);
	TTC_Interval_Mode(AL9000_TTC,clk1);
	Set_Interval_Mode_counter_level(AL9000_TTC,500,clk1);
	enable_interrupt_type(AL9000_TTC,Iv,clk1);
	TTC_wave_Enable(AL9000_TTC,clk1);
	TTC_Counter_Enable(AL9000_TTC,clk1);
#endif
#ifdef Ov_intr
	TTC_Counter_Disable(AL9000_TTC,clk1);
	TTC_Presacle_Enable(AL9000_TTC,clk1);
	TTC_PS_V(AL9000_TTC,clk1,2);
	Set_TTC_wave_pol(AL9000_TTC,clk1);
	TTC_Overflow_Mode(AL9000_TTC,clk1);
	enable_interrupt_type(AL9000_TTC,Ov,clk1);
	TTC_wave_Enable(AL9000_TTC,clk1);
	TTC_Counter_Enable(AL9000_TTC,clk1);
#endif
#ifdef Ev_intr
	TTC_Counter_Disable(AL9000_TTC,clk1);
	TTC_Clock_Select_Ext_clock(AL9000_TTC,clk1);
	TTC_Clock_Select_Ext_clock_edge(AL9000_TTC,clk1);
 	Event_Control_event_Higth_Mode(AL9000_TTC,clk1);
 	Event_Control_event_Enable(AL9000_TTC,clk1);
	enable_interrupt_type(AL9000_TTC,Ev,clk1);
	TTC_Presacle_Enable(AL9000_TTC,clk1);
	TTC_PS_V(AL9000_TTC,clk1,2);
	Set_TTC_wave_pol(AL9000_TTC,clk1);
	TTC_wave_Enable(AL9000_TTC,clk1);
	TTC_Counter_Enable(AL9000_TTC,clk1);
#endif

	return 0; 
}
