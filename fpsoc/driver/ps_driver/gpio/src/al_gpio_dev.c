#include <stdio.h>
#include "stdlib.h"
#include "al_errno.h"
#include "al_gpio_hw.h"
#include "al_gpio_ll.h"
#include "al_gpio_dev.h"


AL_GPIO_HwConfigStruct *AlGpio_Dev_LookupConfig(AL_U32 DeviceId)
{
    AL_U32 Index;
	AL_GPIO_HwConfigStruct *CfgPtr = AL_NULL;

	for (Index = 0; Index < AL_GPIO_NUM_INSTANCE; Index++) {
		if (AlGpio_HwCfg[Index].DeviceId == DeviceId) {
			CfgPtr = &AlGpio_HwCfg[Index];
			break;
		}
	}

	return CfgPtr;
}

AL_S32 AlGpio_Dev_Init(AL_GPIO_DevStruct *Gpio, AL_GPIO_HwConfigStruct *HwConfig)
{
    AL_U8 i;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    
    Gpio->HwConfig.BaseAddress = HwConfig->BaseAddress;
    Gpio->HwConfig.BankMaxPins = HwConfig->BankMaxPins;
	Gpio->HwConfig.MaxBanks  = HwConfig->MaxBanks;

    return AL_OK;
}


// GetBankPin
AL_VOID AlGpio_dev_GetBankPin(AL_U8 PinNumber, AL_U8 *BankNumber, AL_U8 *PinNumberInBank)
{
    AL_U32 AlGpioPinTable[GPIO_MAX_BANKS] = {0};
    AlGpioPinTable[GPIO_BANK0] = MAX_PIN_NUMBER_IN_BANK_0; 
	AlGpioPinTable[GPIO_BANK1] = MAX_PIN_NUMBER_IN_BANK_1; 
	AlGpioPinTable[GPIO_BANK2] = MAX_PIN_NUMBER_IN_BANK_2;
	AlGpioPinTable[GPIO_BANK3] = MAX_PIN_NUMBER_IN_BANK_3;

    *BankNumber = 0U;
    while (*BankNumber < GPIO_MAX_BANKS) {
		if (PinNumber <= AlGpioPinTable[*BankNumber]) {
			break;
		}
		(*BankNumber)++;
	}

    if (*BankNumber == (AL_U8)0) {
		*PinNumberInBank = PinNumber;
	}else {
	    *PinNumberInBank = (AL_U8)((AL_U32)PinNumber %
					(AlGpioPinTable[*BankNumber - (AL_U8)1] + (AL_U32)1));
    }
}


/* ① Bank APIs */ 
// OutputRead
AL_S32 AlGpio_Dev_OutputRead(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    return AlGpio_ll_OutputRead(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

// InputRead
AL_S32 AlGpio_Dev_InputRead(AL_GPIO_DevStruct *Gpio, AL_U8 Bank) 
{
    return AlGpio_ll_InputRead(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

// Write
AL_VOID AlGpio_Dev_WriteBank(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Data)
{
    AlGpio_ll_ClrWrite(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Data);
    AlGpio_ll_Write(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Data);
}

#if 0
// ClrWrite
AL_VOID AlGpio_Dev_ClrWrite(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Data)
{
    AlGpio_ll_ClrWrite(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Data);
}
#endif

// SetDirection
AL_VOID AlGpio_Dev_SetDirection(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Direction)
{
    AlGpio_ll_ClrDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Direction);
    AlGpio_ll_SetDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Direction);
}

#if 0
// AlGpio_ll_ClrDirection
AL_VOID AlGpio_Dev_ClrDirection(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Direction)
{
    AlGpio_ll_ClrDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Direction);
}
#endif

// GetDirection
AL_S32 AlGpio_Dev_GetDirection(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    return AlGpio_ll_GetDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


// /* ② Pin APIs */
// OutputReadPin
AL_S32 AlGpio_Dev_OutputReadPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    return (AlGpio_ll_OutputRead(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> (AL_U32)PinNumber) & (AL_U32)1;
}

// InputReadPin
AL_S32 AlGpio_Dev_InputReadPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    return (AlGpio_ll_InputRead(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> (AL_U32)PinNumber) & (AL_U32)1;
}

// WritePin
AL_VOID AlGpio_Dev_WritePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Data)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 Value;
    AL_U32 DataVar = Data;


    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);

    DataVar &= (AL_U32)0x01; 
    Value = DataVar << PinNumber;
    
    AlGpio_ll_ClrWrite(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value);
    AlGpio_ll_Write(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value);
}

