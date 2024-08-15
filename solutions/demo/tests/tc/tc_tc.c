#include <common.h>
#include <string.h>
#include <stdlib.h>
#include "al_tc_hal.h"

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_TC_DEVICE_ID             (3)
 /* pwm frequency = clock frequency / pow(2, PrescaleVal + 1) / (AL_TTC_INTERVAL_MAX_VAL + 1) */
#define AL_TC_INTERVAL_MAX_VAL      (999)
/* pwm duty cycle = (AL_TTC_MATCH1_VAL + 1) / (AL_TTC_INTERVAL_MAX_VAL + 1) */
#define AL_TC_MATCH1_VAL            (249)

/*******************************************************************************/
/**************************** al_ttc_measure ***********************************/
/*******************************************************************************/


/************************** Variable Definitions *****************************/
static AL_TC_TimerInitStruct EventInitConfigs = {
    .CountDec            = AL_TC_CountUp,
    .ClkSrc              = AL_TC_EXTCLK,
    .EnablePrescale      = AL_TRUE,
    .PrescaleVal         = 2,
};

static AL_VOID AlTc_Test_MeasurePulseWidth(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_TC_HalStruct *TcHandle;
    TickType_t Start = xTaskGetTickCount();

    Ret = AlTc_Hal_Init(&TcHandle, AL_TC_DEVICE_ID, &EventInitConfigs, AL_NULL);
    assert_int_equal(Ret, AL_OK);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }
    
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    AlTc_Hal_SetExtClkEdge(TcHandle, AL_TC_Negedge);
    AlTc_Hal_SetEventTimerOvState(TcHandle, AL_TRUE);
    AlTc_Hal_SetEventTimerLevel(TcHandle, AL_TC_LowLevel);
    AlTc_Hal_EnableEventTimerMode(TcHandle, AL_TRUE);
    AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrEventTimer, AL_TRUE);
    AlTc_Hal_EnableCounter(TcHandle, AL_TRUE);

    while ((xTaskGetTickCount() - Start) < (pdMS_TO_TICKS(25000))) {
        AL_U16 Val = AlTc_Hal_GetEventTimerVal(TcHandle);
        AL_LOG(AL_LOG_LEVEL_INFO, "Event val is %x\r\n", Val);
    }
}

/*******************************************************************************/
/**************************** al_ttc_measure ***********************************/
/*******************************************************************************/
static AL_TC_TimerInitStruct PwmInitConfigs = {
    .CountDec            = AL_TC_CountUp,
    .ClkSrc              = AL_TC_PCLK,
    .EnablePrescale      = AL_TRUE,
    .PrescaleVal         = 2,
};

static AL_VOID AlTc_Test_PwmOutput(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_TC_HalStruct *TcHandle;
    TickType_t Start = xTaskGetTickCount();

    Ret = AlTc_Hal_Init(&TcHandle, AL_TC_DEVICE_ID, &PwmInitConfigs, AL_NULL);
    assert_int_equal(Ret, AL_OK);
    while ((xTaskGetTickCount() - Start) < (pdMS_TO_TICKS(25000))){
   AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlTc_Hal_EnableIntervalMode(TcHandle);
    AlTc_Hal_SetIntervalMaxVal(TcHandle, AL_TC_INTERVAL_MAX_VAL);

    AlTc_Hal_EnableMatchMode(TcHandle, AL_TRUE);
    AlTc_Hal_SetMatchVal(TcHandle, AL_TC_Match1, AL_TC_MATCH1_VAL);

    AlTc_Hal_SetWaveformPolarity(TcHandle, AL_TC_Negedge);

    AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrInterval, AL_TRUE);
    AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrMatch1, AL_TRUE);

    AlTc_Hal_EnableWaveOutput(TcHandle, AL_TRUE);
    AlTc_Hal_EnableCounter(TcHandle, AL_TRUE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Tc pwm output in progress\r\n");
    }

}

CMOCKA_TC_DEFINE(AlTc_Test_MeasurePulseWidth, NULL, NULL, NULL);

CMOCKA_TC_DEFINE(AlTc_Test_PwmOutput, NULL, NULL, NULL);

