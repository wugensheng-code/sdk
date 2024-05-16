#ifndef __FREERTOS_DEMO_CONFIG_H
#define __FREERTOS_DEMO_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_core.h"
#include "al_gpio_hal.h"
#include "al_can_hal.h"
#include "al_dmacahb_hal.h"
#include "al_wdt_hal.h"
#include "al_iic_hal.h"
#include "al_mmc_hal.h"
#include "al_dma_hal.h"
#include "al_spinor.h"
#include "al_spi_hal.h"
#include "al_qspi_hal.h"
#include "al_uart_hal.h"
#include "al_ttc_hal.h"
#include "al_gbe_ethernetif.h"

/* TASK PRIORITY CONFIG */
#define WDT_TASK_PRIORITY           (3)
#define CAN_SEND_TASK_PRIORITY      (2)
#define CAN_DMARECV_TASK_PRIORITY   (2)
#define GPIO_LED_TASK_PRIORITY      (2)
#define DMACAHB_TASK_PRIORITY       (2)
#define IIC_TASK_PRIORITY           (2)
#define UART_TASK_PRIORITY          (2)
#define GBE_TASK_PRIORITY           (2)
#define MMC_TASK_PRIORITY           (2)
#define IIS_TASK_PRIORITY           (2)
#define DMA_TASK_PRIORITY           (2)
#define QSPI_TASK_PRIORITY          (2)
#define DMACAHB2_TASK_PRIORITY      (2)
#define UART_PROC_TASK_PRIORITY     (3)
#define PRINT_TASK_PRIORITY         (4)

/* TASK STACK SIZE IN WORD */
#define WDT_TASK_STACK_SIZE         (256)
#define CAN_SEND_TASK_STACK_SIZE    (256)
#define CAN_DMARECV_TASK_STACK_SIZE (256)
#define GPIO_LED_TASK_STACK_SIZE    (256)
#define DMACAHB_TASK_STACK_SIZE     (256)
#define IIC_TASK_STACK_SIZE         (256)
#define UART_TASK_STACK_SIZE        (256)
#define GBE_TASK_STACK_SIZE         (512)
#define MMC_TASK_STACK_SIZE         (512)
#define IIS_TASK_STACK_SIZE         (512)
#define DMA_TASK_STACK_SIZE         (512)
#define QSPI_TASK_STACK_SIZE        (512)
#define DMACAHB2_TASK_STACK_SIZE    (256)
#define UART_PROC_TASK_STACK_SIZE   (256)
#define PRINT_TASK_STACK_SIZE       (256)

/* GPIO LED MACRO CONFIG */
#define GPIO_LED_DEVICE_ID          (0)
#define GPIO_LED_PIN_NUM            (14)
#define GPIO_LED_TOGGLE_TIME        (pdMS_TO_TICKS(100))

/* CAN MACRO CONFIG */
#define CAN_DEVICE_ID               (0)
#define CAN_DMACAHB_DEVICE_ID       (0)
#define CAN_SEND_DATA_TIMEOUT_MS    (pdMS_TO_TICKS(1000))
#define CAN_RECV_DATA_TIMEOUT_MS    (pdMS_TO_TICKS(3000))
#define CAN_SEND_INTERVAL_TIME      (pdMS_TO_TICKS(200))
#define CAN_DMA_RECV_DATA           (80)

/* WDT MACRO CONFIG */
#define WDT_DEVICE_ID               (0)
#define WDT_FEED_TIME               (pdMS_TO_TICKS(2000))

/* DMACAHB MACRO CONFIG */
#define DMACAHB_DEVICE_ID           (0)
#define DMACAHB_TRANS_TIMEOUT_MS    (pdMS_TO_TICKS(100))
#define DMACAHB_INTERVAL_TIME       (pdMS_TO_TICKS(100))
#define DMACAHB_ARRAY_SIZE          (64)

