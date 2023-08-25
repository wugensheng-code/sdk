#include "al_can_test_config.h"
#include "al_errno.h"

AL_U32 __attribute__((aligned(4))) AL_CAN_TEST_DST_MEM[AL_CAN_DMAC_RECV_DATA_IN_WORD];

static AL_VOID AlCan_Test_ListenOnly(AL_VOID);
static AL_VOID AlCan_Test_InLoopBack(AL_VOID);
static AL_VOID AlCan_Test_ExLoopBack(AL_VOID);
static AL_VOID AlCan_Test_StdIntr(AL_VOID);
static AL_VOID AlCan_Test_FdIntr(AL_VOID);
static AL_VOID AlCan_Test_FdStbFifo(AL_VOID);
static AL_VOID AlCan_Test_DmaCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef);
static AL_VOID AlCan_Test_FdDmaRecv(AL_VOID);

AL_U32 main(AL_VOID)
{
#if CONFIG_CAN_LISTEN_ONLY_TEST
    AlCan_Test_ListenOnly();
#endif

#if CONFIG_CAN_IN_LOOPBACK_TEST
    AlCan_Test_InLoopBack();
#endif

#if CONFIG_CAN_EX_LOOPBACK_TEST
    AlCan_Test_ExLoopBack();
#endif

#if CONFIG_CAN_2_0B_INTR_TEST
    AlCan_Test_StdIntr();
#endif

#if CONFIG_CAN_FD_INTR_TEST
    AlCan_Test_FdIntr();
#endif

#if CONFIG_CAN_FD_STB_FIFO_TEST
    AlCan_Test_FdStbFifo();
#endif

#if CONFIG_CAN_FD_DMA_TEST
    AlCan_Test_FdDmaRecv();
#endif

    while (1) {};
}

static AL_VOID AlCan_Test_ListenOnly(AL_VOID)
{
    AL_CAN_HalStruct Handle;
    AL_CAN_InitStruct Config;
    AL_CAN_FrameStruct Frame;
#ifdef BOARD_DR1X90_AD101_V10
    AL_U32 DeviceId = 0;
#else
    AL_U32 DeviceId = 1;
#endif
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 1000;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Can FD listen only example\r\n");
    Config.OpsMode      = AL_CAN_MODE_LISTENONLY;
    Config.RunMode      = AL_CAN_RUN_INTR;
    Config.Type         = AL_CAN_TYPE_FD;
    Config.SlowBitRate   = AL_CAN_ARBITRATION_0_5M;
    Config.FastBitRate   = AL_CAN_1_M;
    Config.TransMode    = AL_CAN_TRANS_PTB;
    Config.RbAfwl       = AL_CAN_RB_LIMIT_8;

    Ret = AlCan_Hal_Init(&Handle, DeviceId, &Config, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init Error:0x%x\r\n", Ret);
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        // Ret = AlCan_Hal_RecvFrameBlock(&Handle, &Frame, Timeout);
        // if (Ret != AL_OK) {
        //     AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        // }
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame1, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame1 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame3, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame3 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame3 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame5, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame5 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame5 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame7, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame7 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame7 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame9, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame9 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame9 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame11, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame11 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame11 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame13, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame13 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame13 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame15, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame15 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame15 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame16, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame16 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame16 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame17, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame17 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame17 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame18, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame18 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame18 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame19, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame19 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame19 example\r\n");
        AlSys_MDelay(500);
    };
}

static AL_VOID AlCan_Test_InLoopBack(AL_VOID)
{
    AL_CAN_HalStruct Handle;
    AL_CAN_InitStruct Config;
    AL_CAN_FrameStruct Frame;
#ifdef BOARD_DR1X90_AD101_V10
    AL_U32 DeviceId = 0;
#else
    AL_U32 DeviceId = 1;
#endif
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 1000;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B in loopback example\r\n");
    Config.OpsMode      = AL_CAN_MODE_IN_LOOPBACK;
    Config.RunMode      = AL_CAN_RUN_INTR;
    Config.Type         = AL_CAN_TYPE_2_0B;
    Config.SlowBitRate  = AL_CAN_ARBITRATION_0_25M;
    Config.TransMode    = AL_CAN_TRANS_PTB;
    Config.RbAfwl       = AL_CAN_RB_LIMIT_8;

    Ret = AlCan_Hal_Init(&Handle, DeviceId, &Config, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init Error:0x%x\r\n", Ret);
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame1, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame1 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame1 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame3, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame3 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame3 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame3 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame5, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame5 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame5 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame5 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame7, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame7 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame7 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame7 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame9, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame9 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame9 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame9 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame11, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame11 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame11 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame11 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame13, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame13 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame13 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame13 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame15, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame15 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame15 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame15 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame16, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame16 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame16 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame16 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame17, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame17 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame17 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame17 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame18, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame18 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame18 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame18 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame19, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame19 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame19 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame19 example\r\n");
        AlSys_MDelay(500);
    }
}

static AL_VOID AlCan_Test_ExLoopBack(AL_VOID)
{
    AL_CAN_HalStruct Handle;
    AL_CAN_InitStruct Config;
    AL_CAN_FrameStruct Frame;
#ifdef BOARD_DR1X90_AD101_V10
    AL_U32 DeviceId = 0;
#else
    AL_U32 DeviceId = 1;
#endif
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 1000;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B extern loopback example\r\n");
    Config.OpsMode      = AL_CAN_MODE_EX_LOOPBACK;
    Config.RunMode      = AL_CAN_RUN_INTR;
    Config.Type         = AL_CAN_TYPE_2_0B;
    Config.SlowBitRate  = AL_CAN_ARBITRATION_0_25M;
    Config.TransMode    = AL_CAN_TRANS_PTB;
    Config.RbAfwl       = AL_CAN_RB_LIMIT_8;

    Ret = AlCan_Hal_Init(&Handle, DeviceId, &Config, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init Error:0x%x\r\n", Ret);
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame1, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame1 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame1 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame3, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame3 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame3 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame3 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame5, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame5 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame5 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame5 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame7, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame7 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame7 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame7 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame9, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame9 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame9 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame9 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame11, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame11 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame11 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame11 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame13, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame13 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame13 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame13 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame15, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame15 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame15 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame15 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame16, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame16 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame16 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame16 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame17, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame17 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame17 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame17 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame18, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame18 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame18 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame18 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame19, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame19 Error:0x%x\r\n", Ret);
        }
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame19 Error:0x%x\r\n", Ret);
        }
        AlCan_Dev_DisplayFrame(&Frame);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame19 example\r\n");
        AlSys_MDelay(500);
    }
}

AL_VOID AlCan_Test_EventCallBack(AL_CAN_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_U32 Ret = AL_OK;
    AL_CAN_HalStruct *Handle = (AL_CAN_HalStruct *)CallBackRef;
    switch (Event->EventId)
    {
    case AL_CAN_EVENT_RECV_DONE: {
        AL_CAN_FrameStruct Frame;
        Ret = AlCan_Dev_RecvFrame(Handle->Dev, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame Error:0x%x\r\n", Ret);
            return;
        }
        AlCan_Dev_DisplayFrame(&Frame);
        break;
    }
    case AL_CAN_EVENT_ERR: {
        if (AlCan_ll_IsBusOff(Handle->Dev->BaseAddr)) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Bus off bit has been set\r\n");
            AlCan_ll_SetBusOff(Handle->Dev->BaseAddr, AL_TRUE);
        }
        break;
    }
    default:
        break;
    }
}

