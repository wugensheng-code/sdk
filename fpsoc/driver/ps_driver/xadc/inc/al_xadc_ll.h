#ifndef AL_XADC_LL_H
#define AL_XADC_LL_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_xadc_hw.h"

/**************************** Type Definitions ******************************/
typedef enum
{
    AL_XADC_CHANNEL0    = 0,
    AL_XADC_CHANNEL1    = 1,
    AL_XADC_CHANNEL2    = 2,
    AL_XADC_CHANNEL3    = 3,
    AL_XADC_CHANNEL4    = 4,
    AL_XADC_CHANNEL5    = 5,
    AL_XADC_CHANNEL6    = 6,
    AL_XADC_CHANNEL7    = 7
} AL_XADC_ChannelEnum;

typedef enum
{
    AL_XADC_IO_MUX_0     = 0,
    AL_XADC_IO_MUX_1     = 1,
    AL_XADC_IO_MUX_2     = 2,
    AL_XADC_IO_MUX_3     = 3,
    AL_XADC_IO_MUX_4     = 4,
    AL_XADC_IO_MUX_5     = 5,
    AL_XADC_IO_MUX_6     = 6,
    AL_XADC_IO_MUX_7     = 7,
    AL_XADC_IO_MUX_8     = 8,
    AL_XADC_IO_MUX_9     = 9,
    AL_XADC_IO_MUX_10    = 10,
    AL_XADC_IO_MUX_11    = 11,
    AL_XADC_IO_MUX_12    = 12,
    AL_XADC_IO_MUX_13    = 13,
    AL_XADC_IO_MUX_14    = 14,
    AL_XADC_IO_MUX_15    = 15,
    AL_XADC_IO_MUX_16    = 16    /*dedicated*/
} AL_XADC_IoMuxEnum;

typedef enum
{
    AL_XADC_FLAG_DONE        =  0,
    AL_XADC_FLAG_GTH         =  1,
    AL_XADC_FLAG_LTH         =  2,
    AL_XADC_FLAG_ERROR       =  3,
    AL_XADC_FLAG_COMP_VC0    =  8,
    AL_XADC_FLAG_COMP_VC1    =  9,
    AL_XADC_FLAG_COMP_VC2    =  10,
    AL_XADC_FLAG_COMP_VC3    =  11,
    AL_XADC_FLAG_COMP_VC4    =  12,
    AL_XADC_FLAG_COMP_VC5    =  13,
    AL_XADC_FLAG_COMP_VC6    =  14,
    AL_XADC_FLAG_COMP_VC7    =  15
}AL_XADC_FlagEnum;

typedef enum
{
    AL_XADC_REG_ADC_SOC    = 0,
    AL_XADC_PIB_ADC_SOC    = 1
} AL_XADC_TriggerSignalEnum;

typedef enum
{
    AL_XADC_DISALLOW    = 0,
    AL_XADC_ALLOW       = 1
} AL_XADC_AllowAnalogMuxEnum;

typedef enum
{
    AL_XADC_NORMAL_MODE          = 0,
    AL_XADC_PHY_EXTERNAL_MUX     = 1
} AL_XADC_PhyExternalMuxEnum;

typedef enum
{
    AL_XADC_SINGLE_ENDED   = 0,
    AL_XADC_DIFFERENTIAL   = 1,
} AL_XADC_InputSingalEnum;

typedef enum
{
    AL_XADC_6BIT_MODE  = 0,
    AL_XADC_8BIT_MODE  = 1,
    AL_XADC_10BIT_MODE = 2,
    AL_XADC_12BIT_MODE = 3
} AL_XADC_ResolutionSelectlEnum;

typedef enum
{
    AL_XADC_REF_VREF       = 0,
    AL_XADC_REF_INTER_REF  = 1
} AL_XADC_RefVoltagEnum;

typedef enum
{
    AL_XADC_SW_RELEASE = 0,
    AL_XADC_SW_RESET   = 1
} AL_XADC_SwResetEnum;

