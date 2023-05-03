#ifndef AL_CAN_HW_H
#define AL_CAN_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_type.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

#define AL_CAN_NUM_INSTANCE     (1)

#define CAN_TOP_NS_CFG_CTRL     0xF8800164ULL
#define CAN_CAN0_BASE_ADDR      0xF8408000ULL
#define CAN_CAN1_BASE_ADDR      0xF8409000ULL

///////////////////////////////////////////////////////
// Register: CAN_TOP_NS_CFG_CTRL
// Top Nonsecure Config Contrl
///////////////////////////////////////////////////////
#define CAN_TOP_NS_CFG_CTRL_CAN0_FD_ENABLE__SHIFT    0
#define CAN_TOP_NS_CFG_CTRL_CAN0_DMA_ENABLE__SHIFT   1
#define CAN_TOP_NS_CFG_CTRL_CAN1_FD_ENABLE__SHIFT    8	
#define CAN_TOP_NS_CFG_CTRL_CAN1_DMA_ENABLE__SHIFT   9

#define CAN_TOP_NS_CFG_CTRL_CAN0_FD_ENABLE__SIZE    1
#define CAN_TOP_NS_CFG_CTRL_CAN0_DMA_ENABLE__SIZE   1
#define CAN_TOP_NS_CFG_CTRL_CAN1_FD_ENABLE__SIZE    1
#define CAN_TOP_NS_CFG_CTRL_CAN1_DMA_ENABLE__SIZE   1
///////////////////////////////////////////////////////
// Register: RBUF_0_3
// Receive Buffer Registers
///////////////////////////////////////////////////////

#define CAN__RBUF_0_3__OFFSET 0x00UL
#define CAN__RBUF_0_3__NUM  0x1

#define CAN__RBUF_0_3__ID_7_0__SHIFT    0
#define CAN__RBUF_0_3__ID_15_8__SHIFT    8
#define CAN__RBUF_0_3__ID_23_16__SHIFT    16
#define CAN__RBUF_0_3__ID_28_24__SHIFT    24
#define CAN__RBUF_0_3__RSDV_29_30__SHIFT    29
#define CAN__RBUF_0_3__ESI__SHIFT    31
#define CAN__RBUF_0_3__ID_7_0__SIZE 8
#define CAN__RBUF_0_3__ID_15_8__SIZE 8
#define CAN__RBUF_0_3__ID_23_16__SIZE 8
#define CAN__RBUF_0_3__ID_28_24__SIZE 5
#define CAN__RBUF_0_3__RSDV_29_30__SIZE 2
#define CAN__RBUF_0_3__ESI__SIZE 1

#define CAN__RBUF_0_3__ID__EXT__SIZE 29
#define CAN__RBUF_0_3__ID__STD__SIZE 11


///////////////////////////////////////////////////////
// Register: RBUF_4_7
// TTCAN Stauts and control register
///////////////////////////////////////////////////////

#define CAN__RBUF_4_7__OFFSET 0x04UL
#define CAN__RBUF_4_7__NUM  0x1
#define CAN__RBUF_4_7__DLC__SHIFT    0
#define CAN__RBUF_4_7__BRS__SHIFT    4
#define CAN__RBUF_4_7__FDF__SHIFT    5
#define CAN__RBUF_4_7__RTR__SHIFT    6
#define CAN__RBUF_4_7__IDE__SHIFT    7
#define CAN__RBUF_4_7__RSDV_8_11__SHIFT    8
#define CAN__RBUF_4_7__TX__SHIFT    12
#define CAN__RBUF_4_7__KOER__SHIFT    13
#define CAN__RBUF_4_7__CYCLE_TIME__SHIFT    16

#define CAN__RBUF_4_7__DLC__SIZE 4
#define CAN__RBUF_4_7__BRS__SIZE 1
#define CAN__RBUF_4_7__FDF__SIZE 1
#define CAN__RBUF_4_7__RTR__SIZE 1
#define CAN__RBUF_4_7__IDE__SIZE 1
#define CAN__RBUF_4_7__RSDV_8_11__SIZE 4
#define CAN__RBUF_4_7__TX__SIZE 1
#define CAN__RBUF_4_7__KOER__SIZE 3
#define CAN__RBUF_4_7__CYCLE_TIME__SIZE 16

