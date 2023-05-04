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

AL_S32 AlGpio_Dev_Init(AL_GPIO_DevStruct *Gpio, AL_GPIO_InitStruct *InitConfig)
{
    AL_GPIO_InitStruct *Config;
    AL_U32 Status = AL_GPIO_STATE_READY;
    AL_U8 i;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    
    if(InitConfig != AL_NULL) {
        Gpio->Configs = *InitConfig;
    }
    
    Gpio->MaxPinNum = GPIO_MAX_PIN_NUM;
	Gpio->MaxBanks  = GPIO_MAX_BANKS;

    for(i=0; i<Gpio->MaxBanks;i++) {
        AlGpio_ll_ClrWrite(Gpio->Configs.BaseAddr+(AL_U32)(i) * GPIO_REG_OFFSET, AL_ALL_ENABLE);
    }

    return AL_OK;
}


// GetBankPin
AL_VOID AlGpio_dev_GetBankPin(AL_U8 PinNumber, AL_U8 *BankNumber, AL_U8 *PinNumberInBank)
{
    AL_U32 AlGpioPinTable[4] = {0};
    AlGpioPinTable[0] = (AL_U32)31; 
	AlGpioPinTable[1] = (AL_U32)53; 
	AlGpioPinTable[2] = (AL_U32)85;
	AlGpioPinTable[3] = (AL_U32)117;

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
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    return AlGpio_ll_OutputRead(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
}

// InputRead
AL_S32 AlGpio_Dev_InputRead(AL_GPIO_DevStruct *Gpio, AL_U8 Bank) 
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    return AlGpio_ll_InputRead(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
}

// Write
AL_VOID AlGpio_Dev_Write(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Data)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    return AlGpio_ll_Write(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Data);
}

// ClrWrite
AL_VOID AlGpio_Dev_ClrWrite(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Data)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    return AlGpio_ll_ClrWrite(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Data);
}

// SetDirection
AL_VOID AlGpio_Dev_SetDirection(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Direction)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_ll_SetDirection(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Direction);
}

// AlGpio_ll_ClrDirection
AL_VOID AlGpio_Dev_ClrDirection(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Direction)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    return AlGpio_ll_ClrDirection(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Direction);
}

// GetDirection
AL_S32 AlGpio_Dev_GetDirection(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    return AlGpio_ll_GetDirection(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
}

// OutputEnable
AL_VOID AlGpio_Dev_OutputEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 OpEnable)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_ll_OutputEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, OpEnable);
}



// /* ② Pin APIs */
// OutputReadPin
AL_U32 AlGpio_Dev_OutputReadPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    return (AlGpio_ll_OutputRead(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET) >> (AL_U32)PinNumber) & (AL_U32)1;
}

// InputReadPin
AL_U32 AlGpio_Dev_InputReadPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    return (AlGpio_ll_InputRead(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET) >> (AL_U32)PinNumber) & (AL_U32)1;
}

// WritePin
AL_VOID AlGpio_Dev_WritePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Data)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 Value;
    AL_U32 DataVar = Data;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);

    DataVar &= (AL_U32)0x01; 
    Value = DataVar << PinNumber;
    
    AlGpio_ll_Write(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Value);
}

// ClrWritePin
AL_VOID AlGpio_Dev_ClrWritePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Data)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 Value;
    AL_U32 DataVar = Data;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);

    DataVar &= (AL_U32)0x01; 
    Value = DataVar << PinNumber;
    
    AlGpio_ll_ClrWrite(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Value);
}

// SetDirectionPin
AL_VOID AlGpio_Dev_SetDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 DirReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DirReg = AlGpio_ll_GetDirection(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    if(Direction != (AL_U32)0) { /*  Output Direction */
        DirReg |= ((AL_U32)1 << (AL_U32)PinNumber);
    } else { /*  Input Direction */
        DirReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
    }
    
    AlGpio_ll_SetDirection(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, DirReg);
}

// ClrDirectionPin
AL_VOID AlGpio_Dev_ClrDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 DirReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DirReg = AlGpio_ll_GetDirection(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    if(Direction != (AL_U32)0) {
        DirReg |= ((AL_U32)1 << (AL_U32)PinNumber);
    } else {
        DirReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
    }
    
    AlGpio_ll_ClrDirection(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, DirReg);
}

// GetDirectionPin
AL_U32 AlGpio_Dev_GetDirectionPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 Direction)
{
    AL_U8 Bank;
	AL_U8 PinNumber;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    return (AlGpio_ll_GetDirection(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET) >> (AL_U32)PinNumber) & (AL_U32)1;
}

