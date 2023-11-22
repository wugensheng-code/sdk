/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



/***************************** Include Files *********************************/
#include "al_gpio_hal.h"

/************************** Variable Definitions *****************************/
static AL_GPIO_HalStruct AL_GPIO_HalInstance[AL_GPIO_NUM_INSTANCE];
extern AL_GPIO_HwConfigStruct AlGpio_HwCfg[AL_GPIO_NUM_INSTANCE];

/************************** Function Prototypes ******************************/


/**
 * @brief  This function actions when interrupt done.
 * @param  GpioEvent
 * @param  CallbackRef
 * @return AL_VOID
 */
static AL_VOID AlGpio_Hal_DefEventCallBack(AL_GPIO_EventStruct GpioEvent, AL_VOID *CallbackRef)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Bank 0x%x: IntrStatus is 0x%x", GpioEvent.Bank, GpioEvent.EventData);
}

/**
 * @brief  This function looks for the device configuration based on the unique device ID.
 * @param  DeviceId is the unique device ID of the device being looked up.
 * @return A pointer to the configuration table entry corresponding to the given device ID, or NULL if no match is found.
 */
static AL_GPIO_HwConfigStruct *AlGpio_Hal_LookupConfig(AL_U32 DeviceId)
{
    AL_U32 Index = 0;
    AL_GPIO_HwConfigStruct *CfgPtr = AL_NULL;

    for (Index = 0; Index < AL_GPIO_NUM_INSTANCE; Index++) {
        if (AlGpio_HwCfg[Index].DeviceId == DeviceId) {
            CfgPtr = &AlGpio_HwCfg[Index];
            break;
        }
    }

    return CfgPtr;
}

/**
 * @brief  This function sets the status callback function. The callback function is called by the
 *         AL_GPIO_IntrHandler when an interrupt occurs.
 * @param  Gpio is a pointer to the AL_GPIO instance.
 * @param  CallBackRef
 * @param  FunPointer is the pointer to the callback function.
 * @return AL_S32
 */
static AL_S32 AlGpio_Hal_RegisterEventCallBack(AL_GPIO_HalStruct *Handle, AL_GPIO_EventCallBack CallBack, AL_VOID *CallBackRef)
{
    AL_ASSERT((Handle != AL_NULL) && (CallBack != AL_NULL), AL_GPIO_ERR_ILLEGAL_PARAM);

    Handle->EventCallBack        = CallBack;
    Handle->EventCallBackRef     = CallBackRef;

    return AL_OK;
}

/**
 * @brief  This function inits GPIO module.
 * @param  Gpio is pointer to AL_GPIO_HalStruct
 * @param  HwConfig
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_Init(AL_GPIO_HalStruct **Handle, AL_U32 DevId, AL_GPIO_EventCallBack CallBack)
{
    AL_S32 Ret = AL_OK;
    AL_GPIO_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    HwConfig = AlGpio_Hal_LookupConfig(DevId);
    if(HwConfig != AL_NULL){
        (*Handle) = &AL_GPIO_HalInstance[DevId];
    } else {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    (*Handle)->HwConfig.BaseAddress = HwConfig->BaseAddress;
    (*Handle)->HwConfig.BankMaxPins = HwConfig->BankMaxPins;
    (*Handle)->HwConfig.MaxBanks  = HwConfig->MaxBanks;
    (*Handle)->HwConfig.IntrId = HwConfig->IntrId;

    /* register intr */
    if(CallBack) {
        Ret = AlGpio_Hal_RegisterEventCallBack(*Handle, CallBack, (void *)(*Handle));
    } else {
        Ret = AlGpio_Hal_RegisterEventCallBack(*Handle, AlGpio_Hal_DefEventCallBack, (void *)(*Handle));
    }
    if(Ret != AL_OK) {
        return Ret;
    }

    return AL_OK;
}

/**
 * @brief  This function gets the Bank number and the Pin number in the Bank according to the given PinNumber in the GPIO device.
 * @param  PinNumber
 * @param  BankNumber
 * @param  PinNumberInBank
 * @return AL_VOID
 */
