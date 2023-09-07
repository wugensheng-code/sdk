/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



/***************************** Include Files *********************************/
#include "al_gpio_dev.h"


extern AL_GPIO_HwConfigStruct AlGpio_HwCfg[AL_GPIO_NUM_INSTANCE];

/************************** Function Prototypes ******************************/
/**
 * @brief  This function looks for the device configuration based on the unique device ID. The table AlGpio_HwCfg[] contains
 * the configuration information for each device in the system.
 * @param  DeviceId is the unique device ID of the device being looked up.
 * @return A pointer to the configuration table entry corresponding to the given device ID, or NULL if no match is found.
 */
AL_GPIO_HwConfigStruct *AlGpio_Dev_LookupConfig(AL_U32 DeviceId)
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
 * @brief  This function inits GPIO module.
 * @param  Gpio is pointer to AL_GPIO_DevStruct
 * @param  HwConfig
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_Init(AL_GPIO_DevStruct *Gpio, AL_GPIO_HwConfigStruct *HwConfig)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    Gpio->HwConfig.BaseAddress = HwConfig->BaseAddress;
    Gpio->HwConfig.BankMaxPins = HwConfig->BankMaxPins;
    Gpio->HwConfig.MaxBanks  = HwConfig->MaxBanks;
    Gpio->HwConfig.IntrId = HwConfig->IntrId;

    return AL_OK;
}

/**
 * @brief  This function gets the Bank number and the Pin number in the Bank, for the given PinNumber in the GPIO device.
 * @param  PinNumber
 * @param  BankNumber
 * @param  PinNumberInBank
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_GetBankPin(AL_U32 PinNumber, AL_U32 *BankNumber, AL_U32 *PinNumberInBank)
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


/* ① Bank APIs */
/**
 * @brief  This function reads Data register from the specified Bank in Data register.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_OutputRead(AL_GPIO_DevStruct *Gpio, AL_U32 Bank)
{
    return AlGpio_ll_OutputRead(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function reads Data register from the specified Bank in EXT register.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_InputRead(AL_GPIO_DevStruct *Gpio, AL_U32 Bank)
{
    return AlGpio_ll_InputRead(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  Write to the Data register of the specified GPIO bank.
 * @param  Gpio
 * @param  Bank
 * @param  Data
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_WriteBank(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Data)
{
    AlGpio_ll_ClrWrite(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_Write(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Data);
}

/**
 * @brief  Set the Direction of the pins of the specified Bank.
 * @param  Gpio
 * @param  Bank
 * @param  Direction
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_SetDirection(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Direction)
{
    AlGpio_ll_ClrDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_SetDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Direction);
}

/**
 * @brief  Get the Direction of the pins of the specified Bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_GetDirection(AL_GPIO_DevStruct *Gpio, AL_U32 Bank)
{
    return AlGpio_ll_GetDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


/* ② Pin APIs */
/**
 * @brief  This function reads Data register from the specified Pin in Data register.
 * @param  Gpio
 * @param  Pin
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_OutputReadPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    return (AlGpio_ll_OutputRead(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
}

/**
 * @brief  This function reads Data register from the specified Pin in EXT register.
 * @param  Gpio
 * @param  Pin
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_InputReadPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);

    return (AlGpio_ll_InputRead(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
}

/**
 * @brief  Write to the Data register of the specified GPIO Pin.
 * @param  Gpio
 * @param  Pin
 * @param  Data
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_WritePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Data)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 Value = 0;
    AL_U32 DataVar = Data;
    AL_U32 Val = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);

    DataVar &= (AL_U32)0x01;
    Value = DataVar << PinNumber;

    AlGpio_ll_ClrWrite(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Val);
    AlGpio_ll_Write(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value);
}

/**
 * @brief  Set the Direction of the specified pin.
 * @param  Gpio
 * @param  Pin
 * @param  Direction
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_SetDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 DirReg = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    DirReg = AlGpio_ll_GetDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(Direction != (AL_U32)0) { /*  Output Direction */
        DirReg |= BIT(PinNumber);
    } else { /*  Input Direction */
        DirReg &= ~BIT(PinNumber);
    }

    AlGpio_ll_ClrDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_SetDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DirReg);
}

