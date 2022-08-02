/*
 * al9000_ttc.c
 *
 *  Created on: 2021/04/19
 *      Author: wei.pang
 */
#include "al9000_ttc.h"
uint8_t TTC_Presacle_Enable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->CLK_CTRL_REG_1 |=CLK_CTRL_REG_1_PS_En;
		break;
		case clk2:
			TTCX ->CLK_CTRL_REG_2 |=CLK_CTRL_REG_2_PS_En;
		break;
		case clk3:
			TTCX ->CLK_CTRL_REG_3 |=CLK_CTRL_REG_3_PS_En;
		break;
		defalut:
			TTCX ->CLK_CTRL_REG_1 |=CLK_CTRL_REG_1_PS_En;
		break;
    }
	return 0;
}
uint8_t TTC_Presacle_Disable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->CLK_CTRL_REG_1 &=~CLK_CTRL_REG_1_PS_En;
		break;
		case clk2:
			TTCX ->CLK_CTRL_REG_2 &=~CLK_CTRL_REG_2_PS_En;
		break;
		case clk3:
			TTCX ->CLK_CTRL_REG_3 &=~CLK_CTRL_REG_3_PS_En;
		break;
		defalut:
			TTCX ->CLK_CTRL_REG_1 &=~CLK_CTRL_REG_1_PS_En;
		break;
    }
	return 0;
}
uint8_t TTC_Counter_Enable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX -> CNTR_CTRL_REG_1 &=~CNTR_CTRL_REG_1_DIS;
		break;
		case clk2:
			TTCX -> CNTR_CTRL_REG_2 &=~CNTR_CTRL_REG_2_DIS;
		break;
		case clk3:
			TTCX -> CNTR_CTRL_REG_3 &=~CNTR_CTRL_REG_3_DIS;
		break;
		defalut:
			TTCX -> CNTR_CTRL_REG_1 &=~CNTR_CTRL_REG_1_DIS;
		break;
    }
	return 0;

}
uint8_t TTC_Counter_Disable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX -> CNTR_CTRL_REG_1 |=CNTR_CTRL_REG_1_DIS;
		break;
		case clk2:
			TTCX -> CNTR_CTRL_REG_1 |=CNTR_CTRL_REG_2_DIS;
		break;
		case clk3:
			TTCX -> CNTR_CTRL_REG_1 |=CNTR_CTRL_REG_3_DIS;
		break;
		defalut:
			TTCX -> CNTR_CTRL_REG_1 |=CNTR_CTRL_REG_1_DIS;
		break;
    }
	return 0;

}

uint8_t TTC_PS_V(TTC_AL9000_TypeDef *TTCX,enum clk_number clk,uint8_t PS){
	switch(clk){
		case clk1:
			TTCX ->CLK_CTRL_REG_1 |= PS << 1;
		break;
		case clk2:
			TTCX ->CLK_CTRL_REG_2 |= PS << 1;
		break;
		case clk3:
			TTCX ->CLK_CTRL_REG_3 |= PS << 1;
		break;
		defalut:
			TTCX ->CLK_CTRL_REG_1 |= PS << 1;
		break;
    }
	return 0;
	
}
uint8_t TTC_Clock_Select_Ext_clock(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->CLK_CTRL_REG_1 |= CLK_CTRL_REG_1_C_Src;
		break;
		case clk2:
			TTCX ->CLK_CTRL_REG_2 |= CLK_CTRL_REG_2_C_Src;
		break;
		case clk3:
			TTCX ->CLK_CTRL_REG_3 |= CLK_CTRL_REG_3_C_Src;
		break;
		defalut:
			TTCX ->CLK_CTRL_REG_1 |= CLK_CTRL_REG_1_C_Src;
		break;
    }
	return 0;	
}
uint8_t TTC_Clock_Select_Ext_clock_edge(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			if(GET_BIT(TTCX ->CLK_CTRL_REG_1,5)==1){
			TTCX ->CLK_CTRL_REG_1 |= CLK_CTRL_REG_1_Ex_E;
			}
			else{}
		break;
		case clk2:
			if(GET_BIT(TTCX ->CLK_CTRL_REG_2,5)==1){
			TTCX ->CLK_CTRL_REG_2 |= CLK_CTRL_REG_2_Ex_E;
			}
			else{}
		break;
		case clk3:
			if(GET_BIT(TTCX ->CLK_CTRL_REG_3,5)==1){
			TTCX ->CLK_CTRL_REG_3 |= CLK_CTRL_REG_3_Ex_E;
			}
			else{}
		break;
		defalut:
			if(GET_BIT(TTCX ->CLK_CTRL_REG_1,5)==1){
			TTCX ->CLK_CTRL_REG_1 |= CLK_CTRL_REG_1_Ex_E;
			}
			else{}
		break;
    }
	return 0;	
}

uint8_t TTC_Interval_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->CNTR_CTRL_REG_1 |=CNTR_CTRL_REG_1_INT;
		break;
		case clk2:
			TTCX ->CNTR_CTRL_REG_2 |=CNTR_CTRL_REG_2_INT;
		break;
		case clk3:
			TTCX ->CNTR_CTRL_REG_3 |=CNTR_CTRL_REG_3_INT;
		break;
		defalut:
			TTCX ->CNTR_CTRL_REG_1 |=CNTR_CTRL_REG_1_INT;
		break;
    }
	return 0;
}
uint8_t TTC_Overflow_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->CNTR_CTRL_REG_1 &=~CNTR_CTRL_REG_1_INT;
		break;
		case clk2:
			TTCX ->CNTR_CTRL_REG_2 &=~CNTR_CTRL_REG_2_INT;
		break;
		case clk3:
			TTCX ->CNTR_CTRL_REG_3 &=~CNTR_CTRL_REG_3_INT;
		break;
		defalut:
			TTCX ->CNTR_CTRL_REG_1 &=~CNTR_CTRL_REG_1_INT;
		break;
    }
	return 0;
}
uint8_t TTC_DEC_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->CNTR_CTRL_REG_1 |=CNTR_CTRL_REG_1_DEC;
		break;
		case clk2:
			TTCX ->CNTR_CTRL_REG_2 |=CNTR_CTRL_REG_2_DEC;
		break;
		case clk3:
			TTCX ->CNTR_CTRL_REG_3 |=CNTR_CTRL_REG_3_DEC;
		break;
		defalut:
			TTCX ->CNTR_CTRL_REG_1 |=CNTR_CTRL_REG_1_DEC;
		break;
    }
	return 0;
}
uint8_t TTC_Incress_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->CNTR_CTRL_REG_1 &=~CNTR_CTRL_REG_1_DEC;
		break;
		case clk2:
			TTCX ->CNTR_CTRL_REG_2 &=~CNTR_CTRL_REG_2_DEC;
		break;
		case clk3:
			TTCX ->CNTR_CTRL_REG_3 &=~CNTR_CTRL_REG_3_DEC;
		break;
		defalut:
			TTCX ->CNTR_CTRL_REG_1 &=~CNTR_CTRL_REG_1_DEC;
		break;
    }
	return 0;
}

uint8_t TTC_Match_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->CNTR_CTRL_REG_1 |=CNTR_CTRL_REG_1_Match;
		break;
		case clk2:
			TTCX ->CNTR_CTRL_REG_2 |=CNTR_CTRL_REG_2_Match;
		break;
		case clk3:
			TTCX ->CNTR_CTRL_REG_3 |=CNTR_CTRL_REG_3_Match;
		break;
		defalut:
			TTCX ->CNTR_CTRL_REG_1 |=CNTR_CTRL_REG_1_Match;
		break;
    }
	return 0;
}
uint8_t GET_TTC_Counter_Value_Register_Number(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	uint16_t value = 0;
	switch(clk){
		case clk1:
			value = GET_BITS(TTCX ->CNTR_CTRL_REG_1,0,15);
		break;
		case clk2:
			value = GET_BITS(TTCX ->CNTR_CTRL_REG_2,0,15);
		break;
		case clk3:
			value = GET_BITS(TTCX ->CNTR_CTRL_REG_3,0,15);
		break;
		defalut:
			value = GET_BITS(TTCX ->CNTR_CTRL_REG_1,0,15);
		break;
    }
	return value;
}
uint8_t Set_Interval_Mode_counter_level(TTC_AL9000_TypeDef *TTCX,uint16_t counter_level,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->INTERVAL_REG_1  = counter_level;
		break;
		case clk2:
			TTCX ->INTERVAL_REG_2  = counter_level;
		break;
		case clk3:
			TTCX ->INTERVAL_REG_3  = counter_level;
		break;
		defalut:
			TTCX ->INTERVAL_REG_1  = counter_level;
		break;
    }
	return 0;
}
uint8_t Get_Event_Mode_counter_level(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	volatile uint16_t event_count = 0;
	switch(clk){
		case clk1:
			event_count = (uint16_t)GET_BITS(TTCX ->EVENT_REG_1,0,15);
		break;
		case clk2:
			event_count = (uint16_t)GET_BITS(TTCX ->EVENT_REG_2,0,15);
		break;
		case clk3:
			event_count = (uint16_t)GET_BITS(TTCX ->EVENT_REG_3,0,15);
		break;
		defalut:
			event_count = (uint16_t)GET_BITS(TTCX ->EVENT_REG_1,0,15);
		break;
    }
		return event_count;
}

uint8_t Set_Match_counter_level(TTC_AL9000_TypeDef *TTCX,enum match_mode match_num,uint16_t counter_level){
	switch(match_num){
		case match1:
			TTCX ->MATCH_1_REG_1 = counter_level;
		break;
		case match2:
			TTCX ->MATCH_1_REG_2 = counter_level;
		break;
		case match3:
			TTCX ->MATCH_1_REG_3 = counter_level;
		break;
		case match4:
			TTCX ->MATCH_2_REG_1 = counter_level;
		break;
		case match5:
			TTCX ->MATCH_2_REG_2 = counter_level;
		break;
		case match6:
			TTCX ->MATCH_2_REG_3 = counter_level;
		break;
		case match7:
			TTCX ->MATCH_3_REG_1 = counter_level;
		break;
		case match8:
			TTCX ->MATCH_3_REG_2 = counter_level;
		break;
		case match9:
			TTCX ->MATCH_3_REG_3 = counter_level;
		break;
		default:
			TTCX ->MATCH_1_REG_1 = counter_level;
		break;
    }
	return 0;	
}
uint8_t enable_interrupt_type(TTC_AL9000_TypeDef *TTCX,enum intr_type IRQ,enum clk_number clk){
	switch(IRQ){
	case Iv:
		switch(clk){
				case clk1:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_Iv;	
				break;
				case clk2:
					TTCX -> IRQ_EN_REG_2 |= IRQ_REG_2_Iv;	
				break;
				case clk3:
					TTCX -> IRQ_EN_REG_3 |= IRQ_REG_3_Iv;	
				break;
				default:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_Iv;	
				break;
			}	
	break;
	case M1:
		switch(clk){
				case clk1:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_M1;	
				break;
				case clk2:
					TTCX -> IRQ_EN_REG_2 |= IRQ_REG_2_M1;	
				break;
				case clk3:
					TTCX -> IRQ_EN_REG_3 |= IRQ_REG_3_M1;	
				break;
				default:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_M1;	
				break;
			}	
	break;
	case M2:
		switch(clk){
				case clk1:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_M2;	
				break;
				case clk2:
					TTCX -> IRQ_EN_REG_2 |= IRQ_REG_2_M2;	
				break;
				case clk3:
					TTCX -> IRQ_EN_REG_3 |= IRQ_REG_3_M2;	
				break;
				default:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_M2;	
				break;
			}	
	break;
	case M3:
		switch(clk){
				case clk1:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_M3;	
				break;
				case clk2:
					TTCX -> IRQ_EN_REG_2 |= IRQ_REG_2_M3;	
				break;
				case clk3:
					TTCX -> IRQ_EN_REG_3 |= IRQ_REG_3_M3;	
				break;
				default:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_M3;	
				break;
			}	
	break;
	case Ov:
		switch(clk){
				case clk1:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_Ov;	
				break;
				case clk2:
					TTCX -> IRQ_EN_REG_2 |= IRQ_REG_2_Ov;	
				break;
				case clk3:
					TTCX -> IRQ_EN_REG_3 |= IRQ_REG_3_Ov;	
				break;
				default:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_Ov;	
				break;
			}	
	break;
	case Ev:
		switch(clk){
				case clk1:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_Ev;	
				break;
				case clk2:
					TTCX -> IRQ_EN_REG_2 |= IRQ_REG_2_Ev;	
				break;
				case clk3:
					TTCX -> IRQ_EN_REG_3 |= IRQ_REG_3_Ev;	
				break;
				default:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_Ev;	
				break;
			}	
	break;
	default:
		switch(clk){
				case clk1:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_Iv;	
				break;
				case clk2:
					TTCX -> IRQ_EN_REG_2 |= IRQ_REG_2_Iv;	
				break;
				case clk3:
					TTCX -> IRQ_EN_REG_3 |= IRQ_REG_3_Iv;	
				break;
				default:
					TTCX -> IRQ_EN_REG_1 |= IRQ_REG_1_Iv;	
				break;
			}	
	break;
    }
	return 0;
}
uint8_t Event_Control_event_Enable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->EVENT_CTRL_1  |= EVENT_CTRL_1_E_En;
		break;
		case clk2:
			TTCX ->EVENT_CTRL_2  |= EVENT_CTRL_2_E_En;
		break;
		case clk3:
			TTCX ->EVENT_CTRL_3  |= EVENT_CTRL_3_E_En;
		break;
		defalut:
			TTCX ->EVENT_CTRL_1  |= EVENT_CTRL_1_E_En;
		break;
    }
	return 0;	
}
uint8_t Event_Control_event_Higth_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->EVENT_CTRL_1  |= EVENT_CTRL_1_E_Lo;
		break;
		case clk2:
			TTCX ->EVENT_CTRL_2  |= EVENT_CTRL_2_E_Lo;
		break;
		case clk3:
			TTCX ->EVENT_CTRL_3  |= EVENT_CTRL_3_E_Lo;
		break;
		defalut:
			TTCX ->EVENT_CTRL_1  |= EVENT_CTRL_1_E_Lo;
		break;
    }
	return 0;	
}
uint8_t Event_Control_event_Low_Mode(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX ->EVENT_CTRL_1  |= EVENT_CTRL_1_E_Ov;
		break;
		case clk2:
			TTCX ->EVENT_CTRL_2  |= EVENT_CTRL_2_E_Ov;
		break;
		case clk3:
			TTCX ->EVENT_CTRL_3  |= EVENT_CTRL_3_E_Ov;
		break;
		defalut:
			TTCX ->EVENT_CTRL_1  |= EVENT_CTRL_1_E_Ov;
		break;
    }
	return 0;	
}
uint8_t TTC_wave_Enable(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX -> CNTR_CTRL_REG_1 &=~CNTR_CTRL_REG_1_Wave_en;
		break;
		case clk2:
			TTCX -> CNTR_CTRL_REG_2 &=~CNTR_CTRL_REG_2_Wave_en;
		break;
		case clk3:
			TTCX -> CNTR_CTRL_REG_3 &=~CNTR_CTRL_REG_3_Wave_en;
		break;
		defalut:
			TTCX -> CNTR_CTRL_REG_1 &=~CNTR_CTRL_REG_1_Wave_en;
		break;
    	}
	return 0;

}
uint8_t Set_TTC_wave_pol(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			TTCX -> CNTR_CTRL_REG_1 |= CNTR_CTRL_REG_1_Wave_pol;
		break;
		case clk2:
			TTCX -> CNTR_CTRL_REG_2 |= CNTR_CTRL_REG_2_Wave_pol;
		break;
		case clk3:
			TTCX -> CNTR_CTRL_REG_3 |= CNTR_CTRL_REG_3_Wave_pol;
		break;
		defalut:
			TTCX -> CNTR_CTRL_REG_1 |= CNTR_CTRL_REG_1_Wave_pol;
		break;
    	}
	return 0;

}

uint8_t TTC_clear_interrput(TTC_AL9000_TypeDef *TTCX,enum clk_number clk){
	switch(clk){
		case clk1:
			GET_BITS(TTCX -> IRQ_REG_1,0,5);
		break;
		case clk2:
			GET_BITS(TTCX -> IRQ_REG_2,0,5);
		break;
		case clk3:
			GET_BITS(TTCX -> IRQ_REG_3,0,5);	
		break;
		defalut:
			GET_BITS(TTCX -> IRQ_REG_1,0,5);		
		break;
    	}
	return 0;


}
