/*
 * al9000_ttc test case 
 *
 *  Created on: 2021/04/20
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_ttc.h"



void SOC_TTC_HANDLER(void)
{
   TTC_clear_interrput(AL9000_TTC1,clk1);
   Get_Event_Mode_counter_level(AL9000_TTC1,clk1);
}

int main(void){
	Enablepinmuxmode1();
#if 0
	TTC_Counter_Disable(AL9000_TTC1,clk1);
	TTC_Match_Mode(AL9000_TTC1,clk1);
	Set_Match_counter_level(AL9000_TTC1,match1,255);//32768/65535 50%
	enable_interrupt_type(AL9000_TTC1,M1,clk1);
	TTC_Presacle_Enable(AL9000_TTC1,clk1);
	TTC_PS_V(AL9000_TTC1,clk1,2);
	TTC_wave_Enable(AL9000_TTC1,clk1);
	TTC_Counter_Enable(AL9000_TTC1,clk1);
#endif
#if 0
	//__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
    //ECLIC_Register_IRQ(SOC_INT48_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_TTC_HANDLER);
   //__enable_irq();
	TTC_Counter_Disable(AL9000_TTC1,clk1);
	TTC_Match_Mode(AL9000_TTC1,clk1);
	TTC_Interval_Mode(AL9000_TTC1,clk1);
	Set_Match_counter_level(AL9000_TTC1,match1,250);//250/500   //32768/65535 50%
	//TTC_Match_Mode(AL9000_TTC1,clk1);
	//Set_Match_counter_level(AL9000_TTC1,match1,255);//32768/65535 50%
	Set_Interval_Mode_counter_level(AL9000_TTC1,500,clk1);//PWM CLK/500 (HZ)
	enable_interrupt_type(AL9000_TTC1,Iv,clk1);
	TTC_Presacle_Enable(AL9000_TTC1,clk1);
	TTC_PS_V(AL9000_TTC1,clk1,2);
	Set_TTC_wave_pol(AL9000_TTC1,clk1);
	TTC_wave_Enable(AL9000_TTC1,clk1);
	TTC_Counter_Enable(AL9000_TTC1,clk1);
#endif
#if 1

	//__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
    //ECLIC_Register_IRQ(SOC_INT48_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_TTC_HANDLER);
    //__enable_irq();
    Enablepinmuxmode1();
	TTC_Counter_Disable(AL9000_TTC1,clk1);
	TTC_Match_Mode(AL9000_TTC1,clk1);
	TTC_Interval_Mode(AL9000_TTC1,clk1);
	TTC_Overflow_Mode(AL9000_TTC1,clk1);
	Set_Match_counter_level(AL9000_TTC1,match1,250);//32768/65535 50%
	TTC_Match_Mode(AL9000_TTC1,clk1);
	Set_Interval_Mode_counter_level(AL9000_TTC1,500,clk1);
	enable_interrupt_type(AL9000_TTC1,Iv,clk1);
	TTC_Presacle_Enable(AL9000_TTC1,clk1);
	TTC_PS_V(AL9000_TTC1,clk1,2);
	Set_TTC_wave_pol(AL9000_TTC1,clk1);
	TTC_wave_Enable(AL9000_TTC1,clk1);
	TTC_Counter_Enable(AL9000_TTC1,clk1);
#endif
#if 0
	__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
    	ECLIC_Register_IRQ(SOC_INT48_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_TTC_HANDLER);
    	__enable_irq();
	TTC_Counter_Disable(AL9000_TTC1,clk1);
	TTC_Clock_Select_Ext_clock(AL9000_TTC1,clk1);
	TTC_Clock_Select_Ext_clock_edge(AL9000_TTC1,clk1);
 	Event_Control_event_Higth_Mode(AL9000_TTC1,clk1);
 	Event_Control_event_Enable(AL9000_TTC1,clk1);
	//TTC_Match_Mode(AL9000_TTC1,clk1);
	//Set_Match_counter_level(AL9000_TTC1,match1,250);//250/500   //32768/65535 50%
	//TTC_Match_Mode(AL9000_TTC1,clk1);
	//Set_Match_counter_level(AL9000_TTC1,match1,255);//32768/65535 50%
	enable_interrupt_type(AL9000_TTC1,Ev,clk1);
	TTC_Presacle_Enable(AL9000_TTC1,clk1);
	TTC_PS_V(AL9000_TTC1,clk1,2);
	Set_TTC_wave_pol(AL9000_TTC1,clk1);
	TTC_wave_Enable(AL9000_TTC1,clk1);
	TTC_Counter_Enable(AL9000_TTC1,clk1);
#endif

	return 0; 
}
