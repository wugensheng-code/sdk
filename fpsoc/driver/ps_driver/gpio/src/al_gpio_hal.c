#include "al_gpio_hw.h"
#include "al_gpio_ll.h"
#include "al_gpio_dev.h"
#include "al_gpio_hal.h"
#include "al_errno.h"


static AL_GPIO_DevStruct AL_GPIO_DevInstance[AL_GPIO_NUM_INSTANCE];

AL_S32 AlGpio_Hal_Init(AL_GPIO_HalStruct *Handle, AL_GPIO_InitStruct *InitConfig, AL_U32 DevId)
{
    AL_S32 ret = AL_OK;
    AL_GPIO_HwConfigStruct *HwConfig;
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    /* 1. look up hardware config */
    HwConfig = AlGpio_Dev_LookupConfig(DevId);
    
    if(HwConfig != AL_NULL) {
        Handle->Dev = &AL_GPIO_DevInstance[DevId];
    }

    /* 2. Init */
    ret = AlGpio_Dev_Init(Handle->Dev, InitConfig);
    if(ret != AL_OK) {
        //
    }
    printf("config BaseAddress is %x\r\n", Handle->Dev->Configs.BaseAddr);

    return ret;
}

// WritePin
AL_VOID AlGpio_Hal_WritePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U32 Data)
{
    AL_U8 i;
    AL_S32 Ret = AL_OK;
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_Dev_ClrWritePin(Handle->Dev, Pin, AL_ENABLE);
    AlGpio_Dev_SetDirectionPin(Handle->Dev, Pin, AL_OUTPUT_DIRECTION);
    AlGpio_Dev_WritePin(Handle->Dev, Pin, Data);
}

// ReadDRRegister
AL_S32 AlGpio_Hal_OutputReadPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    } 
    AlGpio_Dev_ClrDirectionPin(Handle->Dev, Pin, AL_ENABLE);
    AlGpio_Dev_SetDirectionPin(Handle->Dev, Pin, AL_OUTPUT_DIRECTION);
    AlGpio_Dev_OutputReadPin(Handle->Dev, Pin);
}

// ReadEXTRegisterIn
AL_S32 AlGpio_Hal_ReadEXTRegisterIn(AL_GPIO_HalStruct *Handle, AL_U8 Pin)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    } 
    AlGpio_Dev_ClrDirectionPin(Handle->Dev, Pin, AL_ENABLE);
    AlGpio_Dev_SetDirectionPin(Handle->Dev, Pin, AL_INPUT_DIRECTION);
    AlGpio_Dev_InputReadPin(Handle->Dev, Pin);
}

// ReadEXTRegisterOut
AL_S32 AlGpio_Hal_ReadEXTRegisterOut(AL_GPIO_HalStruct *Handle, AL_U8 Pin)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    } 
    AlGpio_Dev_ClrDirectionPin(Handle->Dev, Pin, AL_ENABLE);
    AlGpio_Dev_SetDirectionPin(Handle->Dev, Pin, AL_OUTPUT_DIRECTION);
    AlGpio_Dev_InputReadPin(Handle->Dev, Pin);
}

AL_VOID AlGpio_Hal_IntrCallbackHandler(AL_GPIO_HalStruct *Handle, AL_VOID *CallBackRef, Gpio_Handler FunPointer)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    if (FunPointer == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    Handle->Dev->Handler = FunPointer;
    Handle->Dev->EventCallBackRef = CallBackRef;
}

AL_VOID AlGpio_Hal_IntrHandler(AL_GPIO_HalStruct *Handle)
{
    AL_U8 Bank;
	AL_U32 IntrStatus;
	AL_U32 IntrEnable;

    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    for(Bank = 0U; Bank < Handle->Dev->MaxBanks; Bank++) {
        IntrStatus = AlGpio_Dev_IntrGetStatus(Handle->Dev, Bank);
        IntrEnable = AlGpio_Dev_IntrGetEnable(Handle->Dev, Bank);
        if((IntrStatus & IntrEnable) != (AL_U32)0) {
            AlGpio_Dev_IntrClrEnable(Handle->Dev, Bank, IntrStatus & IntrEnable);
            Handle->Dev->Handler(Handle->Dev->EventCallBackRef, Bank, (IntrStatus & IntrEnable));
        }
    }
}
