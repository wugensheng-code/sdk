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
#define AL_MMC_TEST_MULTI_BLK_CNT   (10000)
#define AL_MMC_TEST_BLK_SIZE        (0x200)
#define AL_MMC_TEST_WR_BUFF_ADDR    (0x20000000ULL)
#define AL_MMC_TEST_RD_BUFF_ADDR    (0x28000000ULL)

#define AL_MMC_TEST_SD_CASE_NUM     (3)

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
    .FreqKhz            = AL_MMC_FREQ_KHZ_25000,
    .BusWidth           = AL_MMC_BUS_WIDTH_1BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_32K
};

static AL_MMC_InitStruct SdSdmaInit = {
    .CardType           = AL_MMC_CARD_TYPE_SD,
    .DmaMode            = AL_MMC_DMA_MODE_SDMA,
    .FreqKhz            = AL_MMC_FREQ_KHZ_25000,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_4K
};

static AL_MMC_InitStruct SdAdma2Init = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_ADMA2,
    .FreqKhz            = AL_MMC_FREQ_KHZ_25000,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_ENABLE,
    .DmaBdary           = AL_MMC_BDARY_32K
};

/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif
