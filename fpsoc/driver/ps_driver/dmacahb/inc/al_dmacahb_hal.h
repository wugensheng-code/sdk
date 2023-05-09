#ifndef AL_DMACAHB_HAL_H
#define AL_DMACAHB_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_dmacahb_dev.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/
typedef struct
{
    AL_DMACAHB_ChStruct    *Channel;

#ifdef USE_RTOS
    AL_Lock             Lock;
#endif
} AL_DMACAHB_HalStruct;

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
AL_S32 AlDmacAhb_Hal_Init(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_ChInitStruct *InitConfig,
                          AL_DMACAHB_ChCallBackStruct *CallBack, AL_U32 DevId);

AL_S32 AlDmacAhb_Hal_DeInit(AL_DMACAHB_HalStruct *Handle);

AL_S32 AlDmacAhb_Hal_Start(AL_DMACAHB_HalStruct *Handle);

AL_S32 AlDmacAhb_Hal_StartBlock(AL_DMACAHB_HalStruct *Handle, AL_U32 Timeout);

AL_S32 AlDmacAhb_Hal_IoCtl(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_IoCtlCmdEnum Cmd, AL_VOID *Data);

#ifdef __cplusplus
}
#endif

#endif
