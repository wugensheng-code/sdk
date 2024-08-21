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
 *
 * This function is the default event callback for AL_GPIO. It is called when a GPIO event occurs.
 *
 * @param GpioEvent The GPIO event structure containing information about the event.
 * @param CallbackRef A pointer to user-defined data that can be passed to the callback function.
 *
 * @return None.
 *
 */
static AL_VOID AlGpio_Hal_DefEventCallBack(AL_GPIO_EventStruct GpioEvent, AL_VOID *CallbackRef)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Bank 0x%x: IntrStatus is 0x%x", GpioEvent.Bank, GpioEvent.EventData);
}


/**
 *
 * This function searches for the hardware configuration structure that matches the provided device ID.
 *
 * @param DeviceId The ID of the device to look up.
 *
 * @return A pointer to the hardware configuration structure if found, or NULL if not found.
 *
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
 *
 * This function registers an event callback function and a reference pointer for the GPIO handle.
 * The event callback function will be called when a GPIO event occurs.
 *
 * @param Handle The GPIO handle.
 * @param CallBack The event callback function to register.
 * @param CallBackRef The reference pointer to pass to the event callback function.
 *
 * @return AL_OK if the event callback is successfully registered, otherwise an error code.
 *
 */
static AL_S32 AlGpio_Hal_RegisterEventCallBack(AL_GPIO_HalStruct *Handle, AL_GPIO_EventCallBack CallBack, AL_VOID *CallBackRef)
{
    AL_ASSERT((Handle != AL_NULL) && (CallBack != AL_NULL), AL_GPIO_ERR_ILLEGAL_PARAM);

    Handle->EventCallBack        = CallBack;
    Handle->EventCallBackRef     = CallBackRef;

    return AL_OK;
}


/**
 *
 * This function initializes the GPIO HAL structure with the provided device ID and event callback.
 * It also registers the event callback for the GPIO instance.
 *
 * @param Handle Pointer to the GPIO HAL structure pointer.
 * @param DevId Device ID of the GPIO instance.
 * @param CallBack Event callback function for the GPIO instance.
 *
 * @return Returns AL_OK if successful, otherwise returns an error code.
 *
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
 *
 * This function calculates the bank number and pin number within the bank based on the given pin number.
 *
 * @param PinNumber The pin number.
 * @param BankNumber Pointer to store the bank number.
 * @param PinNumberInBank Pointer to store the pin number within the bank.
 *
 * @return None.
 *
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
 *
 * This function retrieves the direction of a GPIO bank specified by the given handle and bank number.
 *
 * @param Handle The handle to the GPIO HAL structure.
 * @param Bank The bank number of the GPIO.
 *
 * @return The direction of the GPIO bank.
 *
 */
static AL_S32 AlGpio_Hal_GetBankDirection(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


/**
 *
 * This function sets the direction of a GPIO bank specified by the `Bank` parameter.
 * The direction can be set to either input or output using the `Direction` parameter.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct structure.
 * @param Bank The GPIO bank number.
 * @param Direction The direction to set for the GPIO bank.
 *        - 0: Input
 *        - 1: Output
 *
 * @return AL_OK if successful, otherwise an error code.
 *
 */
static AL_S32 AlGpio_Hal_SetBankDirection(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Direction)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_SetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Direction);
    AlGpio_ll_ClrDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, ~Direction);

    return AL_OK;
}


/**
 *
 * This function reads the output value of a GPIO bank specified by the `Bank` parameter.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct instance.
 * @param Bank The GPIO bank number.
 *
 * @return The output value of the GPIO bank.
 *
 */
