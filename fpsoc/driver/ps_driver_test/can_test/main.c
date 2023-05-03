#include "al_can_hal.h"
#include "al_errno.h"
#include "mtimer.h"
#include "nuclei_sdk_soc.h"
#include "gic_v3.h"

static AL_CAN_FrameStruct Frame1 = {
    .Id             = 0x1,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
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

AL_U32 main(AL_VOID)
{
    AL_CAN_HalStruct Handle;
    AL_CAN_InitStruct Config;
    AL_CAN_FrameStruct Frame;
    AL_U32 DeviceId = 1;
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 1000;

#if CONFIG_CAN_LISTEN_ONLY_TEST
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Can FD listen only example\r\n");
    Config.OpsMode      = AL_CAN_MODE_LISTENONLY;
    Config.RunMode      = AL_CAN_RUN_INTR;
    Config.Type         = AL_CAN_TYPE_FD;
    Config.SBitTiming   = AL_CAN_ARBITRATION__0_5M;
    Config.FBitTiming   = AL_CAN__1_M;
    Config.TransMode    = AL_CAN_TRANS_PTB;
    Config.RbAfwl       = AL_CAN_RB_LIMIT_8;
    Ret = AlCan_Hal_Init(&Handle, &Config, AL_NULL, DeviceId);

    while (1) {
        AlCan_Hal_RecvFrameBlock(Handle, &Frame);
        AlCan_Dev_DisplayFrame(&Frame);
    };
#endif

#if CONFIG_CAN_IN_LOOPBACK_TEST
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B in loopback example\r\n");
    Config.OpsMode      = AL_CAN_MODE_IN_LOOPBACK;
    Config.RunMode      = AL_CAN_RUN_INTR;
    Config.Type         = AL_CAN_TYPE_2_0B;
    Config.SBitTiming   = AL_CAN_ARBITRATION__0_25M;
    Config.TransMode    = AL_CAN_TRANS_PTB;
    Config.RbAfwl       = AL_CAN_RB_LIMIT_8;
    Ret = AlCan_Hal_Init(&Handle, &Config, AL_NULL, DeviceId);

    while (1) {
        AlCan_Hal_SendFrameBlock(&Handle, &Frame1, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame1 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame3, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame3 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame5, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame5 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame7, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame7 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame9, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame9 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame11, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame11 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame13, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame13 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame15, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame15 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame16, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame16 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame17, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame17 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame18, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame18 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame19, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame19 example\r\n");
        _delay_ms(500);
    }
#endif

#if CONFIG_CAN_EX_LOOPBACK_TEST
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B extern loopback example\r\n");
    Config.OpsMode      = AL_CAN_MODE_EX_LOOPBACK;
    Config.RunMode      = AL_CAN_RUN_INTR;
    Config.Type         = AL_CAN_TYPE_2_0B;
    Config.SBitTiming   = AL_CAN_ARBITRATION__0_25M;
    Config.TransMode    = AL_CAN_TRANS_PTB;
    Config.RbAfwl       = AL_CAN_RB_LIMIT_8;
    Ret = AlCan_Hal_Init(&Handle, &Config, AL_NULL, DeviceId);

    while (1) {
        AlCan_Hal_SendFrameBlock(&Handle, &Frame1, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame1 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame3, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame3 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame5, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame5 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame7, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame7 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame9, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame9 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame11, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame11 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame13, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame13 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame15, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame15 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame16, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame16 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame17, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame17 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame18, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame18 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame19, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame19 example\r\n");
        _delay_ms(500);
    }
#endif

#if CONFIG_CAN_2_0B_INTR_TEST
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B intr example\r\n");
    Config.OpsMode      = AL_CAN_MODE_NORMAL;
    Config.RunMode      = AL_CAN_RUN_INTR;
    Config.Type         = AL_CAN_TYPE_2_0B;
    Config.SBitTiming   = AL_CAN_ARBITRATION__0_25M;
    Config.TransMode    = AL_CAN_TRANS_PTB;
    Config.RbAfwl       = AL_CAN_RB_LIMIT_8;
    Ret = AlCan_Hal_Init(&Handle, &Config, AL_NULL, DeviceId);
    
    while (1) {
        AlCan_Hal_SendFrameBlock(&Handle, &Frame1, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame1 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame3, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame3 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame5, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame5 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame7, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame7 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame9, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame9 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame11, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame11 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame13, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame13 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame15, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame15 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame16, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame16 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame17, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame17 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame18, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame18 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &Frame19, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can 2.0B Frame19 example\r\n");
        _delay_ms(500);
    }
#endif

#if CONFIG_CAN_FD_INTR_TEST
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd intr example\r\n");
    Ret = AlCan_Hal_Init(&Handle, AL_NULL, AL_NULL, DeviceId);

    // AL_CAN_FilterCfgStruct FilterCfg;
    // FilterCfg.FilterIndex = AL_CAN_FILTER_0;
    // AlCan_Dev_GetFilter(Handle.Dev, &FilterCfg);
    // FilterCfg.MaskType  = AL_CAN_FILTER_MASK_STD;
    // FilterCfg.MaskValue = 0xFFFFFFFE;
    // FilterCfg.IdValue   = 0x1;
    // AlCan_Dev_SetFilter(Handle.Dev, &FilterCfg);
    
    while (1) {
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame1, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame1 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame3, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame3 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame5, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame5 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame7, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame7 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame9, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame9 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame11, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame11 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame13, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame13 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame15, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame15 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame16, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame16 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame17, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame17 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame18, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame18 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame19, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame19 example\r\n");
        _delay_ms(500);
    }
#endif

#if CONFIG_CAN_FD_STB_FIFO_TEST
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd STB fifo example\r\n");
    Config.OpsMode      = AL_CAN_MODE_NORMAL;
    Config.RunMode      = AL_CAN_RUN_INTR;
    Config.Type         = AL_CAN_TYPE_FD;
    Config.SBitTiming   = AL_CAN_ARBITRATION__0_25M;
    Config.TransMode    = AL_CAN_TRANS_STB_FIFO;
    Config.RbAfwl       = AL_CAN_RB_LIMIT_8;
    Ret = AlCan_Hal_Init(&Handle, AL_NULL, AL_NULL, DeviceId);

    // AL_CAN_FilterCfgStruct FilterCfg;
    // FilterCfg.FilterIndex = AL_CAN_FILTER_0;
    // AlCan_Dev_GetFilter(Handle.Dev, &FilterCfg);
    // FilterCfg.MaskType  = AL_CAN_FILTER_MASK_STD;
    // FilterCfg.MaskValue = 0xFFFFFFFE;
    // FilterCfg.IdValue   = 0x1;
    // AlCan_Dev_SetFilter(Handle.Dev, &FilterCfg);
    
    while (1) {
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame1, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame1 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame3, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame3 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame5, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame5 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame7, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame7 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame9, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame9 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame11, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame11 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame13, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame13 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame15, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame15 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame16, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame16 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame17, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame17 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame18, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame18 example\r\n");
        _delay_ms(500);
        AlCan_Hal_SendFrameBlock(&Handle, &FdFrame19, Timeout);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can Fd Frame19 example\r\n");
        _delay_ms(500);
    }
#endif
}