/* IIC MACRO CONFIG */
#define IIC_DEVICE_ID               (0)
#define IIC_MUX_ADDRESS             (0x74)
#define IIC_EEPROM_CHANNEL          (0x04)
#define IIC_EEPROM_ADDRESS          (0x54)
#define IIC_EEPROM_PAGE_SIZE        (16)
#define IIC_EEPROM_START_ADDR       (0x0)
#define IIC_TEST_TIMEOUT_MS         (pdMS_TO_TICKS(1000))
#define IIC_INTERVAL_TIME           (pdMS_TO_TICKS(100))
#define IIC_ADDR_SIZE               (sizeof(AL_U8))
#define IIC_WAIT_WRITE_COMPLETE     (pdMS_TO_TICKS(100))

/* UART MACRO CONFIG */
#define UART_DEVICE_ID              (1)
#define UART_RECV_MEM_LENGTH        (64)
#define UART_RECV_SIZE_LENGTH       (4)
#define UART_RECV_MEM_SIZE          (5)
#define UART_INTERVAL_TIME          (pdMS_TO_TICKS(50))
#define UART_QUEUE_BLOCK_TIME       (pdMS_TO_TICKS(100))
#define UART_CMD_LEN_MAX            (20)

/* MMC MACRO CONFIG */
#define MMC_DEVICE_ID               (0)
#define MMC_BLOCK_TIMEOUT_MS        (pdMS_TO_TICKS(5000))
#define MMC_BLOCK_OFFSET            (2*1024*100)
#define MMC_BLOCK_SIZE              (512)
#define MMC_BLOCK_COUNT             (100)
#define MMC_BLOCK_LEN               (MMC_BLOCK_SIZE * MMC_BLOCK_COUNT)
#define MMC_INTERVAL_TIME           (pdMS_TO_TICKS(200))

/* QSPI MACRO CONFIG */
#define QSPI_DEVICE_ID              (0)
#define QSPI_INTERVAL_TIME          (pdMS_TO_TICKS(200))
#define QSPI_ADDR_OFFSET            (0x20)

/* IIS MACRO CONFIG */
#define IIS_DMA_DEVICE_ID           (0)
#define IIS_DMA_DATA_SIZE           (1024)
#define IIS_DMA_TRANS_TIMEOUT       (pdMS_TO_TICKS(2000))

#define IIS_TX_ID                   (0)
#define IIS_RX_ID                   (3)

#define IIS_BASE_ADDR               (0xA0000000)
#define IIS_RESET_REG               (0x0)
#define IIS_CTRL_REG                (0x4)
#define IIS_CLK_CTRL_REG            (0x8)
#define IIS_PERIOD_LEN_REG          (0x24)
#define IIS_DATA_RX_REG             (0x28)
#define IIS_DATA_TX_REG             (0x2c)

#define TOP_NS_PLS_PROT_REG         (0x80)
#define TOP_NS_AXI_DMA_CTRL_0_REG   (0x1A0)
#define TOP_NS_AXI_DMA_CTRL_1_REG   (0x1A4)
#define TOP_CRP_SRST_1_ADDR         (0x74)
#define TOP_S_AXI_DMA_CTRL_2_REG    (0xA0)

#define IIS_INTERVAL_TIME           (pdMS_TO_TICKS(100))

/* DMA MACRO CONFIG */
#define DMA_DEVICE_ID               (0)
#define DMA_TRANS_DATA_SIZE         (1024)
#define DMA_INTERVAL_TIME           (pdMS_TO_TICKS(100))

/* DMACAHB2 MACRO CONFIG */
#define DMACAHB2_DEVICE_ID          (0)
#define DMACAHB2_TRANS_TIMEOUT_MS   (pdMS_TO_TICKS(100))
#define DMACAHB2_INTERVAL_TIME      (pdMS_TO_TICKS(100))
#define DMACAHB2_ARRAY_SIZE         (1024)

/* UART CMD STATE */
#define TASK_STATE_BUF              (4096)

/* TIMER MACRO CONFIG */
#define TTC_DEVICE_ID               (0)
#define TTC_CLK_PRESACLE            (199)
#define TTC_MATCH_VALUE             (999)
#define TTC_INTERVAL_MAX_VALUE      (TTC_MATCH_VALUE)

/* ASYNC MACRO CONFIG */
#define PRINT_INTERVAL_TIME         (pdMS_TO_TICKS(50))