static AL_VOID AlGpio_Hal_GetBankPin(AL_U32 PinNumber, AL_U32 *BankNumber, AL_U32 *PinNumberInBank)
{
    if(PinNumber >= 0 && PinNumber <= MAX_PIN_NUMBER_IN_BANK_0) {
        *BankNumber = AL_GPIO_BANK0;
        *PinNumberInBank = PinNumber;
    } else if(PinNumber > MAX_PIN_NUMBER_IN_BANK_0 && PinNumber <= MAX_PIN_NUMBER_IN_BANK_1) {
        *BankNumber = AL_GPIO_BANK1;
        *PinNumberInBank = PinNumber - MAX_PIN_NUMBER_IN_BANK_0 -1;
    } else if(PinNumber > MAX_PIN_NUMBER_IN_BANK_1 && PinNumber <= MAX_PIN_NUMBER_IN_BANK_2) {
        *BankNumber = AL_GPIO_BANK2;
        * PinNumberInBank = PinNumber - MAX_PIN_NUMBER_IN_BANK_1 -1;
    } else if(PinNumber > MAX_PIN_NUMBER_IN_BANK_2 && PinNumber <= MAX_PIN_NUMBER_IN_BANK_3) {
        *BankNumber = AL_GPIO_BANK3;
        *PinNumberInBank = PinNumber - MAX_PIN_NUMBER_IN_BANK_2 -1;
    }
}

/**
 * @brief  Set the Direction of the pins of the specified Bank.
 * @param  Gpio
 * @param  Bank
 * @param  Direction
 * @return AL_VOID
 */
static AL_S32 AlGpio_Hal_SetBankDirection(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Direction)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_ClrDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_SetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Direction);

    return AL_OK;
}


/**
 * @brief  This function reads Data register from the specified Bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_ReadBankOutput(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_ReadOutput(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


/**
 * @brief  This function reads Data register from the specified Bank in EXT register.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_ReadBankInput(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_SetBankDirection(Handle, Bank, GPIO_BANK_INPUT);
    return AlGpio_ll_ReadInput(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


/**
 * @brief  Write to the Data register of the specified GPIO bank.
 * @param  Gpio
 * @param  Bank
 * @param  Data
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_WriteBank(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Data)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_SetBankDirection(Handle, Bank, GPIO_BANK_OUTPUT);

    AlGpio_ll_ClrWrite(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_Write(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Data);

    return AL_OK;
}

/**
 * @brief  Get the Direction of the pins of the specified Bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
static AL_S32 AlGpio_Hal_GetBankDirection(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}



/* ② Pin APIs */
/**
 * @brief  Set the Direction of the specified pin.
 * @param  Gpio
 * @param  Pin
 * @param  Direction
 * @return AL_VOID
 */
static AL_S32 AlGpio_Hal_SetPinDirection(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U32 Direction)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 DirReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    DirReg = AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(Direction != (AL_U32)0) { /*  Output Direction */
        DirReg |= BIT(PinNumber);
    } else { /*  Input Direction */
        DirReg &= ~BIT(PinNumber);
    }

    AlGpio_ll_ClrDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_SetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DirReg);

    return AL_OK;
}

/**
 * @brief  Get the Direction of the specified pin.
 * @param  Gpio
 * @param  Pin
 * @param  Direction
 * @return AL_S32
 */