#if 0
// ClrWritePin
AL_VOID AlGpio_Dev_ClrWritePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Data)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 Value;
    AL_U32 DataVar = Data;


    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);

    DataVar &= (AL_U32)0x01; 
    Value = DataVar << PinNumber;
    
    AlGpio_ll_ClrWrite(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Value);
}
#endif

// SetDirectionPin
AL_VOID AlGpio_Dev_SetDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 DirReg;


    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DirReg = AlGpio_ll_GetDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(Direction != (AL_U32)0) { /*  Output Direction */
        DirReg |= ((AL_U32)1 << (AL_U32)PinNumber);
    } else { /*  Input Direction */
        DirReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
    }
    
    AlGpio_ll_ClrDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DirReg);
    AlGpio_ll_SetDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DirReg);
}

#if 0
// ClrDirectionPin
AL_VOID AlGpio_Dev_ClrDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 DirReg;


    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DirReg = AlGpio_ll_GetDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(Direction != (AL_U32)0) {
        DirReg |= ((AL_U32)1 << (AL_U32)PinNumber);
    } else {
        DirReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
    }
    
    AlGpio_ll_ClrDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DirReg);
}
#endif

// GetDirectionPin
AL_S32 AlGpio_Dev_GetDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction)
{
    AL_U8 Bank;
	AL_U8 PinNumber;


    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    return (AlGpio_ll_GetDirection(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) >> (AL_U32)PinNumber) & (AL_U32)1;
}



/* ③ Bank APIs intr */
// IntrEnable
AL_VOID AlGpio_Dev_IntrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Raw)
{
    AlGpio_ll_IntrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Raw);
}

// IntrClrEnable
AL_VOID AlGpio_Dev_IntrClrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask)
{
    AlGpio_ll_IntrClrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Mask);
}

// IntrEoi
AL_VOID AlGpio_Dev_IntrEoi(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask)
{
    AlGpio_ll_IntrEoi(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Mask);
}

// IntrGetEnable
AL_S32 AlGpio_Dev_IntrGetEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    return AlGpio_ll_IntrGetEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

// IntrEnableMask
AL_VOID AlGpio_Dev_IntrEnableMask(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask)
{
    AlGpio_ll_IntrEnableMask(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Mask);
}

// IntrClrEnableMask
AL_VOID AlGpio_Dev_IntrClrEnableMask(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask)
{
    AlGpio_ll_IntrClrEnableMask(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Mask);
}

// IntrGetEnableMask
AL_S32 AlGpio_Dev_IntrGetEnableMask(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    return AlGpio_ll_IntrGetEnableMask(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

// IntrGetStatus
AL_S32 AlGpio_Dev_IntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    return AlGpio_ll_IntrGetStatus(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

// AlGpio_ll_RawIntrGetStatus
AL_S32 AlGpio_Dev_RawIntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    return AlGpio_ll_RawIntrGetStatus(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

// IntrSetType IntrSetPolarity IntrSetBothEdge
AL_VOID AlGpio_Dev_IntrSetType(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 IntrType, AL_U32 IntrPolarity, AL_U32 IntrEdge)
{
    AlGpio_ll_IntrClrType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_ALL_ENABLE);
    AlGpio_ll_IntrClrPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_ALL_ENABLE);
    AlGpio_ll_IntrClrBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, AL_ALL_ENABLE);

    AlGpio_ll_IntrSetType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrType);
    AlGpio_ll_IntrSetPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrPolarity);
    AlGpio_ll_IntrSetBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrEdge);
}

