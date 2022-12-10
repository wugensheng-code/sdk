#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_ipc.h"
void CSU2RPU_IRQn_handler(void)
{
	printf("CSU2RPU Intr\r\n");
	for(int i=0;i<IPC_MAILBOX_CHANNELS;i++)
	{
//		printf("Mailbox %d Message:%x\r\n",i,AlIpc_ReadMailbox(AL9000_IPC,i));
	}
	AlIpc_IntrClear(AL9000_IPC,csu2rpu);
	AlIpc_SetSpinlockStatus(AL9000_IPC,spinlock0);
}
void CSU2RPU_IPC()
{
	ECLIC_Register_IRQ(CSU2RPU_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,CSU2RPU_IRQn_handler);
	__enable_irq();
	for(int i=0;i<IPC_MAILBOX_CHANNELS;i++)
	{
		AlIpc_WriteMailbox(AL9000_IPC,0xabcdef00 + i,i);
	}
	AlIpc_IntrReq(AL9000_IPC,csu2rpu);
}
void RPU2CSU_IRQn_handler(void)
{
	printf("RPU2CSU Intr\r\n");
	for(int i=0;i<IPC_MAILBOX_CHANNELS;i++)
	{
//		printf("Mailbox %d Message:%x\r\n",i,AlIpc_ReadMailbox(AL9000_IPC,i));
	}
	AlIpc_IntrClear(AL9000_IPC,rpu2csu);
	AlIpc_SetSpinlockStatus(AL9000_IPC,spinlock0);
}
void RPU2CSU_IPC()
{
	ECLIC_Register_IRQ(RPU2CSU_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,RPU2CSU_IRQn_handler);
	__enable_irq();
	for(int i=0;i<IPC_MAILBOX_CHANNELS;i++)
	{
		AlIpc_WriteMailbox(AL9000_IPC,0xabcdef00 + i,i);
	}
	AlIpc_IntrReq(AL9000_IPC,rpu2csu);
}
void CSU2RPUACK_IRQn_handler(void)
{
	printf("CSU2RPUACK Intr\r\n");
	for(int i=0;i<IPC_MAILBOX_CHANNELS;i++)
	{
//		printf("Mailbox %d Message:%x\r\n",i,AlIpc_ReadMailbox(AL9000_IPC,i));
	}
	AlIpc_IntrAckClear(AL9000_IPC,csu2rpu);
	AlIpc_SetSpinlockStatus(AL9000_IPC,spinlock0);
}
void CSU2RPUACK_IPC()
{
	ECLIC_Register_IRQ(CSU2RPUACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,CSU2RPUACK_IRQn_handler);
	__enable_irq();
	for(int i=0;i<IPC_MAILBOX_CHANNELS;i++)
	{
		AlIpc_WriteMailbox(AL9000_IPC,0xabcdef00 + i,i);
	}
	AlIpc_IntrAck(AL9000_IPC,csu2rpu);
}
void RPU2CSUACK_IRQn_handler(void)
{
	printf("RPU2CSUACK Intr\r\n");
	for(int i=0;i<IPC_MAILBOX_CHANNELS;i++)
	{
//		printf("Mailbox %d Message:%x\r\n",i,AlIpc_ReadMailbox(AL9000_IPC,i));
	}
	AlIpc_IntrAckClear(AL9000_IPC,rpu2csu);
	AlIpc_SetSpinlockStatus(AL9000_IPC,spinlock0);
}
void RPU2CSUACK_IPC()
{
	ECLIC_Register_IRQ(RPU2CSUACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,RPU2CSUACK_IRQn_handler);
	__enable_irq();
	for(int i=0;i<IPC_MAILBOX_CHANNELS;i++)
	{
		AlIpc_WriteMailbox(AL9000_IPC,0xabcdef00 + i,i);
	}
	AlIpc_IntrAck(AL9000_IPC,rpu2csu);
	__disable_irq();
}
void APU2CSU_IRQn_handler(void)
{
	printf("APU2CSU Intr\r\n");
	for(int i=0;i<IPC_MAILBOX_CHANNELS;i++)
	{
//		printf("Mailbox %d Message:%x\r\n",i,AlIpc_ReadMailbox(AL9000_IPC,i));
	}
	AlIpc_IntrClear(AL9000_IPC,apu2csu);
}
void APU2CSUACK_IRQn_handler(void)
{
	printf("APU2CSUACK Intr\r\n");
	AlIpc_IntrAckClear(AL9000_IPC,apu2csu);
}
void APU2CSU_IPC()
{
	ECLIC_Register_IRQ(APU2CSU_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,APU2CSU_IRQn_handler);
	ECLIC_Register_IRQ(APU2CSUACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,APU2CSUACK_IRQn_handler);
	__enable_irq();
	AlIpc_IntrReq(AL9000_IPC,apu2csu);
	AlIpc_IntrAck(AL9000_IPC,apu2csu);
}
void CSU2APU_IRQn_handler(void)
{
	printf("CSU2APU Intr\r\n");
	AlIpc_IntrClear(AL9000_IPC,csu2apu);
}
void CSU2APUACK_IRQn_handler(void)
{
	printf("CSU2APUACK Intr\r\n");
	AlIpc_IntrAckClear(AL9000_IPC,csu2apu);
}
void CSU2APU_IPC()
{
	ECLIC_Register_IRQ(CSU2APU_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,CSU2APU_IRQn_handler);
	ECLIC_Register_IRQ(CSU2APUACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,CSU2APUACK_IRQn_handler);
	__enable_irq();
	AlIpc_IntrReq(AL9000_IPC,csu2apu);
	AlIpc_IntrAck(AL9000_IPC,csu2apu);
}
void APU2RPUREQ0_IRQn_handler(void)
{
	printf("APUREQ0\r\n");
	AlIpc_IntrClear(AL9000_IPC,apu2rpu_1);
}
void APU2RPUACK0_IRQn_handler(void)
{
	printf("APUACK0\r\n");
	AlIpc_IntrAckClear(AL9000_IPC,apu2rpu_1);
}
void APU2RPUREQ1_IRQn_handler(void)
{
	printf("APUREQ1\r\n");
	AlIpc_IntrClear(AL9000_IPC,apu2rpu_2);
}
void APU2RPUACK1_IRQn_handler(void)
{
	printf("APUACK1\r\n");
	AlIpc_IntrAckClear(AL9000_IPC,apu2rpu_2);
}
void APU2RPUREQ2_IRQn_handler(void)
{
	printf("APUREQ2\r\n");
	AlIpc_IntrClear(AL9000_IPC,apu2rpu_0);
}
void APU2RPUACK2_IRQn_handler(void)
{
	printf("APUACK2\r\n");
	AlIpc_IntrAckClear(AL9000_IPC,apu2rpu_0);
}
void APU2RPUREQ3_IRQn_handler(void)
{
	printf("APUREQ3\r\n");
	AlIpc_IntrClear(AL9000_IPC,apu2rpu_3);
}
void APU2RPUACK3_IRQn_handler(void)
{
	printf("APUACK3\r\n");
	AlIpc_IntrAckClear(AL9000_IPC,apu2rpu_3);
}