typedef enum
{
    AL_XADC_CLK_OSC_44MHz       = 0,
    AL_XADC_CLK_PS_50MHz        = 1,
    AL_XADC_CLK_PL_70MHz        = 2
} AL_XADC_ClkSourceEnum;

typedef enum
{
    AL_XADC_END_CONV       = 0,
    AL_XADC_START_CONV     = 1
} AL_XADC_TriggerConvEnum;

typedef enum
{
    AL_XADC_SINGLE_CHANNEL_MODE    = 0,
    AL_XADC_SINGLE_PASS_MODE       = 1,
    AL_XADC_CONTINUE_MODE          = 2
} AL_XADC_ConvModeSelectEnum;

typedef enum
{
    AL_XADC_INTR_DONE     = 0,
    AL_XADC_INTR_GTH      = 1,
    AL_XADC_INTR_LTH      = 2,
    AL_XADC_INTR_ERROR    = 3
}AL_XADC_IntrtypeEnum;

#define XADC_READ_CMD   0x78
#define XADC_WRITE_CMD  0x77

/***************** Macros (Inline Functions) Definitions ********************/

static inline AL_VOID AlXadc_ll_XadcIntr(AL_REG XadcBaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(XadcBaseAddr + XADC_INTR_OFFSET, XADC_INTR_INTR_SHIFT, State);
}

static inline AL_VOID AlXadc_ll_MaskXadcIntr(AL_REG XadcBaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(XadcBaseAddr + XADC_INTR_MASK_OFFSET, XADC_INTR_MASK_M_INTR_SHIFT, State);
}

static inline AL_VOID AlXadc_ll_GpCloseWriteProtection()
{
    AL_REG32_WRITE(PLS_PROT, (AL_REG32_READ(PLS_PROT) & 0xFFFFFFFD));
}

static inline AL_U16 AlXadc_ll_GpGetXadcData(AL_REG BaseAddr, AL_XADC_ChannelEnum ChannelNum)
{
    return (AL_REG16_READ(BaseAddr +XADC_VC0_OFFSET +  ChannelNum * 2) >> 4);

}

static inline AL_U16 AlXadc_ll_GpGetXadcFlag(AL_REG BaseAddr)
{
    return AL_REG16_READ(BaseAddr + XADC_FLAG_OFFSET);
}

static inline AL_VOID AlXadc_ll_GpSetIomuxForChannel0(AL_REG BaseAddr, AL_XADC_IoMuxEnum IomuxForChannel0)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CHANNEL_0_1_OFFSET, XADC_CHANNEL_0_1_CHANNEL0_SEL_SHIFT, XADC_CHANNEL_0_1_CHANNEL0_SEL_SIZE, IomuxForChannel0);
}

static inline AL_VOID AlXadc_ll_GpSetIomuxForChannel1(AL_REG BaseAddr, AL_XADC_IoMuxEnum IomuxForChannel1)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CHANNEL_0_1_OFFSET, XADC_CHANNEL_0_1_CHANNEL1_SEL_SHIFT, XADC_CHANNEL_0_1_CHANNEL1_SEL_SIZE, IomuxForChannel1);
}

static inline AL_VOID AlXadc_ll_GpSetIomuxForChannel2(AL_REG BaseAddr, AL_XADC_IoMuxEnum IomuxForChannel2)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CHANNEL_2_3_OFFSET, XADC_CHANNEL_2_3_CHANNEL2_SEL_SHIFT, XADC_CHANNEL_2_3_CHANNEL2_SEL_SIZE, IomuxForChannel2);
}

static inline AL_VOID AlXadc_ll_GpSetIomuxForChannel3(AL_REG BaseAddr, AL_XADC_IoMuxEnum IomuxForChannel3)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CHANNEL_2_3_OFFSET, XADC_CHANNEL_2_3_CHANNEL3_SEL_SHIFT, XADC_CHANNEL_2_3_CHANNEL3_SEL_SIZE, IomuxForChannel3);
}