/**
 * @brief  Get the Direction of the specified pin.
 * @param  Gpio
 * @param  Pin
 * @param  Direction
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_GetDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    return (AlGpio_ll_GetDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
}



/* ③ Bank APIs intr */
/**
 * @brief  This function enables the interrupts for the specified pins in the specified bank.
 * @param  Gpio
 * @param  Bank
 * @param  Raw
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_IntrEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Raw)
{
    AlGpio_ll_IntrClrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_IntrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Raw);
}

/**
 * @brief  This function clears the interrupts for the specified pins in the specified bank.
 * @param  Gpio
 * @param  Bank
 * @param  Mask
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_IntrClr(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Mask)
{
    AlGpio_ll_IntrEoi(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Mask);
}

/**
 * @brief  This function returns the interrupt enable status for a bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_IntrGetEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank)
{
    return AlGpio_ll_IntrGetEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function clears interrupt(s) with the provided mask.
 * @param  Gpio
 * @param  Bank
 * @param  Mask
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_IntrEnableMask(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Mask)
{
    AlGpio_ll_IntrClrEnableMask(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_IntrEnableMask(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Mask);
}

/**
 * @brief  This function returns the interrupt mask enable status for a bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_IntrGetEnableMask(AL_GPIO_DevStruct *Gpio, AL_U32 Bank)
{
    return AlGpio_ll_IntrGetEnableMask(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function returns interrupt status read from Interrupt Status Register.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_IntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U32 Bank)
{
    return AlGpio_ll_IntrGetStatus(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function returns the raw interrupt enable status for a bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_RawIntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U32 Bank)
{
    return AlGpio_ll_RawIntrGetStatus(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
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
AL_VOID AlGpio_Dev_IntrSetType(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 IntrType, AL_U32 IntrPolarity, AL_U32 IntrEdge)
{
    AlGpio_ll_IntrClrType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_IntrClrPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_IntrClrBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);

    AlGpio_ll_IntrSetType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrType);
    AlGpio_ll_IntrSetPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrPolarity);
    AlGpio_ll_IntrSetBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrEdge);
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
AL_VOID AlGpio_Dev_IntrGetType(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 *IntrType, AL_U32 *IntrPolarity, AL_U32 *IntrEdge)
{
    *IntrType = AlGpio_ll_IntrGetType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    *IntrPolarity = AlGpio_ll_IntrGetPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    *IntrEdge = AlGpio_ll_IntrGetBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function controls whether an external signal that is the source of an interrupt needs to be debounced to remove
 * any spurious glitches.
 * @param  Gpio
 * @param  Bank
 * @param  Debounce
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_DebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Debounce)
{
    AlGpio_ll_DebounceClrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_DebounceEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Debounce);
}

/**
 * @brief  This function gets whether an external signal that is the source of an interrupt needs to be debounced to remove
 * any spurious glitches.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_GetDebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank)
{
    return AlGpio_ll_GetDebounceEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function controls whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  Gpio
 * @param  Bank
 * @param  Debounce
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_SyncEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank, AL_U32 Debounce)
{
    AlGpio_ll_SyncEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Debounce);
}

/**
 * @brief  This function gets whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_GetSyncEnable(AL_GPIO_DevStruct *Gpio, AL_U32 Bank)
{
    return AlGpio_ll_GetSyncEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


/* ④ Pin APIs intr */
/**
 * @brief  This function enables the interrupt for the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_IntrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AlGpio_Dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    AlGpio_ll_IntrClrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_IntrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
}

/**
 * @brief  This function clears the interrupt for the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_IntrClrPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    AlGpio_ll_IntrEoi(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
}

/**
 * @brief  This function enables the interrpt mask for the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_IntrEnableMaskPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    AlGpio_ll_IntrEnableMask(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
}

/**
 * @brief  This function returns the interrpt mask for the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Dev_IntrGetEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrReg = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_IntrGetEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function returns the interrupt mask enable status of the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Dev_IntrGetEnableMaskPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrReg = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_IntrGetEnableMask(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function returns interrupt enable status of the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Dev_IntrGetStatusPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrReg = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_IntrGetStatus(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function returns the interrupt mask status of the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Dev_RawIntrGetStatusPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrReg = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_RawIntrGetStatus(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function is used for setting the IRQ Type of a specific pin.
 * @param  Gpio
 * @param  Pin
 * @param  IntrType
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_IntrSetTypePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 IntrType)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrTypeReg = 0;
    AL_U32 IntrPolarityReg = 0;
    AL_U32 IntrEdgeReg = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    IntrTypeReg = AlGpio_ll_IntrGetType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    IntrPolarityReg = AlGpio_ll_IntrGetPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    IntrEdgeReg = AlGpio_ll_IntrGetBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);

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

    AlGpio_ll_IntrClrType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_IntrClrPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_IntrClrBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));

    AlGpio_ll_IntrSetType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrTypeReg);
    AlGpio_ll_IntrSetPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrPolarityReg);
    AlGpio_ll_IntrSetBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrEdgeReg);
}

/**
 * @brief  This function is used for getting the IRQ Type of a specific pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_U8
 */