///////////////////////////////////////////////////////
// Register: RBUF_8_11
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_8_11__OFFSET 0x08UL
#define CAN__RBUF_8_11__NUM  0x1

#define CAN__RBUF_8_11__D1_D4__SHIFT    0
#define CAN__RBUF_8_11__D1_D4__SIZE 32


///////////////////////////////////////////////////////
// Register: RBUF_12_15
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_12_15__OFFSET 0x0CUL
#define CAN__RBUF_12_15__NUM  0x1
#define CAN__RBUF_12_15__D5_D8__SHIFT    0

#define CAN__RBUF_12_15__D5_D8__SIZE 32

///////////////////////////////////////////////////////
// Register: RBUF_16_19
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_16_19__OFFSET 0x10UL
#define CAN__RBUF_16_19__NUM  0x1

#define CAN__RBUF_16_19__D9_D12__SHIFT    0
#define CAN__RBUF_16_19__D9_D12__SIZE 32


///////////////////////////////////////////////////////
// Register: RBUF_20_23
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_20_23__OFFSET 0x14UL
#define CAN__RBUF_20_23__NUM  0x1
#define CAN__RBUF_20_23__D13_D16__SHIFT    0

#define CAN__RBUF_20_23__D13_D16__SIZE 32

///////////////////////////////////////////////////////
// Register: RBUF_24_27
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_24_27__OFFSET 0x18UL
#define CAN__RBUF_24_27__NUM  0x1

#define CAN__RBUF_24_27__D17_D20__SHIFT    0
#define CAN__RBUF_24_27__D17_D20__SIZE 32


///////////////////////////////////////////////////////
// Register: RBUF_28_31
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_28_31__OFFSET 0x1CUL
#define CAN__RBUF_28_31__NUM  0x1
#define CAN__RBUF_28_31__D21_D24__SHIFT    0

#define CAN__RBUF_28_31__D21_D24__SIZE 32

///////////////////////////////////////////////////////
// Register: RBUF_32_35
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_32_35__OFFSET 0x20UL
#define CAN__RBUF_32_35__NUM  0x1

#define CAN__RBUF_32_35__D25_D28__SHIFT    0
#define CAN__RBUF_32_35__D25_D28__SIZE 32


///////////////////////////////////////////////////////
// Register: RBUF_36_39
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_36_39__OFFSET 0x24UL
#define CAN__RBUF_36_39__NUM  0x1
#define CAN__RBUF_36_39__D29_D32__SHIFT    0

#define CAN__RBUF_36_39__D29_D32__SIZE 32

///////////////////////////////////////////////////////
// Register: RBUF_40_43
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_40_43__OFFSET 0x28UL
#define CAN__RBUF_40_43__NUM  0x1

#define CAN__RBUF_40_43__D33_D36__SHIFT    0
#define CAN__RBUF_40_43__D33_D36__SIZE 32


///////////////////////////////////////////////////////
// Register: RBUF_44_47
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_44_47__OFFSET 0x2CUL
#define CAN__RBUF_44_47__NUM  0x1
#define CAN__RBUF_44_47__D37_D40__SHIFT    0

#define CAN__RBUF_44_47__D37_D40__SIZE 32

///////////////////////////////////////////////////////
// Register: RBUF_48_51
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_48_51__OFFSET 0x30UL
#define CAN__RBUF_48_51__NUM  0x1

#define CAN__RBUF_48_51__D41_D44__SHIFT    0
#define CAN__RBUF_48_51__D41_D44__SIZE 32


///////////////////////////////////////////////////////
// Register: RBUF_52_55
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_52_55__OFFSET 0x34UL
#define CAN__RBUF_52_55__NUM  0x1
#define CAN__RBUF_52_55__D45_D48__SHIFT    0

#define CAN__RBUF_52_55__D45_D48__SIZE 32

///////////////////////////////////////////////////////
// Register: RBUF_56_59
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_56_59__OFFSET 0x38UL
#define CAN__RBUF_56_59__NUM  0x1

#define CAN__RBUF_56_59__D49_D52__SHIFT    0
#define CAN__RBUF_56_59__D49_D52__SIZE 32


///////////////////////////////////////////////////////
// Register: RBUF_60_63
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_60_63__OFFSET 0x3CUL
#define CAN__RBUF_60_63__NUM  0x1
#define CAN__RBUF_60_63__D53_D56__SHIFT    0

