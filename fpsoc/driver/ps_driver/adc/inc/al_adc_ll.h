/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights Reserved4.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __AL_ADC_LL_H_
#define __AL_ADC_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_adc_hw.h"

/**************************** Type Definitions ******************************/
typedef enum
{
    AL_ADC_CHAN0    = 0,
    AL_ADC_CHAN1    = 1,
    AL_ADC_CHAN2    = 2,
    AL_ADC_CHAN3    = 3,
    AL_ADC_CHAN4    = 4,
    AL_ADC_CHAN5    = 5,
    AL_ADC_CHAN6    = 6,
    AL_ADC_CHAN7    = 7
} AL_ADC_ChanEnum;

typedef enum
{
    AL_ADC_IO_MUX_0     = 0,
    AL_ADC_IO_MUX_1     = 1,
    AL_ADC_IO_MUX_2     = 2,
    AL_ADC_IO_MUX_3     = 3,
    AL_ADC_IO_MUX_4     = 4,
    AL_ADC_IO_MUX_5     = 5,
    AL_ADC_IO_MUX_6     = 6,
    AL_ADC_IO_MUX_7     = 7,
    AL_ADC_IO_MUX_8     = 8,
    AL_ADC_IO_MUX_9     = 9,
    AL_ADC_IO_MUX_10    = 10,
    AL_ADC_IO_MUX_11    = 11,
    AL_ADC_IO_MUX_12    = 12,
    AL_ADC_IO_MUX_13    = 13,
    AL_ADC_IO_MUX_14    = 14,
    AL_ADC_IO_MUX_15    = 15,
    AL_ADC_IO_MUX_16    = 16    /*dedicated*/
} AL_ADC_IoMuxEnum;

typedef enum
{
    AL_ADC_FLAG_DONE        =  0,
    AL_ADC_FLAG_GTH         =  1,
    AL_ADC_FLAG_LTH         =  2,
    AL_ADC_FLAG_ERROR       =  3,
    AL_ADC_FLAG_COMP_VC0    =  8,
    AL_ADC_FLAG_COMP_VC1    =  9,
    AL_ADC_FLAG_COMP_VC2    =  10,
    AL_ADC_FLAG_COMP_VC3    =  11,
    AL_ADC_FLAG_COMP_VC4    =  12,
    AL_ADC_FLAG_COMP_VC5    =  13,
    AL_ADC_FLAG_COMP_VC6    =  14,
    AL_ADC_FLAG_COMP_VC7    =  15
}AL_ADC_FlagEnum;

typedef enum
{
    AL_ADC_REG_ADC_SOC    = 0,
    AL_ADC_PIB_ADC_SOC    = 1
} AL_ADC_TriggerSignalEnum;

typedef enum
{
    AL_ADC_DISALLOW    = 0,
    AL_ADC_ALLOW       = 1
} AL_ADC_AllowAnalogMuxEnum;

typedef enum
{
    AL_ADC_NORMAL_MODE          = 0,
    AL_ADC_PHY_EXTERNAL_MUX     = 1
} AL_ADC_PhyExternalMuxEnum;

typedef enum
{
    AL_ADC_UNIPOLAR   = 0,
    AL_ADC_BIPOLAR    = 1,
} AL_ADC_InputSingalEnum;

typedef enum
{
    AL_ADC_6BIT_MODE  = 0,
    AL_ADC_8BIT_MODE  = 1,
    AL_ADC_10BIT_MODE = 2,
    AL_ADC_12BIT_MODE = 3
} AL_ADC_ResolutionSelEnum;

typedef enum
{
    AL_ADC_REF_VREF       = 0,
    AL_ADC_REF_INTER_REF  = 1
} AL_ADC_RefVoltagEnum;

typedef enum
{
    AL_ADC_SW_RELEASE = 0,
    AL_ADC_SW_RESET   = 1
} AL_ADC_SwResetEnum;

typedef enum
{
    AL_ADC_CLK_OSC_44MHz       = 0,
    AL_ADC_CLK_PS_50MHz        = 1,
    AL_ADC_CLK_PL_70MHz        = 2
} AL_ADC_ClkSourceEnum;

typedef enum
{
    AL_ADC_END_CONV       = 0,
    AL_ADC_START_CONV     = 1
} AL_ADC_TriggerConvEnum;

typedef enum
{
    AL_ADC_SINGLE_CHANNEL_MODE    = 0,
    AL_ADC_SINGLE_PASS_MODE       = 1,
    AL_ADC_CONTINUE_MODE          = 2
} AL_ADC_ConvModeSelectEnum;