static AL_VOID AlCan_Test_StdIntr(AL_VOID)
{
    AL_CAN_HalStruct Handle;
    AL_CAN_InitStruct Config;
    AL_CAN_FrameStruct Frame;
    AL_CAN_CallBackStruct CallBack;
#ifdef BOARD_DR1X90_AD101_V10
    AL_U32 DeviceId = 0;
#else
    AL_U32 DeviceId = 1;
#endif
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 1000;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B intr example\r\n");

    Config.OpsMode      = AL_CAN_MODE_NORMAL;
    Config.RunMode      = AL_CAN_RUN_INTR;
    Config.Type         = AL_CAN_TYPE_2_0B;
    Config.SlowBitRate   = AL_CAN_ARBITRATION_0_25M;
    Config.TransMode    = AL_CAN_TRANS_PTB;
    Config.RbAfwl       = AL_CAN_RB_LIMIT_8;

    CallBack.Func = AlCan_Test_EventCallBack;
    CallBack.Ref = &Handle;

    Ret = AlCan_Hal_Init(&Handle, DeviceId, &Config, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init Error:0x%x\r\n", Ret);
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame Error:0x%x\r\n", Ret);
        } else {
            AlCan_Dev_DisplayFrame(&Frame);
        }
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame1, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame1 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame3, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame3 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame3 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame5, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame5 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame5 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame7, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame7 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame7 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame9, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame9 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame9 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame11, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame11 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame11 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame13, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame13 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame13 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame15, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame15 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame15 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame16, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame16 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame16 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame17, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame17 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame17 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame18, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame18 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame18 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &Frame19, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame19 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can 2.0B Frame19 example\r\n");
        AlSys_MDelay(500);
    }
}
static AL_VOID AlCan_Test_FdIntr(AL_VOID)
{
    AL_CAN_HalStruct Handle;
    AL_CAN_CallBackStruct CallBack;
    AL_CAN_FrameStruct Frame;
#ifdef BOARD_DR1X90_AD101_V10
    AL_U32 DeviceId = 0;
#else
    AL_U32 DeviceId = 1;
#endif
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 1000;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd intr example\r\n");

    CallBack.Func = AlCan_Test_EventCallBack;
    CallBack.Ref = &Handle;

    Ret = AlCan_Hal_Init(&Handle, DeviceId, AL_NULL, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init Error:0x%x\r\n", Ret);
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        Ret = AlCan_Hal_RecvFrame(&Handle, &Frame);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame Error:0x%x\r\n", Ret);
        } else {
            AlCan_Dev_DisplayFrame(&Frame);
        }
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame1, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame1 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame3, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame3 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame3 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame5, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame5 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame5 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame7, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame7 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame7 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame9, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame9 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame9 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame11, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame11 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame11 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame13, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame13 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame13 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame15, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame15 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame15 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame16, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame16 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame16 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame17, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame17 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame17 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame18, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame18 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame18 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame19, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame19 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame19 example\r\n");
        AlSys_MDelay(500);
    }
}

static AL_VOID AlCan_Test_FdStbFifo(AL_VOID)
{
    AL_CAN_HalStruct Handle;
    AL_CAN_InitStruct Config;
    AL_CAN_CallBackStruct CallBack;
#ifdef BOARD_DR1X90_AD101_V10
    AL_U32 DeviceId = 0;
#else
    AL_U32 DeviceId = 1;
#endif
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 1000;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd STB fifo example\r\n");

    Config.OpsMode      = AL_CAN_MODE_NORMAL;
    Config.RunMode      = AL_CAN_RUN_INTR;
    Config.Type         = AL_CAN_TYPE_FD;
    Config.SlowBitRate  = AL_CAN_ARBITRATION_0_5M,
    Config.FastBitRate  = AL_CAN_1_M,
    Config.TransMode    = AL_CAN_TRANS_STB_FIFO;
    Config.RbAfwl       = AL_CAN_RB_LIMIT_8;

    Ret = AlCan_Hal_Init(&Handle, DeviceId, AL_NULL, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init Error:0x%x\r\n", Ret);
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame1, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame1 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame3, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame3 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame5, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame5 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame7, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame7 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame9, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame9 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame11, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame11 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame13, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame13 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame15, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame15 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame16, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame16 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame17, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame17 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame18, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame18 example\r\n");
        AlSys_MDelay(500);
        Ret = AlCan_Hal_SendFrameBlock(&Handle, &FdFrame19, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame1 Error:0x%x\r\n", Ret);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can Fd Frame19 example\r\n");
        AlSys_MDelay(500);
    }
}

