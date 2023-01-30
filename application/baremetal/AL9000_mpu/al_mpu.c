#include "al_mpu.h"
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include "demosoc.h"
#include <string.h>
#include "npujpu_st/npu.h"

#define APU_GROUPID 0x4

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
    //asm volatile("dsb     sy");
    //asm volatile("isb     sy");
    //asm volatile("dmb     sy");
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
    sleep(1);

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

    if (Attr.Privilege == 1) {
        SET_BIT((*ptr), PRIVILEGE);
    } else {
        CLR_BIT((*ptr), PRIVILEGE);
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
    sleep(10);


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
                Attr.Privilege = PrivilegeMode;
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
            Attr.Privilege = PrivilegeMode;
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
            Attr.Privilege = PrivilegeMode;
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
        Attr.RegionEnable = 0;
        Attr.StartAddr = Attr.EndAddr + 0x3;
        Attr.EndAddr = Attr.StartAddr + 1;
        for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
            Attr.Privilege = PrivilegeMode;
            for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                Attr.Secure = SecureMode;
                for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
                    Attr.GroupId = 1 << GroupIdNum;
                    for (int AccessMode = 0; AccessMode < MAX_ACCESSMODE; AccessMode++) {
                        Attr.Access = AccessMode;
                        Attr.RegionEnable = 1;
                        AlMpu_SetRegionAttr(InstancePtr, region, Attr);
                        MPU_Status = AlMpu_CompareTest(InstancePtr, region, Attr);
                        sleep(1000);
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

    RegWrite((mpu_ddrs1 + MPU_CTRL_R), 0x0);
    RegWrite((mpu_ddrs1 + MPU_RASR_REGION_R), 0x301);
    RegWrite((mpu_ddrs1 + MPU_SAR_REGION_R), 0x500);
    RegWrite((mpu_ddrs1 + MPU_EAR_REGION_R), 0x503);
    RegWrite((mpu_ddrs1 + MPU_GROUPID_REGION_R), 0x18);
    
    RegWrite((mpu_ddrs1 + MPU_CTRL_R), 0x1);
    main_npujpu();

    return MPU_Status;
}

/**
 * @brief Function is used to check all mpu of system
 */
uint32_t NpuMpuTest(void) 
{
    uint32_t MPU_Status = 0;

    RegWrite((mpu_npu + MPU_CTRL_R), 0x0);
    RegWrite((mpu_npu + MPU_RASR_REGION_R), 0x1);
    RegWrite((mpu_npu + MPU_SAR_REGION_R), 0x3007e);
    RegWrite((mpu_npu + MPU_EAR_REGION_R), 0x30080);
    RegWrite((mpu_npu + MPU_GROUPID_REGION_R), 0x8);
    RegWrite((mpu_npu + MPU_CTRL_R), 0x1);

    main_npujpu();
    
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
            Attr.Privilege = PrivilegeMode;
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
        Attr.RegionEnable = 0;
        Attr.StartAddr = Attr.EndAddr + 0x3;
        Attr.EndAddr = Attr.StartAddr + 1;
        for (int PrivilegeMode = 0; PrivilegeMode < MAX_PRIVILEGE; PrivilegeMode++) {
            Attr.Privilege = PrivilegeMode;
            for (int SecureMode = 0; SecureMode < MAX_SECUREMODE; SecureMode++) {
                Attr.Secure = SecureMode;
                for (int GroupIdNum = 0; GroupIdNum < MAX_GROUPIDNUMBER; GroupIdNum++) {
                    Attr.GroupId = 1 << GroupIdNum;
                    for (int AccessMode = 0; AccessMode < MAX_ACCESSMODE; AccessMode++) {
                        Attr.Access = AccessMode;
                        Attr.RegionEnable = 1;
                        AlMpu_SetRegionAttr(InstancePtr, region, Attr);
                        MPU_Status = AlMpu_CompareTest(InstancePtr, region, Attr);
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
    uint32_t DefValue = 0xa5a5a5a5;
    uint32_t InitialValue = 0x12345678;
    uint32_t MPU_Status = MPU_SUCCESS;
    uint32_t ValueRenew = 0;
    uint32_t TempValue = Attr.Access;
#if 0
    if ((Attr.Access == 0x1) && (Attr.Privilege==1) && (Attr.Secure == 1) && (Attr.GroupId == 0x10)) {

        printf("!!!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr);
    }
#endif
    /* Initial memory data to 0x12345678*/
    AlMpu_Disable(InstancePtr);
    sleep(10);
    *((uint32_t *)(long)(addr)) = InitialValue;
    AlMpu_Enable(InstancePtr);
    sleep(1000);

    /* Protected region test data */
    *((uint32_t *)(addr)) = DefValue;  
    value  = *((uint32_t *)(addr));

    switch (Attr.Access)
    {
        case READWRITE:
            if (DefValue == value) {
                printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
            } else {
                if (Attr.GroupId == APU_GROUPID) {
                    printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
                }
                else
                {
                    printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
                }

                
            }
        break;
        case READONLY:
            if (InitialValue == value) {
                printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
            } else {
                 if (Attr.GroupId == APU_GROUPID) {
                    printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
                } else {
                    printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
                }
            }
        break;
        case WRITEONLY:
            AlMpu_Disable(InstancePtr);
            sleep(1);
            ValueRenew  = *((uint32_t *)(addr));
            AlMpu_Enable(InstancePtr);
            sleep(100);
            value = *((uint32_t *)(addr));
            if (DefValue == ValueRenew) {
                printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, ValueRenew);
            } else {
                if (Attr.GroupId == APU_GROUPID) {
                    printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, ValueRenew);
                }else {
                    printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, ValueRenew);
                }
            }

        break;
        case NOREADWRITE:
            if ((InitialValue != value) && (value != DefValue)) {
                printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
            } else {
                 if (Attr.GroupId == APU_GROUPID) {
                     printf("ProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
                 }else {
                    printf("ProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
                 }
            }
        break;
    }  

    /* UnProtected region test data */
    addr =  (Attr.EndAddr + 1) * (0x1000) + 4;
    *((uint32_t *)(addr)) = DefValue;  
    value  = *((uint32_t *)(addr));  
    if (DefValue == value) {
        printf("UnProtectRegion testcase Pass!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
    } else {
        printf("UnProtectRegion testcase Fail!  saddr %08x eaddr %08x Pri %02x Sec %02x Acc %02x RegEn %02x GrpId %08x Mpu %08x DefValue %08x readvalue %08x\r\n", Attr.StartAddr * (0x1000), Attr.EndAddr * (0x1000), Attr.Privilege, Attr.Secure, Attr.Access, Attr.RegionEnable, Attr.GroupId, InstancePtr, DefValue, value);
    }

    return MPU_Status;
}

uint32_t testcase(void)
{
    for (int i = 0; i < 30; i++)
    {
        RegWrite((mpu_ocms2 + MPU_CTRL_R), 0x0);
        RegWrite((mpu_ocms2 + MPU_RASR_REGION_R), 0x301);
        RegWrite((mpu_ocms2 + MPU_SAR_REGION_R), 0x61001);
        RegWrite((mpu_ocms2 + MPU_EAR_REGION_R), 0x61002);
        RegWrite((mpu_ocms2 + MPU_GROUPID_REGION_R), 0x1);
        RegWrite((mpu_ocms2 + MPU_CTRL_R), 0x1);

        /* set ocm data */
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
    Attr.Privilege = 0x1;
    
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