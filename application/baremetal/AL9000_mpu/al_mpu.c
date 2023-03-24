#include "al_mpu.h"
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include "demosoc.h"
#include <string.h>

#ifdef SWITCH_TO_EL0_FROM_EL3
int cpu_in_priviledge_mode = 0;
#else
int cpu_in_priviledge_mode = 1;
#endif

#ifdef SUPPORT_NONSECURE
int cpu_in_secure_mode = 0;
#else
int cpu_in_secure_mode = 1;
#endif

/**
 * @desc  : do_bad_sync handles the impossible case in the Synchronous Abort vector,
 * 		     you must re-implement event handle.
 * @param {pt_regs} *pt_regs
 * @param {unsigned int} esr
 * @return {*}
 */
#ifndef MPU_PRIVILEGE_TEST
void do_sync_handle(struct pt_regs *pt_regs, unsigned int esr)
{
	asm volatile("mrs x25, elr_el1; add x25, x25, #0x04; msr elr_el1, x25" ::: "x25");

	#ifdef sync_handle_print
	{
		extern char * out_msg;
		printf("%s \r\n", out_msg);
	}
	#endif
}
#endif /* MPU_PRIVILEGE_TEST */

static void sleep(unsigned long tick)
{
    for (int i = 0; i < tick; i++)
    {
        asm volatile("NOP");
    }
#if (defined _AARCH_64 || defined __aarch64__)
    asm volatile("dsb     sy" :::"memory");
    asm volatile("isb     sy" :::"memory");
    asm volatile("dmb     sy" :::"memory");
#endif
}

unsigned int RegRead(uint32_t reg_address)
{
    return *((volatile uint32_t *)(reg_address));
}

void RegWrite(uint32_t reg_address, uint32_t reg_wdata)
{
    *((volatile uint32_t *)(reg_address)) = reg_wdata;
}

uint32_t AlMpu_disable_region(AlMpu *InstancePtr,  uint32_t Reg_Sel)
{
	uint32_t* PtrMpuRasrRegion = (uint32_t *)InstancePtr;
    uint32_t* ptr = (uint32_t *)InstancePtr;
    ptr = ptr + 4;

    for (uint32_t i = 1; i < Reg_Sel; i++)
    {
        ptr = ptr + 4;
    }

    /* store the MpuRasrRegion to enable the region protected */
    PtrMpuRasrRegion = ptr;
	CLR_BIT((*PtrMpuRasrRegion), REGIONEN);

}

uint32_t AlMpu_enable_region(AlMpu *InstancePtr,  uint32_t Reg_Sel)
{
	uint32_t* PtrMpuRasrRegion = (uint32_t *)InstancePtr;
    uint32_t* ptr = (uint32_t *)InstancePtr;
    ptr = ptr + 4;

    for (uint32_t i = 1; i < Reg_Sel; i++)
    {
        ptr = ptr + 4;
    }

    /* store the MpuRasrRegion to enable the region protected */
    PtrMpuRasrRegion = ptr;

	SET_BIT((*PtrMpuRasrRegion), REGIONEN);
}

uint32_t AlMpu_SetRegionAttr(AlMpu* InstancePtr, uint32_t Region, RegionAttr Attr)
{
    uint32_t Status = MPU_SUCCESS;
    uint32_t* ptr = (uint32_t *)InstancePtr;
    uint32_t* PtrMpuRasrRegion = (uint32_t *)InstancePtr;
    ptr = ptr + 4;

    AlMpu_Disable(InstancePtr);

    for (uint32_t i = 1; i < Region; i++)
    {
        ptr = ptr + 4;
    }

    /* store the MpuRasrRegion to enable the region protected */
    PtrMpuRasrRegion = ptr;
    *ptr = 0;

    if (Attr.Secure == 1) {
        SET_BIT((*ptr), SECURE_BIT);
    } else {
        CLR_BIT((*ptr), SECURE_BIT);
    }

    switch (Attr.ReadWrite) {
        case 0:
            CLR_BIT((*ptr), WRITE_BIT);
            CLR_BIT((*ptr), READ_BIT);
        break;
        case 1:
            SET_BIT((*ptr), WRITE_BIT);
            CLR_BIT((*ptr), READ_BIT);
        break;
        case 2:
            CLR_BIT((*ptr), WRITE_BIT);
            SET_BIT((*ptr), READ_BIT);
        break;
        case 3:
            SET_BIT((*ptr), WRITE_BIT);
            SET_BIT((*ptr), READ_BIT);
        break;
    }

    if (Attr.Privilege == 1) {
        SET_BIT((*ptr), PRIVILEGE_BIT);
    } else {
        CLR_BIT((*ptr), PRIVILEGE_BIT);
    }

    if (Attr.InterruptEnable == 1) {
        SET_BIT((*ptr), INTERRUPT_BIT);
    } else {
        CLR_BIT((*ptr), INTERRUPT_BIT);
    }


    *(++ptr) = (uint32_t)Attr.StartAddr;
    *(++ptr) = (uint32_t)Attr.EndAddr;
    *(++ptr) = (uint32_t)Attr.GroupId;

    /* Enable Region Enable */
    if (Attr.RegionEnable == 1) {
        SET_BIT((*PtrMpuRasrRegion), REGIONEN);
    } else {
        CLR_BIT((*PtrMpuRasrRegion), REGIONEN);
    }

    AlMpu_Enable(InstancePtr);

    return Status;
}


uint32_t SetAttrAndCompare(AlMpu *InstancePtr, RegionAttr Attr)
{
    uint32_t MPU_Status = MPU_SUCCESS;
    uint32_t MPU_Counter = 0;
    uint32_t value = 0;
    uint32_t region = 0;
    uint32_t RegionSize = REGIONSIZE;

	if (InstancePtr == MpuApu) {
		RegionSize = APU_REGIONSIZE;
	}

    for (region = 1; region <= RegionSize; region++) {
        AlMpu_Disable(InstancePtr);

        Attr.RegionEnable = 0;
        Attr.StartAddr = Attr.EndAddr + 0x3;
        Attr.EndAddr   = Attr.StartAddr + 1;

        for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
            Attr.Privilege = PrivilegeMode;
            for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                Attr.Secure = SecureMode;

				for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
					int AccessMode = 0x0;

					Attr.GroupId = 1 << GroupIdNum;
					for (AccessMode = 0x0; AccessMode < MAX_ACCESSMODE; AccessMode ++) {
						Attr.ReadWrite = AccessMode;
						Attr.RegionEnable = 1;
						AlMpu_SetRegionAttr(InstancePtr, region, Attr);
						MPU_Status = AlMpu_CompareTest(InstancePtr, region, Attr);
						AlMpu_Disable(InstancePtr);
					}
				}
            }
        }
    }

    return MPU_Status;

}


/**
 * @brief Function for MPU verification
 * @InstancePtr points to the AlMpu structure
 * @Reg_Sel region number of the MPU region
 * @Attr Attribute of the MPU region
 */