static inline AL_VOID AlXadc_ll_GpSetIomuxForChannel4(AL_REG BaseAddr, AL_XADC_IoMuxEnum IomuxForChannel4)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CHANNEL_4_5_OFFSET, XADC_CHANNEL_4_5_CHANNEL4_SEL_SHIFT, XADC_CHANNEL_4_5_CHANNEL4_SEL_SIZE, IomuxForChannel4);
}

static inline AL_VOID AlXadc_ll_GpSetIomuxForChannel5(AL_REG BaseAddr, AL_XADC_IoMuxEnum IomuxForChannel5)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CHANNEL_4_5_OFFSET, XADC_CHANNEL_4_5_CHANNEL5_SEL_SHIFT, XADC_CHANNEL_4_5_CHANNEL5_SEL_SIZE, IomuxForChannel5);
}

static inline AL_VOID AlXadc_ll_GpSetIomuxForChannel6(AL_REG BaseAddr, AL_XADC_IoMuxEnum IomuxForChannel6)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CHANNEL_6_7_OFFSET, XADC_CHANNEL_6_7_CHANNEL6_SEL_SHIFT, XADC_CHANNEL_6_7_CHANNEL6_SEL_SIZE, IomuxForChannel6);
}

static inline AL_VOID AlXadc_ll_GpSetIomuxForChannel7(AL_REG BaseAddr, AL_XADC_IoMuxEnum IomuxForChannel7)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CHANNEL_6_7_OFFSET, XADC_CHANNEL_6_7_CHANNEL7_SEL_SHIFT, XADC_CHANNEL_6_7_CHANNEL7_SEL_SIZE, IomuxForChannel7);
}

static inline AL_VOID AlXadc_ll_GpSetPhyExternalMuxGap(AL_REG BaseAddr, AL_U8 GapData)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CONFIG0_OFFSET, XADC_CONFIG0_PHY_EXTERNAL_MUX_GAP_SHIFT, XADC_CONFIG0_PHY_EXTERNAL_MUX_GAP_SIZE, GapData);
}

static inline AL_VOID AlXadc_ll_GpSetTriggerSignal(AL_REG BaseAddr, AL_XADC_TriggerSignalEnum TriggerSignal)
{
    AL_REG16_SET_BIT(BaseAddr + XADC_CONFIG0_OFFSET, XADC_CONFIG0_REG_SOC_SEL_SHIFT, TriggerSignal);
}

static inline AL_VOID AlXadc_ll_GpEnableAnalogMux(AL_REG BaseAddr, AL_XADC_AllowAnalogMuxEnum IsAllowAnalogMux)
{
    AL_REG16_SET_BIT(BaseAddr + XADC_CONFIG0_OFFSET, XADC_CONFIG0_ANALOG_MUX_EN_SHIFT, IsAllowAnalogMux);
}

static inline AL_VOID AlXadc_ll_GpEnablePhyExternalMux(AL_REG BaseAddr, AL_XADC_PhyExternalMuxEnum MuxMode)
{
    AL_REG16_SET_BIT(BaseAddr + XADC_CONFIG0_OFFSET, XADC_CONFIG0_PHY_EXTERNAL_MUX_SHIFT, MuxMode);
}

static inline AL_VOID AlXadc_ll_GpSetInputSingal(AL_REG BaseAddr, AL_XADC_InputSingalEnum InputSingal)
{
    AL_REG16_SET_BIT(BaseAddr + XADC_CONFIG0_OFFSET, XADC_CONFIG0_DIFF_ENABLE_SHIFT, InputSingal);
}

static inline AL_VOID AlXadc_ll_GpSetResolution(AL_REG BaseAddr, AL_XADC_ResolutionSelectlEnum ResolutionMode)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CONFIG0_OFFSET, XADC_CONFIG0_RES_SEL_SHIFT, XADC_CONFIG0_RES_SEL_SIZE, ResolutionMode);
}