typedef enum
{
    AL_ADC_INTR_DONE    = 0,
    AL_ADC_INTR_GTH     = 1,
    AL_ADC_INTR_LTH     = 2,
    AL_ADC_INTR_ERROR   = 3
}AL_ADC_IntrtypeEnum;

typedef enum
{
    AL_ADC_EDGE_FALLING    = 0,
    AL_ADC_EDGE_RISING     = 1,
}AL_ADC_Edge;

typedef union {
    struct {
        AL_U16 AdcSwReset : 1;
        AL_U16 Enctr : 3;
        AL_U16 Reserved4 : 1;
        AL_U16 RefSel : 1;
        AL_U16 ResSel : 2;
        AL_U16 DiffEnable : 1;
        AL_U16 PhyExternalMux : 1;
        AL_U16 AnalogMuxEn : 1;
        AL_U16 RegAdcSocSel : 1;
        AL_U16 PhyExternalMuxGap : 4;
    };
    AL_U16 RegData;
} AL_ADC_PlReg_Cfg0;

typedef union {
    struct {
        AL_U16 IntrDoneMask : 1;
        AL_U16 IntrGthMask : 1;
        AL_U16 IntrLthMask : 1;
        AL_U16 IntrErrorMask : 1;
        AL_U16 Reserved4 : 4;
        AL_U16 IntrDone : 1;
        AL_U16 IntrGth : 1;
        AL_U16 IntrLth : 1;
        AL_U16 IntrError : 1;
        AL_U16 Reserved12 : 4;
    };
    AL_U16 RegData;
} AL_ADC_PlReg_Cfg1;

typedef union {
    struct {
        AL_U16 RegAdcEnable : 1;
        AL_U16 RegAdcDislvl : 1;
        AL_U16 Reserved4 : 2;
        AL_U16 AdcClkSel : 2;
        AL_U16 AdcClkGate : 1;
        AL_U16 Reserved7 : 1;
        AL_U16 AdcClkDiv : 8;
    };
    AL_U16 RegData;
} AL_ADC_PlReg_Cfg2;

typedef union {
    struct {
        AL_U16 ModeSel : 3;
        AL_U16 Reserved3 : 1;
        AL_U16 ChanSel : 3;
        AL_U16 Reserved7 : 1;
        AL_U16 ExternalChanSel : 3;
        AL_U16 Reserved11 : 1;
        AL_U16 RegAdcSoc : 1;
        AL_U16 Reserved13 : 3;
    };
    AL_U16 RegData;
} AL_ADC_PlReg_Cfg3;


#define ADC_READ_CMD   0x78
#define ADC_WRITE_CMD  0x77

/***************** Macros (Inline Functions) Definitions ********************/


/*
* Adc low-level driver through the PS-ADC Channel
*/

static inline AL_VOID AlAdc_ll_SetIgap(AL_REG AdcBaseAddr, AL_U8 Data)
{
    AL_REG32_SET_BITS(AdcBaseAddr + ADC_CFG_OFFSET, ADC_CFG_IGAP_SHIFT, ADC_CFG_IGAP_SIZE, Data);
}

static inline AL_VOID AlAdc_ll_SetReadIgap(AL_REG AdcBaseAddr, AL_U8 Data)
{
    AL_REG32_SET_BITS(AdcBaseAddr + ADC_CFG_OFFSET, ADC_CFG_READ_IGAP_SHIFT, ADC_CFG_READ_IGAP_SIZE, Data);
}

static inline AL_VOID AlAdc_ll_SetTckRate(AL_REG AdcBaseAddr, AL_U8 Data)
{
    AL_REG32_SET_BITS(AdcBaseAddr + ADC_CFG_OFFSET, ADC_CFG_TCK_RATE_SHIFT, ADC_CFG_TCK_RATE_SIZE, Data);
}

static inline AL_VOID AlAdc_ll_SetDataFifoLowThre(AL_REG AdcBaseAddr, AL_U8 Data)
{
    AL_REG32_SET_BITS(AdcBaseAddr + ADC_CFG_OFFSET, ADC_CFG_DATA_FIFO_LVL_L_SHIFT, ADC_CFG_DATA_FIFO_LVL_L_SIZE, Data);
}

static inline AL_VOID AlAdc_ll_SetCmdFifoLowThre(AL_REG AdcBaseAddr, AL_U8 Data)
{
    AL_REG32_SET_BITS(AdcBaseAddr + ADC_CFG_OFFSET, ADC_CFG_CMD_FIFO_LVL_L_SHIFT, ADC_CFG_CMD_FIFO_LVL_L_SIZE, Data);
}

