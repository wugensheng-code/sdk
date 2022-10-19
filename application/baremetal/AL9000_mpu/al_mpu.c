#include "al_mpu.h"
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include "demosoc.h"
#include <string.h>

//#define OCM_TEST

uint32_t al_mpu[] = {mpu_apu}; 
//mpu_ddrs1, 
//mpu_apu, mpu_hpm0, mpu_hpm1, mpu_npu,mpu_ocms2};
//mpu_hpm0, mpu_hpm1, mpu_npu,mpu_ocms2, mpu_apu, mpu_ddrs1

static void sleep(int tick)
{
    for (int i = 0; i < tick; i++)
    {
        asm volatile("NOP");
    }
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
        SET_BIT((*ptr), SECURE);
    } else {
        CLR_BIT((*ptr), SECURE);
    }

    switch (Attr.Access) {
        case 0:
            CLR_BIT((*ptr), WRITE);
            CLR_BIT((*ptr), READ);
        break;
        case 1:
            SET_BIT((*ptr), WRITE);
            CLR_BIT((*ptr), READ);            
        break;
        case 2:
            CLR_BIT((*ptr), WRITE);
            SET_BIT((*ptr), READ); 
        break;
        case 3:
            SET_BIT((*ptr), WRITE);
            SET_BIT((*ptr), READ);            
        break;
    }

    if (Attr.Priviledge == 1) {
        SET_BIT((*ptr), PRIVILEDGE);
    } else {
        CLR_BIT((*ptr), PRIVILEDGE);
    }


    *(++ptr) = (uint32_t)Attr.StartAddr;
    *(++ptr) = (uint32_t)Attr.EndAddr;
    *(++ptr) = (uint32_t)Attr.GroupId;

    //*((uint32_t *)ptr)   = 0xc1u;
    //*((uint32_t *)++ptr) = 0x1u;
    //*((uint32_t *)++ptr) = 0x2u;
    //*((uint32_t *)++ptr) = 0x1u;

    //*((uint32_t *)(0xf840e010)) = 0xC1;
    //*((uint32_t *)(0xf840e014)) = 0x1; 
    //*((uint32_t *)(0xf840e018)) = 0x2;
    //*((uint32_t *)(0xf840e01c)) = 0x1;

    /* Enable Region Enable */
    if (Attr.RegionEnable == 1) {
        SET_BIT((*PtrMpuRasrRegion), REGIONEN);
    } else {
        CLR_BIT((*PtrMpuRasrRegion), REGIONEN);
    }

    AlMpu_Enable(InstancePtr);

    return Status;
}


/**
 * @brief Function is used to check all mpu of system
 */
uint32_t AlMpu_AllTest(void) 
{
    uint32_t MPU_Status = 0;
    uint32_t MPU_Counter = 0;
    uint32_t value = 0;
    uint32_t region = 0;

    RegionAttr Attr;
    memset(&Attr, 0, sizeof(Attr));
#ifdef OCM_TEST
    Attr.EndAddr = 0x61001;
#endif
    /* release memory */
    //*(uint32_t *)(0xf8801074) |= 0x4000; 
    //Attr.EndAddr = 0x61000;

    for (uint32_t MpuChoiceIndex = 0; MpuChoiceIndex < 32; MpuChoiceIndex++) {
        AlMpu *InstancePtr = (AlMpu *)al_mpu[MpuChoiceIndex];
        //AlMpu_Enable(InstancePtr);
        for (region = 1; region < MAX_REGIONSIZE; region++) {
            MPU_Status = AlMpu_GetMpuStatus(InstancePtr);
            if (MPU_Status == 1) {
                CLR_BIT(InstancePtr->MPU_CTRL, 0);
            }
            Attr.RegionEnable = 0;
            Attr.StartAddr = Attr.EndAddr + 0x00001 + region;
            Attr.EndAddr = Attr.StartAddr + 2;
            for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
                Attr.Priviledge = PrivilegeMode;
                for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                    Attr.Secure = SecureMode;
                    for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
                        Attr.GroupId = 1 << GroupIdNum;
                        for (int AccessMode = 0; AccessMode < MAX_ACCESSMODE; AccessMode++) {
                            Attr.Access = AccessMode;
                            Attr.RegionEnable = 1;
                            AlMpu_SetRegionAttr(InstancePtr, region, Attr);
                            MPU_Status = AlMpu_CompareTest(InstancePtr, region, Attr);
                            MPU_Status = AlMpu_GetMpuStatus(InstancePtr);
                            if (MPU_Status == 1) {
                                CLR_BIT(InstancePtr->MPU_CTRL, 0);
                            }
                        }
                    }
                }
            }
        }
    }

    return MPU_Status;
}