#define CAN__RBUF_60_63__D53_D56__SIZE 32

///////////////////////////////////////////////////////
// Register: RBUF_64_67
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_64_67__OFFSET 0x40UL
#define CAN__RBUF_64_67__NUM  0x1

#define CAN__RBUF_64_67__D57_D60__SHIFT    0
#define CAN__RBUF_64_67__D57_D60__SIZE 32


///////////////////////////////////////////////////////
// Register: RBUF_68_71
// Data register
///////////////////////////////////////////////////////

#define CAN__RBUF_68_71__OFFSET 0x44UL
#define CAN__RBUF_68_71__NUM  0x1
#define CAN__RBUF_68_71__D61_D64__SHIFT    0

#define CAN__RBUF_68_71__D61_D64__SIZE 32

///////////////////////////////////////////////////////
// Register: RBUF_72_75
// The Reception Time Stamp
// s (RTS) for CiA 603 time
///////////////////////////////////////////////////////

#define CAN__RBUF_72_75__OFFSET 0x48UL
#define CAN__RBUF_72_75__NUM  0x1

#define CAN__RBUF_72_75__RTS_31_0__SHIFT    0
#define CAN__RBUF_72_75__RTS_31_0__SIZE 32


///////////////////////////////////////////////////////
// Register: RBUF_76_79
// The Reception Time Stamp
// s (RTS) for CiA 603 time
///////////////////////////////////////////////////////

#define CAN__RBUF_76_79__OFFSET 0x4CUL
#define CAN__RBUF_76_79__NUM  0x1
#define CAN__RBUF_76_79__RTS_63_32__SHIFT    0

#define CAN__RBUF_76_79__RTS_63_32__SIZE 32

///////////////////////////////////////////////////////
// Register: TBUF_0_3
// Transmit Buffer Registers
///////////////////////////////////////////////////////

#define CAN__TBUF_0_3__OFFSET 0x50UL
#define CAN__TBUF_0_3__NUM  0x1

#define CAN__TBUF_0_3__ID_7_0__SHIFT    0
#define CAN__TBUF_0_3__ID_15_8__SHIFT    8
#define CAN__TBUF_0_3__ID_23_16__SHIFT    16
#define CAN__TBUF_0_3__ID_28_24__SHIFT    24
#define CAN__TBUF_0_3__RSDV_29_30__SHIFT    29
#define CAN__TBUF_0_3__TTSEN__SHIFT    31
#define CAN__TBUF_0_3__ID_7_0__SIZE 8
#define CAN__TBUF_0_3__ID_15_8__SIZE 8
#define CAN__TBUF_0_3__ID_23_16__SIZE 8
#define CAN__TBUF_0_3__ID_28_24__SIZE 5
#define CAN__TBUF_0_3__RSDV_29_30__SIZE 2
#define CAN__TBUF_0_3__TTSEN__SIZE 1


///////////////////////////////////////////////////////
// Register: TBUF_4_7
// Transmit Buffer Registers
///////////////////////////////////////////////////////

#define CAN__TBUF_4_7__OFFSET 0x54UL
#define CAN__TBUF_4_7__NUM  0x1
#define CAN__TBUF_4_7__DLC__SHIFT    0
#define CAN__TBUF_4_7__BRS__SHIFT    4
#define CAN__TBUF_4_7__FDF__SHIFT    5
#define CAN__TBUF_4_7__RTR__SHIFT    6
#define CAN__TBUF_4_7__IDE__SHIFT    7
#define CAN__TBUF_4_7__RSDV_8_31__SHIFT    8

#define CAN__TBUF_4_7__DLC__SIZE 4
#define CAN__TBUF_4_7__BRS__SIZE 1
#define CAN__TBUF_4_7__FDF__SIZE 1
#define CAN__TBUF_4_7__RTR__SIZE 1
#define CAN__TBUF_4_7__IDE__SIZE 1
#define CAN__TBUF_4_7__RSDV_8_31__SIZE 24

///////////////////////////////////////////////////////
// Register: TBUF_8_11
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_8_11__OFFSET 0x58UL
#define CAN__TBUF_8_11__NUM  0x1