static AL_VOID AlCan_Test_DmaCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_DMACAHB_HalStruct *Handle = (AL_DMACAHB_HalStruct *)CallBackRef;

    switch (Event->EventId)
    {
    case AL_DMACAHB_EVENT_TRANS_READY:
        Handle->CurMode = Handle->ReqMode;
        break;
    case AL_DMACAHB_EVENT_TRANS_COMP:
    case AL_DMACAHB_EVENT_BLOCK_TRANS_COMP:
        Al_OSAL_Mb_Send(&Handle->EventQueue, Event);
        break;
    case AL_DMACAHB_EVENT_SRC_TRANS_COMP:
    case AL_DMACAHB_EVENT_DST_TRANS_COMP:
        /* Not use, do nothing here */
        break;
    case AL_DMACAHB_EVENT_ERR:
        /* ???????? */
        break;
    case AL_DMACAHB_EVENT_RELOAD:{
        AL_CAN_FrameStruct Frame;
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Dmacahb trans can frame count:%d!\r\n", Handle->Channel->Trans.ReloadCount);
        AlCan_Dev_DecodeFrame((AL_U32 *)(AL_UINTPTR)Handle->Channel->Trans.DstAddr, &Frame);
        AlCan_Dev_DisplayFrame(&Frame);
        break;
    }
    default:
        break;
    }
}

static AL_VOID AlCan_Test_FdDmaRecv(AL_VOID)
{
    AL_CAN_HalStruct Handle;
    AL_CAN_InitStruct Config;
    AL_CAN_FrameStruct Frame;
#ifdef BOARD_DR1X90_AD101_V10
    AL_U32 DeviceId = 0;
#else
    AL_U32 DeviceId = 1;
#endif
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 1000;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Can fd dma example\r\n");
    Config.OpsMode      = AL_CAN_MODE_NORMAL;
    Config.RunMode      = AL_CAN_RUN_INTR_DMA;
    Config.Type         = AL_CAN_TYPE_FD;
    Config.SlowBitRate  = AL_CAN_ARBITRATION_0_5M;
    Config.FastBitRate  = AL_CAN_1_M;
    Config.TransMode    = AL_CAN_TRANS_PTB;
    Config.RbAfwl       = AL_CAN_RB_LIMIT_8;

    Ret = AlCan_Hal_Init(&Handle, DeviceId, &Config, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Can hal Init error:0x%x\r\n", Ret);
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    AL_DMACAHB_HalStruct        DmacHandle;
    AL_DMACAHB_ChInitStruct     DmacChConfig;
    AL_DMACAHB_ChTransStruct    *DmacChTrans;
    AL_DMACAHB_ChCallBackStruct DmacCallBack;
    AL_U32                      DmacDevId = 0;

    DmacChConfig = CanDmaChExample1;

    DmacCallBack.Func = AlCan_Test_DmaCallBack;
    DmacCallBack.Ref = &DmacHandle;

    Ret = AlDmacAhb_Hal_Init(&DmacHandle, &DmacChConfig, &DmacCallBack, DmacDevId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dmacahb hal Init error:0x%x\r\n", Ret);
    }

    DmacChTrans = &(DmacHandle.Channel->Trans);
    DmacChTrans->SrcAddr        = Handle.Dev->BaseAddr;
    DmacChTrans->DstAddr        = (AL_REG)AL_CAN_TEST_DST_MEM;
    DmacChTrans->TransSize      = AL_CAN_DMAC_RECV_DATA_IN_WORD;
    DmacChTrans->ReloadCountNum = AL_CAN_DMAC_RELOAD_COUNT_MAX;   /* max AL_U32 for trans forever */

    // AlDmacAhb_Dev_Start(DmacHandle.Channel);
    AlDmacAhb_Hal_Start(&DmacHandle);

    while (1) {
    }
}