static AL_CAN_InitStruct Task2_CanConfig = {
    .OpsMode      = AL_CAN_MODE_NORMAL,
    .RunMode      = AL_CAN_RUN_INTR_DMA,
    .Type         = AL_CAN_TYPE_FD,
    .SlowBitRate  = AL_CAN_ARBITRATION_0_5M,
    .FastBitRate  = AL_CAN_1_M,
    .TransMode    = AL_CAN_TRANS_PTB,
    .RbAfwl       = AL_CAN_RB_LIMIT_8
};

static AL_CAN_FrameStruct Task2_Frame1 = {
    .Id             = 0x1,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x1,
    .DataLen        = AL_CAN_LEN_0,
    .Data           = {0x01020304},
};

static AL_CAN_FrameStruct Task2_Frame2 = {
    .Id             = 0x3,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_1,
    .Data           = {0x01020304},
};

static AL_CAN_FrameStruct Task2_Frame3 = {
    .Id             = 0x5,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_2,
    .Data           = {0x01020304},
};

static AL_CAN_FrameStruct Task2_Frame4 = {
    .Id             = 0x7,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_3,
    .Data           = {0x01020304},
};

static AL_CAN_FrameStruct Task2_Frame5 = {
    .Id             = 0x9,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_4,
    .Data           = {0x01020304},
};