#define CAN__TBUF_8_11__D1_D4__SHIFT    0
#define CAN__TBUF_8_11__D1_D4__SIZE 32


///////////////////////////////////////////////////////
// Register: TBUF_12_15
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_12_15__OFFSET 0x5CUL
#define CAN__TBUF_12_15__NUM  0x1
#define CAN__TBUF_12_15__D5_D8__SHIFT    0

#define CAN__TBUF_12_15__D5_D8__SIZE 32

///////////////////////////////////////////////////////
// Register: TBUF_16_19
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_16_19__OFFSET 0x60UL
#define CAN__TBUF_16_19__NUM  0x1

#define CAN__TBUF_16_19__D9_D12__SHIFT    0
#define CAN__TBUF_16_19__D9_D12__SIZE 32


///////////////////////////////////////////////////////
// Register: TBUF_20_23
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_20_23__OFFSET 0x64UL
#define CAN__TBUF_20_23__NUM  0x1
#define CAN__TBUF_20_23__D13_D16__SHIFT    0

#define CAN__TBUF_20_23__D13_D16__SIZE 32

///////////////////////////////////////////////////////
// Register: TBUF_24_27
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_24_27__OFFSET 0x68UL
#define CAN__TBUF_24_27__NUM  0x1

#define CAN__TBUF_24_27__D17_D20__SHIFT    0
#define CAN__TBUF_24_27__D17_D20__SIZE 32


///////////////////////////////////////////////////////
// Register: TBUF_28_31
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_28_31__OFFSET 0x6CUL
#define CAN__TBUF_28_31__NUM  0x1
#define CAN__TBUF_28_31__D21_D24__SHIFT    0

#define CAN__TBUF_28_31__D21_D24__SIZE 32

///////////////////////////////////////////////////////
// Register: TBUF_32_35
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_32_35__OFFSET 0x70UL
#define CAN__TBUF_32_35__NUM  0x1

#define CAN__TBUF_32_35__D25_D28__SHIFT    0
#define CAN__TBUF_32_35__D25_D28__SIZE 32


///////////////////////////////////////////////////////
// Register: TBUF_36_39
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_36_39__OFFSET 0x74UL
#define CAN__TBUF_36_39__NUM  0x1
#define CAN__TBUF_36_39__D29_D32__SHIFT    0

#define CAN__TBUF_36_39__D29_D32__SIZE 32

///////////////////////////////////////////////////////
// Register: TBUF_40_43
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_40_43__OFFSET 0x78UL
#define CAN__TBUF_40_43__NUM  0x1

#define CAN__TBUF_40_43__D33_D36__SHIFT    0
#define CAN__TBUF_40_43__D33_D36__SIZE 32


///////////////////////////////////////////////////////
// Register: TBUF_44_47
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_44_47__OFFSET 0x7CUL
#define CAN__TBUF_44_47__NUM  0x1
#define CAN__TBUF_44_47__D37_D40__SHIFT    0

#define CAN__TBUF_44_47__D37_D40__SIZE 32

///////////////////////////////////////////////////////
// Register: TBUF_48_51
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_48_51__OFFSET 0x80UL
#define CAN__TBUF_48_51__NUM  0x1

#define CAN__TBUF_48_51__D41_D44__SHIFT    0
#define CAN__TBUF_48_51__D41_D44__SIZE 32


///////////////////////////////////////////////////////
// Register: TBUF_52_55
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_52_55__OFFSET 0x84UL
#define CAN__TBUF_52_55__NUM  0x1
#define CAN__TBUF_52_55__D45_D48__SHIFT    0

#define CAN__TBUF_52_55__D45_D48__SIZE 32

///////////////////////////////////////////////////////
// Register: TBUF_56_59
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_56_59__OFFSET 0x88UL
#define CAN__TBUF_56_59__NUM  0x1

#define CAN__TBUF_56_59__D49_D52__SHIFT    0
#define CAN__TBUF_56_59__D49_D52__SIZE 32


///////////////////////////////////////////////////////
// Register: TBUF_60_63
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_60_63__OFFSET 0x8CUL
#define CAN__TBUF_60_63__NUM  0x1
#define CAN__TBUF_60_63__D53_D56__SHIFT    0

#define CAN__TBUF_60_63__D53_D56__SIZE 32