static AL_S32 AlGpio_Hal_GetPinDirection(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    return (AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
}

AL_S32 AlGpio_Hal_ReadPinOutput(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);

    return (AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
}


/**
 * @brief  This function reads Data register from the specified Pin in EXT register.
 * @param  Gpio
 * @param  Pin
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_ReadPinInput(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 DirReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    DirReg = AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    DirReg &= ~BIT(PinNumber);

    AlGpio_ll_ClrDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_SetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DirReg); /* Input Direction */

    return (AlGpio_ll_ReadInput(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
}


/**
 * @brief  Write to the Data register of the specified GPIO Pin.
 * @param  Gpio
 * @param  Pin
 * @param  Data
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_WritePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U32 Data)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 Value = 0;
    AL_U32 DataVar = Data;
    AL_U32 DirReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    DirReg = AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    DirReg |= BIT(PinNumber); /* Output Direction */
    DataVar &= (AL_U32)0x01;
    Value = AlGpio_ll_ReadOutput(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if (DataVar != 0)
    {
        Value |= (DataVar << PinNumber);
    } else if (DataVar == 0)
    {
        Value &= ~(~DataVar << PinNumber);
    }

    AlGpio_ll_ClrDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_SetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DirReg);

    AlGpio_ll_ClrWrite(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_Write(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value);

    return AL_OK;
}



/* ③ Bank APIs intr */
/**
 * @brief  This function enables the interrupts for the specified pins in the specified bank.
 * @param  Gpio
 * @param  Bank
 * @param  Raw
 * @return AL_VOID
 */
static AL_S32 AlGpio_Hal_EnableBankIntr(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Raw)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_DisableIntr(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_EnableIntr(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Raw);

    return AL_OK;
}

/**
 * @brief  This function clears the interrupts for the specified pins in the specified bank.
 * @param  Gpio
 * @param  Bank
 * @param  Mask
 * @return AL_VOID
 */
static AL_S32 AlGpio_Hal_ClrBankIntr(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Mask)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_ClrIntr(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Mask);

    return AL_OK;
}

/**
 * @brief  This function returns the interrupt enable status for a bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
static AL_S32 AlGpio_Hal_GetBankIntrEnable(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetIntrEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function returns the interrupt mask enable status for a bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
static AL_S32 AlGpio_Hal_GetBankIntrMask(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetIntrMask(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function clears interrupt(s) with the provided mask.
 * @param  Gpio
 * @param  Bank
 * @param  Mask
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_EnableBankIntrMask(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Value)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);
    AL_U32 IntrMaskVal = 0;
    IntrMaskVal = AlGpio_Hal_GetBankIntrMask(Handle, Bank);

    AlGpio_ll_DisableIntrMask(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value|IntrMaskVal);
    AlGpio_ll_EnableIntrMask(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value|IntrMaskVal);

    return AL_OK;
}

/**
 * @brief  This function returns interrupt status read from Interrupt Status Register.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
static AL_S32 AlGpio_Hal_GetBankIntrStatus(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetIntrStatus(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function returns the raw interrupt enable status for a bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_GetBankRawIntrStatus(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetRawIntrStatus(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function is used for setting the Interrupt Type, Interrupt Polarity and
 * Interrupt Edge for the specified Bank.
 * @param  Gpio
 * @param  Bank
 * @param  IntrType
 * @param  IntrPolarity
 * @param  IntrEdge
 * @return AL_VOID
 */
static AL_S32 AlGpio_Hal_SetIntrBankType(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_GPIO_IntrTypeEnum IntrType, AL_GPIO_IntrPolarityEnum IntrPolarity, AL_GPIO_IntrBothEdgeEnum IntrEdge)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_ClrIntrType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_ClrIntrPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_ClrIntrBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);

    AlGpio_ll_SetIntrType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrType);
    AlGpio_ll_SetIntrPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrPolarity);
    AlGpio_ll_SetIntrBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrEdge);

    return AL_OK;
}

/**
 * @brief  This function is used for getting the Interrupt Type, Interrupt Polarity and
 * Interrupt Edge for the specified Bank.
 * @param  Gpio
 * @param  Bank
 * @param  IntrType
 * @param  IntrPolarity
 * @param  IntrEdge
 * @return AL_VOID
 */
static AL_S32 AlGpio_Hal_GetIntrBankType(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 *IntrType, AL_U32 *IntrPolarity, AL_U32 *IntrEdge)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    *IntrType = AlGpio_ll_GetIntrType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    *IntrPolarity = AlGpio_ll_GetIntrPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    *IntrEdge = AlGpio_ll_GetIntrBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);

    return AL_OK;
}

/**
 * @brief  This function controls whether an external signal that is the source of an interrupt needs to be debounced to remove
 * any spurious glitches.
 * @param  Gpio
 * @param  Bank
 * @param  Debounce
 * @return AL_VOID
 */
static AL_S32 AlGpio_Hal_EnableBankDebounce(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Debounce)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_DisableDebounce(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_EnableDebounce(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Debounce);

    return AL_OK;
}

