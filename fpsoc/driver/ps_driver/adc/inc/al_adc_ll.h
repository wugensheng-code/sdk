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
    AL_ADC_PS_TCK_DIV4     = 0x0,
    AL_ADC_PS_TCK_DIV8     = 0x1,
    AL_ADC_PS_TCK_DIV16    = 0x2,
    AL_ADC_PS_TCK_DIV32    = 0x3
}AL_ADC_PsTckRate;

typedef enum
{
    AL_ADC_PLADC_INTR             = 0,
    AL_ADC_DATA_FIFO_LTH_INTR     = 8,
    AL_ADC_CMD_FIFO_LTH_INTR      = 9,
    AL_ADC_DATA_FIFO_GTH_INTR     = 10,
    AL_ADC_CMD_FIFO_GTH_INTR      = 11
}AL_ADC_PsIntrTypeEnum;

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
    AL_ADC_MUX_0       = 0,
    AL_ADC_MUX_1       = 1,
    AL_ADC_MUX_2       = 2,
    AL_ADC_MUX_3       = 3,
    AL_ADC_MUX_4       = 4,
    AL_ADC_MUX_5       = 5,
    AL_ADC_MUX_6       = 6,
    AL_ADC_MUX_7       = 7,
    AL_ADC_MUX_8       = 8,
    AL_ADC_MUX_9       = 9,
    AL_ADC_MUX_10      = 10,
    AL_ADC_MUX_11      = 11,
    AL_ADC_MUX_12      = 12,
    AL_ADC_MUX_13      = 13,
    AL_ADC_MUX_14      = 14,
    AL_ADC_MUX_15      = 15,
    AL_ADC_MUX_VPVN    = 16    /*Dedicated mux*/
} AL_ADC_MuxEnum;

typedef enum
{
    AL_ADC_FLAG_GTH         =  1,
    AL_ADC_FLAG_LTH         =  2,
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
    AL_ADC_CONTINUOUS_MODE        = 2
} AL_ADC_ConvModeSelectEnum;

typedef enum
{
    AL_ADC_PL_INTR_DONE    = 0,
    AL_ADC_PL_INTR_GTH     = 1,
    AL_ADC_PL_INTR_LTH     = 2,
    AL_ADC_PL_INTR_ERROR   = 3
}AL_ADC_PlIntrTypeEnum;

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


#define ADC_WRITE_CMD  0x77
#define ADC_READ_CMD   0x78

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

static inline AL_VOID AlAdc_ll_SetTckRate(AL_REG AdcBaseAddr, AL_ADC_PsTckRate TckRate)
{
    AL_REG32_SET_BITS(AdcBaseAddr + ADC_CFG_OFFSET, ADC_CFG_TCK_RATE_SHIFT, ADC_CFG_TCK_RATE_SIZE, TckRate);
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

static inline AL_VOID AlAdc_ll_ClrAllPsAdcIntr(AL_REG AdcBaseAddr)
{
    AL_REG32_WRITE(AdcBaseAddr + ADC_INTR_OFFSET, 0xFFFFFFFF);
}

static inline AL_VOID AlAdc_ll_ClrPsAdcIntr(AL_REG AdcBaseAddr, AL_ADC_PsIntrTypeEnum IntrType)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_OFFSET, IntrType, AL_TRUE);
}

static inline AL_U32 AlAdc_ll_GetPsAdcIntrType(AL_REG AdcBaseAddr)
{
    return AL_REG32_READ(AdcBaseAddr + ADC_INTR_OFFSET);
}

static inline AL_VOID AlAdc_ll_MaskPsAdcIntr(AL_REG AdcBaseAddr, AL_ADC_PsIntrTypeEnum IntrType, AL_BOOL State)
{
    AL_REG32_SET_BIT(AdcBaseAddr + ADC_INTR_MASK_OFFSET, IntrType, State);
}

static inline AL_BOOL AlAdc_ll_IsAdcIntr(AL_REG AdcBaseAddr)
{
    return AL_REG32_GET_BIT(AdcBaseAddr + ADC_MSTS_OFFSET, ADC_MSTS_INTR_SHIFT);
}

static inline AL_BOOL AlAdc_ll_IsDataFifoEmpty(AL_REG AdcBaseAddr)
{
    return AL_REG32_GET_BIT(AdcBaseAddr + ADC_MSTS_OFFSET, ADC_MSTS_DATA_FIFO_E_SHIFT);
}

static inline AL_BOOL AlAdc_ll_IsDataFifoFull(AL_REG AdcBaseAddr)
{
    return AL_REG32_GET_BIT(AdcBaseAddr + ADC_MSTS_OFFSET, ADC_MSTS_DATA_FIFO_F_SHIFT);
}

static inline AL_BOOL AlAdc_ll_IsCmdFifoEmpty(AL_REG AdcBaseAddr)
{
    return AL_REG32_GET_BIT(AdcBaseAddr + ADC_MSTS_OFFSET, ADC_MSTS_CMD_FIFO_E_SHIFT);
}

static inline AL_BOOL AlAdc_ll_IsCmdFifoFull(AL_REG AdcBaseAddr)
{
    return AL_REG32_GET_BIT(AdcBaseAddr + ADC_MSTS_OFFSET, ADC_MSTS_CMD_FIFO_F_SHIFT);
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

/*
 * This function is used for writing to PL-ADC Registers using the Command FIFO.
 */
static inline AL_VOID AlAdc_ll_WritePlAdcReg(AL_REG AdcBaseAddr, AL_U8 PlRegAddr, AL_U16 Data)
{
    while (AL_REG32_READ(AdcBaseAddr + ADC_MSTS_OFFSET) & ADC_MSTS_R_CMD_FIFO_F_MASK);
    AL_REG32_WRITE(AdcBaseAddr + ADC_CMD_FIFO_OFFSET, ADC_WRITE_CMD << 24 | PlRegAddr << 16 | Data);
    /* Read the data fifo after writing the cmd fifo, ensure that the data is completed */
    (AL_VOID)(AL_REG32_READ(AdcBaseAddr + ADC_DATA_FIFO_OFFSET));
}

/*
 * This function is drain Data FIFO.
 */
static inline AL_VOID AlAdc_ll_DrainDataFifo(AL_REG AdcBaseAddr)
{
    AL_U32 Status = AL_REG32_READ(AdcBaseAddr + ADC_MSTS_OFFSET);

    while(!(Status & ADC_MSTS_R_DATA_FIFO_E_MASK)) {
        AL_REG32_READ(AdcBaseAddr + ADC_DATA_FIFO_OFFSET);
        Status = AL_REG32_READ(AdcBaseAddr + ADC_MSTS_OFFSET);
    }
}

/*
 * This function is used for reading to PL-ADC Registers using the Data FIFO.
 */
static inline AL_U16 AlAdc_ll_ReadPlAdcReg(AL_REG AdcBaseAddr, AL_U8 PlRegAddr)
{
    AlAdc_ll_DrainDataFifo(AdcBaseAddr);

    AL_REG32_WRITE(AdcBaseAddr + ADC_CMD_FIFO_OFFSET, ADC_READ_CMD << 24 | PlRegAddr << 16);
    while (AL_REG32_READ(AdcBaseAddr + ADC_MSTS_OFFSET) & ADC_MSTS_R_DATA_FIFO_E_MASK);
    return (AL_U16)(AL_REG32_READ(AdcBaseAddr + ADC_DATA_FIFO_OFFSET));
}

/*
 * This function is used to update some bits in the PL-ADC Registers
 */
static inline AL_VOID AlAdc_ll_UpdatePlAdcReg(AL_REG AdcBaseAddr,
        AL_U16 PlAdcAddr, AL_U16 Mask, AL_U16 Val)
{
    AL_U16 TmpVal = AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, PlAdcAddr);

    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, PlAdcAddr, (TmpVal & ~Mask) | Val);
}

static inline AL_VOID AlAdc_ll_ResetPlAdc(AL_REG AdcBaseAddr)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG0_OFFSET,
            ADC_CONFIG0_ADC_SW_RESET_MASK, AL_ADC_SW_RESET);
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG0_OFFSET,
            ADC_CONFIG0_ADC_SW_RESET_MASK, AL_ADC_SW_RELEASE);
}

static inline AL_U16 AlAdc_ll_GetAdcData(AL_REG AdcBaseAddr, AL_ADC_ChanEnum ChanNum)
{
    return (AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, (ADC_VC0_OFFSET + ChanNum * 2)) >> 4);
}

static inline AL_U16 AlAdc_ll_GetAdcFlag(AL_REG AdcBaseAddr)
{
    return AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_FLAG_OFFSET);
}

static inline AL_VOID AlAdc_ll_SetMuxForChan0(AL_REG AdcBaseAddr, AL_ADC_MuxEnum MuxForChan0)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CHANNEL_0_1_OFFSET,
            ADC_CHANNEL_0_1_ADC_CHANNEL0_SEL_MASK, MuxForChan0);
}