static inline AL_VOID AlXadc_ll_GpSetRefVoltag(AL_REG BaseAddr, AL_XADC_RefVoltagEnum RefVoltag)
{
    AL_REG16_SET_BIT(BaseAddr + XADC_CONFIG0_OFFSET, XADC_CONFIG0_REF_SEL_SHIFT, RefVoltag);
}

static inline AL_VOID AlXadc_ll_GpSwReset(AL_REG BaseAddr)
{
    AL_REG16_SET_BIT(BaseAddr + XADC_CONFIG0_OFFSET, XADC_CONFIG0_SW_RESET_SHIFT, AL_XADC_SW_RESET);
    AL_REG16_SET_BIT(BaseAddr + XADC_CONFIG0_OFFSET, XADC_CONFIG0_SW_RESET_SHIFT, AL_XADC_SW_RELEASE);
}

static inline AL_BOOL AlXadc_ll_GpReadSwReset(AL_REG BaseAddr)
{
    AL_REG16_GET_BIT(BaseAddr + XADC_CONFIG0_OFFSET, XADC_CONFIG0_SW_RESET_SHIFT);
}

static inline AL_VOID AlXadc_ll_GpSetXadcChxLth(AL_REG BaseAddr, AL_XADC_ChannelEnum ChannelNum, AL_U16 Data)
{
    AL_REG16_WRITE(BaseAddr + XADC_CH0_LTH_OFFSET + ChannelNum * 2, Data << 4);
}

static inline AL_U16 AlXadc_ll_GpGetXadcChxLth(AL_REG BaseAddr, AL_XADC_ChannelEnum ChannelNum)
{
    return AL_REG16_READ(BaseAddr + XADC_CH0_LTH_OFFSET + ChannelNum * 2) >> 4;
}

static inline AL_VOID AlXadc_ll_GpSetXadcChxGth(AL_REG BaseAddr, AL_XADC_ChannelEnum ChannelNum, AL_U16 Data)
{
    AL_REG16_WRITE(BaseAddr + XADC_CH0_GTH_OFFSET + ChannelNum * 2, Data << 4);
}

static inline AL_U16 AlXadc_ll_GpGetXadcChxGth(AL_REG BaseAddr, AL_XADC_ChannelEnum ChannelNum)
{
    return AL_REG16_READ(BaseAddr + XADC_CH0_GTH_OFFSET + ChannelNum * 2) >> 4;
}

static inline AL_VOID AlXadc_ll_GpMaskAllChannelIntr(AL_REG BaseAddr)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CONFIG1_OFFSET, XADC_CONFIG1_INTR_DONE_MASK_SHIFT, 4, 0xF);
}

static inline AL_VOID AlXadc_ll_GpClrAllChannelIntr(AL_REG BaseAddr)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CONFIG1_OFFSET, XADC_CONFIG1_INTR_DONE_SHIFT, 4, 0xF);
}

static inline AL_VOID AlXadc_ll_GpMaskIntr(AL_REG BaseAddr, AL_XADC_IntrtypeEnum IntrType, AL_BOOL State)
{
    AL_REG16_SET_BIT(BaseAddr + XADC_CONFIG1_OFFSET, IntrType, State);
}

static inline AL_VOID AlXadc_ll_GpClrIntr(AL_REG BaseAddr, AL_XADC_IntrtypeEnum IntrType)
{
    AL_REG16_SET_BIT(BaseAddr + XADC_CONFIG1_OFFSET, (IntrType + 8), AL_TRUE);
}

static inline AL_U16 AlXadc_ll_GpGetIntrType(AL_REG BaseAddr)
{
    return AL_REG16_GET_BITS(BaseAddr + XADC_CONFIG1_OFFSET, XADC_CONFIG1_INTR_DONE_SHIFT, 4);
}

/* clk_divider for adc_clk 0:adc_clk/1 ; 1:adc_clk/2*/
static inline AL_VOID AlXadc_ll_GpSetAdcPlClkDiv(AL_REG BaseAddr, AL_U8 DivNum)
{
    AL_REG16_WRITE(BaseAddr + XADC_CONFIG2_OFFSET + XADC_CONFIG2_CLK_DIV_SHIFT, DivNum << 8);
}