/**
 * @brief  This function controls whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  Gpio
 * @param  Bank
 * @param  Debounce
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_EnableBankSync(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Value)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_EnableSync(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value);

    return AL_OK;
}

/**
 * @brief  This function gets whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_GetBankSync(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetSync(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/* ④ Pin APIs intr */
/**
 * @brief  This function enables the interrupt for the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_VOID
 */
static AL_S32 AlGpio_Hal_EnablePinIntr(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    AlGpio_ll_DisableIntr(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_EnableIntr(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));

    return AL_OK;
}

/**
 * @brief  This function returns the interrpt mask for the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
static AL_BOOL AlGpio_Hal_GetPinIntrEnable(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_GetIntrEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function is used for setting the IRQ Type of a specific pin.
 * @param  Gpio
 * @param  Pin
 * @param  IntrType
 * @return AL_VOID
 */
static AL_S32 AlGpio_Hal_SetPinIntrType(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_GPIO_IntrEnum IntrType)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrTypeReg = 0;
    AL_U32 IntrPolarityReg = 0;
    AL_U32 IntrEdgeReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    IntrTypeReg = AlGpio_ll_GetIntrType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    IntrPolarityReg = AlGpio_ll_GetIntrPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    IntrEdgeReg = AlGpio_ll_GetIntrBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);

    switch(IntrType) {
    case GPIO_INTR_TYPE_EDGE_RISING:
            IntrTypeReg |= BIT(PinNumber);
            IntrPolarityReg |= BIT(PinNumber);
            IntrEdgeReg &= ~BIT(PinNumber);
            break;
    case GPIO_INTR_TYPE_EDGE_FALLING:
            IntrTypeReg |= BIT(PinNumber);
            IntrPolarityReg &= ~BIT(PinNumber);
            IntrEdgeReg &= ~BIT(PinNumber);
            break;
    case GPIO_INTR_TYPE_EDGE_BOTH:
            IntrTypeReg |= BIT(PinNumber);
            IntrEdgeReg |= BIT(PinNumber);
            break;
    case GPIO_INTR_TYPE_LEVEL_HIGH:
            IntrTypeReg &= ~BIT(PinNumber);
            IntrPolarityReg |= BIT(PinNumber);
            break;
    case GPIO_INTR_TYPE_LEVEL_LOW:
            IntrTypeReg &= ~BIT(PinNumber);
            IntrPolarityReg &= ~BIT(PinNumber);
            break;
    default:
            break;
    }

    AlGpio_ll_ClrIntrType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_ClrIntrPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_ClrIntrBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));

    AlGpio_ll_SetIntrType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrTypeReg);
    AlGpio_ll_SetIntrPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrPolarityReg);
    AlGpio_ll_SetIntrBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrEdgeReg);

    return AL_OK;
}

/**
 * @brief  This function is used for getting the IRQ Type of a specific pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_U8
 */
static AL_S32 AlGpio_Hal_GetPinIntrType(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrType = 0;
    AL_U32 IntrPolarity = 0;
    AL_U32 IntrEdge = 0;
    AL_U32 IntrMode = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    IntrType = AlGpio_ll_GetIntrType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & (BIT(PinNumber));
    IntrPolarity = AlGpio_ll_GetIntrPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & (BIT(PinNumber));
    if(IntrType == (BIT(PinNumber))) {
        IntrEdge = AlGpio_ll_GetIntrBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & (BIT(PinNumber));
        if(IntrEdge == BIT(PinNumber)) {
            IntrMode = GPIO_INTR_TYPE_EDGE_BOTH;
        } else if(IntrPolarity == BIT(PinNumber)) {
            IntrMode = GPIO_INTR_TYPE_EDGE_RISING;
        } else {
            IntrMode = GPIO_INTR_TYPE_EDGE_FALLING;
        }
    } else {
        if(IntrPolarity == BIT(PinNumber)) {
            IntrMode = GPIO_INTR_TYPE_LEVEL_HIGH;
        } else {
            IntrMode = GPIO_INTR_TYPE_LEVEL_LOW;
        }
    }

    return IntrMode;
}

/**
 * @brief  This function controls whether an external signal that is the source of an interrupt needs to be debounced to remove
 * any spurious glitches.
 * @param  Gpio
 * @param  Pin
 * @return AL_VOID
 */