///////////////////////////////////////////////////////
// Register: TBUF_64_67
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_64_67__OFFSET 0x90UL
#define CAN__TBUF_64_67__NUM  0x1

#define CAN__TBUF_64_67__D57_D60__SHIFT    0
#define CAN__TBUF_64_67__D57_D60__SIZE 32


///////////////////////////////////////////////////////
// Register: TBUF_68_71
// Data register
///////////////////////////////////////////////////////

#define CAN__TBUF_68_71__OFFSET 0x94UL
#define CAN__TBUF_68_71__NUM  0x1
#define CAN__TBUF_68_71__D61_D64__SHIFT    0

#define CAN__TBUF_68_71__D61_D64__SIZE 32

///////////////////////////////////////////////////////
// Register: TTS
// Transmission Time Stamp
///////////////////////////////////////////////////////

#define CAN__TTS__OFFSET 0x98UL
#define CAN__TTS__NUM  0x2

#define CAN__TTS__TTS__SHIFT    0
#define CAN__TTS__TTS__SIZE 32


///////////////////////////////////////////////////////
// Register: RCTRL__TCTRL__TCMD__CFG_STAT
// (Receive Control Register RCTRL)
// (Transmit Control Register)
// (Command Register)
// (Configuration and stuatus register)
///////////////////////////////////////////////////////

#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__OFFSET 0xA0UL
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__NUM  0x1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__BUSOFF__SHIFT    0
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RSDV_1__SHIFT    1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RSDV_2__SHIFT    2
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSSS__SHIFT    3
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TPSS__SHIFT    4
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__LBMI__SHIFT    5
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__LBME__SHIFT    6
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RESET__SHIFT    7
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSA__SHIFT    8
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSALL__SHIFT    9
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSONE__SHIFT    10
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TPA__SHIFT    11
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TPE__SHIFT    12
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__STBY__SHIFT    13
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__LOM__SHIFT    14
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TBSEL__SHIFT    15
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSSTAT__SHIFT    16
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RSDV_18_19__SHIFT    18
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TTTBM__SHIFT    20
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSMODE__SHIFT    21
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSNEXT__SHIFT    22
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__FD_ISO__SHIFT    23
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RSTAT__SHIFT    24
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RSDV_26__SHIFT    26
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RBALL__SHIFT    27
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RREL__SHIFT    28
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__ROV__SHIFT    29
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__ROM__SHIFT    30
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__SACK__SHIFT    31

#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__BUSOFF__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RSDV_1__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RSDV_2__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSSS__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TPSS__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__LBMI__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__LBME__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RESET__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSA__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSALL__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSONE__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TPA__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TPE__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__STBY__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__LOM__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TBSEL__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSSTAT__SIZE 2
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RSDV_18_19__SIZE 2
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TTTBM__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSMODE__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__TSNEXT__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__FD_ISO__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RSTAT__SIZE 2
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RSDV_26__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RBALL__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__RREL__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__ROV__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__ROM__SIZE 1
#define CAN__RCTRL__TCTRL__TCMD__CFG_STAT__SACK__SIZE 1

///////////////////////////////////////////////////////
// Register: LIMIT__ERRINT__RTIF__RTIE
// (Warning Limits Register)
// (ERRor INTerrupt Enable and Flag Register )
// (Receive and Transmit Interrupt Flag Register RTIF)
// (Receive and Transmit Interrupt Enable Register)
///////////////////////////////////////////////////////

#define CAN__LIMIT__ERRINT__RTIF__RTIE__OFFSET 0xA4UL
#define CAN__LIMIT__ERRINT__RTIF__RTIE__NUM  0x1