static inline AL_VOID AlXadc_ll_GpCloseAdcClk(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG16_SET_BIT(BaseAddr + XADC_CONFIG2_OFFSET, XADC_CONFIG1_INTR_GTH_SHIFT, State);
}

static inline AL_VOID AlXadc_ll_GpSetAdcClkSource(AL_REG BaseAddr, AL_XADC_ClkSourceEnum ClkSource)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CONFIG2_OFFSET, XADC_CONFIG2_CLK_SEL_SHIFT, XADC_CONFIG2_CLK_SEL_SIZE, ClkSource);
}

static inline AL_VOID AlXadc_ll_GpPowerDownAdc(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CONFIG2_OFFSET, XADC_CONFIG2_REG_DISLVL_SHIFT, XADC_CONFIG2_CLK_SEL_SIZE, State);
}

static inline AL_VOID AlXadc_ll_GpEnableAdc(AL_REG BaseAddr)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CONFIG2_OFFSET, XADC_CONFIG2_REG_ENABLE_SHIFT, XADC_CONFIG2_REG_DISLVL_SIZE, AL_TRUE);
}

static inline AL_VOID AlXadc_ll_GpDisableAdc(AL_REG BaseAddr)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CONFIG2_OFFSET, XADC_CONFIG2_REG_ENABLE_SHIFT, XADC_CONFIG2_REG_DISLVL_SIZE, AL_FALSE);
}

static inline AL_VOID AlXadc_ll_GpTriggerConv(AL_REG BaseAddr, AL_XADC_TriggerConvEnum State)
{
    AL_REG16_SET_BIT(BaseAddr + XADC_CONFIG3_OFFSET, XADC_CONFIG3_REG_SOC_SHIFT, State);
}

static inline AL_VOID AlXadc_ll_GpSetExternalMuxChannel(AL_REG BaseAddr, AL_U8 Data)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CONFIG3_OFFSET, XADC_CONFIG3_EXTERNAL_CHANNEL_SEL_SHIFT, XADC_CONFIG3_EXTERNAL_CHANNEL_SEL_SIZE, Data);
}

static inline AL_VOID AlXadc_ll_GpSetMaxConvChannelNum(AL_REG BaseAddr, AL_U8 Data)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CONFIG3_OFFSET, XADC_CONFIG3_CHANNEL_SEL_SHIFT, XADC_CONFIG3_CHANNEL_SEL_SIZE, Data);
}

static inline AL_VOID AlXadc_ll_GpSetConvMode(AL_REG BaseAddr, AL_XADC_ConvModeSelectEnum ConvMode)
{
    AL_REG16_SET_BITS(BaseAddr + XADC_CONFIG3_OFFSET, XADC_CONFIG3_MODE_SEL_SHIFT, XADC_CONFIG3_MODE_SEL_SIZE, ConvMode);
}

/*
* This function is used for writing to PL-XADC Registers using the Command FIFO.
*/
static inline AL_VOID AlXadc_ll_WritePlXadcReg(AL_REG XadcBaseAddr, AL_U8 Addr, AL_U16 Data)
{
    AL_REG32_WRITE(XadcBaseAddr + XADC_CMD_FIFO_OFFSET, XADC_WRITE_CMD << 24 | Addr << 16 | Data);
}

/*
* This function is used for reading to PL-XADC Registers using the Data FIFO.
*/
static inline AL_U32 AlXadc_ll_ReadPlXadcReg(AL_REG XadcBaseAddr, AL_U8 Addr)
{
    AL_REG32_WRITE(XadcBaseAddr + XADC_CMD_FIFO_OFFSET, XADC_READ_CMD << 24 | Addr << 16 | 0x0000);
    while(AL_REG32_READ(XadcBaseAddr + XADC_MSTS_OFFSET) & 0x100);
    return (AL_U32)(AL_REG32_READ(XadcBaseAddr + XADC_DATA_FIFO_OFFSET));
}

#ifdef __cplusplus
}
#endif

#endif
