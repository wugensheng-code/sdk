#ifndef __AL_AXI_DMA_HAL_H_
#define __AL_AXI_DMA_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_axi_dma_dev.h"

typedef struct
{
    AlAxiDma_DevStruct      Dma;
    AL_Lock                 Mm2s_Lock;
    AL_Lock                 S2mm_Lock;
    AL_MailBox              Mm2s_EventQueue;
    AL_MailBox              S2mm_EventQueue;
} AlAxiDma_HalStruct;

/************************** Function Prototypes ******************************/
AL_S32 AlAxiDma_Hal_Init(AlAxiDma_HalStruct **Dev, AL_U32 DevId, AlAxiDma_InitStruct *InitConfig, AlAxiDma_EventCallBack EventCallBack);
AL_U32 AlAxiDma_Hal_Simple_Transfer(AlAxiDma_HalStruct *Handle, AL_U64 BuffAddr, AL_U32 Length, AL_U32 Direction, AL_U32 Timeout);

#ifdef __cplusplus
}
#endif

#endif
