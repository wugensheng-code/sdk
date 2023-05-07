#ifndef AL_AL_GPIO_H
#define AL_AL_GPIO_H


#ifdef __cplusplus
extern "C" {
#endif

#include "al_errno.h"
#include "al_type.h"
#include "al_gpio_dev.h"

struct AL_GPIO_DevStruct;

typedef struct
{
    AL_GPIO_DevStruct            *Dev;
} AL_GPIO_HalStruct;

AL_S32 AlGpio_Hal_Init(AL_GPIO_HalStruct *Handle, AL_GPIO_InitStruct *InitConfig, AL_U32 DevId);
AL_S32 AlGpio_Hal_OutputReadPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin);
AL_VOID AlGpio_Hal_WritePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U32 Data);
AL_S32 AlGpio_Hal_ReadEXTRegisterOut(AL_GPIO_HalStruct *Handle, AL_U8 Pin);
AL_S32 AlGpio_Hal_ReadEXTRegisterIn(AL_GPIO_HalStruct *Handle, AL_U8 Pin);
AL_VOID AlGpio_Hal_IntrCallbackHandler(AL_GPIO_HalStruct *Handle, AL_VOID *CallBackRef, Gpio_Handler FunPointer);
AL_VOID AlGpio_Hal_IntrHandler(AL_GPIO_HalStruct *Handle);


AL_S32 AlGpio_Hal2_InputReadPin(AL_GPIO_HalStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Hal2_SetDirectionPin(AL_GPIO_HalStruct *Gpio, AL_U32 Pin, AL_U32 Direction);

#ifdef __cplusplus
}
#endif

#endif