static AL_S32 AlGpio_Hal_EnablePinDebounce(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 DebounceReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DebounceReg = AlGpio_ll_GetDebounce(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(DebounceReg != (AL_U32)0) {
        DebounceReg |= (BIT(PinNumber));
    } else {
        DebounceReg &= ~(BIT(PinNumber));
    }
    AlGpio_ll_DisableDebounce(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_EnableDebounce(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DebounceReg);

    return AL_OK;
}

/**
 * @brief  This function is the interrupt handler for GPIO interrupts.It checks the interrupt status registers
 * of all the banks to determine the actual bank in which interrupt have been triggered. It then calls the
 * upper layer callback handler set by the function AlGpio_Hal_IntrCallbackHandler.
 * @param  Gpio is a pointer to the AL_GPIO instance.
 * @param  Pin is the pin number to which the Data is to be written.
 * @param  IntrType is the IRQ type for GPIO Pin.
 * @return AL_S32
 */
static AL_VOID AlGpio_Hal_IntrHandler(AL_VOID *Instance)
{
    AL_GPIO_HalStruct *Handle = (AL_GPIO_HalStruct *)Instance;
    AL_U32 Bank = 0, IntrStatus = 0;

    for(Bank = 0; Bank < GPIO_MAX_BANKS; Bank ++) {
        IntrStatus = AlGpio_Hal_GetBankIntrStatus(Handle, Bank);
        if (Handle->IntrBank & BIT(Bank)) {
            if(Handle->EventCallBack) {
                AL_GPIO_EventStruct GpioEvent = {
                    .Bank      = Bank,
                    .EventData = IntrStatus
                };
                Handle->EventCallBack(GpioEvent, Handle->EventCallBackRef);

                AlGpio_Hal_ClrBankIntr(Handle, Bank, IntrStatus);
                /* In edge interrupt mode, GPIO__EOI Register need to be set 0 for the next interrupt. */
                AlGpio_Hal_ClrBankIntr(Handle, Bank, AL_GPIO_DISABLE);
            #if 0
                AL_U32 RawVal = 0;
                /* Mask Interrupt. */
                AlGpio_Hal_EnableBankIntrMask(Handle, Bank, IntrStatus);
                RawVal = AlGpio_Hal_GetBankRawIntrStatus(Handle, GpioEvent.Bank);
                AL_LOG(AL_LOG_LEVEL_INFO, "GPIO Bank 0x%x IntrRaw value is 0x%x", Bank, RawVal);
            #endif
            }
        }
    }
}

/**
 * @brief  This function configure the interrupt, including direction, type and enable register.
 * @param  Handle
 * @param  Pin
 * @param  IntrType
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_IntrPinCfg(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_GPIO_IntrEnum IntrType)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AL_GPIO_Intr_BankEnum Bank = 0;
    AL_U32 GetPinDirection = 0;
    AL_U32 GetPinIntrEnable = 0;
    AL_GPIO_IntrEnum GetIntrMode = 0;

    if (IntrType == GPIO_INTR_TYPE_EDGE_FALLING || IntrType == GPIO_INTR_TYPE_EDGE_RISING || IntrType == GPIO_INTR_TYPE_EDGE_BOTH) {
        AlGpio_Hal_EnablePinDebounce(Handle, Pin);
    }

    if (Pin >= 0 && Pin <= MAX_PIN_NUMBER_IN_BANK_0) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
        Bank = AL_GPIO_INTR_BANK0;
    } else if(Pin > MAX_PIN_NUMBER_IN_BANK_0 && Pin <= MAX_PIN_NUMBER_IN_BANK_1) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId + 1, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
        Bank = AL_GPIO_INTR_BANK1;
    } else if(Pin > MAX_PIN_NUMBER_IN_BANK_1 && Pin <= MAX_PIN_NUMBER_IN_BANK_2) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId + 2, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
        Bank = AL_GPIO_INTR_BANK2;
    } else if (Pin > MAX_PIN_NUMBER_IN_BANK_2 && Pin <= MAX_PIN_NUMBER_IN_BANK_3) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId + 3, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
        Bank = AL_GPIO_INTR_BANK3;
    }

    Handle->IntrBank |= Bank;

    AlGpio_Hal_SetPinDirection(Handle, Pin, GPIO_PIN_INPUT);
    AlGpio_Hal_SetPinIntrType(Handle, Pin, IntrType);
    AlGpio_Hal_EnablePinIntr(Handle, Pin);

    AlGpio_Hal_GetPinDirection(Handle, Pin);
    AlGpio_Hal_GetPinIntrEnable(Handle, Pin);
    AlGpio_Hal_GetPinIntrType(Handle, Pin);

    GetPinDirection = AlGpio_Hal_GetPinDirection(Handle, Pin);
    GetPinIntrEnable = AlGpio_Hal_GetPinIntrEnable(Handle, Pin);
    GetIntrMode = AlGpio_Hal_GetPinIntrType(Handle, Pin);
    if(GetIntrMode != IntrType) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Interrupt mode was set incorrectly, Pin IntrMode got is 0x%x.", GetIntrMode);
        return AL_FALSE;
    } else if (GetPinDirection != 0) {
        AL_LOG(AL_LOG_LEVEL_INFO, "GPIO Pin direction was set incorrectly, Pin Direction got is 0x%x.", GetPinDirection);
        return AL_FALSE;
    } else if(GetPinIntrEnable == 0) {
        AL_LOG(AL_LOG_LEVEL_INFO, "GPIO Pin interrupt was disabled, Pin Status got is 0x%x.", GetPinIntrEnable);
        return AL_FALSE;
    }

    return AL_OK;
}

AL_S32 AlGpio_Hal_IntrBankCfg(AL_GPIO_HalStruct *Handle, AL_U32 IntrBank, AL_U32 IntrVal, AL_GPIO_IntrTypeEnum IntrType, AL_GPIO_IntrPolarityEnum IntrPolarity, AL_GPIO_IntrBothEdgeEnum IntrEdge)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);
    AL_GPIO_Intr_BankEnum Bank = 0;
    AL_GPIO_IntrTypeEnum Type = 0;
    AL_GPIO_IntrPolarityEnum Polarity = 0;
    AL_GPIO_IntrBothEdgeEnum BothEdge = 0;
    AL_U32 GetBankDirection = 0;
    AL_U32 GetBankIntrEnable = 0;

    if (IntrType) {
        AlGpio_Hal_EnableBankDebounce(Handle, Bank, IntrVal);
    }
    if (IntrBank == AL_GPIO_BANK0) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
        Bank = AL_GPIO_INTR_BANK0;
    } else if(IntrBank == AL_GPIO_BANK1) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId + 1, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
        Bank = AL_GPIO_INTR_BANK1;
    } else if(IntrBank == AL_GPIO_BANK2) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId + 2, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
        Bank = AL_GPIO_INTR_BANK2;
    } else if (IntrBank == AL_GPIO_BANK3) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId + 3, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
        Bank = AL_GPIO_INTR_BANK3;
    }

    Handle->IntrBank |= Bank;

    AlGpio_Hal_SetBankDirection(Handle, IntrBank, GPIO_PIN_INPUT);
    AlGpio_Hal_SetIntrBankType(Handle, IntrBank, IntrType, IntrPolarity, IntrEdge);
    AlGpio_Hal_EnableBankIntr(Handle, IntrBank, IntrVal);

    GetBankDirection = AlGpio_Hal_GetBankDirection(Handle, IntrBank);
    GetBankIntrEnable = AlGpio_Hal_GetBankIntrEnable(Handle, IntrBank);
    AlGpio_Hal_GetIntrBankType(Handle, IntrBank, &Type, &Polarity, &BothEdge);
    if(Type != IntrType || Polarity != IntrPolarity || BothEdge != IntrEdge) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Interrupt mode was set incorrectly.");
        return AL_FALSE;
    } else if (GetBankDirection) {
        AL_LOG(AL_LOG_LEVEL_INFO, "GPIO Bank direction was set incorrectly.");
        return AL_FALSE;
    } else if(GetBankIntrEnable != IntrVal) {
        AL_LOG(AL_LOG_LEVEL_INFO, "GPIO Bank was disabled.");
        return AL_FALSE;
    }

    return AL_OK;
}