uint32_t AlMpu_CompareTest(AlMpu *InstancePtr,  uint32_t Reg_Sel, RegionAttr Attr)
{
    uint32_t addr = 0;
    addr = Attr.StartAddr * (0x1000) + 4;

    volatile uint32_t read_value_before_mpu_disable;
	volatile uint32_t read_value_after_mpu_disable;

    uint32_t ModifyValue = 0xa5a5a5a5;
    uint32_t InitialValue = 0x12345678;

    uint32_t MPU_Status = MPU_SUCCESS;

    /* Initial data to 0x12345678*/


	 do {
		AlMpu_disable_region(InstancePtr, Reg_Sel);
		AlMpu_Disable(InstancePtr);

		*((volatile uint32_t *)(long)(addr)) = InitialValue;

		printf("InitialValue write %x %x\r\n", addr, InitialValue);
		printf("read out 0x%x 0x%x\r\n", addr, *((volatile uint32_t *)(long)(addr)));

	}while (*((volatile uint32_t *)(long)(addr)) != InitialValue);

	AlMpu_enable_region(InstancePtr, Reg_Sel);
    AlMpu_Enable(InstancePtr);

    /* Protected region test data */
    *((volatile uint32_t *)(addr)) = ModifyValue;
	printf("write 0x%x 0x%x\r\n", addr, ModifyValue);

	read_value_before_mpu_disable  = *((volatile uint32_t *)(addr));
	printf("read 0x%x 0x%x\r\n", addr, read_value_before_mpu_disable);

    AlMpu_disable_region(InstancePtr, Reg_Sel);
    AlMpu_Disable(InstancePtr);
	read_value_after_mpu_disable = *((volatile uint32_t *)(addr));
	printf("after disable 0x%x 0x%x\r\n", addr, read_value_after_mpu_disable);

	AlMpu_enable_region(InstancePtr, Reg_Sel);
    AlMpu_Enable(InstancePtr);

	{
		// can't write & and can't  read case
		if (((Attr.Secure == 1) && (cpu_in_secure_mode == 0))				//secure match
				|| ((Attr.Privilege == PRIVILEGE_PROTECTED) && (cpu_in_priviledge_mode == 0))	//secure match
#if (defined _AARCH_64 || defined __aarch64__)
				|| (Attr.GroupId == APU_GROUPID_NUM)
#else
				|| (Attr.GroupId == RPU_GROUPID_NUM)
#endif
				|| (Attr.ReadWrite == NOREADWRITE))
		{
			if ((read_value_before_mpu_disable != ModifyValue) && (read_value_after_mpu_disable != ModifyValue)) {
				printf("Pass! saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x readvalue %08x\r\n",
					Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr,
					ModifyValue, read_value_after_mpu_disable, read_value_before_mpu_disable);
			} else {
				printf("Fail! saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x readvalue %08x\r\n",
					Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr,
					ModifyValue, read_value_after_mpu_disable, read_value_before_mpu_disable);
				printf("%d \r\n", __LINE__);
				printf("%d \r\n", __LINE__); while(1);
			}
		} else if(Attr.ReadWrite == WRITEONLY) {// can write & cant read case
			if ((read_value_before_mpu_disable != ModifyValue) && (read_value_after_mpu_disable == ModifyValue)) {
				printf("Pass! saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x readvalue %08x\r\n",
					Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr,
					ModifyValue, read_value_after_mpu_disable, read_value_before_mpu_disable);
			} else {
				printf("Fail! saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x readvalue %08x\r\n",
					Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr,
					ModifyValue, read_value_after_mpu_disable, read_value_before_mpu_disable);
				printf("%d \r\n", __LINE__);
				while(1);
			}
		} else if(Attr.ReadWrite == READONLY) {// can write & cant read case
			if ((read_value_before_mpu_disable == InitialValue) && (read_value_after_mpu_disable == InitialValue)) {
				printf("Pass! saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x readvalue %08x\r\n",
					Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr,
					ModifyValue, read_value_after_mpu_disable, read_value_before_mpu_disable);
			} else {
				printf("Fail! saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x readvalue %08x\r\n",
					Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr,
					ModifyValue, read_value_after_mpu_disable, read_value_before_mpu_disable);
				printf("%d \r\n", __LINE__);
				while(1);
			}
		} else { //read & write enable
			if ((read_value_before_mpu_disable == ModifyValue) && (read_value_after_mpu_disable == ModifyValue)) {
				printf("Pass! saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x readvalue %08x\r\n",
					Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr,
					ModifyValue, read_value_after_mpu_disable, read_value_before_mpu_disable);
			} else {
				printf("Fail! saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x readvalue %08x\r\n",
					Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr,
					ModifyValue, read_value_after_mpu_disable, read_value_before_mpu_disable);
				printf("%d \r\n", __LINE__);
				while(1);
			}
		}
    }

    return MPU_Status;
}