static inline AL_VOID AlAdc_ll_SetDataFifoHighThre(AL_REG AdcBaseAddr, AL_U8 Data)
{
    AL_REG32_SET_BITS(AdcBaseAddr + ADC_CFG_OFFSET, ADC_CFG_DATA_FIFO_LVL_H_SHIFT, ADC_CFG_DATA_FIFO_LVL_H_SIZE, Data);
}

static inline AL_VOID AlAdc_ll_SetCmdFifoHighThre(AL_REG AdcBaseAddr, AL_U8 Data)
{
    AL_REG32_SET_BITS(AdcBaseAddr + ADC_CFG_OFFSET, ADC_CFG_CMD_FIFO_LVL_H_SHIFT, ADC_CFG_CMD_FIFO_LVL_H_SIZE, Data);
}

static inline AL_VOID AlAdc_ll_SetJtagRedge(AL_REG AdcBaseAddr, AL_ADC_Edge Edge)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_CFG_OFFSET, ADC_CFG_JTAG_REDGE_SHIFT, Edge);
}

static inline AL_VOID AlAdc_ll_SetJtagWedge(AL_REG AdcBaseAddr, AL_ADC_Edge Edge)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_CFG_OFFSET, ADC_CFG_JTAG_WEDGE_SHIFT, Edge);
}

static inline AL_VOID AlAdc_ll_EnablePsAdc(AL_REG AdcBaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_CFG_OFFSET, ADC_CFG_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAdc_ll_ClrPsAdcIntr(AL_REG AdcBaseAddr)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_OFFSET, ADC_INTR_INTR_SHIFT, AL_TRUE);
}

static inline AL_VOID AlAdc_ll_ClrDataFifoLth(AL_REG AdcBaseAddr)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_OFFSET, ADC_INTR_DATA_FIFO_LTH_SHIFT, AL_TRUE);
}

static inline AL_VOID AlAdc_ll_ClrCmdFifoLth(AL_REG AdcBaseAddr)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_OFFSET, ADC_INTR_CMD_FIFO_LTH_SHIFT, AL_TRUE);
}

static inline AL_VOID AlAdc_ll_ClrDataFifoGth(AL_REG AdcBaseAddr)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_OFFSET, ADC_INTR_DATA_FIFO_GTH_SHIFT, AL_TRUE);
}

static inline AL_VOID AlAdc_ll_ClrCmdFifoGth(AL_REG AdcBaseAddr)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_OFFSET, ADC_INTR_CMD_FIFO_GTH_SHIFT, AL_TRUE);
}

static inline AL_VOID AlAdc_ll_MaskAdcIntr(AL_REG AdcBaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_MASK_OFFSET, ADC_INTR_MASK_M_INTR_SHIFT, State);
}

static inline AL_VOID AlAdc_ll_MaskDataFifoLthIntr(AL_REG AdcBaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_MASK_OFFSET, ADC_INTR_MASK_M_DATA_FIFO_LTH_SHIFT, State);
}

static inline AL_VOID AlAdc_ll_MaskCmdFifoLthIntr(AL_REG AdcBaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_MASK_OFFSET, ADC_INTR_MASK_M_CMD_FIFO_LTH_SHIFT, State);
}

static inline AL_VOID AlAdc_ll_MaskDataFifoGthIntr(AL_REG AdcBaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_MASK_OFFSET, ADC_INTR_MASK_M_DATA_FIFO_GTH_SHIFT, State);
}

static inline AL_VOID AlAdc_ll_MaskCmdFifoGthIntr(AL_REG AdcBaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_MASK_OFFSET, ADC_INTR_MASK_M_CMD_FIFO_GTH_SHIFT, State);
}

static inline AL_BOOL AlAdc_ll_IsAdcIntr(AL_REG AdcBaseAddr)
{
    AL_REG32_GET_BIT(AdcBaseAddr + ADC_MSTS_OFFSET, ADC_MSTS_INTR_SHIFT);
}

static inline AL_BOOL AlAdc_ll_IsDataFifoEmpty(AL_REG AdcBaseAddr)
{
    AL_REG32_GET_BIT(AdcBaseAddr + ADC_MSTS_OFFSET, ADC_MSTS_DATA_FIFO_E_SHIFT);
}

static inline AL_BOOL AlAdc_ll_IsDataFifoFull(AL_REG AdcBaseAddr)
{
    AL_REG32_GET_BIT(AdcBaseAddr + ADC_MSTS_OFFSET, ADC_MSTS_DATA_FIFO_F_SHIFT);
}