AL_S32 AlGpio_Hal_ReadBankOutput(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_ReadOutput(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


/**
 *
 * This function reads the input value of the specified GPIO bank.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct instance.
 * @param Bank The GPIO bank number.
 *
 * @return The input value of the GPIO bank.
 *
 */
AL_S32 AlGpio_Hal_ReadBankInput(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_SetBankDirection(Handle, Bank, GPIO_BANK_INPUT);
    return AlGpio_ll_ReadInput(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


/**
 *
 * This function sets the direction of the GPIO bank to output and writes the specified data to it.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct instance.
 * @param Bank The GPIO bank number.
 * @param Data The data to be written to the GPIO bank.
 *
 * @return AL_OK if the operation is successful, otherwise an error code.
 *
 */
AL_S32 AlGpio_Hal_WriteBank(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Data)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);
    AL_U32 index = 0;
    AlGpio_Hal_SetBankDirection(Handle, Bank, GPIO_BANK_OUTPUT);

    AlGpio_ll_Write(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Data);
    AlGpio_ll_ClrWrite(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, ~Data);

    return AL_OK;
}



/* ② Pin APIs */
/**
 *
 * This function sets the direction of a GPIO pin to either input or output.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct structure.
 * @param Pin The pin number to set the direction for.
 * @param Direction The direction to set for the pin (0 for input, non-zero for output).
 *
 * @return AL_OK if the direction is set successfully, otherwise an error code.
 *
 */
static AL_S32 AlGpio_Hal_SetPinDirection(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U32 Direction)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 DirReg = 0;

    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);

    DirReg = AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(Direction != (AL_U32)0) { /*  Output Direction 1 */
        DirReg |= BIT(PinNumber);
        AlGpio_ll_SetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    } else { /*  Input Direction 0 */
        DirReg &= ~BIT(PinNumber);
        AlGpio_ll_ClrDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    }

    return AL_OK;
}


/**
 *
 * This function retrieves the direction (input or output) of the specified GPIO pin.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct instance.
 * @param Pin The GPIO pin number.
 *
 * @return The direction of the GPIO pin (0 for input, 1 for output).
 *
 */
static AL_S32 AlGpio_Hal_GetPinDirection(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    return (AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
}


/**
 *
 * This function reads the output value of a specific pin in the GPIO module.
 *
 * @param Handle Pointer to the GPIO HAL structure.
 * @param Pin The pin number to read the output value from.
 *
 * @return The output value of the specified pin (0 or 1).
 *
 */
AL_S32 AlGpio_Hal_ReadPinOutput(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;

    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);

    return (AlGpio_ll_ReadOutput(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
}


/**
 *
 * This function reads the input value of a specific GPIO pin.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct instance.
 * @param Pin The pin number to read the input value from.
 *
 * @return The input value of the specified GPIO pin (0 or 1).
 *
 */
AL_S32 AlGpio_Hal_ReadPinInput(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 DirReg = 0;

    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    DirReg = AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    DirReg &= ~BIT(PinNumber);

    AlGpio_ll_ClrDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber)); /* Input Direction */

    return (AlGpio_ll_ReadInput(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> PinNumber) & (AL_U32)1;
}


/**
 *
 * This function sets the output value of a specific GPIO pin to the specified data.
 *
 * @param Handle Pointer to the GPIO HAL structure.
 * @param Pin The pin number to write data to.
 * @param Data The data to be written to the pin (0 or 1).
 *
 * @return AL_OK if successful, an error code otherwise.
 *
 */
AL_S32 AlGpio_Hal_WritePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U32 Data)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 Value = 0;
    AL_U32 DataVar = Data;
    AL_U32 DirReg = 0;
    AL_U32 Read_Pin_Value = 0;


    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);

    /* Set direction. */
    DirReg = AlGpio_ll_GetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    DirReg |= BIT(PinNumber); /* Output Direction */
    AlGpio_ll_SetDirection(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DirReg);

    /* Write Pin */
    DataVar &= (AL_U32)0x01;
    Value = AlGpio_ll_ReadOutput(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if (DataVar == 1)
    {
        AlGpio_ll_Write(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value | (DataVar << PinNumber));
    } else if (DataVar == 0)
    {
        AlGpio_ll_ClrWrite(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    }

    return AL_OK;
}



/* ③ Bank APIs intr */

/**
 *
 * This function clears the interrupt for a specific bank of GPIO pins.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct instance.
 * @param Bank The bank number of the GPIO pins.
 * @param Value The value representing the pins to clear the interrupt for.
 *
 * @return AL_OK if successful, otherwise an error code.
 *
 */
static AL_S32 AlGpio_Hal_ClrBankIntr(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Value)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_ClrIntr(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value);

    return AL_OK;
}


/**
 *
 * This function retrieves the interrupt mask for the specified GPIO bank.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct instance.
 * @param Bank The GPIO bank number.
 *
 * @return The interrupt mask for the specified GPIO bank.
 *
 */