#define CAN__LIMIT__ERRINT__RTIF__RTIE__TSFF__SHIFT    0
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EIE__SHIFT    1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__TSIE__SHIFT    2
#define CAN__LIMIT__ERRINT__RTIF__RTIE__TPIE__SHIFT    3
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RAFIE__SHIFT    4
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RFIE__SHIFT    5
#define CAN__LIMIT__ERRINT__RTIF__RTIE__ROIE__SHIFT    6
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RIE__SHIFT    7
#define CAN__LIMIT__ERRINT__RTIF__RTIE__AIF__SHIFT    8
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EIF__SHIFT    9
#define CAN__LIMIT__ERRINT__RTIF__RTIE__TSIF__SHIFT    10
#define CAN__LIMIT__ERRINT__RTIF__RTIE__TPIF__SHIFT    11
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RAFIF__SHIFT    12
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RFIF__SHIFT    13
#define CAN__LIMIT__ERRINT__RTIF__RTIE__ROIF__SHIFT    14
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RIF__SHIFT    15
#define CAN__LIMIT__ERRINT__RTIF__RTIE__BEIF__SHIFT    16
#define CAN__LIMIT__ERRINT__RTIF__RTIE__BEIE__SHIFT    17
#define CAN__LIMIT__ERRINT__RTIF__RTIE__ALIF__SHIFT    18
#define CAN__LIMIT__ERRINT__RTIF__RTIE__ALIE__SHIFT    19
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EPIF__SHIFT    20
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EPIE__SHIFT    21
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EPASS__SHIFT    22
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EWARN__SHIFT    23
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EWL__SHIFT    24
#define CAN__LIMIT__ERRINT__RTIF__RTIE__AFWL__SHIFT    28
#define CAN__LIMIT__ERRINT__RTIF__RTIE__TSFF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EIE__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__TSIE__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__TPIE__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RAFIE__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RFIE__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__ROIE__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RIE__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__AIF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EIF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__TSIF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__TPIF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RAFIF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RFIF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__ROIF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__RIF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__BEIF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__BEIE__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__ALIF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__ALIE__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EPIF__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EPIE__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EPASS__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EWARN__SIZE 1
#define CAN__LIMIT__ERRINT__RTIF__RTIE__EWL__SIZE 4
#define CAN__LIMIT__ERRINT__RTIF__RTIE__AFWL__SIZE 4

#define CAN__LIMIT__ERRINT__RTIF__RTIE__ALLINTR_MASK 0x0015FF00


///////////////////////////////////////////////////////
// Register: S_PRESC__S_SJW__S_Seg_2__S_Seg_1
// (Prescaler Registers)
// (Bit Timing Register)
// (Bit Timing Register 2)
// (Bit Timing Register 1)
///////////////////////////////////////////////////////

#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__OFFSET 0xA8UL
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__NUM  0x1
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__S_SEG_1__SHIFT    0
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__S_SEG_2__SHIFT    8
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__REDV_15__SHIFT    15
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__S_SJW__SHIFT    16
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__REDV_23__SHIFT    23
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__S_PRESC__SHIFT    24

#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__S_SEG_1__SIZE 8
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__S_SEG_2__SIZE 7
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__REDV_15__SIZE 1
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__S_SJW__SIZE 7
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__REDV_23__SIZE 1
#define CAN__S_PRESC__S_SJW__S_SEG_2__S_SEG_1__S_PRESC__SIZE 8

///////////////////////////////////////////////////////
// Register: F_PRESC__F_SJW__F_Seg_2__F_Seg_1
// (Prescaler Registers)
// (Bit Timing Register)
// (Bit Timing Register 2)
// (Bit Timing Register 1)
///////////////////////////////////////////////////////

#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__OFFSET 0xACUL
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__NUM  0x1

#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__F_SEG_1__SHIFT    0
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__REDV_5_7__SHIFT    5
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__F_SEG_2__SHIFT    8
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__REDV_12_15__SHIFT    12
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__F_SJW__SHIFT    16
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__REDV_20_23__SHIFT    20
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__F_PRESC__SHIFT    24
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__F_SEG_1__SIZE 5
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__REDV_5_7__SIZE 3
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__F_SEG_2__SIZE 4
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__REDV_12_15__SIZE 4
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__F_SJW__SIZE 4
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__REDV_20_23__SIZE 4
#define CAN__F_PRESC__F_SJW__F_SEG_2__F_SEG_1__F_PRESC__SIZE 8


///////////////////////////////////////////////////////
// Register: TECNT__REVNT__TDC__EALCAP
// (Error Counter Registers)
// (Error Counter Registers)
// (Transmitter Delay Compensation Register)
// (Error and Arbitration Lost Capture Register EALCAP)
///////////////////////////////////////////////////////

#define CAN__TECNT__REVNT__TDC__EALCAP__OFFSET 0xB0UL
#define CAN__TECNT__REVNT__TDC__EALCAP__NUM  0x1
#define CAN__TECNT__REVNT__TDC__EALCAP__ALC__SHIFT    0
#define CAN__TECNT__REVNT__TDC__EALCAP__KOER__SHIFT    5
#define CAN__TECNT__REVNT__TDC__EALCAP__SSPOFF__SHIFT    8
#define CAN__TECNT__REVNT__TDC__EALCAP__TDCEN__SHIFT    15
#define CAN__TECNT__REVNT__TDC__EALCAP__RECNT__SHIFT    16
#define CAN__TECNT__REVNT__TDC__EALCAP__TECNT__SHIFT    24

#define CAN__TECNT__REVNT__TDC__EALCAP__ALC__SIZE 5
#define CAN__TECNT__REVNT__TDC__EALCAP__KOER__SIZE 3
#define CAN__TECNT__REVNT__TDC__EALCAP__SSPOFF__SIZE 7
#define CAN__TECNT__REVNT__TDC__EALCAP__TDCEN__SIZE 1
#define CAN__TECNT__REVNT__TDC__EALCAP__RECNT__SIZE 8
#define CAN__TECNT__REVNT__TDC__EALCAP__TECNT__SIZE 8

///////////////////////////////////////////////////////
// Register: ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL
// (Acceptance Filter Enable)
// (Acceptance filter Enable)
// (CiA 603 Time Stamping TIMECFG)
// (Acceptance Filter Control Register)
///////////////////////////////////////////////////////

#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__OFFSET 0xB4UL
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__NUM  0x1

#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__ACFADR__SHIFT    0
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__RSDV_4__SHIFT    4
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__SELMASK__SHIFT    5
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__RSDV_6_7__SHIFT    6
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__TIMEEN__SHIFT    8
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__TIMEPOS__SHIFT    9
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__RSDV_10_15__SHIFT    10
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_0__SHIFT    16
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_1__SHIFT    17
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_2__SHIFT    18
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_3__SHIFT    19
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_4__SHIFT    20
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_5__SHIFT    21
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_6__SHIFT    22
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_7__SHIFT    23
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_8__SHIFT    24
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_9__SHIFT    25
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_10__SHIFT    26
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_11__SHIFT    27
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_12__SHIFT    28
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_13__SHIFT    29
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_14__SHIFT    30
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_15__SHIFT    31
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__ACFADR__SIZE 4
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__RSDV_4__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__SELSIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__RSDV_6_7__SIZE 2
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__TIMEEN__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__TIMEPOS__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__RSDV_10_15__SIZE 6
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_0__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_1__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_2__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_3__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_4__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_5__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_6__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_7__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_8__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_9__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_10__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_11__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_12__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_13__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_14__SIZE 1
#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_15__SIZE 1

#define CAN__ACF_EN_1__ACF_EN_0__TIMECFG__ACFCTRL__AE_X__SIZE 3


///////////////////////////////////////////////////////
// Register: ACF_3__ACF_2__ACF_1__ACF_0
// (Bits in Register AC F_3 , if SELMASK=1)
// Acceptance CODE ACODE_x
// Acceptance MASK AMASK_x
///////////////////////////////////////////////////////

#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__OFFSET 0xB8UL
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__NUM  0x1
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_7_0__SHIFT    0
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_15_8__SHIFT    8
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_23_16__SHIFT    16
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_28_24__SHIFT    24
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__AIDE__SHIFT    29
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__AIDEE__SHIFT    30
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__RSDV_31__SHIFT    31

#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_7_0__SIZE 8
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_15_8__SIZE 8
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_23_16__SIZE 8
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_28_24__SIZE 5
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__AIDE__SIZE 1
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__AIDEE__SIZE 1
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__RSDV_31__SIZE 1

#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_7_0__MASK    0x000000ff
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_15_8__MASK    0x0000ff00
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_23_16__MASK    0x00ff0000
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__ACODE_AMASK_28_24__MASK    0x1f000000
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__AIDE__MASK    0x20000000
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__AIDEE__MASK    0x40000000
#define CAN__ACF_3__ACF_2__ACF_1__ACF_0__RSDV_31__MASK    0x80000000


///////////////////////////////////////////////////////
// Register: TTCFG__TBSLOT__VER_1__VER_0
// (Time Trigger Configuration)
// (TT CAN: TB Slot Pointer TBSLOT)
// (Version Information VER_1)
// (Version Information VER_0)
///////////////////////////////////////////////////////

