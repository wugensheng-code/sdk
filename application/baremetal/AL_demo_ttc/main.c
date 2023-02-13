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
	printf("TTC Value=%d\r\n",AL9000_TTC->CNTR_VAL_REG_1);
	if(intr_value == 0x1)
		printf("TTC0 Iv INTR\r\n");
	else if(intr_value == 0x2)
		printf("TTC0 M1 INTR\r\n");
	else if(intr_value == 0x4)
		printf("TTC0 M2 INTR\r\n");
	else if(intr_value == 0x8)
		printf("TTC0 M3 INTR\r\n");
	else if(intr_value == 0x10)
		printf("TTC0 ov INTR\r\n");
	else if(intr_value == 0x20)
		printf("TTC0 ev INTR\r\n");
	else
		printf("TTC0 INTR value = %x\r\n",intr_value);
}
void SOC_TTC1_HANDLER(void)
{
	uint8_t intr_value;
	intr_value = TTC_clear_interrput(AL9000_TTC,clk1);
   	TTC_Counter_Disable(AL9000_TTC,clk1);
	printf("TTC Value=%d\r\n",AL9000_TTC->CNTR_VAL_REG_1);
	if(intr_value == 0x1)
		printf("TTC1 Iv INTR\r\n");
	else if(intr_value == 0x2)
		printf("TTC1 M1 INTR\r\n");
	else if(intr_value == 0x4)
		printf("TTC1 M2 INTR\r\n");
	else if(intr_value == 0x8)
		printf("TTC1 M3 INTR\r\n");
	else if(intr_value == 0x10)
		printf("TTC1 ov INTR\r\n");
	else if(intr_value == 0x20)
		printf("TTC1 ev INTR\r\n");
	else
		printf("TTC1 INTR value = %x\r\n",intr_value);
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

#if 0
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
#if 0
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
#if 0
	TTC_Counter_Disable(AL9000_TTC,clk1);
	TTC_Presacle_Enable(AL9000_TTC,clk1);
	TTC_PS_V(AL9000_TTC,clk1,2);
	Set_TTC_wave_pol(AL9000_TTC,clk1);
	TTC_Overflow_Mode(AL9000_TTC,clk1);
	enable_interrupt_type(AL9000_TTC,Ov,clk1);
	TTC_wave_Enable(AL9000_TTC,clk1);
	TTC_Counter_Enable(AL9000_TTC,clk1);
#endif
#if 0
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
