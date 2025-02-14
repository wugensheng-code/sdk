/* ptpd.c */

#include "ptpd.h"


// Statically allocated run-time configuration data.
RunTimeOpts rtOpts;
PtpClock ptpClock;
ForeignMasterRecord ptpForeignRecords[DEFAULT_MAX_FOREIGN_RECORDS];

void ptpd_periodic_handle()
{
    static unsigned int old_localtime = 0;

    unsigned int localtime = sys_now();

    catchAlarm(localtime - old_localtime);
    old_localtime = localtime;

    do
    {
        doState(&ptpClock);
    }
    while (netSelect(&ptpClock.netPath, 0) > 0);
}

void ptpd_alert(void)
{
    return;
}

void ptpd_init(ptp_role role)
{
    if (role == PTP_ROLE_MASTER)
    {
        // Initialize run-time options to default values.
        rtOpts.announceInterval = DEFAULT_ANNOUNCE_INTERVAL;
        rtOpts.syncInterval = DEFAULT_SYNC_INTERVAL;
        rtOpts.clockQuality.clockAccuracy = DEFAULT_CLOCK_ACCURACY;
        rtOpts.clockQuality.clockClass = DEFAULT_CLOCK_CLASS;
        rtOpts.clockQuality.offsetScaledLogVariance = DEFAULT_CLOCK_VARIANCE; /* 7.6.3.3 */
        rtOpts.priority1 = DEFAULT_PRIORITY1;
        rtOpts.priority2 = DEFAULT_PRIORITY2;
        rtOpts.domainNumber = DEFAULT_DOMAIN_NUMBER;
        rtOpts.slaveOnly = FALSE;
        rtOpts.currentUtcOffset = DEFAULT_UTC_OFFSET;
        rtOpts.servo.noResetClock = DEFAULT_NO_RESET_CLOCK;
        rtOpts.servo.noAdjust = NO_ADJUST;
        rtOpts.inboundLatency.nanoseconds = DEFAULT_INBOUND_LATENCY;
        rtOpts.outboundLatency.nanoseconds = DEFAULT_OUTBOUND_LATENCY;
        rtOpts.servo.sDelay = DEFAULT_DELAY_S;
        rtOpts.servo.sOffset = DEFAULT_OFFSET_S;
        rtOpts.servo.ap = DEFAULT_AP;
        rtOpts.servo.ai = DEFAULT_AI;
        rtOpts.maxForeignRecords = sizeof(ptpForeignRecords) / sizeof(ptpForeignRecords[0]);
        rtOpts.stats = PTP_TEXT_STATS;
        rtOpts.delayMechanism = DEFAULT_DELAY_MECHANISM;
    }
    else
    {
        rtOpts.announceInterval = DEFAULT_ANNOUNCE_INTERVAL;
        rtOpts.syncInterval = DEFAULT_SYNC_INTERVAL;
        rtOpts.clockQuality.clockAccuracy = DEFAULT_CLOCK_ACCURACY;
        rtOpts.clockQuality.clockClass = DEFAULT_CLOCK_CLASS;
        rtOpts.clockQuality.offsetScaledLogVariance = DEFAULT_CLOCK_VARIANCE; /* 7.6.3.3 */
        rtOpts.priority1 = DEFAULT_PRIORITY1;
        rtOpts.priority2 = DEFAULT_PRIORITY2;
        rtOpts.domainNumber = DEFAULT_DOMAIN_NUMBER;
        rtOpts.slaveOnly = SLAVE_ONLY;
        rtOpts.currentUtcOffset = DEFAULT_UTC_OFFSET;
        rtOpts.servo.noResetClock = DEFAULT_NO_RESET_CLOCK;
        rtOpts.servo.noAdjust = NO_ADJUST;
        rtOpts.inboundLatency.nanoseconds = DEFAULT_INBOUND_LATENCY;
        rtOpts.outboundLatency.nanoseconds = DEFAULT_OUTBOUND_LATENCY;
        rtOpts.servo.sDelay = DEFAULT_DELAY_S;
        rtOpts.servo.sOffset = DEFAULT_OFFSET_S;
        rtOpts.servo.ap = DEFAULT_AP;
        rtOpts.servo.ai = DEFAULT_AI;
        rtOpts.maxForeignRecords = sizeof(ptpForeignRecords) / sizeof(ptpForeignRecords[0]);
        rtOpts.stats = PTP_TEXT_STATS;
        rtOpts.delayMechanism = DEFAULT_DELAY_MECHANISM;
    }

    // Initialize run time options.
    if (ptpdStartup(&ptpClock, &rtOpts, ptpForeignRecords) != 0)
    {
        printf("PTPD: startup failed");
        return;
    }
}

