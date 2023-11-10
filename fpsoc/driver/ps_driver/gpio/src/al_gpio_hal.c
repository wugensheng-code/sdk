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

}

/**
 * @brief  This function looks for the device configuration based on the unique device ID.
 * @param  DeviceId is the unique device ID of the device being looked up.
 * @return A pointer to the configuration table entry corresponding to the given device ID, or NULL if no match is found.
 */
AL_GPIO_HwConfigStruct *AlGpio_Hal_LookupConfig(AL_U32 DeviceId)
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

    // (AL_VOID)AlIntr_RegHandler((*Handle)->HwConfig.IntrId, AL_NULL, AlGpio_Hal_IntrHandler, *Handle);

    return AL_OK;
}

/**
 * @brief  This function gets the Bank number and the Pin number in the Bank according to the given PinNumber in the GPIO device.
 * @param  PinNumber
 * @param  BankNumber
 * @param  PinNumberInBank
 * @return AL_VOID
 */
AL_VOID AlGpio_Hal_GetBankPin(AL_U32 PinNumber, AL_U32 *BankNumber, AL_U32 *PinNumberInBank)
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
AL_S32 AlGpio_Hal_SetDirection(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Direction)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_ClrDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_SetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Direction);

    return AL_OK;
}


/**
 * @brief  This function reads Data register from the specified Bank in EXT register.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_ReadBank(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_SetDirection(Handle, Bank, GPIO_BANK_INPUT);
    return AlGpio_ll_InputRead(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
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

    AlGpio_Hal_SetDirection(Handle, Bank, GPIO_BANK_OUTPUT);

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
AL_S32 AlGpio_Hal_GetDirection(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
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
AL_S32 AlGpio_Hal_SetDirectionPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U32 Direction)
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
AL_S32 AlGpio_Hal_GetDirectionPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    return (AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
}



AL_S32 AlGpio_Hal_ReadDRPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);

    return (AlGpio_ll_OutputRead(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
}


/**
 * @brief  This function reads Data register from the specified Pin in EXT register.
 * @param  Gpio
 * @param  Pin
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_ReadPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
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

    return (AlGpio_ll_InputRead(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
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
    Value = AlGpio_ll_OutputRead(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
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
AL_S32 AlGpio_Hal_IntrEnable(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Raw)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_IntrClrEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_IntrEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Raw);

    return AL_OK;
}

/**
 * @brief  This function clears the interrupts for the specified pins in the specified bank.
 * @param  Gpio
 * @param  Bank
 * @param  Mask
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_IntrClr(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Mask)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_IntrEoi(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Mask);

    return AL_OK;
}

/**
 * @brief  This function returns the interrupt enable status for a bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_IntrGetEnable(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_IntrGetEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function clears interrupt(s) with the provided mask.
 * @param  Gpio
 * @param  Bank
 * @param  Mask
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_IntrEnableMask(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Mask)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_IntrClrEnableMask(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_IntrEnableMask(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Mask);

    return AL_OK;
}

/**
 * @brief  This function returns the interrupt mask enable status for a bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_IntrGetEnableMask(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_IntrGetEnableMask(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function returns interrupt status read from Interrupt Status Register.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_IntrGetStatus(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_IntrGetStatus(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function returns the raw interrupt enable status for a bank.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_RawIntrGetStatus(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_RawIntrGetStatus(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
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
AL_S32 AlGpio_Hal_IntrSetType(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_GPIO_IntrTypeEnum IntrType, AL_GPIO_IntrPolarityEnum IntrPolarity, AL_GPIO_IntrBothEdgeEnum IntrEdge)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_IntrClrType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_IntrClrPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_IntrClrBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);

    AlGpio_ll_IntrSetType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrType);
    AlGpio_ll_IntrSetPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrPolarity);
    AlGpio_ll_IntrSetBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrEdge);

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
AL_S32 AlGpio_Hal_IntrGetType(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 *IntrType, AL_U32 *IntrPolarity, AL_U32 *IntrEdge)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    *IntrType = AlGpio_ll_IntrGetType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    *IntrPolarity = AlGpio_ll_IntrGetPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    *IntrEdge = AlGpio_ll_IntrGetBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);

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
AL_S32 AlGpio_Hal_DebounceEnable(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Debounce)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_DebounceClrEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_ALL_ENABLE);
    AlGpio_ll_DebounceEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Debounce);

    return AL_OK;
}

/**
 * @brief  This function gets whether an external signal that is the source of an interrupt needs to be debounced to remove
 * any spurious glitches.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_GetDebounceEnable(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetDebounceEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 * @brief  This function controls whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  Gpio
 * @param  Bank
 * @param  Debounce
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_SyncEnable(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Debounce)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_SyncEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Debounce);

    return AL_OK;
}

/**
 * @brief  This function gets whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  Gpio
 * @param  Bank
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_GetSyncEnable(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetSyncEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


/* ④ Pin APIs intr */
/**
 * @brief  This function enables the interrupt for the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_IntrEnablePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    AlGpio_ll_IntrClrEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_IntrEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));

    return AL_OK;
}

/**
 * @brief  This function clears the interrupt for the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_IntrClrPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    AlGpio_ll_IntrEoi(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));

    return AL_OK;
}

/**
 * @brief  This function disable clear the interrupt for the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_IntrDisableClrPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    AlGpio_ll_IntrEoi(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_GPIO_DISABLE);

    return AL_OK;
}

/**
 * @brief  This function enables the interrpt mask for the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_IntrEnableMaskPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    AlGpio_ll_IntrEnableMask(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));

    return AL_OK;
}

/**
 * @brief  This function returns the interrpt mask for the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Hal_IntrGetEnablePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_IntrGetEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function returns the interrupt mask enable status of the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Hal_IntrGetEnableMaskPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_IntrGetEnableMask(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function returns interrupt enable status of the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Hal_IntrGetStatusPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_IntrGetStatus(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function returns the interrupt mask status of the specified pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Hal_RawIntrGetStatusPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_RawIntrGetStatus(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function is used for setting the IRQ Type of a specific pin.
 * @param  Gpio
 * @param  Pin
 * @param  IntrType
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_IntrSetTypePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_GPIO_IntrEnum IntrType)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrTypeReg = 0;
    AL_U32 IntrPolarityReg = 0;
    AL_U32 IntrEdgeReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    IntrTypeReg = AlGpio_ll_IntrGetType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    IntrPolarityReg = AlGpio_ll_IntrGetPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    IntrEdgeReg = AlGpio_ll_IntrGetBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);

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

    AlGpio_ll_IntrClrType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_IntrClrPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_IntrClrBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));

    AlGpio_ll_IntrSetType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrTypeReg);
    AlGpio_ll_IntrSetPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrPolarityReg);
    AlGpio_ll_IntrSetBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrEdgeReg);

    return AL_OK;
}

/**
 * @brief  This function is used for getting the IRQ Type of a specific pin.
 * @param  Gpio
 * @param  Pin
 * @return AL_U8
 */