static inline AL_BOOL AlAdc_ll_IsCmdFifoEmpty(AL_REG AdcBaseAddr)
{
    AL_REG32_GET_BIT(AdcBaseAddr + ADC_MSTS_OFFSET, ADC_MSTS_CMD_FIFO_E_SHIFT);
}

static inline AL_BOOL AlAdc_ll_IsCmdFifoFull(AL_REG AdcBaseAddr)
{
    AL_REG32_GET_BIT(AdcBaseAddr + ADC_MSTS_OFFSET, ADC_MSTS_CMD_FIFO_F_SHIFT);
}

/*
* This function is used for writing to PL-ADC Registers using the Command FIFO.
*/
static inline AL_VOID AlAdc_ll_WritePlAdcReg(AL_REG AdcBaseAddr, AL_U8 PLRegAddr, AL_U16 Data)
{
    AL_REG32_WRITE(AdcBaseAddr + ADC_CMD_FIFO_OFFSET, ADC_WRITE_CMD << 24 | PLRegAddr << 16 | Data);
	/**
	 * Read the Read FIFO after any write since for each write
	 * one location of Read FIFO gets updated
	 */
    (AL_VOID)(AL_REG32_READ(AdcBaseAddr + ADC_DATA_FIFO_OFFSET));
}

/*
* This function is used for reading to PL-ADC Registers using the Data FIFO.
*/
static inline AL_U16 AlAdc_ll_ReadPlAdcReg(AL_REG AdcBaseAddr, AL_U8 PLRegAddr)
{
    AL_REG32_WRITE(AdcBaseAddr + ADC_CMD_FIFO_OFFSET, ADC_READ_CMD << 24 | PLRegAddr << 16);
    while (AL_REG32_READ(AdcBaseAddr + ADC_MSTS_OFFSET) & ADC_MSTS_R_DATA_FIFO_E_MASK);
    return (AL_U16)(AL_REG32_READ(AdcBaseAddr + ADC_DATA_FIFO_OFFSET));
}

static inline AL_VOID AlAdc_ll_FlushCmdFifo(AL_REG AdcBaseAddr)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_MCTRL_OFFSET, ADC_MCTRL_CMD_FIFO_FLUSH_SHIFT, AL_TRUE);
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_MCTRL_OFFSET, ADC_MCTRL_CMD_FIFO_FLUSH_SHIFT, AL_FALSE);
}

static inline AL_VOID AlAdc_ll_FlushDataFifo(AL_REG AdcBaseAddr)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_MCTRL_OFFSET, ADC_MCTRL_DATA_FIFO_FLUSH_SHIFT, AL_TRUE);
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_MCTRL_OFFSET, ADC_MCTRL_DATA_FIFO_FLUSH_SHIFT, AL_FALSE);
}

static inline AL_VOID AlAdc_ll_ResetPsAdc(AL_REG AdcBaseAddr)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_MCTRL_OFFSET, ADC_MCTRL_RESET_SHIFT, AL_TRUE);
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_MCTRL_OFFSET, ADC_MCTRL_RESET_SHIFT, AL_FALSE);
}

static inline AL_U16 AlAdc_ll_GetAdcData(AL_REG AdcBaseAddr, AL_ADC_ChanEnum ChanNum)
{
    return (AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, (ADC_VC0_OFFSET + ChanNum * 2)) >> 4);
}

static inline AL_U16 AlAdc_ll_GetAdcFlag(AL_REG AdcBaseAddr)
{
    return AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_FLAG_OFFSET);
}

