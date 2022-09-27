/*
 * AL_IPC.H
 *
 *  Created on: 2022年6月28日
 *      Author: wei.pang
 */

/**
 ****************************************************************************************
 * @defgroup IPC
 * @ingroup DRIVERS
 * @brief MPU Drivers.
 *        Functions specified for UART.
 *
 * @{
 ****************************************************************************************
 */

#ifndef _AL9000_IPC_H
#define _AL9000_IPC_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "demosoc.h"

#define IPC_INTRRPUT_ENABLE         BIT(0)
#define u32 uint32_t
#define u8 uint8_t
#define IPC_INTRRPUT_ENABLE         BIT(0)
#define IPC_MAILBOX_CHANNELS	16
#define TOP_S_BASE_ADDR 0xF8806000ULL
#define TOP_S_XPU_BASE_ADDR (TOP_S_BASE_ADDR + 0x334ULL)
#define TOP_S_RPU_RESET_H_ADDR (TOP_S_BASE_ADDR + 0x320ULL)
#define TOP_S_RPU_RESET_L_ADDR (TOP_S_BASE_ADDR + 0x324ULL)
#define XPU_SRST_ADDR TOP_S_XPU_BASE_ADDR


typedef union{
	struct{
		unsigned apu_core_pulse_srst_n : 2;
		unsigned Reserved_3_2 : 2;
		unsigned apu_core_srst_n : 2;
		unsigned Reserved_7_6 : 2;
		unsigned rpu_pulse_srst_n : 1;
		unsigned Reserved_9 : 1;
		unsigned rpu_srst_n : 1;
		unsigned Reserved_3_11 : 21;
	};
	unsigned reg;
}XpuSrstReg_t;
typedef union{
	struct{
		unsigned rvbaraddr0_h : 6;
		unsigned reserved_31_6 : 26;
	};
	unsigned reg;
}ApuVecRegH_t;
typedef union{
	struct{
		unsigned rvbaraddr0_l : 32;
	};
	unsigned reg;
}ApuVecRegL_t;
typedef union{
	struct{
		unsigned rpu_reset_vector_l : 32;
	};
	unsigned reg;
}RpuVecRegL_t;
typedef union{
	struct{
		unsigned rpu_reset_vector_h : 32;
	};
	unsigned reg;
}RpuVecRegH_t;

enum IRQ_Type{
	apu2csu,
	csu2apu,
	rpu2csu,
	csu2rpu,
	apu2rpu_0,
	rpu2apu_0,
	apu2rpu_1,
	rpu2apu_1,
	apu2rpu_2,
	rpu2apu_2,
	apu2rpu_3,
	rpu2apu_3	
};
enum spinlock_num{	
	spinlock0,
	spinlock1,
	spinlock2,
	spinlock3,
};
uint8_t AlIpc_WriteMailbox(IPC_TypeDef *IPC,uint32_t val,uint8_t mailbox_num);
uint8_t AlIpc_IntrReq(IPC_TypeDef *IPC,enum IRQ_Type type);
uint8_t AlIpc_IntrClear(IPC_TypeDef *IPC,enum IRQ_Type type);
uint8_t AlIpc_IntrAck(IPC_TypeDef *IPC,enum IRQ_Type type);
uint8_t AlIpc_IntrAckClear(IPC_TypeDef *IPC,enum IRQ_Type type);
uint8_t AlIpc_GetSpinlockStatus(IPC_TypeDef *IPC,enum spinlock_num number);
uint8_t AlIpc_SetSpinlockStatus(IPC_TypeDef *IPC,enum spinlock_num number);
uint8_t AlIpc_SpinlockHandler(IPC_TypeDef *IPC);
uint8_t AlIpc_GetIrqStatus(IPC_TypeDef *IPC,enum IRQ_Type type);
uint8_t AlIpc_GetAckStatus(IPC_TypeDef *IPC,enum IRQ_Type type);
uint32_t Xpu_ApuHoldReset();
uint32_t Xpu_ApuRelease0Reset();
uint32_t Xpu_ApuReleaseReset();
uint32_t Xpu_RpuHoldReset();
uint32_t Xpu_Rpu0ReleaseReset();
uint32_t Xpu_RpuReleaseReset();
static inline uint32_t REG32_READ(uintptr_t Address);
static inline uint32_t REG32_WRITE(uintptr_t Address, u32 Value);
static inline uint32_t REG32_READ(uintptr_t Address){
	return *(volatile u32 *) Address;
}
static inline uint32_t REG32_WRITE(uintptr_t Address, u32 Value){
	volatile u32 *LocalAddress = (volatile u32*) Address;
	*LocalAddress = Value;
	return 0;
}


#ifdef __cplusplus
}
#endif
#endif /* _HBIRD_MPU_H */