AL_S32 AlGpio_Hal_IntrGetTypePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrType = 0;
    AL_U32 IntrPolarity = 0;
    AL_U32 IntrEdge = 0;
    AL_U32 IntrMode = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    IntrType = AlGpio_ll_IntrGetType(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & (BIT(PinNumber));
    IntrPolarity = AlGpio_ll_IntrGetPolarity(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & (BIT(PinNumber));
    if(IntrType == (BIT(PinNumber))) {
        IntrEdge = AlGpio_ll_IntrGetBothEdge(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & (BIT(PinNumber));
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
AL_S32 AlGpio_Hal_DebounceEnablePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 DebounceReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DebounceReg = AlGpio_ll_GetDebounceEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(DebounceReg != (AL_U32)0) {
        DebounceReg |= (BIT(PinNumber));
    } else {
        DebounceReg &= ~(BIT(PinNumber));
    }
    AlGpio_ll_DebounceClrEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    AlGpio_ll_DebounceEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DebounceReg);

    return AL_OK;
}

/**
 * @brief  This function returns whether an external signal that is the source of an interrupt needs to be debounced to remove
 * any spurious glitches.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Hal_GetDebounceEnablePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 DebounceReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    DebounceReg = AlGpio_ll_GetDebounceEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((DebounceReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

/**
 * @brief  This function controls whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  Gpio
 * @param  Pin
 * @return AL_VOID
 */
AL_S32 AlGpio_Hal_SyncEnablePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 SyncReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    SyncReg = AlGpio_ll_GetSyncEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(SyncReg != (AL_U32)0) {
        SyncReg |= (BIT(PinNumber));
    } else {
        SyncReg &= ~(BIT(PinNumber));
    }

    AlGpio_ll_SyncEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, SyncReg);

    return AL_OK;
}

/**
 * @brief  This function returns whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  Gpio
 * @param  Pin
 * @return AL_BOOL
 */
AL_BOOL AlGpio_Hal_GetSyncEnablePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 SyncReg = 0;

    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    SyncReg = AlGpio_ll_GetSyncEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
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
AL_S32 AlGpio_Hal_RegisterEventCallBack(AL_GPIO_HalStruct *Handle, AL_GPIO_EventCallBack CallBack, AL_VOID *CallBackRef)
{
    AL_ASSERT((Handle != AL_NULL) && (CallBack != AL_NULL), AL_GPIO_ERR_ILLEGAL_PARAM);

    Handle->EventCallBack        = CallBack;
    Handle->EventCallBackRef     = CallBackRef;

    return AL_OK;
}


/**
 * @brief  This function is the interrupt handler for GPIO Edge interrupts.
 * @param  Gpio is a pointer to the AL_GPIO instance.
 * @param  Pin is the pin number to which the Data is to be written.
 * @param  IntrType is the IRQ type for GPIO Pin.
 * @return AL_S32
 */