// OutputEnablePin
AL_VOID AlGpio_Dev_OutputEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin, AL_U32 OpEnable)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 OpEnableReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    OpEnableReg = AlGpio_ll_GetOutputEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    if(OpEnable != (AL_U32)0) { /*  Output Direction */
        OpEnableReg |= ((AL_U32)1 << (AL_U32)PinNumber);
    } else { /*  Input Direction */
        OpEnableReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
    }
    
    AlGpio_ll_OutputEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, OpEnableReg);
}



/* ③ Bank APIs intr */
// IntrEnable
AL_VOID AlGpio_Dev_IntrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Raw)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_ll_IntrEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Raw);
}

// IntrClrEnable
AL_VOID AlGpio_Dev_IntrClrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_ll_IntrClrEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Mask);
}

// IntrEoi
AL_VOID AlGpio_Dev_IntrEoi(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_ll_IntrEoi(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Mask);
}

// IntrGetEnable
AL_U32 AlGpio_Dev_IntrGetEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    return AlGpio_ll_AlGpio_ll_IntrGetEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
}

// IntrEnableMask
AL_VOID AlGpio_Dev_IntrEnableMask(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_ll_IntrEnableMask(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Mask);
}

// IntrClrEnableMask
AL_VOID AlGpio_Dev_IntrClrEnableMask(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Mask)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_ll_IntrClrEnableMask(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Mask);
}

// IntrGetEnableMask
AL_U32 AlGpio_Dev_IntrGetEnableMask(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    return AlGpio_ll_AlGpio_ll_IntrGetEnableMask(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
}

// IntrGetStatus
AL_U32 AlGpio_Dev_IntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    return AlGpio_ll_IntrGetStatus(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
}

// AlGpio_ll_RawIntrGetStatus
AL_U32 AlGpio_Dev_RawIntrGetStatus(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    return AlGpio_ll_RawIntrGetStatus(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
}

// IntrSetType IntrSetPolarity IntrSetBothEdge
AL_VOID AlGpio_Dev_IntrSetType(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 IntrType, AL_U32 IntrPolarity, AL_U32 IntrEdge)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_ll_IntrClrType(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, AL_ALL_ENABLE);
    AlGpio_ll_IntrClrPolarity(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, AL_ALL_ENABLE);
    AlGpio_ll_IntrClrBothEdge(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, AL_ALL_ENABLE);

    AlGpio_ll_IntrSetType(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, IntrType);
    AlGpio_ll_IntrSetPolarity(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, IntrPolarity);
    AlGpio_ll_IntrSetBothEdge(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, IntrEdge);
}

// No return value.  IntrGetType IntrGetPolarity IntrGetBothEdge
AL_VOID AlGpio_Dev_IntrGetType(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 *IntrType, AL_U32 *IntrPolarity, AL_U32 *IntrEdge)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    *IntrType = AlGpio_ll_IntrGetType(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    *IntrPolarity = AlGpio_ll_IntrGetPolarity(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    *IntrEdge = AlGpio_ll_IntrGetBothEdge(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
}

// DebounceEnable
AL_VOID AlGpio_Dev_DebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Debounce)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    
    AlGpio_ll_DebounceEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Debounce);
}

// DebounceClrEnable
AL_VOID AlGpio_Dev_DebounceClrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Debounce)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_ll_DebounceClrEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Debounce);
}

// GetDebounce
AL_U32 AlGpio_Dev_GetDebounceEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    return AlGpio_ll_GetDebounceEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
}

// GetDebounceClr
AL_U32 AlGpio_Dev_GetDebounceClrEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    return AlGpio_ll_GetDebounceClrEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
}

// SyncEnable
AL_VOID AlGpio_Dev_SyncEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank, AL_U32 Debounce)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    
    AlGpio_ll_SyncEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, Debounce);
}

// GetSyncEnable
AL_U32 AlGpio_Dev_GetSyncEnable(AL_GPIO_DevStruct *Gpio, AL_U8 Bank)
{
    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    return AlGpio_ll_GetSyncEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
}


/* ④ Pin APIs intr */
// IntrEnablePin
AL_VOID AlGpio_Dev_IntrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrReg = ((AL_U32)1 << (AL_U32)PinNumber);
    AlGpio_ll_IntrEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, IntrReg);
}

// IntrClrEnablePin
AL_VOID AlGpio_Dev_IntrClrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrClrReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrClrReg = ((AL_U32)1 << (AL_U32)PinNumber);
    AlGpio_ll_IntrClrEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, IntrClrReg);
}

// IntrEoiPin
AL_VOID AlGpio_Dev_IntrEoiPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrEoiReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrEoiReg = ((AL_U32)1 << (AL_U32)PinNumber);
    AlGpio_ll_IntrEoi(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, IntrEoiReg);
}

