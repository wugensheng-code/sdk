/*
 * AL9000_ipc.c
 *
 *  Created on: 2022年6月28日
 *      Author: wei.pang
 */

#include "al9000_ipc.h"
#include "demosoc.h"
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"

uint32_t AlIpc_ReadMailbox(IPC_TypeDef *IPC,uint8_t mailbox_num)
{
	switch(mailbox_num){
	case 0:
		return IPC -> Mailbox_0;
	case 1:
		return IPC -> Mailbox_1 ;
	case 2:              ;
		return IPC -> Mailbox_2 ;
	case 3:              ;
		return IPC -> Mailbox_3 ;
	case 4:              ;
		return IPC -> Mailbox_4 ;
	case 5:              ;
		return IPC -> Mailbox_5 ;
	case 6:              ;
		return IPC -> Mailbox_6 ;
	case 7:              ;
		return IPC -> Mailbox_7 ;
	case 8:              ;
		return IPC -> Mailbox_8 ;
	case 9:              ;
		return IPC -> Mailbox_9 ;
	case 10:             ;
		return IPC -> Mailbox_10;
	case 11:             ;
		return IPC -> Mailbox_11;
	case 12:             ;
		return IPC -> Mailbox_12;
	case 13:             ;
		return IPC -> Mailbox_13;
	case 14:             ;
		return IPC -> Mailbox_14;
	case 15:             ;
		return IPC -> Mailbox_15;
	default:             ;
		return IPC -> Mailbox_0 ;
	}
}
uint8_t AlIpc_WriteMailbox(IPC_TypeDef *IPC,uint32_t val,uint8_t mailbox_num){
	switch(mailbox_num){
	case 0:
		IPC -> Mailbox_0 = val;
	break;
	case 1:
		IPC -> Mailbox_1 = val;
	break;
	case 2:
		IPC -> Mailbox_2 = val;
	break;
	case 3:
		IPC -> Mailbox_3 = val;
	break;
	case 4:
		IPC -> Mailbox_4 = val;
	break;
	case 5:
		IPC -> Mailbox_5 = val;
	break;
	case 6:
		IPC -> Mailbox_6 = val;
	break;
	case 7:
		IPC -> Mailbox_7 = val;
	break;
	case 8:
		IPC -> Mailbox_8 = val;
	break;
	case 9:
		IPC -> Mailbox_9 = val;
	break;
	case 10:
		IPC -> Mailbox_10 = val;
	break;
	case 11:
		IPC -> Mailbox_11 = val;
	break;
	case 12:
		IPC -> Mailbox_12 = val;
	break;
	case 13:
		IPC -> Mailbox_13 = val;
	break;
	case 14:
		IPC -> Mailbox_14 = val;
	break;
	case 15:
		IPC -> Mailbox_15 = val;
	break;
	default:
		IPC -> Mailbox_0 = val;
	break;

	}	
	return 0;
}
uint8_t AlIpc_IntrReq(IPC_TypeDef *IPC,enum IRQ_Type type){
	switch(type){
	case 0:
		IPC -> intr_req0 = IPC_INTRRPUT_ENABLE;
		break;
	case 1:
		IPC -> intr_req1 = IPC_INTRRPUT_ENABLE;
	break;
	case 2:
		IPC -> intr_req2 = IPC_INTRRPUT_ENABLE;
	break;
	case 3:
		IPC -> intr_req3 = IPC_INTRRPUT_ENABLE;
	break;
	case 4:
		IPC -> intr_req4= IPC_INTRRPUT_ENABLE;
	break;
	case 5:
		IPC -> intr_req5= IPC_INTRRPUT_ENABLE;
	break;
	case 6:
		IPC -> intr_req6= IPC_INTRRPUT_ENABLE;
	break;
	case 7:
		IPC -> intr_req7= IPC_INTRRPUT_ENABLE;
	break;
	case 8:
		IPC -> intr_req8= IPC_INTRRPUT_ENABLE;
	break;
	case 9:
		IPC -> intr_req9= IPC_INTRRPUT_ENABLE;
	break;
	case 10:
		IPC -> intr_req10= IPC_INTRRPUT_ENABLE;
	break;
	case 11:
		IPC -> intr_req11= IPC_INTRRPUT_ENABLE;
	break;
	default:
		IPC -> intr_req0= IPC_INTRRPUT_ENABLE;
	break;

	}
	return 0;
}
uint8_t AlIpc_IntrClear(IPC_TypeDef *IPC,enum IRQ_Type type){
	switch(type){
	case 0:
		IPC -> intr_req0 = 0;
		break;
	case 1:
		IPC -> intr_req1 = 0;
	break;
	case 2:
		IPC -> intr_req2 = 0;
	break;
	case 3:
		IPC -> intr_req3 = 0;
	break;
	case 4:
		IPC -> intr_req4= 0;
	break;
	case 5:
		IPC -> intr_req5= 0;
	break;
	case 6:
		IPC -> intr_req6= 0;
	break;
	case 7:
		IPC -> intr_req7= 0;
	break;
	case 8:
		IPC -> intr_req8= 0;
	break;
	case 9:
		IPC -> intr_req9= 0;
	break;
	case 10:
		IPC -> intr_req10= 0;
	break;
	case 11:
		IPC -> intr_req11= 0;
	break;
	default:
		IPC -> intr_req0= 0;
	break;

	}
	return 0;
}
uint8_t AlIpc_IntrAck(IPC_TypeDef *IPC,enum IRQ_Type type){
	switch(type){
	case 0:
		IPC -> intr_ack0 = IPC_INTRRPUT_ENABLE;
		break;
	case 1:
		IPC -> intr_ack1 = IPC_INTRRPUT_ENABLE;
	break;
	case 2:
		IPC -> intr_ack2 = IPC_INTRRPUT_ENABLE;
	break;
	case 3:
		IPC -> intr_ack3 = IPC_INTRRPUT_ENABLE;
	break;
	case 4:
		IPC -> intr_ack4= IPC_INTRRPUT_ENABLE;
	break;
	case 5:
		IPC -> intr_ack5= IPC_INTRRPUT_ENABLE;
	break;
	case 6:
		IPC -> intr_ack6= IPC_INTRRPUT_ENABLE;
	break;
	case 7:
		IPC -> intr_ack7= IPC_INTRRPUT_ENABLE;
	break;
	case 8:
		IPC -> intr_ack8= IPC_INTRRPUT_ENABLE;
	break;
	case 9:
		IPC -> intr_ack9= IPC_INTRRPUT_ENABLE;
	break;
	case 10:
		IPC -> intr_ack10= IPC_INTRRPUT_ENABLE;
	break;
	case 11:
		IPC -> intr_ack11= IPC_INTRRPUT_ENABLE;
	break;
	default:
		IPC -> intr_ack0= IPC_INTRRPUT_ENABLE;
	break;

	}
	return 0;
}
uint8_t AlIpc_IntrAckClear(IPC_TypeDef *IPC,enum IRQ_Type type){
	switch(type){
	case 0:
		IPC -> intr_ack0 = 0;
		break;
	case 1:
		IPC -> intr_ack1 = 0;
	break;
	case 2:
		IPC -> intr_ack2 = 0;
	break;
	case 3:
		IPC -> intr_ack3 = 0;
	break;
	case 4:
		IPC -> intr_ack4= 0;
	break;
	case 5:
		IPC -> intr_ack5= 0;
	break;
	case 6:
		IPC -> intr_ack6= 0;
	break;
	case 7:
		IPC -> intr_ack7= 0;
	break;
	case 8:
		IPC -> intr_ack8= 0;
	break;
	case 9:
		IPC -> intr_ack9= 0;
	break;
	case 10:
		IPC -> intr_ack10= 0;
	break;
	case 11:
		IPC -> intr_ack11= 0;
	break;
	default:
		IPC -> intr_ack0= 0;
	break;

	}
	return 0;
}
uint8_t AlIpc_GetSpinlockStatus(IPC_TypeDef *IPC,enum spinlock_num number){
	uint8_t status = 0;
	switch(number){
		case 0:
			status = GET_BIT((IPC -> Spinlock_0),0);
		break;
		case 1:
			status = GET_BIT((IPC -> Spinlock_1),0);
		break;
		case 2:
			status = GET_BIT((IPC -> Spinlock_2),0);
		break;
		case 3:
			status = GET_BIT((IPC -> Spinlock_3),0);
		break;
		default:
			status = GET_BIT((IPC -> Spinlock_0),0);
		break;
	}
	return status ;
}
uint8_t AlIpc_SetSpinlockStatus(IPC_TypeDef *IPC,enum spinlock_num number){
	switch(number){
		case 0:
			IPC -> Spinlock_0 |=0x1;
		break;
		case 1:
			IPC -> Spinlock_1 |=0x1;
		break;
		case 2:
			IPC -> Spinlock_2 |=0x1;
		break;
		case 3:
			IPC -> Spinlock_3 |=0x1;
		break;
		default:
			IPC -> Spinlock_0 |=0x1;
		break;
	}
}
uint8_t AlIpc_SpinlockHandler(IPC_TypeDef *IPC){
	u8 spin_lock_num;
	while(1)
	{
		while(AlIpc_GetSpinlockStatus(AL9000_IPC,spinlock0)){
			spin_lock_num = spinlock0;
			printf("Get Spinlock 0\r\n");
			return spin_lock_num;
		}
		while(AlIpc_GetSpinlockStatus(AL9000_IPC,spinlock1)){
			spin_lock_num = spinlock1;
			printf("Get Spinlock 1\r\n");
			return spin_lock_num;
		}
		while(AlIpc_GetSpinlockStatus(AL9000_IPC,spinlock2)){
			spin_lock_num = spinlock2;
			printf("Get Spinlock 2\r\n");
			return spin_lock_num;
		}
		while(AlIpc_GetSpinlockStatus(AL9000_IPC,spinlock3)){
			spin_lock_num = spinlock3;
			printf("Get Spinlock 3\r\n");
			return spin_lock_num;
		}
	}
}
uint8_t AlIpc_GetAckStatus(IPC_TypeDef *IPC,enum IRQ_Type type){
	uint8_t status = 0;
switch(type){
	case 0:
		status = GET_BIT(IPC -> intr_ack0,0);
		break;
	case 1:
		status = GET_BIT(IPC -> intr_ack1,0);
	break;
	case 2:
		status = GET_BIT(IPC -> intr_ack2,0);
	break;
	case 3:
		status = GET_BIT(IPC -> intr_ack3,0);
	break;
	case 4:
		status = GET_BIT(IPC -> intr_ack4,0);
	break;
	case 5:
		status = GET_BIT(IPC -> intr_ack5,0);
	break;
	case 6:
		status = GET_BIT(IPC -> intr_ack6,0);
	break;
	case 7:
		status = GET_BIT(IPC -> intr_ack7,0);
	break;
	case 8:
		status = GET_BIT(IPC -> intr_ack8,0);
	break;
	case 9:
		status = GET_BIT(IPC -> intr_ack9,0);
	break;
	case 10:
		status = GET_BIT(IPC -> intr_ack10,0);
	break;
	case 11:
		status = GET_BIT(IPC -> intr_ack11,0);
	break;
	default:
		status = GET_BIT(IPC -> intr_ack0,0);
	break;

	}
	return status;	
}
uint8_t AlIpc_GetIrqStatus(IPC_TypeDef *IPC,enum IRQ_Type type){
	uint8_t status = 0;
switch(type){
	case 0:
		status = GET_BIT(IPC -> intr_req0,0);
		break;
	case 1:
		status = GET_BIT(IPC -> intr_req1,0);
	break;
	case 2:
		status = GET_BIT(IPC -> intr_req2,0);
	break;
	case 3:
		status = GET_BIT(IPC -> intr_req3,0);
	break;
	case 4:
		status = GET_BIT(IPC -> intr_req4,0);
	break;
	case 5:
		status = GET_BIT(IPC -> intr_req5,0);
	break;
	case 6:
		status = GET_BIT(IPC -> intr_req6,0);
	break;
	case 7:
		status = GET_BIT(IPC -> intr_req7,0);
	break;
	case 8:
		status = GET_BIT(IPC -> intr_req8,0);
	break;
	case 9:
		status = GET_BIT(IPC -> intr_req9,0);
	break;
	case 10:
		status = GET_BIT(IPC -> intr_req10,0);
	break;
	case 11:
		status = GET_BIT(IPC -> intr_req11,0);
	break;
	default:
		status = GET_BIT(IPC -> intr_req0,0);
	break;

	}
	return status;	
}
uint32_t Xpu_ApuHoldReset()
{
	XpuSrstReg_t RegData;
	RegData.reg = (*(volatile uint32_t *) (XPU_SRST_ADDR));
	if(0x0 != RegData.apu_core_srst_n)
	{
		RegData.apu_core_srst_n = 0x0;
		(*(volatile uint32_t *) (XPU_SRST_ADDR)) = RegData.reg;
	}
	return 0;
}
uint32_t Xpu_Apu0ReleaseReset()
{
	XpuSrstReg_t RegData;
	RegData.reg = REG32_READ(XPU_SRST_ADDR);
	RegData.apu_core_srst_n = 0x1;
	REG32_WRITE(XPU_SRST_ADDR, RegData.reg);
	return 0;
}
uint32_t Xpu_ApuReleaseReset()
{
	XpuSrstReg_t RegData;
	RegData.reg = REG32_READ(XPU_SRST_ADDR);
	RegData.apu_core_srst_n = 0x3;
	REG32_WRITE(XPU_SRST_ADDR, RegData.reg);
	return 0;
}
uint32_t Xpu_RpuHoldReset()
{
	XpuSrstReg_t RegData;
	RegData.reg = REG32_READ(XPU_SRST_ADDR);
	if(0x0 != RegData.rpu_srst_n)
	{
		RegData.rpu_srst_n = 0x0;
		REG32_WRITE(XPU_SRST_ADDR, RegData.reg);
	}
	return 0;
}
uint32_t Xpu_RpuReleaseReset()
{
	XpuSrstReg_t RegData;
	RegData.reg = REG32_READ(XPU_SRST_ADDR);
	if(0x1 != RegData.rpu_srst_n)
	{
		RegData.rpu_srst_n = 0x1;
		REG32_WRITE(XPU_SRST_ADDR, RegData.reg);
	}

	return 0;
}
uint32_t Xpu_RpuFsbl()
{
	RpuVecRegL_t VL;
	RpuVecRegH_t VH;
	Xpu_RpuHoldReset();
	VL.reg = 0;
	VH.reg = 0;
	VL.rpu_reset_vector_l = 0x61000000;
	REG32_WRITE(TOP_S_RPU_RESET_L_ADDR, VL.reg);
	REG32_WRITE(TOP_S_RPU_RESET_H_ADDR, VH.reg);
	Xpu_RpuReleaseReset();
	return 0;
}