AL_VOID AlGpio_Hal_EdgeIntrHandler(AL_VOID *Instance)
{
    AL_U32 IntrStatus = 0;
    AL_GPIO_HalStruct *Handle = (AL_GPIO_HalStruct *)Instance;
    AL_U32 Pin = Handle->IntrPin;

    IntrStatus = AlGpio_Hal_IntrGetStatusPin(Handle, Pin);
    AL_LOG(AL_LOG_LEVEL_INFO, "Pin %d IntrStatus is %x", Pin, IntrStatus);
    if(Handle->EventCallBack) {
        AL_GPIO_EventStruct GpioEvent = {
            .Events  = AL_GPIO_Level_Event,
        };
        Handle->EventCallBack(GpioEvent, Handle->EventCallBackRef);

        AlGpio_Hal_IntrClrPin(Handle, Pin);
        /* In edge interrupt mode, GPIO__EOI Register need to be set 0 for the next interrupt.*/
        AlGpio_Hal_IntrDisableClrPin(Handle, Pin);
    }
}

/**
 * @brief  This function is the interrupt handler for GPIO Level interrupts.
 * @param  Gpio is a pointer to the AL_GPIO instance.
 * @param  Pin is the pin number to which the Data is to be written.
 * @param  IntrType is the IRQ type for GPIO Pin.
 * @return AL_S32
 */
AL_VOID AlGpio_Hal_LevelIntrHandler(AL_VOID *Instance)
{
    AL_U32 IntrStatus = 0;
    AL_GPIO_HalStruct *Handle = (AL_GPIO_HalStruct *)Instance;
    AL_U32 Pin = Handle->IntrPin;
    AL_U32 MaskPinVal = 0;

    IntrStatus = AlGpio_Hal_IntrGetStatusPin(Handle, Pin);
    AL_LOG(AL_LOG_LEVEL_INFO, "IntrStatus is %x", IntrStatus);
    if(Handle->EventCallBack) {
        AL_GPIO_EventStruct GpioEvent = {
            .Events  = AL_GPIO_Level_Event,
        };
        Handle->EventCallBack(GpioEvent, Handle->EventCallBackRef);

        AlGpio_Hal_IntrClrPin(Handle, Pin);
        /* In level interrupt mode, GPIO__INTMASK__CLR Register need to be set 1 to mask interrupt.*/
        AlGpio_Hal_IntrEnableMaskPin(Handle, Pin);
        MaskPinVal = AlGpio_Hal_IntrGetEnableMaskPin(Handle, Pin);
        AL_LOG(AL_LOG_LEVEL_INFO, "Mask Pin Value is %x", MaskPinVal);
    }
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
AL_VOID AlGpio_Hal_IntrHandler(AL_VOID *Instance)
{
    AL_GPIO_HalStruct *Handle = (AL_GPIO_HalStruct *)Instance;

    if (Handle->IntrType == GPIO_INTR_TYPE_LEVEL_HIGH || Handle->IntrType == GPIO_INTR_TYPE_LEVEL_LOW) {
        AlGpio_Hal_LevelIntrHandler(Instance);
    } else if (Handle->IntrType == GPIO_INTR_TYPE_EDGE_RISING || Handle->IntrType == GPIO_INTR_TYPE_EDGE_FALLING || Handle->IntrType == GPIO_INTR_TYPE_EDGE_BOTH) {
        AlGpio_Hal_EdgeIntrHandler(Instance);
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "Gpio Interrupt Type not exist.");
    }
}

/**
 * @brief  This function configure the interrupt, including direction, type and enable register.
 * @param  Handle
 * @param  Pin
 * @param  IntrType
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_IntrCfg(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_GPIO_IntrEnum IntrType)
{
    AL_ASSERT(Handle != AL_NULL, AL_GPIO_ERR_ILLEGAL_PARAM);

    if (IntrType == GPIO_INTR_TYPE_EDGE_FALLING || IntrType == GPIO_INTR_TYPE_EDGE_RISING || IntrType == GPIO_INTR_TYPE_EDGE_BOTH) {
        AlGpio_Hal_DebounceEnablePin(Handle, Pin);
    }

    if (Pin >= 0 && Pin <= MAX_PIN_NUMBER_IN_BANK_0) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
    } else if(Pin > MAX_PIN_NUMBER_IN_BANK_0 && Pin <= MAX_PIN_NUMBER_IN_BANK_1) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId + 1, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
    } else if(Pin > MAX_PIN_NUMBER_IN_BANK_1 && Pin <= MAX_PIN_NUMBER_IN_BANK_2) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId + 2, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
    } else if (Pin > MAX_PIN_NUMBER_IN_BANK_2 && Pin <= MAX_PIN_NUMBER_IN_BANK_3) {
        (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId + 3, AL_NULL, AlGpio_Hal_IntrHandler, Handle);
    }

    Handle->IntrType = IntrType;
    Handle->IntrPin = Pin;
    AlGpio_Hal_SetDirectionPin(Handle, Pin, GPIO_PIN_INPUT);
    AlGpio_Hal_IntrSetTypePin(Handle, Pin, IntrType);
    AlGpio_Hal_IntrEnablePin(Handle, Pin);

    return AL_OK;
}