static AL_S32 AlGpio_Hal_GetBankIntrMask(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetIntrMask(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


/**
 *
 * This function masks the interrupt for a specific GPIO bank by disabling and enabling the interrupt mask.
 *
 * @param Handle Pointer to the GPIO HAL structure.
 * @param Bank The GPIO bank number.
 * @param Value The value to be masked.
 *
 * @return AL_OK if successful, otherwise an error code.
 *
 */
AL_S32 AlGpio_Hal_MaskBankIntr(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Value)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);
    AL_U32 IntrMaskVal = 0;
    IntrMaskVal = AlGpio_Hal_GetBankIntrMask(Handle, Bank);

    AlGpio_ll_EnableIntrMask(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value|IntrMaskVal);

    return AL_OK;
}


/**
 *
 * This function retrieves the raw interrupt status of a GPIO bank.
 *
 * @param Handle Pointer to the GPIO HAL structure.
 * @param Bank The GPIO bank number.
 *
 * @return The raw interrupt status of the GPIO bank.
 *
 */
AL_S32 AlGpio_Hal_GetBankRawIntrStatus(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetRawIntrStatus(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


/**
  *
 * This function enables bank synchronization for a specific GPIO bank, allowing
 * multiple GPIO pins within the bank to be updated simultaneously.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct instance.
 * @param Bank The bank number to enable synchronization for.
 * @param Value The synchronization value to set for the bank.
 *
 * @return AL_OK if successful, an error code otherwise.
 *
 */
AL_S32 AlGpio_Hal_EnableBankSync(AL_GPIO_HalStruct *Handle, AL_U32 Bank, AL_U32 Value)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_ll_EnableSync(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value);

    return AL_OK;
}


/**
 *
 * This function retrieves the interrupt status of a specific GPIO bank.
 *
 * @param Handle Pointer to the GPIO HAL structure.
 * @param Bank The GPIO bank number.
 *
 * @return The interrupt status of the GPIO bank.
 *
 */
static AL_S32 AlGpio_Hal_GetBankIntrStatus(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetIntrStatus(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

/**
 *
 * This function retrieves the synchronization status of a GPIO bank.
 *
 * @param Handle Pointer to the GPIO HAL structure.
 * @param Bank The bank number of the GPIO.
 *
 * @return The synchronization status of the GPIO bank.
 *
 */
AL_S32 AlGpio_Hal_GetBankSync(AL_GPIO_HalStruct *Handle, AL_U32 Bank)
{
    AL_ASSERT((Handle != AL_NULL) && (Bank < Handle->HwConfig.MaxBanks), AL_GPIO_ERR_ILLEGAL_PARAM);

    return AlGpio_ll_GetSync(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}



/* ④ Pin APIs intr */
/**
 *
 * This function checks whether the interrupt is enabled for the specified GPIO pin.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct instance.
 * @param Pin The GPIO pin number.
 *
 * @return AL_TRUE if the interrupt is enabled for the pin, AL_FALSE otherwise.
 *
 */
static AL_BOOL AlGpio_Hal_GetPinIntrEnable(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrReg = 0;

    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_GetIntrEnable(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}


/**
 *
 * This function enables the interrupt for the specified GPIO pin.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct instance.
 * @param Pin The GPIO pin number to enable interrupt for.
 *
 * @return AL_OK if successful, otherwise an error code.
 *
 */
static AL_S32 AlGpio_Hal_EnablePinIntr(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 GetPinIntrEnable = 0;

    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);

    /* Check if the Pin is interrupt enabled. */
    GetPinIntrEnable = AlGpio_Hal_GetPinIntrEnable(Handle, Pin);
    if (!GetPinIntrEnable) {
        AlGpio_ll_EnableIntr(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    }

    return AL_OK;
}


/**
 *
 * This function sets the interrupt type for a specific GPIO pin. The interrupt type can be one of the following:
 * - GPIO_INTR_TYPE_EDGE_RISING: Interrupt triggered on rising edge.
 * - GPIO_INTR_TYPE_EDGE_FALLING: Interrupt triggered on falling edge.
 * - GPIO_INTR_TYPE_EDGE_BOTH: Interrupt triggered on both rising and falling edges.
 * - GPIO_INTR_TYPE_LEVEL_HIGH: Interrupt triggered when the pin is at a high level.
 * - GPIO_INTR_TYPE_LEVEL_LOW: Interrupt triggered when the pin is at a low level.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct instance.
 * @param Pin The GPIO pin number.
 * @param IntrType The interrupt type to set.
 *
 * @return AL_OK if successful, an error code otherwise.
 *
 */
static AL_S32 AlGpio_Hal_SetPinIntrType(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_GPIO_IntrEnum IntrType)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrTypeReg = 0;
    AL_U32 IntrPolarityReg = 0;
    AL_U32 IntrEdgeReg = 0;

    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

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
 *
 * This function retrieves the interrupt type for a specific GPIO pin
 * based on the provided handle and pin number.
 *
 * @param Handle The handle to the GPIO HAL structure.
 * @param Pin The pin number for which to retrieve the interrupt type.
 *
 * @return The interrupt mode for the specified pin.
 *
 */
static AL_S32 AlGpio_Hal_GetPinIntrType(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrType = 0;
    AL_U32 IntrPolarity = 0;
    AL_U32 IntrEdge = 0;
    AL_U32 IntrMode = 0;

    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

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


static AL_BOOL AlGpio_Hal_GetPinDebounce(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 IntrReg = 0;

    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin(Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_GetDebounce(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & (BIT(PinNumber))) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}


/**
 *
 * This function enables pin debounce for a specific GPIO pin. Pin debounce is a technique used to eliminate
 * the effects of mechanical switch bouncing, which can cause multiple false readings when a switch is pressed
 * or released. By enabling pin debounce, the GPIO pin will only register a single stable state change, ignoring
 * any temporary fluctuations caused by switch bouncing.
 *
 * @param Handle Pointer to the AL_GPIO_HalStruct representing the GPIO handle.
 * @param Pin The GPIO pin number to enable debounce for.
 *
 * @return Returns AL_OK if the pin debounce was successfully enabled, or an error code if an error occurred.
 *
 */
static AL_S32 AlGpio_Hal_EnablePinDebounce(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    AL_U32 Bank = 0;
    AL_U32 PinNumber = 0;
    AL_U32 DebounceReg = 0;
    AL_U32 GetPinDebounce = 0;

    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

    AlGpio_Hal_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DebounceReg = AlGpio_ll_GetDebounce(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(DebounceReg != (AL_U32)0) {
        DebounceReg |= (BIT(PinNumber));
    } else {
        DebounceReg &= ~(BIT(PinNumber));
    }

    GetPinDebounce = AlGpio_Hal_GetPinDebounce(Handle, Pin);
    if (!GetPinDebounce) {
        AlGpio_ll_EnableDebounce(Handle->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, BIT(PinNumber));
    }

    return AL_OK;
}


/**
 *
 * This function is responsible for handling interrupts for the GPIO HAL.
 * It iterates through each bank of GPIO pins, checks the interrupt status,
 * and calls the event callback function if an interrupt is detected and the
 * corresponding bank is enabled for interrupts.
 *
 * @param Instance Pointer to the GPIO HAL instance.
 *
 * @return None.
 *
 */
static AL_VOID AlGpio_Hal_IntrHandler(AL_VOID *Instance)
{
    AL_GPIO_HalStruct *Handle = (AL_GPIO_HalStruct *)Instance;
    AL_U32 Bank = 0, IntrStatus = 0;

    for(Bank = 0; Bank < GPIO_MAX_BANKS; Bank ++) {
        IntrStatus = AlGpio_Hal_GetBankIntrStatus(Handle, Bank);
        if (IntrStatus != 0) {
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
                }
            }
        }

    }
}


/**
 *
 * This function configures the interrupt settings for a specific GPIO pin, including the interrupt type,
 * polarity, and edge.
 *
 * @param Handle The GPIO handle.
 * @param Pin The pin number to configure.
 * @param IntrType The interrupt type to set for the pin.
 *
 * @return AL_OK if the configuration was successful, AL_FALSE otherwise.
 *
 */
AL_S32 AlGpio_Hal_IntrPinCfg(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_GPIO_IntrEnum IntrType)
{
    AL_ASSERT((Handle != AL_NULL) && (Pin < Handle->HwConfig.BankMaxPins), AL_GPIO_ERR_ILLEGAL_PARAM);

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
    if(GetIntrMode != IntrType || GetPinDirection != 0 || GetPinIntrEnable == 0) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Interrupt configuration was set incorrectly, Pin IntrMode got is 0x%x, \
            Pin Direction got is 0x%x, Pin Status got is 0x%x.", GetIntrMode, GetPinDirection, GetPinIntrEnable);
        return AL_FALSE;
    }

    return AL_OK;
}