// No return value.  IntrGetType IntrGetPolarity IntrGetBothEdge
AL_VOID AlGpio_Dev_IntrGetType(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 *IntrType, AL_U32 *IntrPolarity, AL_U32 *IntrEdge)
{
    *IntrType = AlGpio_ll_IntrGetType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    *IntrPolarity = AlGpio_ll_IntrGetPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    *IntrEdge = AlGpio_ll_IntrGetBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

// DebounceEnable
AL_VOID AlGpio_Dev_DebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Debounce)
{    
    AlGpio_ll_DebounceEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Debounce);
}

// DebounceClrEnable
AL_VOID AlGpio_Dev_DebounceClrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Debounce)
{
    AlGpio_ll_DebounceClrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Debounce);
}

// GetDebounce
AL_S32 AlGpio_Dev_GetDebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    return AlGpio_ll_GetDebounceEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

// GetDebounceClr
AL_S32 AlGpio_Dev_GetDebounceClrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    return AlGpio_ll_GetDebounceClrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}

// SyncEnable
AL_VOID AlGpio_Dev_SyncEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Debounce)
{    
    AlGpio_ll_SyncEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, Debounce);
}

// GetSyncEnable
AL_S32 AlGpio_Dev_GetSyncEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    return AlGpio_ll_GetSyncEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
}


/* ④ Pin APIs intr */
// IntrEnablePin
AL_VOID AlGpio_Dev_IntrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrReg = ((AL_U32)1 << (AL_U32)PinNumber);
    AlGpio_ll_IntrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrReg);
}

// IntrClrEnablePin
AL_VOID AlGpio_Dev_IntrClrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrClrReg;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrClrReg = ((AL_U32)1 << (AL_U32)PinNumber);
    AlGpio_ll_IntrClrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrClrReg);
}

// IntrEoiPin
AL_VOID AlGpio_Dev_IntrEoiPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrEoiReg;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrEoiReg = ((AL_U32)1 << (AL_U32)PinNumber);
    AlGpio_ll_IntrEoi(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrEoiReg);
}

// IntrEnableMaskPin
AL_VOID AlGpio_Dev_IntrEnableMaskPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrReg = ((AL_U32)1 << (AL_U32)PinNumber);
    AlGpio_ll_IntrEnableMask(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrReg);
}

// IntrGetEnablePin
AL_BOOL AlGpio_Dev_IntrGetEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    IntrReg = AlGpio_ll_IntrGetEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

// IntrGetEnableMaskPin
AL_BOOL AlGpio_Dev_IntrGetEnableMaskPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    IntrReg = AlGpio_ll_IntrGetEnableMask(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

// IntrGetStatusPin
AL_BOOL AlGpio_Dev_IntrGetStatusPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_IntrGetStatus(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

// RawIntrGetStatusPin
AL_BOOL AlGpio_Dev_RawIntrGetStatusPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_RawIntrGetStatus(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

// IntrSetTypePin
AL_VOID AlGpio_Dev_IntrSetTypePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U8 IntrType)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrTypeReg;
    AL_U32 IntrPolarityReg;
    AL_U32 IntrEdgeReg;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrTypeReg = AlGpio_ll_IntrGetType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    IntrPolarityReg = AlGpio_ll_IntrGetPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    IntrEdgeReg = AlGpio_ll_IntrGetBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);

    switch(IntrType) {
        case GPIO_INTR_TYPE_EDGE_RISING:
            IntrTypeReg |= ((AL_U32)1 << (AL_U32)PinNumber);
			IntrPolarityReg |= ((AL_U32)1 << (AL_U32)PinNumber);
			IntrEdgeReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
			break;
        case GPIO_INTR_TYPE_EDGE_FALLING:
            IntrTypeReg |= ((AL_U32)1 << (AL_U32)PinNumber);
			IntrPolarityReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
			IntrEdgeReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
			break;
        case GPIO_INTR_TYPE_EDGE_BOTH:
            IntrTypeReg |= ((AL_U32)1 << (AL_U32)PinNumber);
			IntrEdgeReg |= ((AL_U32)1 << (AL_U32)PinNumber);
			break;
        case GPIO_INTR_TYPE_LEVEL_HIGH:
            IntrTypeReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
			IntrPolarityReg |= ((AL_U32)1 << (AL_U32)PinNumber);
			break;
        case GPIO_INTR_TYPE_LEVEL_LOW:
            IntrTypeReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
			IntrPolarityReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
			break;
        default:
            break;
    }

    AlGpio_ll_IntrSetType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrTypeReg);
    AlGpio_ll_IntrSetPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrPolarityReg);
    AlGpio_ll_IntrSetBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, IntrEdgeReg);
}

