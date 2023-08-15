/**
 * @file    al_mmc_test_config.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-05-06
 * @brief   dmac ahb test channel config file
 */

#ifndef AL_MMC_TEST_CONFIG_H
#define AL_MMC_TEST_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_mmc_hal.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

#define AL_MMC_TEST_SINGLE_BLK_CNT  (1)
#define AL_MMC_TEST_MULTI_BLK_CNT   (100)
#define AL_MMC_TEST_BLK_SIZE        (0x200)
#define AL_MMC_TEST_BLK_NUM         (0xb)
#define AL_MMC_TEST_BLK_NUM_1       (0x1)
#define AL_MMC_TEST_WR_BUFF_ADDR    (0x30000000ULL)
#define AL_MMC_TEST_RD_BUFF_ADDR    (0x38000000ULL)

#define AL_MMC_TEST_SD_CASE_NUM     (3)
#define AL_MMC_TEST_EMMC_CASE_NUM   (3)

/**************************** Type Definitions *******************************/

typedef struct
{
    AL_U64      Start;
    AL_U64      End;
    AL_U64      TimeInUs;
    AL_U64      DatInByte;
    AL_DOUBLE   BytePerSec;
} AL_MMC_PerCalcStruct;


/************************** Variable Definitions *****************************/

static AL_MMC_InitStruct SdNoDmaInit = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_NONE,
#ifdef AUTO_TEST
    .FreqKhz            = AL_MMC_FREQ_KHZ_DEF,
#else
    .FreqKhz            = AL_MMC_FREQ_KHZ_50000,
#endif
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_32K,
    .ForceVer3          = AL_TRUE
};

static AL_MMC_InitStruct SdSdmaInit = {
    .CardType           = AL_MMC_CARD_TYPE_SD,
    .DmaMode            = AL_MMC_DMA_MODE_SDMA,
#ifdef AUTO_TEST
    .FreqKhz            = AL_MMC_FREQ_KHZ_DEF,
#else
    .FreqKhz            = AL_MMC_FREQ_KHZ_50000,
#endif
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_32K,
    .ForceVer3          = AL_TRUE
};

static AL_MMC_InitStruct SdAdma2Init = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_ADMA2,
#ifdef AUTO_TEST
    .FreqKhz            = AL_MMC_FREQ_KHZ_DEF,
#else
    .FreqKhz            = AL_MMC_FREQ_KHZ_50000,
#endif
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_ENABLE,
    .DmaBdary           = AL_MMC_BDARY_32K,
    .ForceVer3          = AL_TRUE
};

static AL_MMC_InitStruct EmmcNoDmaInit = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_NONE,
#ifdef AUTO_TEST
    .FreqKhz            = AL_MMC_FREQ_KHZ_DEF,
#else
    .FreqKhz            = AL_MMC_FREQ_KHZ_50000,
#endif
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_8BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_32K,
    .ForceVer3          = AL_TRUE
};

static AL_MMC_InitStruct EmmcSdmaInit = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_SDMA,
#ifdef AUTO_TEST
    .FreqKhz            = AL_MMC_FREQ_KHZ_DEF,
#else
    .FreqKhz            = AL_MMC_FREQ_KHZ_50000,
#endif
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_8BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_4K,
    .ForceVer3          = AL_TRUE
};

static AL_MMC_InitStruct EmmcAdma2Init = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_ADMA2,
#ifdef AUTO_TEST
    .FreqKhz            = AL_MMC_FREQ_KHZ_DEF,
#else
    .FreqKhz            = AL_MMC_FREQ_KHZ_50000,
#endif
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_8BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_ENABLE,
    .DmaBdary           = AL_MMC_BDARY_32K,
    .ForceVer3          = AL_TRUE
};

/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif
