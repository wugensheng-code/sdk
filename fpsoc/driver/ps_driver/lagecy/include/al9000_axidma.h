#ifndef _AL_AXIDMA_H
#define _AL_AXIDMA_H
extern int PL330_InitDma();
#define REG_WRITE(reg_address, reg_wdata)  *(unsigned int*)(reg_address) = reg_wdata
#define REG_READ(reg_address)  *(unsigned int*)reg_address

#endif