// IntrGetTypePin
AL_U8 AlGpio_Dev_IntrGetTypePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrType;
    AL_U32 IntrPolarity;
    AL_U32 IntrEdge;
    AL_U8 IntrMode;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrType = AlGpio_ll_IntrGetType(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & ((AL_U32)1 << (AL_U32)PinNumber);
    IntrPolarity = AlGpio_ll_IntrGetPolarity(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & ((AL_U32)1 << (AL_U32)PinNumber);
    if(IntrType == ((AL_U32)1 << (AL_U32)PinNumber)) {
        IntrEdge = AlGpio_ll_IntrGetBothEdge(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET) & ((AL_U32)1 << (AL_U32)PinNumber);
        if(IntrEdge == (AL_U32)1 << (AL_U32)PinNumber) {
            IntrMode = GPIO_INTR_TYPE_EDGE_BOTH;
        } else if(IntrPolarity == (AL_U32)1 << (AL_U32)PinNumber) {
            IntrMode = GPIO_INTR_TYPE_EDGE_RISING;
        } else {
            IntrMode = GPIO_INTR_TYPE_EDGE_FALLING;
        }
    } else {
        if(IntrPolarity == (AL_U32)1 << (AL_U32)PinNumber) {
            IntrMode = GPIO_INTR_TYPE_LEVEL_HIGH;
        } else {
            IntrMode = GPIO_INTR_TYPE_LEVEL_LOW;
        }
    }

    return IntrMode;
}

// DebounceEnablePin
AL_VOID AlGpio_Dev_DebounceEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 DebounceReg;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DebounceReg = AlGpio_ll_GetDebounceEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(DebounceReg != (AL_U32)0) {
        DebounceReg |= ((AL_U32)1 << (AL_U32)PinNumber);
    } else {
        DebounceReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
    }

    AlGpio_ll_DebounceEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DebounceReg);
}

// DebounceClrEnablePin
AL_VOID AlGpio_Dev_DebounceClrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 DebounceClrReg;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DebounceClrReg = AlGpio_ll_GetDebounceClrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(DebounceClrReg != (AL_U32)0) {
        DebounceClrReg |= ((AL_U32)1 << (AL_U32)PinNumber);
    } else {
        DebounceClrReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
    }

    AlGpio_ll_DebounceClrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, DebounceClrReg);
}

// GetDebounceEnablePin
AL_BOOL AlGpio_Dev_GetDebounceEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 DebounceReg;

    DebounceReg = AlGpio_ll_GetDebounceEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((DebounceReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

// GetDebounceClrEnablePin
AL_BOOL AlGpio_Dev_GetDebounceClrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 DebounceClrReg;

    DebounceClrReg = AlGpio_ll_GetDebounceClrEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((DebounceClrReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

// SyncEnablePin
AL_VOID AlGpio_Dev_SyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 SyncReg;

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    SyncReg = AlGpio_ll_GetSyncEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    if(SyncReg != (AL_U32)0) {
        SyncReg |= ((AL_U32)1 << (AL_U32)PinNumber);
    } else {
        SyncReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
    }

    AlGpio_ll_SyncEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET, SyncReg);
}

// GetSyncEnablePin
AL_BOOL AlGpio_Dev_GetSyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 SyncReg;

    SyncReg = AlGpio_ll_GetSyncEnable(Gpio->HwConfig.BaseAddress + Bank * GPIO_REG_OFFSET);
    return ((SyncReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}