static AL_CAN_FrameStruct Task2_Frame6 = {
    .Id             = 0xb,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_5,
    .Data           = {0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct Task2_Frame7 = {
    .Id             = 0xd,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_6,
    .Data           = {0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct Task2_Frame8 = {
    .Id             = 0xf,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_7,
    .Data           = {0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct Task2_Frame9 = {
    .Id             = 0x10,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_8,
    .Data           = {0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct Task2_Frame10 = {
    .Id             = 0x11,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_64,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, \
                        0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, \
                        0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00},
};

static AL_CAN_FrameStruct Task2_Frame11 = {
    .Id             = 0x12,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x1,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_64,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, \
                        0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, \
                        0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00},
};

static AL_CAN_FrameStruct Task2_Frame12 = {
    .Id             = 0x13,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x1,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_64,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, \
                        0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, \
                        0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00},
};

static AL_CAN_FrameStruct* Task2_FrameList[] = {&Task2_Frame1, &Task2_Frame2, &Task2_Frame3, &Task2_Frame4,
                                                 &Task2_Frame5, &Task2_Frame6, &Task2_Frame7, &Task2_Frame8,
                                                 &Task2_Frame9, &Task2_Frame10, &Task2_Frame11, &Task2_Frame12};

static AL_DMACAHB_ChInitStruct Task3_DmaChConfig = {
    .Id                     = AL_DMACAHB_CHANNEL_0,
    .TransType              = AL_DMACAHB_TRANS_TYPE_4,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_BLOCK | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_4,
    .DstBurstLength         = AL_DMACAHB_MSIZE_4,
    .Direction              = AL_DMACAHB_TT_FC_PER2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .HandShaking.SrcPer     = AL_DMACAHB_PER_CAN0_RX,
    .HandShaking.SrcHsSel   = AL_DMACAHB_HAND_SHAKING_HARDWARE,
    .HandShaking.SrcHsPol   = AL_DMACAHB_HS_POL_ACTIVE_HI,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_7,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

static AL_WDT_InitStruct Task4_WdtConfig = {
    .ResetPuaseLength = WDT_RPL_PCLK_CYCLES_8,
    .ResponseMode = WDT_INTR_MODE,
    .TimeOutValue = WDT_TIMEOUT_PERIOD_2G_CLOCKS,
};

static AL_DMACAHB_ChInitStruct Task5_DmaChConfig = {
    .Id                     = AL_DMACAHB_CHANNEL_0,
    .TransType              = AL_DMACAHB_TRANS_TYPE_1,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_1,
    .DstBurstLength         = AL_DMACAHB_MSIZE_1,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_7,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

static AL_IIC_InitStruct task6_IicConfig = {
    .Mode       = AL_IIC_MODE_MASTER,
    .AddrMode   = AL_IIC_ADDR_7BIT,
};

static AL_UART_InitStruct task7_UartConfig = {
    .BaudRate           = 115200,
    .Parity             = AL_UART_NO_PARITY,
    .WordLength         = AL_UART_CHAR_8BITS,
    .StopBits           = AL_UART_STOP_1BIT,
    .HwFlowCtl          = AL_FALSE,
    .CharTimeoutEnable  = AL_TRUE
};

static AL_MMC_InitStruct task10_MmcSdmaConfig = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_SDMA,
    .FreqKhz            = AL_MMC_FREQ_KHZ_50000,
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_32K
};

static AL_QSPI_ConfigsStruct task11_QspiX4Config = {
    .SamplDelay         = 2,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SpiFrameFormat     = SPI_QUAD_FORMAT,
    .ClockStretch       = QSPI_EnableClockStretch
};

static AL_DMA_ChCfgStruct task12_IisM2PConfig = {
    .Direction      = DMA_MEM_TO_DEV,
    .SrcAddrWidth   = DMA_CH_BUSWIDTH_4_BYTES,
    .DstAddrWidth   = DMA_CH_BUSWIDTH_4_BYTES,
    .SrcMaxBurst    = 1,
    .DstMaxBurst    = 1,
    .DstAddr        = IIS_BASE_ADDR + IIS_DATA_TX_REG,
};

static AL_DMA_ChCfgStruct task12_IisP2MConfig = {
    .Direction      = DMA_DEV_TO_MEM,
    .SrcAddrWidth   = DMA_CH_BUSWIDTH_4_BYTES,
    .DstAddrWidth   = DMA_CH_BUSWIDTH_4_BYTES,
    .SrcMaxBurst    = 1,
    .DstMaxBurst    = 1,
    .SrcAddr        = IIS_BASE_ADDR + IIS_DATA_RX_REG,
};

static AL_DMA_ChCfgStruct task13_DmaConfig = {
    .Direction      = DMA_MEM_TO_MEM,
    .SrcAddrWidth   = DMA_CH_BUSWIDTH_32_BYTES,
    .DstAddrWidth   = DMA_CH_BUSWIDTH_32_BYTES,
    .SrcMaxBurst    = 4,
    .DstMaxBurst    = 4,
};

static AL_DMACAHB_ChInitStruct Task14_DmaChConfig = {
    .Id                     = AL_DMACAHB_CHANNEL_0,
    .TransType              = AL_DMACAHB_TRANS_TYPE_1,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_1,
    .DstBurstLength         = AL_DMACAHB_MSIZE_1,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_6,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

static inline AL_VOID AlQspi_freertos_Reset(AL_QSPI_HalStruct *Handle, AL_U8 *SendData)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_INFINEON_SRST;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_RESET error:0x%x\r\n", Ret);
    }
}

void AlQspi_freertos_WrEn(AL_QSPI_HalStruct *Handle, AL_U8 *SendData)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_WREN;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_WrEn error:0x%x\r\n", Ret);
    }
}

void AlQspi_freertos_SetStatus(AL_QSPI_HalStruct *Handle, AL_U8 *SendData,AL_U8 data)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_WRSR;
    SendData[1] = data;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_SetStatus error:0x%x\r\n", Ret);
    }
}

void AlQspi_freertos_WaitWip(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8 *RecvData)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    SendData[0] = NOR_OP_RDSR;

    do {
        Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_WaitWip error:0x%x\r\n", Ret);
        }
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_DEBUG, "WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}


void AlQspi_freertos_ReadStatus(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8 *RecvData)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_RDSR;

    AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_ReadStatus error:0x%x\r\n", Ret);
    }
#ifdef QSPI_DEBUG
    AL_LOG(AL_LOG_LEVEL_DEBUG, "Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
}


void AlQspi_freertos_Erase(AL_QSPI_HalStruct *Handle, AL_U8 *SendData)
{
    AL_S32 Ret = AL_OK;

   // AlQspi_Dev_DumpReg(Handle->Dev.BaseAddr);

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 4, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_Erase error:0x%x\r\n", Ret);
    }
}

void AlQspi_freertos_EraseChip(AL_QSPI_HalStruct *Handle, AL_U8 *SendData)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_CE;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_freertos_EraseChip running\r\n");
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_EraseChip error:0x%x\r\n", Ret);
    }
}


