#ifndef AL_GPIO_DEV_H
#define AL_GPIO_DEV_H

#ifdef __cplusplus
extern "C" {
#endif


#include "al_type.h"
#include "al_errno.h"
#include "al_gpio_hw.h"

#define GPIO_BANK0               0x00U  
#define GPIO_BANK1               0x01U 
#define GPIO_BANK2               0x02U  
#define GPIO_BANK3               0x03U 
#define GPIO_REG_OFFSET          0x100U  
#define GPIO_MAX_BANKS		     (AL_U8)4  
#define GPIO_BANK_MAX_PINS		 (AL_U32)32 
#define GPIO_MAX_PIN_NUM	     (AL_U32)118 
					      
#define AL_OUTPUT_DIRECTION      0x1
#define AL_INPUT_DIRECTION       0x0
#define AL_ENABLE                0x1
#define AL_ALL_ENABLE            0xffffffffU

#define AL_GPIO_STATE_READY      (AL_U32)0x0

#define AL_GPIO_ERR_ILLEGAL_PARAM    (AL_DEF_ERR(AL_GPIO, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))


typedef void (*Gpio_Handler) (AL_VOID *CallBackRef, AL_U32 Bank, AL_U32 Status);

typedef struct 
{
	AL_REG       BaseAddr;	    /**< Register base address */
} AL_GPIO_InitStruct;

typedef struct
{
    AL_GPIO_InitStruct       Configs;
    //AL_REG32                 BaseAddr;
    AL_VOID                  *EventCallBackRef;
    AL_U32                   MaxPinNum; 
	AL_U32                   MaxBanks;  
    Gpio_Handler             Handler;
} AL_GPIO_DevStruct;

extern AL_GPIO_HwConfigStruct AlGpio_HwCfg[];

AL_GPIO_HwConfigStruct *AlGpio_Dev_LookupConfig(AL_U32 DeviceId);
AL_S32 AlGpio_Dev_Init(AL_GPIO_DevStruct *Gpio, AL_GPIO_InitStruct *InitConfig);
AL_VOID AlGpio_dev_GetBankPin(AL_U8 PinNumber, AL_U8 *BankNumber, AL_U8 *PinNumberInBank);

/* 
 * Bank APIs
*/
AL_S32 AlGpio_Dev_OutputRead(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_S32 AlGpio_Dev_InputRead(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_VOID AlGpio_Dev_Write(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Data);
AL_VOID AlGpio_Dev_ClrWrite(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Data);
AL_VOID AlGpio_Dev_SetDirection(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Direction);
AL_VOID AlGpio_Dev_ClrDirection(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Direction);
AL_S32 AlGpio_Dev_GetDirection(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_VOID AlGpio_Dev_OutputEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 OpEnable);

/* 
 * Pin APIs
*/
AL_U32 AlGpio_Dev_OutputReadPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_U32 AlGpio_Dev_InputReadPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_WritePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Data);
AL_VOID AlGpio_Dev_ClrWritePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Data);
AL_VOID AlGpio_Dev_SetDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction);
AL_VOID AlGpio_Dev_ClrDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction);
AL_U32 AlGpio_Dev_GetDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction);
AL_VOID AlGpio_Dev_OutputEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 OpEnable);

/* 
 * Bank intr APIs
*/
AL_VOID AlGpio_Dev_IntrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Raw);
AL_VOID AlGpio_Dev_IntrClrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask);
AL_VOID AlGpio_Dev_IntrEoi(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask);
AL_U32 AlGpio_Dev_IntrGetEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_VOID AlGpio_Dev_IntrEnableMask(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask);
AL_VOID AlGpio_Dev_IntrClrEnableMask(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask);
AL_U32 AlGpio_Dev_IntrGetEnableMask(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_U32 AlGpio_Dev_IntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_U32 AlGpio_Dev_RawIntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_VOID AlGpio_Dev_IntrSetType(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 IntrType, AL_U32 IntrPolarity, AL_U32 IntrEdge);
AL_VOID AlGpio_Dev_IntrGetType(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 *IntrType, AL_U32 *IntrPolarity, AL_U32 *IntrEdge);
AL_VOID AlGpio_Dev_DebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Debounce);
AL_VOID AlGpio_Dev_DebounceClrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Debounce);
AL_U32 AlGpio_Dev_GetDebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_U32 AlGpio_Dev_GetDebounceClrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_VOID AlGpio_Dev_SyncEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Debounce);
AL_U32 AlGpio_Dev_GetSyncEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);

/* 
 * Pin intr APIs
*/
AL_VOID AlGpio_Dev_IntrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_IntrClrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_IntrEoiPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_IntrEnableMaskPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_IntrGetEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_IntrGetEnableMaskPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_IntrGetStatusPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_RawIntrGetStatusPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_IntrSetTypePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U8 IntrType);
AL_VOID AlGpio_Dev_IntrGetTypePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_DebounceEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_DebounceClrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_GetDebounceEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_GetDebounceClrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_SyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_GetSyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);


#ifdef __cplusplus
}
#endif

#endif