static inline AL_VOID AlAdc_ll_SetMuxForChan1(AL_REG AdcBaseAddr, AL_ADC_MuxEnum MuxForChan1)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CHANNEL_0_1_OFFSET,
        ADC_CHANNEL_0_1_ADC_CHANNEL1_SEL_MASK, MuxForChan1 << ADC_CHANNEL_0_1_CHANNEL1_SEL_SHIFT);
}

static inline AL_VOID AlAdc_ll_SetMuxForChan2(AL_REG AdcBaseAddr, AL_ADC_MuxEnum MuxForChan2)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CHANNEL_2_3_OFFSET,
            ADC_CHANNEL_2_3_ADC_CHANNEL2_SEL_MASK, MuxForChan2);
}

static inline AL_VOID AlAdc_ll_SetMuxForChan3(AL_REG AdcBaseAddr, AL_ADC_MuxEnum MuxForChan3)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CHANNEL_2_3_OFFSET,
        ADC_CHANNEL_2_3_ADC_CHANNEL3_SEL_MASK, MuxForChan3 << ADC_CHANNEL_2_3_CHANNEL3_SEL_SHIFT);
}

static inline AL_VOID AlAdc_ll_SetMuxForChan4(AL_REG AdcBaseAddr, AL_ADC_MuxEnum MuxForChan4)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CHANNEL_4_5_OFFSET,
            ADC_CHANNEL_4_5_ADC_CHANNEL4_SEL_MASK, MuxForChan4);
}

static inline AL_VOID AlAdc_ll_SetMuxForChan5(AL_REG AdcBaseAddr, AL_ADC_MuxEnum MuxForChan5)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CHANNEL_4_5_OFFSET,
        ADC_CHANNEL_4_5_ADC_CHANNEL5_SEL_MASK, MuxForChan5 << ADC_CHANNEL_4_5_CHANNEL5_SEL_SHIFT);
}

static inline AL_VOID AlAdc_ll_SetMuxForChan6(AL_REG AdcBaseAddr, AL_ADC_MuxEnum MuxForChan6)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CHANNEL_6_7_OFFSET,
            ADC_CHANNEL_6_7_ADC_CHANNEL6_SEL_MASK, MuxForChan6);
}

static inline AL_VOID AlAdc_ll_SetMuxForChan7(AL_REG AdcBaseAddr, AL_ADC_MuxEnum MuxForChan7)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CHANNEL_6_7_OFFSET,
        ADC_CHANNEL_6_7_ADC_CHANNEL7_SEL_MASK, MuxForChan7 << ADC_CHANNEL_6_7_CHANNEL7_SEL_SHIFT);
}

static inline AL_VOID AlAdc_ll_SetAdcChanXLth(AL_REG AdcBaseAddr, AL_ADC_ChanEnum ChanNum, AL_U16 Data)
{
    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, (ADC_CH0_LTH_OFFSET + ChanNum * 2), Data << 4);
}

static inline AL_VOID AlAdc_ll_SetAdcChanXGth(AL_REG AdcBaseAddr, AL_ADC_ChanEnum ChanNum, AL_U16 Data)
{
    AlAdc_ll_WritePlAdcReg(AdcBaseAddr, (ADC_CH0_GTH_OFFSET + ChanNum * 2), Data << 4);
}

static inline AL_VOID AlAdc_ll_MaskAllPlIntr(AL_REG AdcBaseAddr)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET,
        0xf, 0xf << ADC_CONFIG1_INTR_DONE_MASK_SHIFT);
}

static inline AL_VOID AlAdc_ll_MaskPlAdcIntrDone(AL_REG AdcBaseAddr, AL_BOOL State)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET,
        ADC_CONFIG1_INTR_DONE_MASK_MASK, State << ADC_CONFIG1_INTR_DONE_MASK_SHIFT);
}

static inline AL_VOID AlAdc_ll_MaskPlAdcIntrGth(AL_REG AdcBaseAddr, AL_BOOL State)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET,
        ADC_CONFIG1_INTR_GTH_MASK_MASK, State << ADC_CONFIG1_INTR_GTH_MASK_SHIFT);
}

static inline AL_VOID AlAdc_ll_MaskPlAdcIntrLth(AL_REG AdcBaseAddr, AL_BOOL State)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET,
        ADC_CONFIG1_INTR_LTH_MASK_MASK, State << ADC_CONFIG1_INTR_LTH_MASK_SHIFT);
}

