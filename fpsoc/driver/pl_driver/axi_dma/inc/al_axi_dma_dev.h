#ifndef __AL_AXI_DMA_DEV_H_
#define __AL_AXI_DMA_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_axi_dma_ll.h"

#define AL_AxiDma_EVENT_START_BIT    6

#define AL_AxiDma_ERR_NULL_PTR                 (AL_DEF_ERR(AL_AxiDma, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_AxiDma_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_AxiDma, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_AxiDma_ERR_BUSY                     (AL_DEF_ERR(AL_AxiDma, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY))
#define AL_AxiDma_ERR_NOT_SUPPORT              (AL_DEF_ERR(AL_AxiDma, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT))
#define AL_AxiDma_EVENTS_TO_ERRS(Events)       (AL_DEF_ERR(AL_AxiDma, AL_LOG_LEVEL_ERROR, Events << AL_AxiDma_EVENT_START_BIT))

typedef enum
{
    EVENT_S2MM_DONE     = 0,
    EVENT_MM2S_DONE     = 1,
    EVENT_S2MM_ERROR    = 2,
    EVENT_MM2S_ERROR    = 3,
} AlAxiDma_EventEnum;

typedef struct
{
    AlAxiDma_EventEnum      Event;
    AL_U32                  EventData;
} AlAxiDma_EventStruct;

typedef (*AlAxiDma_EventCallBack)(AlAxiDma_EventStruct *Event, AL_VOID *CallbackRef);

typedef struct {
    AL_U32 DeviceId;
    AL_U32 HasKeyHole;
} AlAxiDma_InitStruct;

typedef struct 
{
    AlAxiDma_HwConfigStruct         HwConfig;
    AlAxiDma_InitStruct             InitConfig;
    AlAxiDma_EventCallBack          EventCallBack;
    AL_VOID                         *EventCallBackRef;

    AL_U64 RegBase;
    AL_U32 HasKeyHole;
    AL_U32 Initialized;
} AlAxiDma_DevStruct;


/************************** Function Prototypes ******************************/
AlAxiDma_HwConfigStruct *AlAxiDma_Dev_LookupConfig(AL_U32 DevId);
AL_S32 AlAxiDma_Dev_Init(AlAxiDma_DevStruct *Dma, AlAxiDma_HwConfigStruct *HwConfig, AlAxiDma_InitStruct *InitConfig);
AL_S32 AlAxiDma_Dev_Simple_Transfer(AlAxiDma_DevStruct *Dma, AL_U64 BuffAddr, AL_U32 Length, AlAxiDma_TransDirEnum Direction);
AL_VOID AlAxiDma_Dev_Reset(AlAxiDma_DevStruct *InstancePtr);
AL_BOOL AlAxiDma_Dev_Busy(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction);
AL_S32 AlAxiDma_Dev_ResetIsDone(AlAxiDma_DevStruct *Dma);
AL_S32 AlAxiDma_Dev_RegisterEventCallBack(AlAxiDma_DevStruct *Dma, AlAxiDma_EventCallBack CallBack, AL_VOID *CallBackRef);
AL_S32 AlAxiDma_Dev_Start_Transfer(AlAxiDma_DevStruct *Dma, AL_U64 BuffAddr, AL_U32 Length, AlAxiDma_TransDirEnum Direction);
AL_VOID AlAxiDma_Dev_Stop_Transfer(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction);
AL_VOID AlAxiDma_Dev_Transfer_DoneHandler(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction);
AL_VOID AlAxiDma_Dev_Transfer_ErrorHandler(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction);
AL_VOID AlAxiDma_Dev_Mm2s_IntrHandler(AL_VOID *Instance);
AL_VOID AlAxiDma_Dev_S2mm_IntrHandler(AL_VOID *Instance);

#ifdef __cplusplus
}
#endif

#endif