static inline AL_VOID AlAdc_ll_SetIomuxForChan0(AL_REG AdcBaseAddr, AL_ADC_IoMuxEnum IomuxForChan0)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CHANNEL_0_1_OFFSET);

    TmpData = (TmpData & 0xFF00) | (IomuxForChan0);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CHANNEL_0_1_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_SetIomuxForChan1(AL_REG AdcBaseAddr, AL_ADC_IoMuxEnum IomuxForChan1)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CHANNEL_0_1_OFFSET);

    TmpData = (TmpData & 0xFF) | (IomuxForChan1 << 8);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CHANNEL_0_1_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_SetIomuxForChan2(AL_REG AdcBaseAddr, AL_ADC_IoMuxEnum IomuxForChan2)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CHANNEL_2_3_OFFSET);

    TmpData = (TmpData & 0xFF00) | (IomuxForChan2);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CHANNEL_2_3_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_SetIomuxForChan3(AL_REG AdcBaseAddr, AL_ADC_IoMuxEnum IomuxForChan3)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CHANNEL_2_3_OFFSET);

    TmpData = (TmpData & 0xFF) | (IomuxForChan3 << 8);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CHANNEL_2_3_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_SetIomuxForChan4(AL_REG AdcBaseAddr, AL_ADC_IoMuxEnum IomuxForChan4)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CHANNEL_4_5_OFFSET);

    TmpData = (TmpData & 0xFF00) | (IomuxForChan4);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CHANNEL_4_5_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_SetIomuxForChan5(AL_REG AdcBaseAddr, AL_ADC_IoMuxEnum IomuxForChan5)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CHANNEL_4_5_OFFSET);

    TmpData = (TmpData & 0xFF) | (IomuxForChan5 << 8);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CHANNEL_4_5_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_SetIomuxForChan6(AL_REG AdcBaseAddr, AL_ADC_IoMuxEnum IomuxForChan6)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CHANNEL_6_7_OFFSET);

    TmpData = (TmpData & 0xFF00) | (IomuxForChan6);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CHANNEL_6_7_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_SetIomuxForChan7(AL_REG AdcBaseAddr, AL_ADC_IoMuxEnum IomuxForChan7)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CHANNEL_6_7_OFFSET);

    TmpData = (TmpData & 0xFF) | (IomuxForChan7 << 8);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CHANNEL_6_7_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_SetAdcChanXLth(AL_REG AdcBaseAddr, AL_ADC_ChanEnum ChanNum, AL_U16 Data)
{
    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, (ADC_CH0_LTH_OFFSET + ChanNum * 2), Data << 4);
}

static inline AL_VOID AlAdc_ll_SetAdcChanXGth(AL_REG AdcBaseAddr, AL_ADC_ChanEnum ChanNum, AL_U16 Data)
{
    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, (ADC_CH0_GTH_OFFSET + ChanNum * 2), Data << 4);
}

static inline AL_VOID AlAdc_ll_DisableChanIntr(AL_REG AdcBaseAddr, AL_ADC_IntrtypeEnum IntrType)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET);
    TmpData = ((TmpData & ~(1 << IntrType)) | (1 << IntrType));
    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_EnableChanIntr(AL_REG AdcBaseAddr, AL_ADC_IntrtypeEnum IntrType)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET);
    TmpData = (TmpData & ~(1 << IntrType));
    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_ClrChanIntr(AL_REG AdcBaseAddr, AL_ADC_IntrtypeEnum IntrType)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET);
    TmpData |= (AL_TRUE << (IntrType + ADC_CONFIG1_INTR_DONE_SHIFT));
    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET, TmpData);
}

static inline AL_U16 AlAdc_ll_GetAdcChanIntrType(AL_REG AdcBaseAddr)
{
    return (AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET) >> 8);
}


static inline AL_VOID AlAdc_ll_SetAdcCfg0Reg(AL_REG AdcBaseAddr, AL_U16 Data)
{
    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG0_OFFSET, Data);
}

static inline AL_VOID AlAdc_ll_SetAdcCfg1Reg(AL_REG AdcBaseAddr, AL_U16 Data)
{
    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET, Data);
}

static inline AL_VOID AlAdc_ll_SetAdcCfg2Reg(AL_REG AdcBaseAddr, AL_U16 Data)
{
    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG2_OFFSET, Data);
}

static inline AL_VOID AlAdc_ll_SetAdcCfg3Reg(AL_REG AdcBaseAddr, AL_U16 Data)
{
    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG3_OFFSET, Data);
}

static inline AL_VOID AlAdc_ll_EnableAdc(AL_REG AdcBaseAddr)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG2_OFFSET);

    TmpData = (TmpData & 0xFFFE) | (AL_TRUE);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG2_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_DisableAdc(AL_REG AdcBaseAddr)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG2_OFFSET);

    TmpData = (TmpData & 0xFFFE) | (AL_FALSE);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG2_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_StartConv(AL_REG AdcBaseAddr)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG3_OFFSET);

    TmpData = (TmpData & 0xEFFF) | (AL_TRUE << 12);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG3_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_StopConv(AL_REG AdcBaseAddr)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG3_OFFSET);

    TmpData = (TmpData & 0xEFFF) | (AL_FALSE << 12);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG3_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_SetRefVoltag(AL_REG AdcBaseAddr, AL_ADC_RefVoltagEnum RefVoltag)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG0_OFFSET);

    TmpData = (TmpData & ~ADC_CONFIG0_REF_SEL_MASK) | (RefVoltag << ADC_CONFIG0_REF_SEL_SIZE);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG3_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_SetResolution(AL_REG AdcBaseAddr, AL_ADC_ResolutionSelEnum ResolutionMode)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG0_OFFSET);

    TmpData = (TmpData & ~ADC_CONFIG0_RES_SEL_MASK) | (ResolutionMode << ADC_CONFIG0_RES_SEL_SHIFT);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG3_OFFSET, TmpData);
}

