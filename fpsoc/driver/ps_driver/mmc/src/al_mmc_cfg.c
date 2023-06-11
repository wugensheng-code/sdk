#include "al_mmc_hw.h"

AL_MMC_HwConfigStruct AlMmc_HwConfig[AL_MMC_NUM_INSTANCE] = {
    {
        .DeviceId       = 0,
        .BaseAddress    = MMC_MMC0_BASE_ADDR,
        .IntrId         = MMC_MMC0_INTR_ID,
        .BusWidth       = MMC_MMC0_BUS_WIDTH,
        .InputClkKhz    = MMC_MMC0_PROGRAM_INPUT_CLK_KHZ,
        .CardDetectSig  = MMC_MMC0_CARD_DETECT_SIG,
        .WritePortSig   = MMC_MMC0_WR_PROT_SIG,
        .CardDetect     = MMC_MMC0_CARD_DETECT_STATE,
        .WritePort      = MMC_MMC0_WR_PROT_STATE,
        .ClkPhase       = MMC_MMC0_CLK_PHASE
    },
    {
        .DeviceId       = 1,
        .BaseAddress    = MMC_MMC1_BASE_ADDR,
        .IntrId         = MMC_MMC1_INTR_ID,
        .BusWidth       = MMC_MMC1_BUS_WIDTH,
        .InputClkKhz    = MMC_MMC1_PROGRAM_INPUT_CLK_KHZ,
        .CardDetectSig  = MMC_MMC1_CARD_DETECT_SIG,
        .WritePortSig   = MMC_MMC1_WR_PROT_SIG,
        .CardDetect     = MMC_MMC1_CARD_DETECT_STATE,
        .WritePort      = MMC_MMC1_WR_PROT_STATE,
        .ClkPhase       = MMC_MMC1_CLK_PHASE
    }
};