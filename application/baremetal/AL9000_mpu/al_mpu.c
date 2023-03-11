#include "al_mpu.h"
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include "demosoc.h"
#include <string.h>

static void sleep(int tick)
{
    for (int i = 0; i < tick; i++)
    {
        asm volatile("NOP");
    }
#ifdef _AARCH_64
    asm volatile("dsb     sy");
    asm volatile("isb     sy");
    asm volatile("dmb     sy");
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

#ifdef MPU_APUTEST
    RegionSize = APU_REGIONSIZE;
#endif
    for (region = 1; region <= RegionSize; region++) {
        AlMpu_Disable(InstancePtr);
        Attr.RegionEnable = 0;
        Attr.StartAddr = Attr.EndAddr + 0x3;
        Attr.EndAddr = Attr.StartAddr + 1;
        for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
            Attr.Privilege = PrivilegeMode;
            for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                Attr.Secure = SecureMode;
                #if defined(MPU_APUTEST)
                {Attr.GroupId = APU_GROUPID_NUM;
                #else
                for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
                    Attr.GroupId = 1 << GroupIdNum;
                #endif
                    for (int AccessMode = 0; AccessMode < MAX_ACCESSMODE; AccessMode++) {
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

    uint32_t value = 0x7777777;
    uint32_t ModifyValue = 0xa5a5a5a5;
    uint32_t InitialValue = 0x12345678;
    uint32_t MPU_Status = MPU_SUCCESS;
    uint32_t TempValue = Attr.ReadWrite;

    /* Initial data to 0x12345678*/
    AlMpu_Disable(InstancePtr);

    *((uint32_t *)(long)(addr)) = InitialValue;
    AlMpu_Enable(InstancePtr);

    /* Protected region test data */
    *((uint32_t *)(addr)) = ModifyValue;
    AlMpu_Disable(InstancePtr);  
    value  = *((uint32_t *)(addr));
    AlMpu_Enable(InstancePtr);
    sleep(100);

#if defined(SUPPORT_NONSECURE)
    if (Attr.Secure == SECURE)
    {
        if (ModifyValue != value) {
            printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
        } else {
            printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
        }
        return MPU_Status;
    }
    
#endif

#if defined(SWITCH_TO_EL0_FROM_EL3)
    if (Attr.Privilege == PRIVILEGE_PROTECTED)
    {
        if (ModifyValue != value) {
            printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
        } else {
            printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
        }
        return MPU_Status;
    }
#endif

    switch (Attr.ReadWrite)
    {
        case READWRITE:     
            #if defined(MPU_APUTEST)
                if (ModifyValue == value) {
                    printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                } else {
                    printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                }
            #else
                if (ModifyValue == value) {
                    printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                } else {
                    #if defined(MPU_APU_RUN)
                    if (Attr.GroupId == APU_GROUPID_NUM) {
                    #elif defined(MPU_RPU_RUN)
                    if (Attr.GroupId == RPU_GROUPID_NUM) {
                    #else
                    if (Attr.GroupId == APU_GROUPID_NUM) {
                    #endif
                        printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                    }
                    else
                    {
                        printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                    } 
                }
            #endif              


        break;
        case READONLY:
            #if defined(MPU_APUTEST)
                if (ModifyValue == value) {
                    printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                } else {
                    printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                }
            #else
                if (InitialValue == value) {
                    printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                } else {
                    #if defined(MPU_APU_RUN)
                    if (Attr.GroupId == APU_GROUPID_NUM) {
                    #elif defined(MPU_RPU_RUN)
                    if (Attr.GroupId == RPU_GROUPID_NUM) {
                    #else
                    if (Attr.GroupId == APU_GROUPID_NUM) {
                    #endif
                        printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                    } else {
                        printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                    }
                }
            #endif    
        break;

        case WRITEONLY:
            #if defined(MPU_APUTEST)
                if (ModifyValue != value) {
                    printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                } else {
                    printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                }
            #else
                if (ModifyValue == value) {
                    printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                } else {
                    #if defined(MPU_APU_RUN)
                    if (Attr.GroupId == APU_GROUPID_NUM) {
                    #elif defined(MPU_RPU_RUN)
                    if (Attr.GroupId == RPU_GROUPID_NUM) {
                    #else
                    if (Attr.GroupId == APU_GROUPID_NUM) {
                    #endif
                        printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                    }else {
                        printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                    }
                }
            #endif   

        break;
        case NOREADWRITE:
            #if defined(MPU_APUTEST)
                if (InitialValue != value)
                {
                    printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                } else {
                    printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                }
            #else
                if (InitialValue == value) {
                    printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                } else {
                    #if defined(MPU_APU_RUN)
                    if (Attr.GroupId == APU_GROUPID_NUM) {
                    #elif defined(MPU_RPU_RUN)
                    if (Attr.GroupId == RPU_GROUPID_NUM) {
                    #else
                    if (Attr.GroupId == APU_GROUPID_NUM) {
                    #endif
                        printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                    }else {
                        printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
                    }
                }
            #endif
        break;

    }

    /* UnProtected region test data */
    addr =  (Attr.EndAddr + 1) * (0x1000) + 4;
    *((uint32_t *)(addr)) = ModifyValue;  
    value  = *((uint32_t *)(addr));  
    if (ModifyValue == value) {
        printf("UnProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
    } else {
        printf("UnProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x ModifyValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.ReadWrite, Attr.RegionEnable, Attr.GroupId, InstancePtr, ModifyValue, value);
    }

    return MPU_Status;
}