static inline AL_VOID AlAdc_ll_SetInputSingal(AL_REG AdcBaseAddr, AL_ADC_InputSingalEnum InputSingal)
{
    AL_U16 TmpData = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG0_OFFSET);

    TmpData = (TmpData & ~ADC_CONFIG0_DIFF_ENABLE_MASK) | (InputSingal << ADC_CONFIG0_DIFF_ENABLE_SHIFT);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, ADC_CONFIG3_OFFSET, TmpData);
}


/*
* Adc low-level driver through AXI2APB Channel
*/

static inline AL_VOID AlAdc_ll_GpCloseWriteProtection()
{
    AL_REG32_WRITE(PLS_PROT, (AL_REG32_READ(PLS_PROT) & 0xFFFFFFFD));
}

static inline AL_U16 AlAdc_ll_GpGetAdcData(AL_REG BaseAddr, AL_ADC_ChanEnum ChanNum)
{
    return (AL_REG16_READ(BaseAddr +ADC_VC0_OFFSET +  ChanNum * 2) >> 4);
}

static inline AL_U16 AlAdc_ll_GpGetAdcFlag(AL_REG BaseAddr)
{
    return AL_REG16_READ(BaseAddr + ADC_FLAG_OFFSET);
}

static inline AL_VOID AlAdc_ll_GpSetIomuxForChan0(AL_REG BaseAddr, AL_ADC_IoMuxEnum IomuxForChan0)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CHANNEL_0_1_OFFSET, ADC_CHANNEL_0_1_CHANNEL0_SEL_SHIFT, ADC_CHANNEL_0_1_CHANNEL0_SEL_SIZE, IomuxForChan0);
}

static inline AL_VOID AlAdc_ll_GpSetIomuxForChan1(AL_REG BaseAddr, AL_ADC_IoMuxEnum IomuxForChan1)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CHANNEL_0_1_OFFSET, ADC_CHANNEL_0_1_CHANNEL1_SEL_SHIFT, ADC_CHANNEL_0_1_CHANNEL1_SEL_SIZE, IomuxForChan1);
}

static inline AL_VOID AlAdc_ll_GpSetIomuxForChan2(AL_REG BaseAddr, AL_ADC_IoMuxEnum IomuxForChan2)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CHANNEL_2_3_OFFSET, ADC_CHANNEL_2_3_CHANNEL2_SEL_SHIFT, ADC_CHANNEL_2_3_CHANNEL2_SEL_SIZE, IomuxForChan2);
}

static inline AL_VOID AlAdc_ll_GpSetIomuxForChan3(AL_REG BaseAddr, AL_ADC_IoMuxEnum IomuxForChan3)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CHANNEL_2_3_OFFSET, ADC_CHANNEL_2_3_CHANNEL3_SEL_SHIFT, ADC_CHANNEL_2_3_CHANNEL3_SEL_SIZE, IomuxForChan3);
}

static inline AL_VOID AlAdc_ll_GpSetIomuxForChan4(AL_REG BaseAddr, AL_ADC_IoMuxEnum IomuxForChan4)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CHANNEL_4_5_OFFSET, ADC_CHANNEL_4_5_CHANNEL4_SEL_SHIFT, ADC_CHANNEL_4_5_CHANNEL4_SEL_SIZE, IomuxForChan4);
}

static inline AL_VOID AlAdc_ll_GpSetIomuxForChan5(AL_REG BaseAddr, AL_ADC_IoMuxEnum IomuxForChan5)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CHANNEL_4_5_OFFSET, ADC_CHANNEL_4_5_CHANNEL5_SEL_SHIFT, ADC_CHANNEL_4_5_CHANNEL5_SEL_SIZE, IomuxForChan5);
}

static inline AL_VOID AlAdc_ll_GpSetIomuxForChan6(AL_REG BaseAddr, AL_ADC_IoMuxEnum IomuxForChan6)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CHANNEL_6_7_OFFSET, ADC_CHANNEL_6_7_CHANNEL6_SEL_SHIFT, ADC_CHANNEL_6_7_CHANNEL6_SEL_SIZE, IomuxForChan6);
}

static inline AL_VOID AlAdc_ll_GpSetIomuxForChan7(AL_REG BaseAddr, AL_ADC_IoMuxEnum IomuxForChan7)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CHANNEL_6_7_OFFSET, ADC_CHANNEL_6_7_CHANNEL7_SEL_SHIFT, ADC_CHANNEL_6_7_CHANNEL7_SEL_SIZE, IomuxForChan7);
}

