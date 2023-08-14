#include "al_type.h"
#include "al_core.h"
#include "al_rv_core.h"


AL_U8 __get_PMPxCFG(AL_U32 idx)
{
    rv_csr_t pmpcfg = 0;

    if (idx >= __PMP_ENTRY_NUM) return 0;

    if (idx < 8) {
        pmpcfg = __RV_CSR_READ(CSR_PMPCFG0);
    } else {
        idx -= 8;
        pmpcfg = __RV_CSR_READ(CSR_PMPCFG2);
    }
    idx = idx << 3;
    return (AL_U8)((pmpcfg>>idx) & 0xFF);
}


void __set_PMPxCFG(AL_U32 idx, AL_U8 pmpxcfg)
{
    rv_csr_t pmpcfgx = 0;
    if (idx >= __PMP_ENTRY_NUM) return;

    if (idx < 8) {
        pmpcfgx = __RV_CSR_READ(CSR_PMPCFG0);
        idx = idx << 3;
        pmpcfgx = (pmpcfgx & ~(0xFFULL << idx)) | ((rv_csr_t)pmpxcfg << idx);
        __RV_CSR_WRITE(CSR_PMPCFG0, pmpcfgx);
    } else {
        idx -= 8;
        pmpcfgx = __RV_CSR_READ(CSR_PMPCFG2);
        idx = idx << 3;
        pmpcfgx = (pmpcfgx & ~(0xFFULL << idx)) | ((rv_csr_t)pmpxcfg << idx);
        __RV_CSR_WRITE(CSR_PMPCFG2, pmpcfgx);
    }
}

rv_csr_t __get_PMPCFGx(AL_U32 idx)
{
    switch (idx) {
        case 0: return __RV_CSR_READ(CSR_PMPCFG0);
        case 1: return __RV_CSR_READ(CSR_PMPCFG1);
        case 2: return __RV_CSR_READ(CSR_PMPCFG2);
        case 3: return __RV_CSR_READ(CSR_PMPCFG3);
        default: return 0;
    }
}

void __set_PMPCFGx(AL_U32 idx, rv_csr_t pmpcfg)
{
    switch (idx) {
        case 0: __RV_CSR_WRITE(CSR_PMPCFG0, pmpcfg); break;
        case 1: __RV_CSR_WRITE(CSR_PMPCFG1, pmpcfg); break;
        case 2: __RV_CSR_WRITE(CSR_PMPCFG2, pmpcfg); break;
        case 3: __RV_CSR_WRITE(CSR_PMPCFG3, pmpcfg); break;
        default: return;
    }
}

rv_csr_t __get_PMPADDRx(AL_U32 idx)
{
    switch (idx) {
        case 0: return __RV_CSR_READ(CSR_PMPADDR0);
        case 1: return __RV_CSR_READ(CSR_PMPADDR1);
        case 2: return __RV_CSR_READ(CSR_PMPADDR2);
        case 3: return __RV_CSR_READ(CSR_PMPADDR3);
        case 4: return __RV_CSR_READ(CSR_PMPADDR4);
        case 5: return __RV_CSR_READ(CSR_PMPADDR5);
        case 6: return __RV_CSR_READ(CSR_PMPADDR6);
        case 7: return __RV_CSR_READ(CSR_PMPADDR7);
        case 8: return __RV_CSR_READ(CSR_PMPADDR8);
        case 9: return __RV_CSR_READ(CSR_PMPADDR9);
        case 10: return __RV_CSR_READ(CSR_PMPADDR10);
        case 11: return __RV_CSR_READ(CSR_PMPADDR11);
        case 12: return __RV_CSR_READ(CSR_PMPADDR12);
        case 13: return __RV_CSR_READ(CSR_PMPADDR13);
        case 14: return __RV_CSR_READ(CSR_PMPADDR14);
        case 15: return __RV_CSR_READ(CSR_PMPADDR15);
        default: return 0;
    }
}

void __set_PMPADDRx(AL_U32 idx, rv_csr_t pmpaddr)
{
    switch (idx) {
        case 0: __RV_CSR_WRITE(CSR_PMPADDR0, pmpaddr); break;
        case 1: __RV_CSR_WRITE(CSR_PMPADDR1, pmpaddr); break;
        case 2: __RV_CSR_WRITE(CSR_PMPADDR2, pmpaddr); break;
        case 3: __RV_CSR_WRITE(CSR_PMPADDR3, pmpaddr); break;
        case 4: __RV_CSR_WRITE(CSR_PMPADDR4, pmpaddr); break;
        case 5: __RV_CSR_WRITE(CSR_PMPADDR5, pmpaddr); break;
        case 6: __RV_CSR_WRITE(CSR_PMPADDR6, pmpaddr); break;
        case 7: __RV_CSR_WRITE(CSR_PMPADDR7, pmpaddr); break;
        case 8: __RV_CSR_WRITE(CSR_PMPADDR8, pmpaddr); break;
        case 9: __RV_CSR_WRITE(CSR_PMPADDR9, pmpaddr); break;
        case 10: __RV_CSR_WRITE(CSR_PMPADDR10, pmpaddr); break;
        case 11: __RV_CSR_WRITE(CSR_PMPADDR11, pmpaddr); break;
        case 12: __RV_CSR_WRITE(CSR_PMPADDR12, pmpaddr); break;
        case 13: __RV_CSR_WRITE(CSR_PMPADDR13, pmpaddr); break;
        case 14: __RV_CSR_WRITE(CSR_PMPADDR14, pmpaddr); break;
        case 15: __RV_CSR_WRITE(CSR_PMPADDR15, pmpaddr); break;
        default: return;
    }
}


