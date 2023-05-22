#ifndef AL_GPIO_DEV_H
#define AL_GPIO_DEV_H

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Include  Files *******************************/
#include "al_type.h"
#include "al_errno.h"
#include "al_gpio_hw.h"

/********************************* Exported Constant *****************************/
#define GPIO_REG_OFFSET              0x100U

#define AL_GPIO_ERR_ILLEGAL_PARAM    (AL_DEF_ERR(AL_GPIO, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))

typedef void (*Gpio_Handler) (AL_VOID *CallBackRef, AL_U32 Bank, AL_U32 Status);

/******************************** Exported Typedef *******************************/
typedef enum
{
    GPIO_INTR_TYPE_EDGE_RISING    =	 0x00U,
    GPIO_INTR_TYPE_EDGE_FALLING   =  0x01U,
    GPIO_INTR_TYPE_EDGE_BOTH      =	 0x02U,
    GPIO_INTR_TYPE_LEVEL_HIGH     =	 0x03U,
    GPIO_INTR_TYPE_LEVEL_LOW      =	 0x04U,
} AL_GPIO_IntrEnum;

typedef enum
{
    GPIO_INTR_LEVEL   =  0x0U,
    GPIO_INTR_EDGE    =  0x1U
} AL_GPIO_IntrTypeEnum;

typedef enum
{
    GPIO_INTR_LOW     =  0x0U,
    GPIO_INTR_HIGH    =  0x1U
} AL_GPIO_IntrPolarityEnum;

typedef enum
{
    GPIO_INTR_Single    =  0x0U,
    GPIO_INTR_Both      =  0x1U
} AL_GPIO_IntrBothEdgeEnum;

typedef enum
{
    GPIO_OUTPUT  =  0x1,
    GPIO_INPUT   =  0x0,
} AL_GPIO_DirEnum;

typedef enum
{
    AL_GPIO_BANK0   =  0,
    AL_GPIO_BANK1   =  1,
    AL_GPIO_BANK2   =  2,
    AL_GPIO_BANK3   =  3,
} AL_GPIO_BankEnum;

typedef enum
{
    AL_GPIO_ENABLE       =  0x1,
    AL_GPIO_DISABLE      =  0x0,
    AL_GPIO_ALL_ENABLE   =  0xffffffffU,
} AL_GPIO_ENEnum;

typedef enum
{
    MAX_PIN_NUMBER_IN_BANK_0   =   (AL_U32)31,
    MAX_PIN_NUMBER_IN_BANK_1   =   (AL_U32)53,
    MAX_PIN_NUMBER_IN_BANK_2   =   (AL_U32)85,
    MAX_PIN_NUMBER_IN_BANK_3   =   (AL_U32)117,
} AL_GPIO_PinNumEnum;

typedef struct
{
    AL_GPIO_HwConfigStruct   HwConfig;
    AL_VOID                  *EventCallBackRef;
    Gpio_Handler             Handler;
} AL_GPIO_DevStruct;

extern AL_GPIO_HwConfigStruct AlGpio_HwCfg[];

/************************** Exported Function *****************************/
AL_GPIO_HwConfigStruct *AlGpio_Dev_LookupConfig(AL_U32 DeviceId);
AL_S32 AlGpio_Dev_Init(AL_GPIO_DevStruct *Gpio, AL_GPIO_HwConfigStruct *HwConfig);
AL_VOID AlGpio_dev_GetBankPin(AL_U8 PinNumber, AL_U8 *BankNumber, AL_U8 *PinNumberInBank);

/*
 * Bank APIs
*/
AL_S32 AlGpio_Dev_OutputRead(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_S32 AlGpio_Dev_InputRead(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_VOID AlGpio_Dev_WriteBank(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Data);
AL_VOID AlGpio_Dev_SetDirection(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Direction);
AL_S32 AlGpio_Dev_GetDirection(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);

/*
 * Pin APIs
*/
AL_S32 AlGpio_Dev_OutputReadPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_S32 AlGpio_Dev_InputReadPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_WritePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Data);
AL_VOID AlGpio_Dev_SetDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction);
AL_S32 AlGpio_Dev_GetDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction);

/*
 * Bank intr APIs
*/
AL_VOID AlGpio_Dev_IntrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Raw);
AL_VOID AlGpio_Dev_IntrClr(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask);
AL_S32 AlGpio_Dev_IntrGetEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_VOID AlGpio_Dev_IntrEnableMask(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask);
AL_S32 AlGpio_Dev_IntrGetEnableMask(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_S32 AlGpio_Dev_IntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_S32 AlGpio_Dev_RawIntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_VOID AlGpio_Dev_IntrSetType(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 IntrType, AL_U32 IntrPolarity, AL_U32 IntrEdge);
AL_VOID AlGpio_Dev_IntrGetType(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 *IntrType, AL_U32 *IntrPolarity, AL_U32 *IntrEdge);
AL_VOID AlGpio_Dev_DebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Debounce);
AL_S32 AlGpio_Dev_GetDebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);
AL_VOID AlGpio_Dev_SyncEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Debounce);
AL_S32 AlGpio_Dev_GetSyncEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank);

/*
 * Pin intr APIs
*/
AL_VOID AlGpio_Dev_IntrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_IntrClrPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_IntrEnableMaskPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_IntrGetEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_IntrGetEnableMaskPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_IntrGetStatusPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_RawIntrGetStatusPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_IntrSetTypePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U8 IntrType);
AL_U8 AlGpio_Dev_IntrGetTypePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_DebounceEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_GetDebounceEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_SyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_GetSyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);

AL_S32 AlGpio_Dev_IntrCallbackHandler(AL_GPIO_DevStruct *Gpio, AL_VOID *CallBackRef, Gpio_Handler FunPointer);
AL_S32 AlGpio_Dev_IntrHandler(AL_GPIO_DevStruct *Gpio);

#ifdef __cplusplus
}
#endif

#endif