static inline AL_VOID AlAdc_ll_GpSetPhyExternalMuxGap(AL_REG BaseAddr, AL_U8 GapData)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CONFIG0_OFFSET, ADC_CONFIG0_PHY_EXTERNAL_MUX_GAP_SHIFT, ADC_CONFIG0_PHY_EXTERNAL_MUX_GAP_SIZE, GapData);
}

static inline AL_VOID AlAdc_ll_GpSetTriggerSignal(AL_REG BaseAddr, AL_ADC_TriggerSignalEnum TriggerSignal)
{
    AL_REG16_SET_BIT(BaseAddr + ADC_CONFIG0_OFFSET, ADC_CONFIG0_REG_SOC_SEL_SHIFT, TriggerSignal);
}

static inline AL_VOID AlAdc_ll_GpEnableAnalogMux(AL_REG BaseAddr, AL_ADC_AllowAnalogMuxEnum IsAllowAnalogMux)
{
    AL_REG16_SET_BIT(BaseAddr + ADC_CONFIG0_OFFSET, ADC_CONFIG0_ANALOG_MUX_EN_SHIFT, IsAllowAnalogMux);
}

static inline AL_VOID AlAdc_ll_GpEnablePhyExternalMux(AL_REG BaseAddr, AL_ADC_PhyExternalMuxEnum MuxMode)
{
    AL_REG16_SET_BIT(BaseAddr + ADC_CONFIG0_OFFSET, ADC_CONFIG0_PHY_EXTERNAL_MUX_SHIFT, MuxMode);
}

static inline AL_VOID AlAdc_ll_GpSetInputSingal(AL_REG BaseAddr, AL_ADC_InputSingalEnum InputSingal)
{
    AL_REG16_SET_BIT(BaseAddr + ADC_CONFIG0_OFFSET, ADC_CONFIG0_DIFF_ENABLE_SHIFT, InputSingal);
}

static inline AL_VOID AlAdc_ll_GpSetResolution(AL_REG BaseAddr, AL_ADC_ResolutionSelEnum ResolutionMode)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CONFIG0_OFFSET, ADC_CONFIG0_RES_SEL_SHIFT, ADC_CONFIG0_RES_SEL_SIZE, ResolutionMode);
}

static inline AL_VOID AlAdc_ll_GpSetRefVoltag(AL_REG BaseAddr, AL_ADC_RefVoltagEnum RefVoltag)
{
    AL_REG16_SET_BIT(BaseAddr + ADC_CONFIG0_OFFSET, ADC_CONFIG0_REF_SEL_SHIFT, RefVoltag);
}

static inline AL_VOID AlAdc_ll_GpSwReset(AL_REG BaseAddr)
{
    AL_REG16_SET_BIT(BaseAddr + ADC_CONFIG0_OFFSET, ADC_CONFIG0_SW_RESET_SHIFT, AL_ADC_SW_RESET);
    AL_REG16_SET_BIT(BaseAddr + ADC_CONFIG0_OFFSET, ADC_CONFIG0_SW_RESET_SHIFT, AL_ADC_SW_RELEASE);
}

static inline AL_VOID AlAdc_ll_GpSetAdcChxLth(AL_REG BaseAddr, AL_ADC_ChanEnum ChanNum, AL_U16 Data)
{
    AL_REG16_WRITE(BaseAddr + ADC_CH0_LTH_OFFSET + ChanNum * 2, Data << 4);
}

static inline AL_U16 AlAdc_ll_GpGetAdcChxLth(AL_REG BaseAddr, AL_ADC_ChanEnum ChanNum)
{
    return AL_REG16_READ(BaseAddr + ADC_CH0_LTH_OFFSET + ChanNum * 2) >> 4;
}

static inline AL_VOID AlAdc_ll_GpSetAdcChxGth(AL_REG BaseAddr, AL_ADC_ChanEnum ChanNum, AL_U16 Data)
{
    AL_REG16_WRITE(BaseAddr + ADC_CH0_GTH_OFFSET + ChanNum * 2, Data << 4);
}

static inline AL_U16 AlAdc_ll_GpGetAdcChxGth(AL_REG BaseAddr, AL_ADC_ChanEnum ChanNum)
{
    return AL_REG16_READ(BaseAddr + ADC_CH0_GTH_OFFSET + ChanNum * 2) >> 4;
}