/**
 * @brief Function is used to check all mpu of system
 */
uint32_t ApuMpuTest(void) 
{
    uint32_t MPU_Status = 0;
    uint32_t MPU_Counter = 0;
    uint32_t value = 0;
    uint32_t region = 0;

    RegionAttr Attr;
    memset(&Attr, 0, sizeof(Attr));

    AlMpu *InstancePtr = (AlMpu *)mpu_apu;
    for (region = 1; region <= MAX_APU_REGIONSIZE; region++) {
        AlMpu_Disable(InstancePtr);
        Attr.RegionEnable = 0;
        Attr.StartAddr = Attr.EndAddr + 0x3;
        Attr.EndAddr = Attr.StartAddr + 1;
        for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
            Attr.Priviledge = PrivilegeMode;
            for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                Attr.Secure = SecureMode;
                for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
                    Attr.GroupId = 1 << GroupIdNum;
                    for (int AccessMode = 0; AccessMode < MAX_ACCESSMODE; AccessMode++) {
                        Attr.Access = AccessMode;
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
 * @brief Function is used to check all mpu of system
 */
uint32_t Ddrs0MpuTest(void) 
{
    uint32_t MPU_Status = 0;
    uint32_t MPU_Counter = 0;
    uint32_t value = 0;
    uint32_t region = 0;

    RegionAttr Attr;
    memset(&Attr, 0, sizeof(Attr));

    AlMpu *InstancePtr = (AlMpu *)mpu_ddrs0;
    for (region = 1; region <= MAX_REGIONSIZE; region++) {
        AlMpu_Disable(InstancePtr);
        Attr.RegionEnable = 0;
        Attr.StartAddr = Attr.EndAddr + 0x3;
        Attr.EndAddr = Attr.StartAddr + 1;
        for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
            Attr.Priviledge = PrivilegeMode;
            for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                Attr.Secure = SecureMode;
                for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
                    Attr.GroupId = 1 << GroupIdNum;
                    for (int AccessMode = 0; AccessMode < MAX_ACCESSMODE; AccessMode++) {
                        Attr.Access = AccessMode;
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
 * @brief Function is used to check all mpu of system
 */
uint32_t Ddrs1MpuTest(void) 
{
    uint32_t MPU_Status = 0;
    uint32_t MPU_Counter = 0;
    uint32_t value = 0;
    uint32_t region = 0;

    RegionAttr Attr;
    memset(&Attr, 0, sizeof(Attr));

    AlMpu *InstancePtr = (AlMpu *)mpu_ddrs1;
    for (region = 1; region <= MAX_REGIONSIZE; region++) {
        AlMpu_Disable(InstancePtr);
        Attr.RegionEnable = 0;
        Attr.StartAddr = Attr.EndAddr + 0x3;
        Attr.EndAddr = Attr.StartAddr + 1;
        for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
            Attr.Priviledge = PrivilegeMode;
            for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                Attr.Secure = SecureMode;
                for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
                    Attr.GroupId = 1 << GroupIdNum;
                    for (int AccessMode = 0; AccessMode < MAX_ACCESSMODE; AccessMode++) {
                        Attr.Access = AccessMode;
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
 * @brief Function is used to check all mpu of system
 */
uint32_t NpuMpuTest(void) 
{
    uint32_t MPU_Status = 0;
    uint32_t MPU_Counter = 0;
    uint32_t value = 0;
    uint32_t region = 0;

    RegionAttr Attr;
    memset(&Attr, 0, sizeof(Attr));

    AlMpu *InstancePtr = (AlMpu *)mpu_npu;
    for (region = 1; region <= MAX_REGIONSIZE; region++) {
        AlMpu_Disable(InstancePtr);
        Attr.RegionEnable = 0;
        Attr.StartAddr = Attr.EndAddr + 0x3;
        Attr.EndAddr = Attr.StartAddr + 1;
        for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
            Attr.Priviledge = PrivilegeMode;
            for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                Attr.Secure = SecureMode;
                for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
                    Attr.GroupId = 1 << GroupIdNum;
                    for (int AccessMode = 0; AccessMode < MAX_ACCESSMODE; AccessMode++) {
                        Attr.Access = AccessMode;
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
 * @brief Function is used to check all mpu of system
 */
uint32_t Hp0MpuTest(void) 
{
    uint32_t MPU_Status = 0;
    uint32_t MPU_Counter = 0;
    uint32_t value = 0;
    uint32_t region = 0;

    RegionAttr Attr;
    memset(&Attr, 0, sizeof(Attr));

    AlMpu *InstancePtr = (AlMpu *)mpu_hpm0;
    for (region = 1; region <= MAX_REGIONSIZE; region++) {
        AlMpu_Disable(InstancePtr);
        Attr.RegionEnable = 0;
        Attr.StartAddr = Attr.EndAddr + 0x3;
        Attr.EndAddr = Attr.StartAddr + 1;
        for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
            Attr.Priviledge = PrivilegeMode;
            for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                Attr.Secure = SecureMode;
                for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
                    Attr.GroupId = 1 << GroupIdNum;
                    for (int AccessMode = 0; AccessMode < MAX_ACCESSMODE; AccessMode++) {
                        Attr.Access = AccessMode;
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
 * @brief Function is used to check all mpu of system
 */
uint32_t Hp1MpuTest(void) 
{
    uint32_t MPU_Status = 0;
    uint32_t MPU_Counter = 0;
    uint32_t value = 0;
    uint32_t region = 0;

    RegionAttr Attr;
    memset(&Attr, 0, sizeof(Attr));

    AlMpu *InstancePtr = (AlMpu *)mpu_hpm1;
    for (region = 1; region <= MAX_REGIONSIZE; region++) {
        AlMpu_Disable(InstancePtr);
        Attr.RegionEnable = 0;
        Attr.StartAddr = Attr.EndAddr + 0x3;
        Attr.EndAddr = Attr.StartAddr + 1;
        for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
            Attr.Priviledge = PrivilegeMode;
            for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                Attr.Secure = SecureMode;
                for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
                    Attr.GroupId = 1 << GroupIdNum;
                    for (int AccessMode = 0; AccessMode < MAX_ACCESSMODE; AccessMode++) {
                        Attr.Access = AccessMode;
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
 * @brief Function is used to check all mpu of system
 */
uint32_t OcmMpuTest(void) 
{
    uint32_t MPU_Status = 0;
    uint32_t MPU_Counter = 0;
    uint32_t value = 0;
    uint32_t region = 0;

    RegionAttr Attr;
    memset(&Attr, 0, sizeof(Attr));
    Attr.EndAddr = 0x61010;

    AlMpu *InstancePtr = (AlMpu *)mpu_ocms2;
    for (region = 1; region <= MAX_REGIONSIZE; region++) {
        AlMpu_Disable(InstancePtr);
        Attr.RegionEnable = 0;
        Attr.StartAddr = Attr.EndAddr + 0x3;
        Attr.EndAddr = Attr.StartAddr + 1;
        for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
            Attr.Priviledge = PrivilegeMode;
            for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                Attr.Secure = SecureMode;
                for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
                    Attr.GroupId = 1 << GroupIdNum;
                    for (int AccessMode = 0; AccessMode < MAX_ACCESSMODE; AccessMode++) {
                        Attr.Access = AccessMode;
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

    uint32_t value = 0;
    uint32_t DefValue = 0xa5a5a5a5;
    uint32_t InitialValue = 0x12345678;
    uint32_t MPU_Status = MPU_SUCCESS;
    uint32_t ValueRenew = 0;
    uint32_t TempValue = Attr.Access;

    /* Initial memory data to 0x12345678*/
    Attr.Access = READWRITE;
    Attr.RegionEnable = 0;
    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, Reg_Sel, Attr);
    *((uint32_t *)(long)(addr)) = InitialValue;
    Attr.Access = TempValue;
    Attr.RegionEnable = 1;
    AlMpu_SetRegionAttr(InstancePtr, Reg_Sel, Attr);
    AlMpu_Enable(InstancePtr);

    /* Protected region test data */
    *((uint32_t *)(addr)) = DefValue;  
    value  = *((uint32_t *)(addr));

    switch (Attr.Access)
    {
        case READWRITE:
            if (DefValue == value) {
                printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Priviledge, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
            } else {
                printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Priviledge, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
            }
        break;
        case READONLY:
            if (InitialValue == value) {
                printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Priviledge, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
            } else {
                printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Priviledge, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
            }
        break;
        case WRITEONLY:
            Attr.Access = READWRITE;
            AlMpu_Disable(InstancePtr);
            AlMpu_SetRegionAttr(InstancePtr, Reg_Sel, Attr);
            ValueRenew  = *((uint32_t *)(addr));
            Attr.Access = WRITEONLY;
            AlMpu_SetRegionAttr(InstancePtr, Reg_Sel, Attr);
            AlMpu_Enable(InstancePtr);
            value = *((uint32_t *)(addr));
            if (DefValue == ValueRenew) {
                printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Priviledge, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
            } else {
                printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Priviledge, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
            }

        break;
        case NOREADWRITE:
            if ((InitialValue != value) && (value != DefValue)) {
                printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Priviledge, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
            } else {
                printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Priviledge, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
            }
        break;
    }  

    /* UnProtected region test data */
    addr =  (Attr.EndAddr + 1) * (0x1000) + 4;
    *((uint32_t *)(addr)) = DefValue;  
    value  = *((uint32_t *)(addr));  
    if (DefValue == value) {
        printf("UnProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Priviledge, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
    } else {
        printf("UnProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Priviledge, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
    }

    return MPU_Status;
}

uint32_t testcase(void)
{
    for (int i = 0; i < 30; i++)
    {
        *((uint32_t *)(0xF841B004)) = 0x0;
        *((uint32_t *)(0xF841B010)) = 0x301;
        *((uint32_t *)(0xF841B014)) = 0x61001; 
        *((uint32_t *)(0xF841B018)) = 0x61002;
        *((uint32_t *)(0xF841B01c)) = 0x1;
        *((uint32_t *)(0xF841B004)) = 0x1;

        *((uint32_t *)(0x61001034)) = 0x9999999; 
    }
}

/**
 * @brief Function is used to check all mpu of system
 */
uint32_t GroupIdTest_AllTest(void) 
{
    uint32_t MPU_Status = 0;
    uint32_t MPU_Counter = 0;
    uint32_t value = 0;
    uint32_t region = 1;

    //PL330_Init();

    RegionAttr Attr;
    memset(&Attr, 0, sizeof(Attr));

    /* release memory */
    *(uint32_t *)(0xf8801074) |= 0x4000; 

    AlMpu *InstancePtr = (AlMpu *)al_mpu[0];
    MPU_Status = AlMpu_GetMpuStatus(InstancePtr);
    
    if (MPU_Status == 1) {
        CLR_BIT(InstancePtr->MPU_CTRL, 0);
    }
    AlMpu_Enable(InstancePtr);

    Attr.StartAddr = 0x1;
    Attr.EndAddr = 0x2;          
    Attr.Priviledge = 0x1;
    
    Attr.Secure = 1;
    Attr.GroupId = 1;

    Attr.Access = 0;
    Attr.RegionEnable = 1;
    AlMpu_SetRegionAttr(InstancePtr, region, Attr);
    //MPU_Status = AlMpu_CompareTest(InstancePtr, region, Attr);
    //*((uint32_t *)(0x61001000)) = 0x99999;  
    //value  = *((uint32_t *)(0x61001000));
    *((uint32_t *)(0x1034)) = 0x9999999; 

    AlMpu_Disable(InstancePtr);



    return MPU_Status;
}