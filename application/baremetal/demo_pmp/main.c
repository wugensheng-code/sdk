#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"


extern void pmpSetup();
extern void pmpTest1();

// Overwrite Exception handle function
void handler0(unsigned long mcause, unsigned long sp)
{
    printf("Enter_exp_handler\n");
    printf("MCAUSE: 0x%lx\r\n", mcause);
    printf("MEPC  : 0x%lx\r\n", __RV_CSR_READ(CSR_MEPC));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    printf("MDCAUSE : 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("InsUnalign_EXCn=0\n");
    while(1);
}

void handler1(unsigned long mcause, unsigned long sp)
{
    printf("Enter_exp_handler\n");
    printf("MCAUSE: 0x%lx\r\n", mcause);
    printf("MEPC  : 0x%lx\r\n", __RV_CSR_READ(CSR_MEPC));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    printf("MDCAUSE : 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("InsAccFault_EXCn=1\n");
    while(1);
}

void handler2(unsigned long mcause, unsigned long sp)
{
    printf("Enter_exp_handler\n");
    printf("MCAUSE: 0x%lx\r\n", mcause);
    printf("MEPC  : 0x%lx\r\n", __RV_CSR_READ(CSR_MEPC));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    printf("MDCAUSE : 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("IlleIns_EXCn=2!!\n");
    while(1);
}

void handler3(unsigned long mcause, unsigned long sp)
{
    printf("Enter_exp_handler\n");
    printf("MCAUSE: 0x%lx\r\n", mcause);
    printf("MEPC  : 0x%lx\r\n", __RV_CSR_READ(CSR_MEPC));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    printf("MDCAUSE : 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("Break_EXCn=3!!\n");
    while(1);
}

void handler4(unsigned long mcause, unsigned long sp)
{
    printf("Enter_exp_handler\n");
    printf("MCAUSE: 0x%lx\r\n", mcause);
    printf("MEPC  : 0x%lx\r\n", __RV_CSR_READ(CSR_MEPC));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    printf("MDCAUSE : 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("LdAddrUnalign_EXCn=4!!\n");
    while(1);
}

void handler5(unsigned long mcause, unsigned long sp)
{
    printf("Enter_Load_exp_handler\n");
    printf("MCAUSE: 0x%lx\r\n", mcause);
    printf("MEPC  : 0x%lx\r\n", __RV_CSR_READ(CSR_MEPC));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    printf("MDCAUSE : 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("UnmatchedPMPEntryLoad!!\n");
    while(1);
}

void handler6(unsigned long mcause, unsigned long sp)
{
    printf("Enter_exp_handler\n");
    printf("MCAUSE: 0x%lx\r\n", mcause);
    printf("MEPC  : 0x%lx\r\n", __RV_CSR_READ(CSR_MEPC));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    printf("MDCAUSE : 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("StAddrUnalign_EXCn=6!!\n");
    while(1);
}

void handler7(unsigned long mcause, unsigned long sp)
{
    printf("Enter_exp_handler\n");
    printf("MCAUSE: 0x%lx\r\n", mcause);
    printf("MEPC  : 0x%lx\r\n", __RV_CSR_READ(CSR_MEPC));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    printf("MDCAUSE : 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("StAccessFault_EXCn=7!!\n");
    while(1);
}

void handler8(unsigned long mcause, unsigned long sp)
{
    printf("Enter_exp_handler\n");
    printf("MCAUSE: 0x%lx\r\n", mcause);
    printf("MEPC  : 0x%lx\r\n", __RV_CSR_READ(CSR_MEPC));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    printf("MDCAUSE : 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("UmodeEcall_EXC=8!!\n");
    while(1);
}

void handler11(unsigned long mcause, unsigned long sp)
{
    printf("Enter_exp_handler\n");
    printf("MCAUSE: 0x%lx\r\n", mcause);
    printf("MEPC  : 0x%lx\r\n", __RV_CSR_READ(CSR_MEPC));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    printf("MDCAUSE : 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("MmodeEcall_EXCn=11!!\n");
    while(1);
}

void handlerNMI(unsigned long mcause, unsigned long sp)
{
    printf("Enter_exp_handler\n");
    printf("MCAUSE: 0x%lx\r\n", mcause);
    printf("MEPC  : 0x%lx\r\n", __RV_CSR_READ(CSR_MEPC));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    printf("MDCAUSE : 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("NMI_EXCn!!\n");
    while(1);
}


int main(void)
{
    printf("MSUBM : 0x%lx\r\n", __RV_CSR_READ(CSR_MSUBM));
    printf("ExcRegConfig_Start!\n");
    Exception_Register_EXC(InsUnalign_EXCn,(unsigned long)handler0);
    Exception_Register_EXC(InsAccFault_EXCn,(unsigned long)handler1);
    Exception_Register_EXC(IlleIns_EXCn,(unsigned long)handler2);
    Exception_Register_EXC(Break_EXCn,(unsigned long)handler3);
    Exception_Register_EXC(LdAddrUnalign_EXCn,(unsigned long)handler4);
    Exception_Register_EXC(LdFault_EXCn,(unsigned long)handler5);
    Exception_Register_EXC(StAddrUnalign_EXCn ,(unsigned long)handler6);
    Exception_Register_EXC(StAccessFault_EXCn,(unsigned long)handler7);
    Exception_Register_EXC(UmodeEcall_EXCn,(unsigned long)handler8);
    Exception_Register_EXC(MmodeEcall_EXCn,(unsigned long)handler11);
    Exception_Register_EXC(NMI_EXCn,(unsigned long)handlerNMI);
    printf("ExcRegConfig_Completed!\n");

    // Read PMP CSR
    printf("0 PMPADDRx:0x%x  PMPxCFG:0x%x\n", __get_PMPADDRx(0), __get_PMPxCFG(0));
    printf("1 PMPADDRx:0x%x  PMPxCFG:0x%x\n", __get_PMPADDRx(1), __get_PMPxCFG(1));
    printf("2 PMPADDRx:0x%x  PMPxCFG:0x%x\n", __get_PMPADDRx(2), __get_PMPxCFG(2));
    printf("3 PMPADDRx:0x%x  PMPxCFG:0x%x\n", __get_PMPADDRx(3), __get_PMPxCFG(3));
    printf("PMPSetup_Start!\n");

    //Set PMP CSR
    pmpSetup();

    // READ PMP CSR
    printf("0 PMPADDRx:0x%x  PMPxCFG:0x%x\n", __get_PMPADDRx(0), __get_PMPxCFG(0));
    printf("1 PMPADDRx:0x%x  PMPxCFG:0x%x\n", __get_PMPADDRx(1), __get_PMPxCFG(1));
    printf("2 PMPADDRx:0x%x  PMPxCFG:0x%x\n", __get_PMPADDRx(2), __get_PMPxCFG(2));
    printf("3 PMPADDRx:0x%x  PMPxCFG:0x%x\n", __get_PMPADDRx(3), __get_PMPxCFG(3));
    printf("PMPSetup_Completed!\n");
    printf("Start_PMPTest!\n");

    //Test Case 1
    pmpTest1();
    printf("Finished!\n");
    return 0;
}

