#include "al_type.h"
#include "al_rv_core.h"


AL_U8 __get_PMPxCFG(AL_U32 idx)
{
    AL_REGISTER pmpcfg = 0;

    if (idx >= __PMP_ENTRY_NUM) return 0;

    if (idx < 8) {
        pmpcfg = ARCH_SYSREG_READ(CSR_PMPCFG0);
    } else {
        idx -= 8;
        pmpcfg = ARCH_SYSREG_READ(CSR_PMPCFG2);
    }
    idx = idx << 3;
    return (AL_U8)((pmpcfg>>idx) & 0xFF);
}


void __set_PMPxCFG(AL_U32 idx, AL_U8 pmpxcfg)
{
    AL_REGISTER pmpcfgx = 0;
    if (idx >= __PMP_ENTRY_NUM) return;

    if (idx < 8) {
        pmpcfgx = ARCH_SYSREG_READ(CSR_PMPCFG0);
        idx = idx << 3;
        pmpcfgx = (pmpcfgx & ~(0xFFULL << idx)) | ((AL_REGISTER)pmpxcfg << idx);
        ARCH_SYSREG_WRITE(CSR_PMPCFG0, pmpcfgx);
    } else {
        idx -= 8;
        pmpcfgx = ARCH_SYSREG_READ(CSR_PMPCFG2);
        idx = idx << 3;
        pmpcfgx = (pmpcfgx & ~(0xFFULL << idx)) | ((AL_REGISTER)pmpxcfg << idx);
        ARCH_SYSREG_WRITE(CSR_PMPCFG2, pmpcfgx);
    }
}

AL_REGISTER __get_PMPCFGx(AL_U32 idx)
{
    switch (idx) {
        case 0: return ARCH_SYSREG_READ(CSR_PMPCFG0);
        case 1: return ARCH_SYSREG_READ(CSR_PMPCFG1);
        case 2: return ARCH_SYSREG_READ(CSR_PMPCFG2);
        case 3: return ARCH_SYSREG_READ(CSR_PMPCFG3);
        default: return 0;
    }
}

void __set_PMPCFGx(AL_U32 idx, AL_REGISTER pmpcfg)
{
    switch (idx) {
        case 0: ARCH_SYSREG_WRITE(CSR_PMPCFG0, pmpcfg); break;
        case 1: ARCH_SYSREG_WRITE(CSR_PMPCFG1, pmpcfg); break;
        case 2: ARCH_SYSREG_WRITE(CSR_PMPCFG2, pmpcfg); break;
        case 3: ARCH_SYSREG_WRITE(CSR_PMPCFG3, pmpcfg); break;
        default: return;
    }
}

AL_REGISTER __get_PMPADDRx(AL_U32 idx)
{
    switch (idx) {
        case 0: return ARCH_SYSREG_READ(CSR_PMPADDR0);
        case 1: return ARCH_SYSREG_READ(CSR_PMPADDR1);
        case 2: return ARCH_SYSREG_READ(CSR_PMPADDR2);
        case 3: return ARCH_SYSREG_READ(CSR_PMPADDR3);
        case 4: return ARCH_SYSREG_READ(CSR_PMPADDR4);
        case 5: return ARCH_SYSREG_READ(CSR_PMPADDR5);
        case 6: return ARCH_SYSREG_READ(CSR_PMPADDR6);
        case 7: return ARCH_SYSREG_READ(CSR_PMPADDR7);
        case 8: return ARCH_SYSREG_READ(CSR_PMPADDR8);
        case 9: return ARCH_SYSREG_READ(CSR_PMPADDR9);
        case 10: return ARCH_SYSREG_READ(CSR_PMPADDR10);
        case 11: return ARCH_SYSREG_READ(CSR_PMPADDR11);
        case 12: return ARCH_SYSREG_READ(CSR_PMPADDR12);
        case 13: return ARCH_SYSREG_READ(CSR_PMPADDR13);
        case 14: return ARCH_SYSREG_READ(CSR_PMPADDR14);
        case 15: return ARCH_SYSREG_READ(CSR_PMPADDR15);
        default: return 0;
    }
}

void __set_PMPADDRx(AL_U32 idx, AL_REGISTER pmpaddr)
{
    switch (idx) {
        case 0: ARCH_SYSREG_WRITE(CSR_PMPADDR0, pmpaddr); break;
        case 1: ARCH_SYSREG_WRITE(CSR_PMPADDR1, pmpaddr); break;
        case 2: ARCH_SYSREG_WRITE(CSR_PMPADDR2, pmpaddr); break;
        case 3: ARCH_SYSREG_WRITE(CSR_PMPADDR3, pmpaddr); break;
        case 4: ARCH_SYSREG_WRITE(CSR_PMPADDR4, pmpaddr); break;
        case 5: ARCH_SYSREG_WRITE(CSR_PMPADDR5, pmpaddr); break;
        case 6: ARCH_SYSREG_WRITE(CSR_PMPADDR6, pmpaddr); break;
        case 7: ARCH_SYSREG_WRITE(CSR_PMPADDR7, pmpaddr); break;
        case 8: ARCH_SYSREG_WRITE(CSR_PMPADDR8, pmpaddr); break;
        case 9: ARCH_SYSREG_WRITE(CSR_PMPADDR9, pmpaddr); break;
        case 10: ARCH_SYSREG_WRITE(CSR_PMPADDR10, pmpaddr); break;
        case 11: ARCH_SYSREG_WRITE(CSR_PMPADDR11, pmpaddr); break;
        case 12: ARCH_SYSREG_WRITE(CSR_PMPADDR12, pmpaddr); break;
        case 13: ARCH_SYSREG_WRITE(CSR_PMPADDR13, pmpaddr); break;
        case 14: ARCH_SYSREG_WRITE(CSR_PMPADDR14, pmpaddr); break;
        case 15: ARCH_SYSREG_WRITE(CSR_PMPADDR15, pmpaddr); break;
        default: return;
    }
}


