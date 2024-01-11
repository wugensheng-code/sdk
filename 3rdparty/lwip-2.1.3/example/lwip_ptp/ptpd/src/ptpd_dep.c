#include "ptpd_dep.h"
#include "al_gbe_hal.h"

extern AL_GBE_HalStruct *GbeHandle;

void ETH_PTPTime_GetTime(struct ptptime_t * timestamp)
{
    AL_GBE_PtpTimeStruct Time;

    AlGbe_Hal_GetPtpTimestamp(GbeHandle, &Time);

    timestamp->tv_sec = Time.Sec;
    timestamp->tv_nsec = Time.Nsec;
}

/*******************************************************************************
* Function Name  : ETH_PTPTimeStampSetTime
* Description    : Initialize time base
* Input          : Time with sign
* Output         : None
* Return         : None
*******************************************************************************/
void ETH_PTPTime_SetTime(struct ptptime_t * timestamp)
{
    AL_GBE_PtpTimeStruct Time;

    Time.Sign = 0;
    Time.Sec = timestamp->tv_sec;
    Time.Nsec = timestamp->tv_nsec;

    AlGbe_Hal_SetPtpTimestamp(GbeHandle, &Time);
}

/*******************************************************************************
* Function Name  : ETH_PTPTimeStampUpdateOffset
* Description    : Updates time base offset
* Input          : Time offset with sign
* Output         : None
* Return         : None
*******************************************************************************/
void ETH_PTPTime_UpdateOffset(struct ptptime_t * time)
{
    AL_GBE_PtpTimeStruct TimeOffset;

    if(time->tv_sec < 0 || (time->tv_sec == 0 && time->tv_nsec < 0)) {
        TimeOffset.Sign = 1;
        TimeOffset.Sec = -time->tv_sec;
        TimeOffset.Nsec = -time->tv_nsec;
    } else {
        TimeOffset.Sign = 0;
        TimeOffset.Sec = time->tv_sec;
        TimeOffset.Nsec = time->tv_nsec;
    }


    /* Coarse update method */
    AlGbe_Hal_UpdatePtpTimeOffset(GbeHandle, &TimeOffset);
}

/*******************************************************************************
* Function Name  : ETH_PTPTimeStampAdjFreq
* Description    : Updates time stamp addend register
* Input          : Correction value in thousandth of ppm (Adj*10^9)
* Output         : None
* Return         : None
*******************************************************************************/
void ETH_PTPTime_AdjFreq(int32_t ppb)
{
    uint32_t addend;

    uint32_t diff;
    int neg_adj = 0;
    uint64_t adj;

    if (ppb < 0) {
        neg_adj = 1;
        ppb = -ppb;
    }

    addend = GbeHandle->Dev.PtpConfig.DefaultAddend;
    adj = addend;
    adj *= ppb;
    diff = adj / AL_GBE_ONE_SEC_IN_NANOSEC;
    addend = neg_adj ? (addend - diff) : (addend + diff);

    AlGbe_Hal_AdjustPtpTimeFreq(GbeHandle, addend);
}