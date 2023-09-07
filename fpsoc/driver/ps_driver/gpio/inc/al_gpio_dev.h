/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef __AL_GPIO_DEV_H_
#define __AL_GPIO_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Include  Files *******************************/
#include "al_gpio_ll.h"

/********************************* Exported Constant *****************************/
#define GPIO_REG_OFFSET              0x100

#define AL_GPIO_ERR_ILLEGAL_PARAM    (AL_DEF_ERR(AL_GPIO, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))



/******************************** Exported Typedef *******************************/
typedef enum
{
    GPIO_INTR_TYPE_EDGE_RISING    =  0x00,
    GPIO_INTR_TYPE_EDGE_FALLING   =  0x01,
    GPIO_INTR_TYPE_EDGE_BOTH      =  0x02,
    GPIO_INTR_TYPE_LEVEL_HIGH     =  0x03,
    GPIO_INTR_TYPE_LEVEL_LOW      =  0x04,
} AL_GPIO_IntrEnum;

typedef enum
{
    AL_GPIO_Event = 0x1,
} AL_GPIO_EventId;

typedef struct
{
    AL_GPIO_EventId     Events;
} AL_GPIO_EventStruct;

typedef AL_VOID (*AL_GPIO_EventCallBack)(AL_GPIO_EventStruct GpioEvent, AL_VOID *CallbackRef);

typedef enum
{
    GPIO_INTR_LEVEL   =  0x0,
    GPIO_INTR_EDGE    =  0x1,
} AL_GPIO_IntrTypeEnum;

typedef enum
{
    GPIO_INTR_LOW     =  0x0,
    GPIO_INTR_HIGH    =  0x1,
} AL_GPIO_IntrPolarityEnum;

typedef enum
{
    GPIO_INTR_Single    =  0x0,
    GPIO_INTR_Both      =  0x1,
} AL_GPIO_IntrBothEdgeEnum;

typedef enum
{
    GPIO_OUTPUT  =  0x1,
    GPIO_INPUT   =  0x0,
} AL_GPIO_DirEnum;

typedef enum
{
    AL_GPIO_BANK0   =  (AL_U32)0,
    AL_GPIO_BANK1   =  (AL_U32)1,
    AL_GPIO_BANK2   =  (AL_U32)2,
    AL_GPIO_BANK3   =  (AL_U32)3,
} AL_GPIO_BankEnum;

typedef enum
{
    AL_GPIO_ENABLE       =  0x1,
    AL_GPIO_DISABLE      =  0x0,
    AL_GPIO_ALL_ENABLE   =  0xffffffff,
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
    AL_GPIO_EventCallBack    EventCallBack;
} AL_GPIO_DevStruct;


/************************** Exported Function *****************************/
AL_GPIO_HwConfigStruct *AlGpio_Dev_LookupConfig(AL_U32 DeviceId);
AL_S32 AlGpio_Dev_Init(AL_GPIO_DevStruct *Gpio, AL_GPIO_HwConfigStruct *HwConfig);
AL_VOID AlGpio_dev_GetBankPin(AL_U32 PinNumber, AL_U32 *BankNumber, AL_U32 *PinNumberInBank);

/*
 * Bank APIs
*/
AL_S32 AlGpio_Dev_OutputRead(AL_GPIO_DevStruct *Gpio, AL_U32 Bank);
AL_S32 AlGpio_Dev_InputRead(AL_GPIO_DevStruct *Gpio, AL_U32 Bank);
AL_VOID AlGpio_Dev_WriteBank(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Data);
AL_VOID AlGpio_Dev_SetDirection(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Direction);
AL_S32 AlGpio_Dev_GetDirection(AL_GPIO_DevStruct *Gpio, AL_U32 Bank);

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
AL_VOID AlGpio_Dev_IntrEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Raw);
AL_VOID AlGpio_Dev_IntrClr(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Mask);
AL_S32 AlGpio_Dev_IntrGetEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank);
AL_VOID AlGpio_Dev_IntrEnableMask(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Mask);
AL_S32 AlGpio_Dev_IntrGetEnableMask(AL_GPIO_DevStruct *Gpio, AL_U32 Bank);
AL_S32 AlGpio_Dev_IntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U32 Bank);
AL_S32 AlGpio_Dev_RawIntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U32 Bank);
AL_VOID AlGpio_Dev_IntrSetType(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 IntrType, AL_U32 IntrPolarity, AL_U32 IntrEdge);
AL_VOID AlGpio_Dev_IntrGetType(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 *IntrType, AL_U32 *IntrPolarity, AL_U32 *IntrEdge);
AL_VOID AlGpio_Dev_DebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Debounce);
AL_S32 AlGpio_Dev_GetDebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank);
AL_VOID AlGpio_Dev_SyncEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Debounce);
AL_S32 AlGpio_Dev_GetSyncEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank);

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
AL_VOID AlGpio_Dev_IntrSetTypePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 IntrType);
AL_U32 AlGpio_Dev_IntrGetTypePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_DebounceEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_GetDebounceEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_VOID AlGpio_Dev_SyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);
AL_BOOL AlGpio_Dev_GetSyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin);

AL_S32 AlGpio_Dev_RegisterEventCallBack(AL_GPIO_DevStruct *Gpio, AL_GPIO_EventCallBack CallBack, AL_VOID *CallBackRef);
AL_VOID AlGpio_Dev_IntrHandler(void *Instance);

#ifdef __cplusplus
}
#endif

#endif