AL_U32 AlGpio_Dev_IntrGetTypePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrType = 0;
    AL_U32 IntrPolarity = 0;
    AL_U32 IntrEdge = 0;
    AL_U32 IntrMode = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    IntrType = AlGpio_ll_IntrGetType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & (BIT(PinNumber));
    IntrPolarity = AlGpio_ll_IntrGetPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & (BIT(PinNumber));
    if(IntrType == (BIT(PinNumber))) {
        IntrEdge = AlGpio_ll_IntrGetBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & (BIT(PinNumber));
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
AL_VOID AlGpio_Dev_DebounceEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 DebounceReg = 0;

    AlGpio_Dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DebounceReg = AlGpio_ll_GetDebounceEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(DebounceReg != (AL_U32)0) {
        DebounceReg |= (BIT(PinNumber));
    } else {
        DebounceReg &= ~(BIT(PinNumber));
    }
    AlGpio_ll_DebounceClrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_DebounceEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DebounceReg);
}

/**
 * @brief  This function returns whether an external signal that is the source of an interrupt needs to be debounced to remove
 * any spurious glitches.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Dev_GetDebounceEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 DebounceReg = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    DebounceReg = AlGpio_ll_GetDebounceEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((DebounceReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function controls whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  Gpio
 * @param  Pin
 * @return AL_VOID
 */
AL_VOID AlGpio_Dev_SyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 SyncReg = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    SyncReg = AlGpio_ll_GetSyncEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(SyncReg != (AL_U32)0) {
        SyncReg |= (BIT(PinNumber));
    } else {
        SyncReg &= ~(BIT(PinNumber));
    }

    AlGpio_ll_SyncEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, SyncReg);
}

/**
 * @brief  This function returns whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Dev_GetSyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 SyncReg = 0;

    AlGpio_Dev_GetBankPin(Pin, &Bank, &PinNumber);
    SyncReg = AlGpio_ll_GetSyncEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((SyncReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function sets the status callback function. The callback function is called by the
 *         AL_GPIO_IntrHandler when an interrupt occurs.
 * @param  Gpio is a pointer to the AL_GPIO instance.
 * @param  CallBackRef
 * @param  FunPointer is the pointer to the callback function.
 * @return AL_S32
 */
AL_S32 AlGpio_Dev_RegisterEventCallBack(AL_GPIO_DevStruct *Gpio, AL_GPIO_EventCallBack CallBack, AL_VOID *CallBackRef)
{
    if (Gpio == AL_NULL || CallBack == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    Gpio->EventCallBack        = CallBack;
    Gpio->EventCallBackRef     = CallBackRef;

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
AL_VOID AlGpio_Dev_IntrHandler(void *Instance)
{
    AL_U32 Bank = 0;
    AL_U32 IntrStatus = 0;
    AL_U32 IntrEnable = 0;
    AL_GPIO_DevStruct *Gpio = (AL_GPIO_DevStruct *)Instance;

    AL_LOG(AL_LOG_LEVEL_INFO, "Enter handler!\r\n");

    for(Bank = 0; Bank < Gpio->HwConfig.MaxBanks; Bank++) {
        IntrStatus = AlGpio_Dev_IntrGetStatus(Gpio, Bank);
        IntrEnable = AlGpio_Dev_IntrGetEnable(Gpio, Bank);
        printf("IntrStatus %d is %x\r\n", Bank, IntrStatus);
        printf("IntrEnable %d is %x\r\n", Bank, IntrEnable);

        if(((IntrStatus & IntrEnable) != (AL_U32)0) && (Gpio->EventCallBack)) {
            if(Gpio->EventCallBack) {
                AL_GPIO_EventStruct GpioEvent = {
                    .Events  = AL_GPIO_Event,
                };
                Gpio->EventCallBack(GpioEvent, Gpio->EventCallBackRef);
            }
            AlGpio_Dev_IntrClr(Gpio, Bank, IntrStatus & IntrEnable);
            AlGpio_Dev_IntrEnableMask(Gpio, Bank, IntrEnable & IntrEnable);
        }
    }
}
