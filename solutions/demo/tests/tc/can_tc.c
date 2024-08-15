#include <common.h>

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_CAN_EX_DEVICE_ID             (0)
#define AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS (100)
#define AL_CAN_EX_MEM_SIZE              (1)
#define AL_CAN_EX_ARRAY_SIZE            (256)

static AL_CAN_InitStruct InLoopbackConfig = {
    .OpsMode        = AL_CAN_MODE_IN_LOOPBACK,
    .RunMode        = AL_CAN_RUN_INTR,
    .Type           = AL_CAN_TYPE_2_0B,
    .SlowBitRate    = AL_CAN_ARBITRATION_0_25M,
    .TransMode      = AL_CAN_TRANS_PTB,
    .RbAfwl         = AL_CAN_RB_LIMIT_8
};

static AL_CAN_FrameStruct ExFrame = {
    .Id             = 0x10,
    .IsBitSwitch    = 0x0,
    .IsEnTts        = 0x0,
    .IsIdExt        = 0x0,
    .IsRemote       = 0x0,
    .DataLen        = AL_CAN_LEN_8,
    .Data           = {0x01020304, 0x05060708}
};


static void AlCan_Test_InLoopbackBlocked_Tc(void **state)
{
    AL_U32 Ret = AL_OK;
    AL_CAN_FrameStruct Frame;
    AL_CAN_HalStruct *Handle = AL_NULL;

    Ret = AlCan_Hal_Init(&Handle, AL_CAN_EX_DEVICE_ID, &InLoopbackConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    TickType_t Start = xTaskGetTickCount();

    while ((xTaskGetTickCount() - Start) < pdMS_TO_TICKS(2000))
    {
        Ret = AlCan_Hal_SendFrameBlock(Handle, &ExFrame, AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS);
        assert_int_equal(Ret, AL_OK);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Send Frame Error:0x%x\r\n", Ret);
        }

        Ret = AlCan_Hal_RecvFrameBlock(Handle, &Frame, AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS);
        assert_int_equal(Ret, AL_OK);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Recv Frame Error:0x%x\r\n", Ret);
        }

        AlCan_Dev_DisplayFrame(&Frame);

        Ret = memcmp(ExFrame.Data, Frame.Data, AL_CAN_LEN_8);
        assert_int_equal(Ret, AL_OK);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
            return Ret;
        }

            vTaskDelay(pdMS_TO_TICKS(500));
    }
    
}

CMOCKA_TC_DEFINE(AlCan_Test_InLoopbackBlocked_Tc, NULL, NULL, NULL);