static inline AL_VOID AlAdc_ll_GpMaskAllChannelIntr(AL_REG BaseAddr)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CONFIG1_OFFSET, ADC_CONFIG1_INTR_DONE_MASK_SHIFT, 4, 0xF);
}

static inline AL_VOID AlAdc_ll_GpClrAllChannelIntr(AL_REG BaseAddr)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CONFIG1_OFFSET, ADC_CONFIG1_INTR_DONE_SHIFT, 4, 0xF);
}

static inline AL_VOID AlAdc_ll_GpMaskIntr(AL_REG BaseAddr, AL_ADC_IntrtypeEnum IntrType, AL_BOOL State)
{
    AL_REG16_SET_BIT(BaseAddr + ADC_CONFIG1_OFFSET, IntrType, State);
}

static inline AL_VOID AlAdc_ll_GpClrIntr(AL_REG BaseAddr, AL_ADC_IntrtypeEnum IntrType)
{
    AL_REG16_SET_BIT(BaseAddr + ADC_CONFIG1_OFFSET, (IntrType + 8), AL_TRUE);
}

static inline AL_U16 AlAdc_ll_GpGetIntrType(AL_REG BaseAddr)
{
    return AL_REG16_GET_BITS(BaseAddr + ADC_CONFIG1_OFFSET, ADC_CONFIG1_INTR_DONE_SHIFT, 4);
}

/* clk_divider for adc_clk 0:adc_clk/1 ; 1:adc_clk/2*/
static inline AL_VOID AlAdc_ll_GpSetAdcPlClkDiv(AL_REG BaseAddr, AL_U8 DivNum)
{
    AL_REG16_WRITE(BaseAddr + ADC_CONFIG2_OFFSET + ADC_CONFIG2_CLK_DIV_SHIFT, DivNum << 8);
}

static inline AL_VOID AlAdc_ll_GpCloseAdcClk(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG16_SET_BIT(BaseAddr + ADC_CONFIG2_OFFSET, ADC_CONFIG1_INTR_GTH_SHIFT, State);
}

static inline AL_VOID AlAdc_ll_GpSetAdcClkSource(AL_REG BaseAddr, AL_ADC_ClkSourceEnum ClkSource)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CONFIG2_OFFSET, ADC_CONFIG2_CLK_SEL_SHIFT, ADC_CONFIG2_CLK_SEL_SIZE, ClkSource);
}

static inline AL_VOID AlAdc_ll_GpPowerDownAdc(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CONFIG2_OFFSET, ADC_CONFIG2_REG_DISLVL_SHIFT, ADC_CONFIG2_CLK_SEL_SIZE, State);
}

static inline AL_VOID AlAdc_ll_GpEnableAdc(AL_REG BaseAddr)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CONFIG2_OFFSET, ADC_CONFIG2_REG_ENABLE_SHIFT, ADC_CONFIG2_REG_DISLVL_SIZE, AL_TRUE);
}

static inline AL_VOID AlAdc_ll_GpDisableAdc(AL_REG BaseAddr)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CONFIG2_OFFSET, ADC_CONFIG2_REG_ENABLE_SHIFT, ADC_CONFIG2_REG_DISLVL_SIZE, AL_FALSE);
}

static inline AL_VOID AlAdc_ll_GpTriggerConv(AL_REG BaseAddr, AL_ADC_TriggerConvEnum State)
{
    AL_REG16_SET_BIT(BaseAddr + ADC_CONFIG3_OFFSET, ADC_CONFIG3_REG_SOC_SHIFT, State);
}

static inline AL_VOID AlAdc_ll_GpSetExternalMuxChannel(AL_REG BaseAddr, AL_U8 Data)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CONFIG3_OFFSET, ADC_CONFIG3_EXTERNAL_CHANNEL_SEL_SHIFT, ADC_CONFIG3_EXTERNAL_CHANNEL_SEL_SIZE, Data);
}

static inline AL_VOID AlAdc_ll_GpSetMaxConvChanNum(AL_REG BaseAddr, AL_U8 Data)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CONFIG3_OFFSET, ADC_CONFIG3_CHANNEL_SEL_SHIFT, ADC_CONFIG3_CHANNEL_SEL_SIZE, Data);
}

static inline AL_VOID AlAdc_ll_GpSetConvMode(AL_REG BaseAddr, AL_ADC_ConvModeSelectEnum ConvMode)
{
    AL_REG16_SET_BITS(BaseAddr + ADC_CONFIG3_OFFSET, ADC_CONFIG3_MODE_SEL_SHIFT, ADC_CONFIG3_MODE_SEL_SIZE, ConvMode);
}

#ifdef __cplusplus
}
#endif

#endif