static inline AL_VOID AlAdc_ll_MaskPlAdcIntrError(AL_REG AdcBaseAddr, AL_BOOL State)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET,
        ADC_CONFIG1_INTR_ERROR_MASK_MASK, State << ADC_CONFIG1_INTR_ERROR_MASK_SHIFT);
}

static inline AL_VOID AlAdc_ll_ClrPlAdcIntrDone(AL_REG AdcBaseAddr)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET,
        ADC_CONFIG1_INTR_DONE_MASK, AL_TRUE << ADC_CONFIG1_INTR_DONE_SHIFT);
}

static inline AL_VOID AlAdc_ll_ClrPlAdcIntrGth(AL_REG AdcBaseAddr)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET,
        ADC_CONFIG1_INTR_GTH_MASK, AL_TRUE << ADC_CONFIG1_INTR_GTH_SHIFT);
}

static inline AL_VOID AlAdc_ll_ClrPlAdcIntrLth(AL_REG AdcBaseAddr)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET,
        ADC_CONFIG1_INTR_LTH_MASK, AL_TRUE << ADC_CONFIG1_INTR_LTH_SHIFT);
}

static inline AL_VOID AlAdc_ll_ClrPlAdcIntrError(AL_REG AdcBaseAddr)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET,
        ADC_CONFIG1_INTR_ERROR_MASK, AL_TRUE << ADC_CONFIG1_INTR_ERROR_SHIFT);
}

static inline AL_U16 AlAdc_ll_GetPlAdcIntrType(AL_REG AdcBaseAddr)
{
    return (AlAdc_ll_ReadPlAdcReg(AdcBaseAddr, ADC_CONFIG1_OFFSET) >> 8);
}

static inline AL_VOID AlAdc_ll_EnablePlAdc(AL_REG AdcBaseAddr, AL_BOOL State)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG2_OFFSET,
        ADC_CONFIG2_REG_ADC_ENABLE_MASK, State << ADC_CONFIG2_REG_ENABLE_SHIFT);
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

static inline AL_VOID AlAdc_ll_StartConv(AL_REG AdcBaseAddr)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG3_OFFSET,
        ADC_CONFIG3_REG_ADC_SOC_MASK, AL_TRUE << ADC_CONFIG3_REG_SOC_SHIFT);
}

static inline AL_VOID AlAdc_ll_StopConv(AL_REG AdcBaseAddr)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG3_OFFSET,
        ADC_CONFIG3_REG_ADC_SOC_MASK, AL_FALSE << ADC_CONFIG3_REG_SOC_SHIFT);
}

static inline AL_VOID AlAdc_ll_SetRefVoltag(AL_REG AdcBaseAddr, AL_ADC_RefVoltagEnum RefVoltag)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG0_OFFSET,
        ADC_CONFIG0_REF_SEL_MASK, RefVoltag << ADC_CONFIG0_REF_SEL_SHIFT);
}

static inline AL_VOID AlAdc_ll_SetResolution(AL_REG AdcBaseAddr, AL_ADC_ResolutionSelEnum Resolution)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG0_OFFSET,
        ADC_CONFIG0_RES_SEL_MASK, Resolution << ADC_CONFIG0_RES_SEL_SHIFT);
}

static inline AL_VOID AlAdc_ll_SetInputSingal(AL_REG AdcBaseAddr, AL_ADC_InputSingalEnum InputSingal)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG0_OFFSET,
        ADC_CONFIG0_DIFF_ENABLE_MASK, InputSingal << ADC_CONFIG0_DIFF_ENABLE_SHIFT);
}

static inline AL_VOID AlAdc_ll_SetClkGate(AL_REG AdcBaseAddr, AL_U8 State)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG2_OFFSET,
        ADC_CONFIG2_ADC_CLK_GATE_MASK, State << ADC_CONFIG2_CLK_GATE_SHIFT);
}

static inline AL_VOID AlAdc_ll_SetPowerDown(AL_REG AdcBaseAddr, AL_U8 State)
{
    AlAdc_ll_UpdatePlAdcReg(AdcBaseAddr, ADC_CONFIG2_OFFSET,
        ADC_CONFIG2_REG_ADC_DISLVL_MASK, State << ADC_CONFIG2_REG_DISLVL_SHIFT);
}


#ifdef __cplusplus
}
#endif

#endif