// IntrEnableMaskPin
AL_VOID AlGpio_Dev_IntrEnableMaskPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrReg = ((AL_U32)1 << (AL_U32)PinNumber);
    AlGpio_ll_IntrEnableMask(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, IntrReg);
}

// IntrGetEnablePin
AL_BOOL AlGpio_Dev_IntrGetEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    IntrReg = AlGpio_ll_IntrGetEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

// IntrGetEnableMaskPin
AL_BOOL AlGpio_Dev_IntrGetEnableMaskPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    IntrReg = AlGpio_ll_IntrGetEnableMask(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

// IntrGetStatusPin
AL_BOOL AlGpio_Dev_IntrGetStatusPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_IntrGetStatus(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    return ((IntrReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

// RawIntrGetStatusPin
AL_BOOL AlGpio_Dev_RawIntrGetStatusPin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrReg = AlGpio_ll_RawIntrGetStatus(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
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

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    if (IntrType > GPIO_INTR_TYPE_LEVEL_LOW) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    AlGpio_ll_IntrClrType(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, AL_ENABLE);
    AlGpio_ll_IntrClrPolarity(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, AL_ENABLE);
    AlGpio_ll_IntrClrBothEdge(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, AL_ENABLE);

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrTypeReg = AlGpio_ll_IntrGetType(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    IntrPolarityReg = AlGpio_ll_IntrGetPolarity(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    IntrEdgeReg = AlGpio_ll_IntrGetBothEdge(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);

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
}

// IntrGetTypePin
AL_VOID AlGpio_Dev_IntrGetTypePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 IntrType;
    AL_U32 IntrPolarity;
    AL_U32 IntrEdge;
    AL_U8 IntrMode;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    IntrType = AlGpio_ll_IntrGetType(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET) & ((AL_U32)1 << (AL_U32)PinNumber);
    IntrPolarity = AlGpio_ll_IntrGetPolarity(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET) & ((AL_U32)1 << (AL_U32)PinNumber);
    if(IntrType == ((AL_U32)1 << (AL_U32)PinNumber)) {
        IntrEdge = AlGpio_ll_IntrGetBothEdge(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET) & ((AL_U32)1 << (AL_U32)PinNumber);
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

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DebounceReg = AlGpio_ll_GetDebounce(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    if(DebounceReg != (AL_U32)0) {
        DebounceReg |= ((AL_U32)1 << (AL_U32)PinNumber);
    } else {
        DebounceReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
    }

    AlGpio_ll_Debounce(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, DebounceReg);
}

// DebounceClrEnablePin
AL_VOID AlGpio_Dev_DebounceClrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 DebounceClrReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    DebounceClrReg = AlGpio_ll_GetDebounceClr(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    if(DebounceClrReg != (AL_U32)0) {
        DebounceClrReg |= ((AL_U32)1 << (AL_U32)PinNumber);
    } else {
        DebounceClrReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
    }

    AlGpio_ll_DebounceClr(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, DebounceClrReg);
}

// GetDebounceEnablePin
AL_BOOL AlGpio_Dev_GetDebounceEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 DebounceReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    DebounceReg = AlGpio_ll_GetDebounceEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    return ((DebounceReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

// GetDebounceClrEnablePin
AL_BOOL AlGpio_Dev_GetDebounceClrEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 DebounceClrReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    DebounceClrReg = AlGpio_ll_GetDebounceClrEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    return ((DebounceClrReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}

// SyncEnablePin
AL_VOID AlGpio_Dev_SyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
    AL_U32 SyncReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_dev_GetBankPin((AL_U8)Pin, &Bank, &PinNumber);
    SyncReg = AlGpio_ll_GetSyncEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    if(SyncReg != (AL_U32)0) {
        SyncReg |= ((AL_U32)1 << (AL_U32)PinNumber);
    } else {
        SyncReg &= ~((AL_U32)1 << (AL_U32)PinNumber);
    }

    AlGpio_ll_SyncEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET, SyncReg);
}

// GetSyncEnablePin
AL_BOOL AlGpio_Dev_GetSyncEnablePin(AL_GPIO_DevStruct *Gpio, AL_U32 Pin)
{
    AL_U8 Bank;
	AL_U8 PinNumber;
	AL_U32 SyncReg;

    if (Gpio == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    SyncReg = AlGpio_ll_GetSyncEnable(Gpio->Configs.BaseAddr + Bank * GPIO_REG_OFFSET);
    return ((SyncReg & ((AL_U32)1 << PinNumber)) == (AL_U32)0 ? (AL_U32)AL_FALSE : (AL_U32)AL_TRUE);
}