void AlQspi_freertos_X4_1_1_4_Read(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8 Addr, AL_U8 *RecvData)
{
    AL_S32 Ret = AL_OK;
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_freertos_ReadPage\r\n");
    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
        Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;

    SendData[0] = NOR_OP_READ_1_1_4;
    SendData[1] = Addr;
    SendData[2] = 0;
    SendData[3] = 0;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_ReadPage error:0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_freertos_ReadPage end\r\n");
}


void AlQspi_freertos_Write_X4_1_1_4_Page(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8 Addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_PP_1_1_4;
    SendData[1] = Addr;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_WritePage error:0x%x\r\n", Ret);
    }
}



void AlQspi_freertos_X4_1_4_4_ReadPage(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8 *RecvData)
{
    AL_S32 Ret = AL_OK;
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_freertos_X4_1_4_4_ReadPage\r\n");
    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;

    SendData[0] = NOR_OP_READ_1_1_4;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_ReadPage error:0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_freertos_ReadPage end\r\n");
}

void AlQspi_freertos_ReadPage(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8 *RecvData)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_ReadPage error:0x%x\r\n", Ret);
    }
}

void AlQspi_freertos_WritePage(AL_QSPI_HalStruct *Handle, AL_U8 *SendData)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_WritePage error:0x%x\r\n", Ret);
    }
}

AL_VOID AlQspi_freertos_ReadId(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8 *FlashId)
{
    AL_S32 Ret = AL_OK;
    SendData[0] = NOR_OP_RDID;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, FlashId, 3, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_ReadId error:0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG, "Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}


AL_VOID AlQspi_freertos_RemoveProtect(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8 *RecvData)
{
    AL_S32 Ret = AL_OK;
    AL_U8 Temp;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendData[0] = NOR_OP_RDCR;
    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_REMOVE_PTOTECT1 error:0x%x\r\n", Ret);
    }
    Temp = RecvData[0];
    AlQspi_freertos_WrEn(Handle, SendData);

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    RecvData[2] = Temp | 0x2;
    RecvData[1] = 0;
    RecvData[0] = NOR_OP_WRSR;

    Ret = AlQspi_Hal_SendDataBlock(Handle, RecvData, 3, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_REMOVE_PTOTECT2 error:0x%x\r\n", Ret);
    }
    AlQspi_freertos_WaitWip(Handle, SendData, RecvData);

}

AL_VOID AlQspi_freertos_MxIcSetQE(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8 *RecvData)
{
    AL_S32 Ret = AL_OK;
    AL_U8 Temp;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendData[0] = NOR_OP_RDSR;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_MxIcSetQE error:0x%x\r\n", Ret);
    }
    Temp = RecvData[0];

    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_freertos_MxIcSetQE Status:%d\r\n", Temp);
    AlQspi_freertos_WrEn(Handle, SendData);

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    RecvData[1] = Temp | (1 << 6);
    RecvData[0] = NOR_OP_WRSR;

    Ret = AlQspi_Hal_SendDataBlock(Handle, RecvData, 2, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_MxIcSetQE error:0x%x\r\n", Ret);
    }

    AlQspi_freertos_WaitWip(Handle, SendData, RecvData);

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendData[0] = NOR_OP_RDSR;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_MxIcSetQE error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_freertos_MxIcSetQE Status:%d\r\n", RecvData[0]);

}

AL_S32 AlQspi_freertos_SetQuad(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8 *RecvData, AL_U8 SetQuadCmd, AL_U8 ReadQuadCmd, AL_U8 QuadPos)
{
    AL_S32 Data = 0, Ret = AL_OK;

    SendData[0] = ReadQuadCmd;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, &Data, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_SetQuad ReadQuadCmd error:0x%x\r\n", Ret);
    }

    Data = Data | (1 << QuadPos);

    AlQspi_freertos_WrEn(Handle, SendData);

    SendData[0] = SetQuadCmd;
    SendData[1] = Data;

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_freertos_SetQuad SetQuadCmd error:0x%x\r\n", Ret);
    }

    AlQspi_freertos_WaitWip(Handle, SendData, RecvData);

    return Ret;
}

#ifdef __cplusplus
}
#endif

#endif
