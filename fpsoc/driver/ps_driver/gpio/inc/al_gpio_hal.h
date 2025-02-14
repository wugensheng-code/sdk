/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef __AL_GPIO_HAL_H_
#define __AL_GPIO_HAL_H_


#ifdef __cplusplus
extern "C" {
#endif

/********************************* Include  Files ********************************/
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
    GPIO_PIN_OUTPUT   =  0x1,
    GPIO_PIN_INPUT    =  0x0,
    GPIO_BANK_OUTPUT  =  0xffffffff,
    GPIO_BANK_INPUT   =  0x0,
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
    AL_GPIO_INTR_BANK0   =  BIT(0),
    AL_GPIO_INTR_BANK1   =  BIT(1),
    AL_GPIO_INTR_BANK2   =  BIT(2),
    AL_GPIO_INTR_BANK3   =  BIT(3),
} AL_GPIO_Intr_BankEnum;

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
    AL_GPIO_BankEnum        Bank;
    AL_U32                  EventData;
} AL_GPIO_EventStruct;

typedef AL_VOID (*AL_GPIO_EventCallBack)(AL_GPIO_EventStruct GpioEvent, AL_VOID *CallbackRef);

/**************************** Exported Typedef ******************************/

typedef struct
{
    AL_GPIO_HwConfigStruct   HwConfig;
    AL_VOID                  *EventCallBackRef;
    AL_GPIO_EventCallBack    EventCallBack;
    AL_U32                   IntrBank;
} AL_GPIO_HalStruct;

/************************** Exported Function *****************************/
AL_S32 AlGpio_Hal_Init(AL_GPIO_HalStruct **Handle, AL_U32 DevId, AL_GPIO_EventCallBack CallBack);

/*
 * Bank APIs
*/
AL_S32 AlGpio_Hal_ReadBankInput(AL_GPIO_HalStruct *Handle, AL_U32 Bank);
AL_S32 AlGpio_Hal_ReadBankOutput(AL_GPIO_HalStruct *Handle, AL_U32 Bank);
AL_S32 AlGpio_Hal_WriteBank(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Data);

/*
 * Pin APIs
*/
AL_S32 AlGpio_Hal_ReadPinInput(AL_GPIO_HalStruct *Handle, AL_U32 Pin);
AL_S32 AlGpio_Hal_ReadPinOutput(AL_GPIO_HalStruct *Handle, AL_U32 Pin);
AL_S32 AlGpio_Hal_WritePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U32 Data);

/*
 * Bank intr APIs
*/
AL_S32 AlGpio_Hal_GetBankRawIntrStatus(AL_GPIO_HalStruct *Handle, AL_U32 Bank);
AL_S32 AlGpio_Hal_MaskBankIntr(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Value);
AL_S32 AlGpio_Hal_EnableBankSync(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Value);
AL_S32 AlGpio_Hal_GetBankSync(AL_GPIO_HalStruct *Handle, AL_U32 Bank);
/*
 * Pin intr APIs
*/
AL_S32 AlGpio_Hal_IntrPinCfg(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_GPIO_IntrEnum IntrType);

#ifdef __cplusplus
}
#endif

#endif