#define CAN__TTCFG__TBSLOT__VER_1__VER_0__OFFSET 0xBCUL
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__NUM  0x1

#define CAN__TTCFG__TBSLOT__VER_1__VER_0__VER_1__VER_0__SHIFT    0
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TBPTR__SHIFT    16
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TBF__SHIFT    22
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TBE__SHIFT    23
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TTEN__SHIFT    24
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__T_PRESC__SHIFT    25
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TTIF__SHIFT    27
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TTIE__SHIFT    28
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TEIF__SHIFT    29
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__WTIF__SHIFT    30
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__WTIE__SHIFT    31
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__VER_1__VER_0__SIZE 16
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TBPTR__SIZE 6
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TBF__SIZE 1
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TBE__SIZE 1
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TTEN__SIZE 1
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__T_PRESC__SIZE 2
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TTIF__SIZE 1
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TTIE__SIZE 1
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__TEIF__SIZE 1
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__WTIF__SIZE 1
#define CAN__TTCFG__TBSLOT__VER_1__VER_0__WTIE__SIZE 1


///////////////////////////////////////////////////////
// Register: REF_MSG
// TTCAN: Reference Mes sage REF_MSG_0 to REF_MSG_ 3
///////////////////////////////////////////////////////

#define CAN__REF_MSG__OFFSET 0xC0UL
#define CAN__REF_MSG__NUM  0x1
#define CAN__REF_MSG__REF_ID__SHIFT    0
#define CAN__REF_MSG__RSDV_29_30__SHIFT    29
#define CAN__REF_MSG__REF_IDE__SHIFT    31

#define CAN__REF_MSG__REF_ID__SIZE 29
#define CAN__REF_MSG__RSDV_29_30__SIZE 2
#define CAN__REF_MSG__REF_IDE__SIZE 1

///////////////////////////////////////////////////////
// Register: TT_TRIG__TRIG_CFG
// (TTCAN: Trigger Time TT_TRIG_0 and TT_TRIG_1 )
// (TTCAN: Trigger Configuration TRIG_CFG_1)
///////////////////////////////////////////////////////

#define CAN__TT_TRIG__TRIG_CFG__OFFSET 0xC4UL
#define CAN__TT_TRIG__TRIG_CFG__NUM  0x1

#define CAN__TT_TRIG__TRIG_CFG__TTPTR__SHIFT    0
#define CAN__TT_TRIG__TRIG_CFG__RSDV_6_7__SHIFT    6
#define CAN__TT_TRIG__TRIG_CFG__TTYPE__SHIFT    8
#define CAN__TT_TRIG__TRIG_CFG__RSDV_11__SHIFT    11
#define CAN__TT_TRIG__TRIG_CFG__TEW_3_0__SHIFT    12
#define CAN__TT_TRIG__TRIG_CFG__TT_TRIG__SHIFT    16
#define CAN__TT_TRIG__TRIG_CFG__TTPTR__SIZE 6
#define CAN__TT_TRIG__TRIG_CFG__RSDV_6_7__SIZE 2
#define CAN__TT_TRIG__TRIG_CFG__TTYPE__SIZE 3
#define CAN__TT_TRIG__TRIG_CFG__RSDV_11__SIZE 1
#define CAN__TT_TRIG__TRIG_CFG__TEW_3_0__SIZE 4
#define CAN__TT_TRIG__TRIG_CFG__TT_TRIG__SIZE 16


///////////////////////////////////////////////////////
// Register: TT_WTRIG
// TTCAN: Watch Trigger Time TT_WTRIG_0 and TT_WTRIG_ 1
///////////////////////////////////////////////////////

#define CAN__TT_WTRIG__OFFSET 0xC8UL
#define CAN__TT_WTRIG__NUM  0x1
#define CAN__TT_WTRIG__TT_WTRIG__SHIFT    0
#define CAN__TT_WTRIG__RSDV_16_32__SHIFT    16

#define CAN__TT_WTRIG__TT_WTRIG__SIZE 16
#define CAN__TT_WTRIG__RSDV_16_32__SIZE 16

/**************************** Type Definitions ******************************/
typedef struct {
	AL_U32                  DeviceId;
	AL_U32                  BaseAddress;
} AL_CAN_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
