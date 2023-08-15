/**
 * @file    al_can_test_config.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-05-09
 * @brief   can test channel config file
 */

#ifndef AL_CAN_TEST_CONFIG_H
#define AL_CAN_TEST_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_can_hal.h"
#include "al_dmacahb_hal.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_CAN_DMAC_RECV_DATA_IN_WORD   20
#define AL_CAN_DMAC_RELOAD_COUNT_MAX    (0xFFFFFFFFUL)

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/
static AL_CAN_FrameStruct Frame1 = {
    .Id             = 0x1,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x1,
    .DataLen        = AL_CAN_LEN_0,
    .Data           = {0x01020304},
};

static AL_CAN_FrameStruct Frame3 = {
    .Id             = 0x3,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_1,
    .Data           = {0x01020304},
};

static AL_CAN_FrameStruct Frame5 = {
    .Id             = 0x5,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_2,
    .Data           = {0x01020304},
};

static AL_CAN_FrameStruct Frame7 = {
    .Id             = 0x7,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_3,
    .Data           = {0x01020304},
};

static AL_CAN_FrameStruct Frame9 = {
    .Id             = 0x9,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_4,
    .Data           = {0x01020304},
};

static AL_CAN_FrameStruct Frame11 = {
    .Id             = 0xb,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_5,
    .Data           = {0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct Frame13 = {
    .Id             = 0xd,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_6,
    .Data           = {0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct Frame15 = {
    .Id             = 0xf,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_7,
    .Data           = {0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct Frame16 = {
    .Id             = 0x10,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_8,
    .Data           = {0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct Frame17 = {
    .Id             = 0x11,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x1,
    .DataLen        = AL_CAN_LEN_8,
    .Data           = {0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct Frame18 = {
    .Id             = 0x12,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x1,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_8,
    .Data           = {0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct Frame19 = {
    .Id             = 0x13,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x1,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_8,
    .Data           = {0x01020304, 0x05060708},
};


static AL_CAN_FrameStruct FdFrame1 = {
    .Id             = 0x1,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_8,
    .Data           = {0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct FdFrame3 = {
    .Id             = 0x3,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_12,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c},
};

static AL_CAN_FrameStruct FdFrame5 = {
    .Id             = 0x5,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_16,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00},
};

static AL_CAN_FrameStruct FdFrame7 = {
    .Id             = 0x7,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_20,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, 0x01020304},
};

static AL_CAN_FrameStruct FdFrame9 = {
    .Id             = 0x9,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_24,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708},
};

static AL_CAN_FrameStruct FdFrame11 = {
    .Id             = 0xb,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_32,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, \
                        0x090a0b0c, 0x0d0e0f00},
};

static AL_CAN_FrameStruct FdFrame13 = {
    .Id             = 0xd,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_48,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, \
                        0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00},
};

static AL_CAN_FrameStruct FdFrame15 = {
    .Id             = 0xf,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_64,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, \
                        0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, \
                        0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00},
};

static AL_CAN_FrameStruct FdFrame16 = {
    .Id             = 0x10,
    .IsBitSwitch    = 0x1,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_64,
    .Data           = {0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, \
                        0x090a0b0c, 0x0d0e0f00, 0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00, \
                        0x01020304, 0x05060708, 0x090a0b0c, 0x0d0e0f00},
};

static AL_CAN_FrameStruct FdFrame17 = {
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

static AL_CAN_FrameStruct FdFrame18 = {
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

static AL_CAN_FrameStruct FdFrame19 = {
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

static AL_DMACAHB_ChInitStruct CanDmaChExample1 = {
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
#ifdef BOARD_DR1X90_AD101_V10
    .HandShaking.SrcPer     = AL_DMACAHB_PER_CAN0_RX,
#else
    .HandShaking.SrcPer     = AL_DMACAHB_PER_CAN1_RX,
#endif
    .HandShaking.SrcHsSel   = AL_DMACAHB_HAND_SHAKING_HARDWARE,
    .HandShaking.SrcHsPol   = AL_DMACAHB_HS_POL_ACTIVE_HI,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_7,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif
