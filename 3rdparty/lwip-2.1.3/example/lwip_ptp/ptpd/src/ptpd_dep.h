#ifndef PTPD_DEP_H_
#define PTPD_DEP_H_

#include "datatypes_dep.h"
#include "../datatypes.h"

//#define PTPD_DBGVV
//#define PTPD_DBGV
#define PTPD_DBG
//#define PTPD_ERR


/** \name Debug messages */
/**\{*/
#ifdef PTPD_DBGVV
#define PTPD_DBGV
#define PTPD_DBG
#define PTPD_ERR
#define DBGVV(...) printf("(V) " __VA_ARGS__)
#else
#define DBGVV(...)
#endif

#ifdef PTPD_DBGV
#define PTPD_DBG
#define PTPD_ERR
#define DBGV(...)  { TimeInternal tmpTime; getTime(&tmpTime); printf("(d %d.%09d) ", tmpTime.seconds, tmpTime.nanoseconds); printf(__VA_ARGS__); }
#else
#define DBGV(...)
#endif

#ifdef PTPD_DBG
#define PTPD_ERR
#define DBG(...)  { TimeInternal tmpTime; getTime(&tmpTime); printf("(D %d.%09d) ", tmpTime.seconds, tmpTime.nanoseconds); printf(__VA_ARGS__); }
#else
#define DBG(...)
#endif
/** \}*/

/** \name System messages */
/**\{*/
#ifdef PTPD_ERR
#define ERROR(...)  { TimeInternal tmpTime; getTime(&tmpTime); printf("(E %d.%09d) ", tmpTime.seconds, tmpTime.nanoseconds); printf(__VA_ARGS__); }
/* #define ERROR(...)  { printf("(E) "); printf(__VA_ARGS__); } */
#else
#define ERROR(...)
#endif
/** \}*/

/** \name Endian corrections */
/**\{*/

#if defined(PTPD_MSBF)
#define shift8(x,y)   ( (x) << ((3-y)<<3) )
#define shift16(x,y)  ( (x) << ((1-y)<<4) )
#elif defined(PTPD_LSBF)
#define shift8(x,y)   ( (x) << ((y)<<3) )
#define shift16(x,y)  ( (x) << ((y)<<4) )
#endif

#define flip16(x) htons(x)
#define flip32(x) htonl(x)


struct ptptime_t {
    int32_t tv_sec;
    int32_t tv_nsec;
};

#if 1

/**--------------------------------------------------------------------------**/
/**
  * @brief                           Ethernet PTP defines
  */
/**--------------------------------------------------------------------------**/
/**
  * @}
  */

/** @defgroup ETH_PTP_time_update_method
  * @{
  */

/**
  * @}
  */


/** @defgroup ETH_PTP_Flags
  * @{
  */
#define ETH_PTP_FLAG_TSARU        ((uint32_t)0x00000020)  /*!< Addend Register Update */
#define ETH_PTP_FLAG_TSSTU        ((uint32_t)0x00000008)  /*!< Time Stamp Update */
#define ETH_PTP_FLAG_TSSTI        ((uint32_t)0x00000004)  /*!< Time Stamp Initialize */

#define ETH_PTP_FLAG_TSTTR        ((uint32_t)0x10000002)  /* Time stamp target time reached */
#define ETH_PTP_FLAG_TSSO         ((uint32_t)0x10000001)  /* Time stamp seconds overflow */

#define IS_ETH_PTP_GET_FLAG(FLAG) (((FLAG) == ETH_PTP_FLAG_TSARU) || \
                                    ((FLAG) == ETH_PTP_FLAG_TSSTU) || \
                                    ((FLAG) == ETH_PTP_FLAG_TSSTI) || \
                                    ((FLAG) == ETH_PTP_FLAG_TSTTR) || \
                                    ((FLAG) == ETH_PTP_FLAG_TSSO))

/**
  * @brief  ETH PTP subsecond increment
  */
#define IS_ETH_PTP_SUBSECOND_INCREMENT(SUBSECOND) ((SUBSECOND) <= 0xFF)

/** @defgroup ETH_PTP_time_sign
  * @{
  */
#define ETH_PTP_PositiveTime      ((uint32_t)0x00000000)  /*!< Positive time value */
#define ETH_PTP_NegativeTime      ((uint32_t)0x80000000)  /*!< Negative time value */
#define IS_ETH_PTP_TIME_SIGN(SIGN) (((SIGN) == ETH_PTP_PositiveTime) || \
                                    ((SIGN) == ETH_PTP_NegativeTime))

/**
  * @brief  ETH PTP time stamp low update
  */
#define IS_ETH_PTP_TIME_STAMP_UPDATE_SUBSECOND(SUBSECOND) ((SUBSECOND) <= 0x7FFFFFFF)

/**
  * @brief  ETH PTP PPS frequency check
  */
#define IS_PPS_FREQ(FREQUENCY) ((FREQUENCY) <= 0x0F)

void ETH_PTPTime_SetTime(struct ptptime_t * timestamp);
void ETH_PTPTime_GetTime(struct ptptime_t * timestamp);
void ETH_PTPTime_UpdateOffset(struct ptptime_t * timeoffset);
void ETH_PTPTime_AdjFreq(int32_t Adj);
void ETH_PTPStart(uint32_t UpdateMethod);
void ETH_EnablePTPTimeStampAddend(void);
void ETH_EnablePTPTimeStampInterruptTrigger(void);
void ETH_EnablePTPTimeStampUpdate(void);
void ETH_InitializePTPTimeStamp(void);
void ETH_PTPSetPPSFreq(uint8_t Freq);
void ETH_PTPUpdateMethodConfig(uint32_t UpdateMethod);
void ETH_PTPTimeStampCmd(uint8_t NewState);
uint8_t ETH_GetPTPFlagStatus(uint32_t ETH_PTP_FLAG);
void ETH_SetPTPSubSecondIncrement(uint32_t SubSecondValue);
void ETH_SetPTPTimeStampUpdate(uint32_t Sign, uint32_t SecondValue, uint32_t SubSecondValue);
void ETH_SetPTPTimeStampAddend(uint32_t Value);
void ETH_SetPTPTargetTime(uint32_t HighValue, uint32_t LowValue);

/* i don't know any target platforms that do not have htons and htonl,
     but here are generic funtions just in case */
/*
#if defined(PTPD_MSBF)
#define flip16(x) (x)
#define flip32(x) (x)
#elif defined(PTPD_LSBF)
static inline int16_t flip16(int16_t x)
{
     return (((x) >> 8) & 0x00ff) | (((x) << 8) & 0xff00);
}

static inline int32_t flip32(x)
{
    return (((x) >> 24) & 0x000000ff) | (((x) >> 8 ) & 0x0000ff00) |
                 (((x) << 8 ) & 0x00ff0000) | (((x) << 24) & 0xff000000);
}
#endif
*/

/** \}*/


#endif

/** \name Bit array manipulations */
/**\{*/
#define getFlag(flagField, mask) (bool)(((flagField)  & (mask)) == (mask))
#define setFlag(flagField, mask) (flagField) |= (mask)
#define clearFlag(flagField, mask) (flagField) &= ~(mask)
/* #define getFlag(x,y)  (bool)!!(  *(uint8_t*)((x)+((y)<8?1:0)) &   (1<<((y)<8?(y):(y)-8)) ) */
/* #define setFlag(x,y)    ( *(uint8_t*)((x)+((y)<8?1:0)) |=   1<<((y)<8?(y):(y)-8)  ) */
/* #define clearFlag(x,y)  ( *(uint8_t*)((x)+((y)<8?1:0)) &= ~(1<<((y)<8?(y):(y)-8)) ) */
/** \}*/

/** \name msg.c
 *-Pack and unpack PTP messages */
/**\{*/

void msgUnpackHeader(const octet_t*, MsgHeader*);
void msgUnpackAnnounce(const octet_t*, MsgAnnounce*);
void msgUnpackSync(const octet_t*, MsgSync*);
void msgUnpackFollowUp(const octet_t*, MsgFollowUp*);
void msgUnpackDelayReq(const octet_t*, MsgDelayReq*);
void msgUnpackDelayResp(const octet_t*, MsgDelayResp*);
void msgUnpackPDelayReq(const octet_t*, MsgPDelayReq*);
void msgUnpackPDelayResp(const octet_t*, MsgPDelayResp*);
void msgUnpackPDelayRespFollowUp(const octet_t*, MsgPDelayRespFollowUp*);
void msgUnpackManagement(const octet_t*, MsgManagement*);
void msgUnpackManagementPayload(const octet_t *buf, MsgManagement *manage);
void msgPackHeader(const PtpClock*, octet_t*);
void msgPackAnnounce(const PtpClock*, octet_t*);
void msgPackSync(const PtpClock*, octet_t*, const Timestamp*);
void msgPackFollowUp(const PtpClock*, octet_t*, const Timestamp*);
void msgPackDelayReq(const PtpClock*, octet_t*, const Timestamp*);
void msgPackDelayResp(const PtpClock*, octet_t*, const MsgHeader*, const Timestamp*);
void msgPackPDelayReq(const PtpClock*, octet_t*, const Timestamp*);
void msgPackPDelayResp(octet_t*, const MsgHeader*, const Timestamp*);
void msgPackPDelayRespFollowUp(octet_t*, const MsgHeader*, const Timestamp*);
int16_t msgPackManagement(const PtpClock*,  octet_t*, const MsgManagement*);
int16_t msgPackManagementResponse(const PtpClock*,  octet_t*, MsgHeader*, const MsgManagement*);
/** \}*/

/** \name net.c (Linux API dependent)
 * -Init network stuff, send and receive datas */
/**\{*/

bool  netInit(NetPath*, PtpClock*);
bool  netShutdown(NetPath*);
int32_t netSelect(NetPath*, const TimeInternal*);
ssize_t netRecvEvent(NetPath*, octet_t*, TimeInternal*);
ssize_t netRecvGeneral(NetPath*, octet_t*, TimeInternal*);
ssize_t netSendEvent(NetPath*, const octet_t*, int16_t, TimeInternal*);
ssize_t netSendGeneral(NetPath*, const octet_t*, int16_t);
ssize_t netSendPeerGeneral(NetPath*, const octet_t*, int16_t);
ssize_t netSendPeerEvent(NetPath*, const octet_t*, int16_t, TimeInternal*);
void netEmptyEventQ(NetPath *netPath);
/** \}*/

/** \name servo.c
 * -Clock servo */
/**\{*/

void initClock(PtpClock*);
void updatePeerDelay(PtpClock*, const TimeInternal*, bool);
void updateDelay(PtpClock*, const TimeInternal*, const TimeInternal*, const TimeInternal*);
void updateOffset(PtpClock *, const TimeInternal*, const TimeInternal*, const TimeInternal*);
void updateClock(PtpClock*);
/** \}*/

/** \name startup.c (Linux API dependent)
 * -Handle with runtime options */
/**\{*/
int16_t ptpdStartup(PtpClock*, RunTimeOpts*, ForeignMasterRecord*);
void ptpdShutdown(PtpClock *);
/** \}*/

/** \name sys.c (Linux API dependent)
 * -Manage timing system API */
/**\{*/
void displayStats(const PtpClock *ptpClock);
bool  nanoSleep(const TimeInternal*);
void getTime(TimeInternal*);
void setTime(const TimeInternal*);
void updateTime(const TimeInternal*);
bool  adjFreq(int32_t);
uint32_t getRand(uint32_t);
/** \}*/

/** \name timer.c (Linux API dependent)
 * -Handle with timers */
/**\{*/
#if 0
void initTimer(void);
void timerStop(int32_t);
void timerStart(int32_t,  uint32_t);
bool timerExpired(int32_t);
#else

void catchAlarm(UInteger32 time_ms);

void initTimer(PtpClock *ptpClock);

void timerStop(UInteger16 index);

void timerUpdate(IntervalTimer *itimer);

void timerStart(UInteger16 index, UInteger32 interval_ms);

char timerExpired(UInteger16 index);
#endif
/** \}*/


/* Test functions */


#endif /* PTPD_DEP_H_*/