void RPU2APUREQ0_IRQn_handler(void)
{
	printf("RPUREQ0\r\n");
	AlIpc_IntrClear(AL9000_IPC,rpu2apu_0);
}
void RPU2APUACK0_IRQn_handler(void)
{
	printf("RPUACK0\r\n");
	AlIpc_IntrAckClear(AL9000_IPC,rpu2apu_0);
}
void RPU2APUREQ1_IRQn_handler(void)
{
	printf("RPUREQ1\r\n");
	AlIpc_IntrClear(AL9000_IPC,rpu2apu_1);
}
void RPU2APUACK1_IRQn_handler(void)
{
	printf("RPUACK1\r\n");
	AlIpc_IntrAckClear(AL9000_IPC,rpu2apu_1);
}
void RPU2APUREQ2_IRQn_handler(void)
{
	printf("RPUREQ2\r\n");
	AlIpc_IntrClear(AL9000_IPC,rpu2apu_2);
}
void RPU2APUACK2_IRQn_handler(void)
{
	printf("RPUACK2\r\n");
	AlIpc_IntrAckClear(AL9000_IPC,rpu2apu_2);
}
void RPU2APUREQ3_IRQn_handler(void)
{
	u8 spin_lock_num;
	printf("RPUREQ3\r\n");
	spin_lock_num=AlIpc_SpinlockHandler(AL9000_IPC);
	for(int i =0;i < IPC_MAILBOX_CHANNELS; i++)
	{
		printf("Mailbox %d Message:%x\r\n",i,AlIpc_ReadMailbox(AL9000_IPC,i));
	}
	switch (spin_lock_num)
	{
	case spinlock0:
		AlIpc_SetSpinlockStatus(AL9000_IPC, spinlock0);
		printf("Set Spinlock 0\r\n");
		break;
	case spinlock1:
		AlIpc_SetSpinlockStatus(AL9000_IPC, spinlock1);
		printf("Set Spinlock 1\r\n");
		break;
	case spinlock2:
		AlIpc_SetSpinlockStatus(AL9000_IPC, spinlock2);
		printf("Set Spinlock 2\r\n");
		break;
	case spinlock3:
		AlIpc_SetSpinlockStatus(AL9000_IPC, spinlock3);
		printf("Set Spinlock 3\r\n");
		break;	
	default:
		break;
	}
	AlIpc_IntrClear(AL9000_IPC,rpu2apu_3);
}
void RPU2APUACK3_IRQn_handler(void)
{
	printf("RPUACK3\r\n");
	AlIpc_IntrAckClear(AL9000_IPC,rpu2apu_3);
}
void APU2RPU_IPC_test()
{
	ECLIC_Register_IRQ(RPU2APU0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,RPU2APUREQ0_IRQn_handler);
	ECLIC_Register_IRQ(RPU2APU0ACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,RPU2APUACK0_IRQn_handler);
	ECLIC_Register_IRQ(RPU2APU1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,RPU2APUREQ1_IRQn_handler);
	ECLIC_Register_IRQ(RPU2APU1ACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,RPU2APUACK1_IRQn_handler);
	ECLIC_Register_IRQ(RPU2APU2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,RPU2APUREQ2_IRQn_handler);
	ECLIC_Register_IRQ(RPU2APU2ACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,RPU2APUACK2_IRQn_handler);
	ECLIC_Register_IRQ(RPU2APU3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,RPU2APUREQ3_IRQn_handler);
	ECLIC_Register_IRQ(RPU2APU3ACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,RPU2APUACK3_IRQn_handler);
	
	__enable_irq();
	printf("enable apu intr\r\n");
	Xpu_RpuFsbl();
	AlIpc_IntrReq(AL9000_IPC,apu2rpu_0);
	AlIpc_IntrAck(AL9000_IPC,apu2rpu_0);
	AlIpc_IntrReq(AL9000_IPC,apu2rpu_1);
	AlIpc_IntrAck(AL9000_IPC,apu2rpu_1);
	AlIpc_IntrReq(AL9000_IPC,apu2rpu_2);
	AlIpc_IntrAck(AL9000_IPC,apu2rpu_2);
	AlIpc_IntrReq(AL9000_IPC,apu2rpu_3);
	AlIpc_IntrAck(AL9000_IPC,apu2rpu_3);
	while(1);
}
void RPU2APU_IPC_test()
{
	ECLIC_Register_IRQ(APU2RPU0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,APU2RPUREQ0_IRQn_handler);
	ECLIC_Register_IRQ(APU2RPU0ACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,APU2RPUACK0_IRQn_handler);
	ECLIC_Register_IRQ(APU2RPU1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,APU2RPUREQ1_IRQn_handler);
	ECLIC_Register_IRQ(APU2RPU1ACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,APU2RPUACK1_IRQn_handler);
	ECLIC_Register_IRQ(APU2RPU2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,APU2RPUREQ2_IRQn_handler);
	ECLIC_Register_IRQ(APU2RPU2ACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,APU2RPUACK2_IRQn_handler);
	ECLIC_Register_IRQ(APU2RPU3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,APU2RPUREQ3_IRQn_handler);
	ECLIC_Register_IRQ(APU2RPU3ACK_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,APU2RPUACK3_IRQn_handler);
	__enable_irq();
//	*(uint32_t *)(0xf8801074) |= 0x4000;
//	REG32_WRITE(0xf840e004, 0x0);
//	Rpu_Apu0FsblStart(0x1a001000);
//	printf("release apu done\r\n");
	AlIpc_IntrReq(AL9000_IPC,rpu2apu_0);
	AlIpc_IntrAck(AL9000_IPC,rpu2apu_0);
	AlIpc_IntrReq(AL9000_IPC,rpu2apu_1);
	AlIpc_IntrAck(AL9000_IPC,rpu2apu_1);
	AlIpc_IntrReq(AL9000_IPC,rpu2apu_2);
	AlIpc_IntrAck(AL9000_IPC,rpu2apu_2);
	AlIpc_IntrReq(AL9000_IPC,rpu2apu_3);
	AlIpc_IntrAck(AL9000_IPC,rpu2apu_3);
	for(int i=0;i<IPC_MAILBOX_CHANNELS;i++)
	{
		AlIpc_WriteMailbox(AL9000_IPC,0xabcdef00 + i,i);
	}
	AlIpc_SetSpinlockStatus(AL9000_IPC,spinlock3);

	while(1);
}
void APU_spinlock_test()
{
	u32 apu_spinlock_cnt;
	apu_spinlock_cnt=0;
	REG32_WRITE(0x0a000004,apu_spinlock_cnt);
	Xpu_RpuFsbl();
	printf("apu boot rpu\r\n");
	while(1)
	{
		while(!AlIpc_GetSpinlockStatus(AL9000_IPC,spinlock0));
		apu_spinlock_cnt ++;
		REG32_WRITE(0x0a000004,REG32_READ(0x0a000004) + 1);
		AlIpc_SetSpinlockStatus(AL9000_IPC, spinlock0);
		if(apu_spinlock_cnt >= SPINLOCK_TEST_NUM)
			break;
	}
	printf("apu_spinlock0_cnt = %d\r\n",apu_spinlock_cnt);
	while (AlIpc_ReadMailbox(AL9000_IPC,0) != 0xaabbcc00);
	printf("apu_ddr_cnt = %d\r\n",REG32_READ(0x0a000004));
	AlIpc_WriteMailbox(AL9000_IPC,0xaabbcc01,0);
}
void RPU_spinlock_test()
{
	u32 rpu_spinlock_cnt;
	rpu_spinlock_cnt=0;
	printf("RPU main test\r\n");
	REG32_WRITE(0xf8801074,0x000073f0);
	REG32_WRITE(0xf840e004,0);
	printf("RPU spinlock test\r\n");

	rpu_spinlock_cnt=0;
	AlIpc_SetSpinlockStatus(AL9000_IPC, spinlock0);
	while(1)
	{
		while(!AlIpc_GetSpinlockStatus(AL9000_IPC,spinlock0));
		rpu_spinlock_cnt ++;
		REG32_WRITE(0x0a000004,REG32_READ(0x0a000004) + 1);
		AlIpc_SetSpinlockStatus(AL9000_IPC, spinlock0);
		if(rpu_spinlock_cnt >= SPINLOCK_TEST_NUM)
			break;
	}
	printf("rpu_spinlock0_cnt = %d\r\n",rpu_spinlock_cnt);
	AlIpc_WriteMailbox(AL9000_IPC,0xaabbcc00,0);
	while (AlIpc_ReadMailbox(AL9000_IPC,0) != 0xaabbcc01);
	printf("rpu_ddr_cnt = %d\r\n",REG32_READ(0x0a000004));
}