/*
 * al9000_ttc.h
 *
 *  Created on: 2022/4/19
 *      Author: wei.pang
 */


#ifndef _AL9000_TTC_H
#define _AL9000_TTC_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "demosoc.h"


/****************EVENT_CTRL REG BIT DEFINE******************/
#define EVENT_CTRL_1_E_Ov		BIT(2)
#define EVENT_CTRL_1_E_Lo		BIT(1)
#define EVENT_CTRL_1_E_En		BIT(0)
#define EVENT_CTRL_2_E_Ov		BIT(2)
#define EVENT_CTRL_2_E_Lo		BIT(1)
#define EVENT_CTRL_2_E_En		BIT(0)
#define EVENT_CTRL_3_E_Ov		BIT(2)
#define EVENT_CTRL_3_E_Lo		BIT(1)
#define EVENT_CTRL_3_E_En		BIT(0)
/****************CNTR_CTRL REG BIT DEFINE******************/
#define CNTR_CTRL_REG_1_Wave_pol	BIT(6)
#define CNTR_CTRL_REG_1_Wave_en		BIT(5)
#define CNTR_CTRL_REG_1_RST		BIT(4)
#define CNTR_CTRL_REG_1_Match		BIT(3)
#define CNTR_CTRL_REG_1_DEC		BIT(2)
#define CNTR_CTRL_REG_1_INT		BIT(1)
#define CNTR_CTRL_REG_1_DIS		BIT(0)
#define CNTR_CTRL_REG_2_Wave_pol	BIT(6)
#define CNTR_CTRL_REG_2_Wave_en		BIT(5)
#define CNTR_CTRL_REG_2_RST		BIT(4)
#define CNTR_CTRL_REG_2_Match		BIT(3)
#define CNTR_CTRL_REG_2_DEC		BIT(2)
#define CNTR_CTRL_REG_2_INT		BIT(1)
#define CNTR_CTRL_REG_2_DIS		BIT(0)
#define CNTR_CTRL_REG_3_Wave_pol	BIT(6)
#define CNTR_CTRL_REG_3_Wave_en		BIT(5)
#define CNTR_CTRL_REG_3_RST		BIT(4)
#define CNTR_CTRL_REG_3_Match		BIT(3)
#define CNTR_CTRL_REG_3_DEC		BIT(2)
#define CNTR_CTRL_REG_3_INT		BIT(1)
#define CNTR_CTRL_REG_3_DIS		BIT(0)
/****************CLK_CTRL REG BIT DEFINE******************/
#define CLK_CTRL_REG_1_Ex_E		BIT(6)
#define CLK_CTRL_REG_1_C_Src		BIT(5)
#define CLK_CTRL_REG_1_PS_V		BIT(1)
#define CLK_CTRL_REG_1_PS_En		BIT(0)
#define CLK_CTRL_REG_2_Ex_E		BIT(6)
#define CLK_CTRL_REG_2_C_Src		BIT(5)
#define CLK_CTRL_REG_2_PS_V		BIT(1)
#define CLK_CTRL_REG_2_PS_En		BIT(0)
#define CLK_CTRL_REG_3_Ex_E		BIT(6)
#define CLK_CTRL_REG_3_C_Src		BIT(5)
#define CLK_CTRL_REG_3_PS_V		BIT(1)
#define CLK_CTRL_REG_3_PS_En		BIT(0)
/****************IRQ_REG REG BIT DEFINE******************/
#define IRQ_REG_1_Ev			BIT(5)
#define IRQ_REG_1_Ov			BIT(4)
#define IRQ_REG_1_M3			BIT(3)
#define IRQ_REG_1_M2			BIT(2)
#define IRQ_REG_1_M1			BIT(1)
#define IRQ_REG_1_Iv			BIT(0)
#define IRQ_REG_2_Ev			BIT(5)
#define IRQ_REG_2_Ov			BIT(4)
#define IRQ_REG_2_M3			BIT(3)
#define IRQ_REG_2_M2			BIT(2)
#define IRQ_REG_2_M1			BIT(1)
#define IRQ_REG_2_Iv			BIT(0)
#define IRQ_REG_3_Ev			BIT(5)
#define IRQ_REG_3_Ov			BIT(4)
#define IRQ_REG_3_M3			BIT(3)
#define IRQ_REG_3_M2			BIT(2)
#define IRQ_REG_3_M1			BIT(1)
#define IRQ_REG_3_Iv			BIT(0)
enum clk_number{
	clk1,
	clk2,
	clk3
};
enum match_mode{
	match1,
	match2,
	match3,
	match4,
	match5,
	match6,
	match7,
	match8,
	match9
};
enum intr_type{
	Iv,
	M1,
	M2,
	M3,
	Ov,
	Ev
};
uint8_t TTC_Presacle_Enable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_Presacle_Disable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_Counter_Enable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_Counter_Disable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_PS_V(TTC_AL9000_TypeDef *TTCX,enum clk_number clk,uint8_t PS);
uint8_t TTC_Clock_Select_Ext_clock(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_Clock_Select_Ext_clock_edge(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_Interval_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_Overflow_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_DEC_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_Incress_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_Match_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t GET_TTC_Counter_Value_Register_Number(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t Set_Interval_Mode_counter_level(TTC_AL9000_TypeDef *TTCX,uint16_t counter_level,enum clk_number clk);
uint8_t Get_Event_Mode_counter_level(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t Set_Match_counter_level(TTC_AL9000_TypeDef *TTCX,enum match_mode match_num,uint16_t counter_level);
uint8_t enable_interrupt_type(TTC_AL9000_TypeDef *TTCX,enum intr_type IRQ,enum clk_number clk);
uint8_t Event_Control_event_Enable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t Event_Control_event_Higth_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t Event_Control_event_Low_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_wave_Enable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t Set_TTC_wave_pol(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
uint8_t TTC_clear_interrput(TTC_AL9000_TypeDef *TTCX,enum clk_number clk);
#ifdef __cplusplus

}
#endif



#endif /* SOC_DEMOSOC_COMMON_INCLUDE_AL9000_TTC_H_ */
