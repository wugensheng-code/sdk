/*
 * Copyright (c) 2011-2012 George V. Neville-Neil,
 *                         Steven Kreuzer, 
 *                         Martin Burnicki, 
 *                         Jan Breuer,
 *                         Gael Mace, 
 *                         Alexandre Van Kempen,
 *                         Inaqui Delgado,
 *                         Rick Ratzel,
 *                         National Instruments.
 * Copyright (c) 2009-2010 George V. Neville-Neil, 
 *                         Steven Kreuzer, 
 *                         Martin Burnicki, 
 *                         Jan Breuer,
 *                         Gael Mace, 
 *                         Alexandre Van Kempen
 *
 * Copyright (c) 2005-2008 Kendall Correll, Aidan Williams
 *
 * All Rights Reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   display.c
 * @date   Thu Aug 12 09:06:21 2010
 * 
 * @brief  General routines for displaying internal data.
 * 
 * 
 */

#include "ptpd.h"

/**\brief Display an Integer64 type*/
void
integer64_display(Integer64 * bigint)
{
	DBGV("Integer 64 : \r\n");
	DBGV("LSB : %u\r\n", bigint->lsb);
	DBGV("MSB : %d\r\n", bigint->msb);
}

/**\brief Display an UInteger48 type*/
void
uInteger48_display(UInteger48 * bigint)
{
	DBGV("Integer 48 : \r\n");
	DBGV("LSB : %u\r\n", bigint->lsb);
	DBGV("MSB : %u\r\n", bigint->msb);
}

/** \brief Display a TimeInternal Structure*/
void
timeInternal_display(TimeInternal * timeInternal)
{
	DBGV("seconds : %d \r\n", timeInternal->seconds);
	DBGV("nanoseconds %d \r\n", timeInternal->nanoseconds);
}

/** \brief Display a Timestamp Structure*/
void
timestamp_display(Timestamp * timestamp)
{
	uInteger48_display(&timestamp->secondsField);
	DBGV("nanoseconds %u \r\n", timestamp->nanosecondsField);
}

/**\brief Display a Clockidentity Structure*/
void
clockIdentity_display(ClockIdentity clockIdentity)
{
	DBGV(
	    "ClockIdentity : %02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx\r\n",
	    clockIdentity[0], clockIdentity[1], clockIdentity[2],
	    clockIdentity[3], clockIdentity[4], clockIdentity[5],
	    clockIdentity[6], clockIdentity[7]
	);
}

/**\brief Display MAC address*/
void
clockUUID_display(Octet * sourceUuid)
{
	DBGV(
	    "sourceUuid %02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx\r\n",
	    sourceUuid[0], sourceUuid[1], sourceUuid[2], sourceUuid[3],
	    sourceUuid[4], sourceUuid[5]
	);
}

/**\brief Display Network info*/
void
netPath_display(NetPath * net)
{
	struct in_addr addr;

	DBGV("eventSock : %d \r\n", net->eventSock);
	DBGV("generalSock : %d \r\n", net->generalSock);
	addr.s_addr = net->multicastAddr;
	DBGV("multicastAdress : %s \r\n", inet_ntoa(addr));
	addr.s_addr = net->peerMulticastAddr;
	DBGV("peerMulticastAddress : %s \r\n", inet_ntoa(addr));
	addr.s_addr = net->unicastAddr;
	DBGV("unicastAddress : %s \r\n", inet_ntoa(addr));
}

/**\brief Display a IntervalTimer Structure*/
void
intervalTimer_display(IntervalTimer * ptimer)
{
	DBGV("interval : %d \r\n", ptimer->interval);
	DBGV("left : %d \r\n", ptimer->left);
	DBGV("expire : %d \r\n", ptimer->expire);
}

/**\brief Display a TimeInterval Structure*/
void
timeInterval_display(TimeInterval * timeInterval)
{
	integer64_display(&timeInterval->scaledNanoseconds);
}

/**\brief Display a Portidentity Structure*/
void
portIdentity_display(PortIdentity * portIdentity)
{
	clockIdentity_display(portIdentity->clockIdentity);
	DBGV("port number : %d \r\n", portIdentity->portNumber);
}

/**\brief Display a Clockquality Structure*/
void
clockQuality_display(ClockQuality * clockQuality)
{
	DBGV("clockClass : %d \r\n", clockQuality->clockClass);
	DBGV("clockAccuracy : %d \r\n", clockQuality->clockAccuracy);
	DBGV("offsetScaledLogVariance : %d \r\n", clockQuality->offsetScaledLogVariance);
}

/**\brief Display PTPText Structure*/
void
PTPText_display(PTPText *p, PtpClock *ptpClock)
{
	/* Allocate new memory to append null-terminator to the text field.
	 * This allows printing the textField as a string
	 */
	Octet *str;
	XMALLOC(str, p->lengthField + 1);
	memcpy(str, p->textField, p->lengthField);
	str[p->lengthField] = '\0';
	DBGV("    lengthField : %d \r\n", p->lengthField);
	DBGV("    textField : %s \r\n", str);
	free(str);
}

/**\brief Display the Network Interface Name*/
void
iFaceName_display(Octet * iFaceName)
{

	int i;

	DBGV("iFaceName : ");

	for (i = 0; i < IFACE_NAME_LENGTH; i++) {
		DBGV("%c", iFaceName[i]);
	}
	DBGV("\r\n");

}

/**\brief Display an Unicast Adress*/
void
unicast_display(Octet * unicast)
{

	int i;

	DBGV("Unicast adress : ");

	for (i = 0; i < NET_ADDRESS_LENGTH; i++) {
		DBGV("%c", unicast[i]);
	}
	DBGV("\r\n");

}


/**\brief Display Sync message*/
void
msgSync_display(MsgSync * sync)
{
	DBGV("Message Sync : \r\n");
	timestamp_display(&sync->originTimestamp);
	DBGV("\r\n");
}

/**\brief Display Header message*/
void
msgHeader_display(MsgHeader * header)
{
	DBGV("Message header : \r\n");
	DBGV("\r\n");
	DBGV("transportSpecific : %d\r\n", header->transportSpecific);
	DBGV("messageType : %d\r\n", header->messageType);
	DBGV("versionPTP : %d\r\n", header->versionPTP);
	DBGV("messageLength : %d\r\n", header->messageLength);
	DBGV("domainNumber : %d\r\n", header->domainNumber);
	DBGV("FlagField %02hhx:%02hhx\r\n", header->flagField0, header->flagField1);
	DBGV("CorrectionField : \r\n");
	integer64_display(&header->correctionField);
	DBGV("SourcePortIdentity : \r\n");
	portIdentity_display(&header->sourcePortIdentity);
	DBGV("sequenceId : %d\r\n", header->sequenceId);
	DBGV("controlField : %d\r\n", header->controlField);
	DBGV("logMessageInterval : %d\r\n", header->logMessageInterval);
	DBGV("\r\n");
}

/**\brief Display Announce message*/
void
msgAnnounce_display(MsgAnnounce * announce)
{
	DBGV("Announce Message : \r\n");
	DBGV("\r\n");
	DBGV("originTimestamp : \r\n");
	DBGV("secondField  : \r\n");
	timestamp_display(&announce->originTimestamp);
	DBGV("currentUtcOffset : %d \r\n", announce->currentUtcOffset);
	DBGV("grandMasterPriority1 : %d \r\n", announce->grandmasterPriority1);
	DBGV("grandMasterClockQuality : \r\n");
	clockQuality_display(&announce->grandmasterClockQuality);
	DBGV("grandMasterPriority2 : %d \r\n", announce->grandmasterPriority2);
	DBGV("grandMasterIdentity : \r\n");
	clockIdentity_display(announce->grandmasterIdentity);
	DBGV("stepsRemoved : %d \r\n", announce->stepsRemoved);
	DBGV("timeSource : %d \r\n", announce->timeSource);
	DBGV("\r\n");
}

/**\brief Display Follow_UP message*/
void
msgFollowUp_display(MsgFollowUp * follow)
{
	timestamp_display(&follow->preciseOriginTimestamp);
}

/**\brief Display DelayReq message*/
void
msgDelayReq_display(MsgDelayReq * req)
{
	timestamp_display(&req->originTimestamp);
}

/**\brief Display DelayResp message*/
void
msgDelayResp_display(MsgDelayResp * resp)
{
	timestamp_display(&resp->receiveTimestamp);
	portIdentity_display(&resp->requestingPortIdentity);
}

/**\brief Display Pdelay_Req message*/
void
msgPDelayReq_display(MsgPDelayReq * preq)
{
	timestamp_display(&preq->originTimestamp);
}

/**\brief Display Pdelay_Resp message*/
void
msgPDelayResp_display(MsgPDelayResp * presp)
{

	timestamp_display(&presp->requestReceiptTimestamp);
	portIdentity_display(&presp->requestingPortIdentity);
}

/**\brief Display Pdelay_Resp Follow Up message*/
void
msgPDelayRespFollowUp_display(MsgPDelayRespFollowUp * prespfollow)
{

	timestamp_display(&prespfollow->responseOriginTimestamp);
	portIdentity_display(&prespfollow->requestingPortIdentity);
}

/**\brief Display Management message*/
void
msgManagement_display(MsgManagement * manage)
{
        DBGV("Management Message : \r\n");
        DBGV("\r\n");
        DBGV("targetPortIdentity : \r\n");
	portIdentity_display(&manage->targetPortIdentity);
	DBGV("startingBoundaryHops : %d \r\n", manage->startingBoundaryHops);
	DBGV("boundaryHops : %d \r\n", manage->boundaryHops);
	DBGV("actionField : %d\r\n", manage->actionField);
}

/**\brief Display ManagementTLV Slave Only message*/
void
mMSlaveOnly_display(MMSlaveOnly *slaveOnly, PtpClock *ptpClock)
{
	DBGV("Slave Only ManagementTLV message \r\n");
	DBGV("SO : %d \r\n", slaveOnly->so);
}

/**\brief Display ManagementTLV Clock Description message*/
void
mMClockDescription_display(MMClockDescription *clockDescription, PtpClock *ptpClock)
{
	DBGV("Clock Description ManagementTLV message \r\n");
	DBGV("clockType0 : %d \r\n", clockDescription->clockType0);
	DBGV("clockType1 : %d \r\n", clockDescription->clockType1);
	DBGV("physicalLayerProtocol : \r\n");
	PTPText_display(&clockDescription->physicalLayerProtocol, ptpClock);
	DBGV("physicalAddressLength : %d \r\n", clockDescription->physicalAddress.addressLength);
	if(clockDescription->physicalAddress.addressField) {
		DBGV("physicalAddressField : \r\n");
		clockUUID_display(clockDescription->physicalAddress.addressField);
	}
	DBGV("protocolAddressNetworkProtocol : %d \r\n", clockDescription->protocolAddress.networkProtocol);
	DBGV("protocolAddressLength : %d \r\n", clockDescription->protocolAddress.addressLength);
	if(clockDescription->protocolAddress.addressField) {
		DBGV("protocolAddressField : %d.%d.%d.%d \r\n",
			(UInteger8)clockDescription->protocolAddress.addressField[0],
			(UInteger8)clockDescription->protocolAddress.addressField[1],
			(UInteger8)clockDescription->protocolAddress.addressField[2],
			(UInteger8)clockDescription->protocolAddress.addressField[3]);
	}
	DBGV("manufacturerIdentity0 : %d \r\n", clockDescription->manufacturerIdentity0);
	DBGV("manufacturerIdentity1 : %d \r\n", clockDescription->manufacturerIdentity1);
	DBGV("manufacturerIdentity2 : %d \r\n", clockDescription->manufacturerIdentity2);
	DBGV("productDescription : \r\n");
	PTPText_display(&clockDescription->productDescription, ptpClock);
	DBGV("revisionData : \r\n");
	PTPText_display(&clockDescription->revisionData, ptpClock);
	DBGV("userDescription : \r\n");
	PTPText_display(&clockDescription->userDescription, ptpClock);
	DBGV("profileIdentity0 : %d \r\n", clockDescription->profileIdentity0);
	DBGV("profileIdentity1 : %d \r\n", clockDescription->profileIdentity1);
	DBGV("profileIdentity2 : %d \r\n", clockDescription->profileIdentity2);
	DBGV("profileIdentity3 : %d \r\n", clockDescription->profileIdentity3);
	DBGV("profileIdentity4 : %d \r\n", clockDescription->profileIdentity4);
	DBGV("profileIdentity5 : %d \r\n", clockDescription->profileIdentity5);
}

void
mMUserDescription_display(MMUserDescription* userDescription, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMInitialize_display(MMInitialize* initialize, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMDefaultDataSet_display(MMDefaultDataSet* defaultDataSet, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMCurrentDataSet_display(MMCurrentDataSet* currentDataSet, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMParentDataSet_display(MMParentDataSet* parentDataSet, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMTimePropertiesDataSet_display(MMTimePropertiesDataSet* timePropertiesDataSet, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMPortDataSet_display(MMPortDataSet* portDataSet, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMPriority1_display(MMPriority1* priority1, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMPriority2_display(MMPriority2* priority2, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMDomain_display(MMDomain* domain, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMLogAnnounceInterval_display(MMLogAnnounceInterval* logAnnounceInterval, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMAnnounceReceiptTimeout_display(MMAnnounceReceiptTimeout* announceReceiptTimeout, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMLogSyncInterval_display(MMLogSyncInterval* logSyncInterval, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMVersionNumber_display(MMVersionNumber* versionNumber, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMTime_display(MMTime* time, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMClockAccuracy_display(MMClockAccuracy* clockAccuracy, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMUtcProperties_display(MMUtcProperties* utcProperties, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMTraceabilityProperties_display(MMTraceabilityProperties* traceabilityProperties, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMDelayMechanism_display(MMDelayMechanism* delayMechanism, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMLogMinPdelayReqInterval_display(MMLogMinPdelayReqInterval* logMinPdelayReqInterval, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

void
mMErrorStatus_display(MMErrorStatus* errorStatus, PtpClock *ptpClock)
{
	/* TODO: implement me */
}

/**\brief Display runTimeOptions structure*/
void
displayRunTimeOpts(RunTimeOpts * rtOpts)
{

	DBGV("---Run time Options Display-- \r\n");
	DBGV("\r\n");
	DBGV("announceInterval : %d \r\n", rtOpts->announceInterval);
	DBGV("syncInterval : %d \r\n", rtOpts->syncInterval);
	clockQuality_display(&(rtOpts->clockQuality));
	DBGV("priority1 : %d \r\n", rtOpts->priority1);
	DBGV("priority2 : %d \r\n", rtOpts->priority2);
	DBGV("domainNumber : %d \r\n", rtOpts->domainNumber);
	DBGV("slaveOnly : %d \r\n", rtOpts->slaveOnly);
	DBGV("currentUtcOffset : %d \r\n", rtOpts->currentUtcOffset);
	unicast_display(rtOpts->unicastAddress);
	DBGV("noAdjust : %d \r\n", rtOpts->noAdjust);
	DBGV("displayStats : %d \r\n", rtOpts->displayStats);
	iFaceName_display(rtOpts->ifaceName);
	DBGV("ap : %d \r\n", rtOpts->ap);
	DBGV("aI : %d \r\n", rtOpts->ai);
	DBGV("s : %d \r\n", rtOpts->s);
	DBGV("inbound latency : \r\n");
	timeInternal_display(&(rtOpts->inboundLatency));
	DBGV("outbound latency : \r\n");
	timeInternal_display(&(rtOpts->outboundLatency));
	DBGV("max_foreign_records : %d \r\n", rtOpts->max_foreign_records);
	DBGV("ethernet mode : %d \r\n", rtOpts->ethernet_mode);
	DBGV("\r\n");
}


/**\brief Display Default data set of a PtpClock*/
void
displayDefault(PtpClock * ptpClock)
{
	DBGV("---Ptp Clock Default Data Set-- \r\n");
	DBGV("\r\n");
	DBGV("twoStepFlag : %d \r\n", ptpClock->twoStepFlag);
	clockIdentity_display(ptpClock->clockIdentity);
	DBGV("numberPorts : %d \r\n", ptpClock->numberPorts);
	clockQuality_display(&(ptpClock->clockQuality));
	DBGV("priority1 : %d \r\n", ptpClock->priority1);
	DBGV("priority2 : %d \r\n", ptpClock->priority2);
	DBGV("domainNumber : %d \r\n", ptpClock->domainNumber);
	DBGV("slaveOnly : %d \r\n", ptpClock->slaveOnly);
	DBGV("\r\n");
}


/**\brief Display Current data set of a PtpClock*/
void
displayCurrent(PtpClock * ptpClock)
{
	DBGV("---Ptp Clock Current Data Set-- \r\n");
	DBGV("\r\n");

	DBGV("stepsremoved : %d \r\n", ptpClock->stepsRemoved);
	DBGV("Offset from master : \r\n");
	timeInternal_display(&ptpClock->offsetFromMaster);
	DBGV("Mean path delay : \r\n");
	timeInternal_display(&ptpClock->meanPathDelay);
	DBGV("\r\n");
}



/**\brief Display Parent data set of a PtpClock*/
void
displayParent(PtpClock * ptpClock)
{
	DBGV("---Ptp Clock Parent Data Set-- \r\n");
	DBGV("\r\n");
	portIdentity_display(&(ptpClock->parentPortIdentity));
	DBGV("parentStats : %d \r\n", ptpClock->parentStats);
	DBGV("observedParentOffsetScaledLogVariance : %d \r\n", ptpClock->observedParentOffsetScaledLogVariance);
	DBGV("observedParentClockPhaseChangeRate : %d \r\n", ptpClock->observedParentClockPhaseChangeRate);
	DBGV("--GrandMaster--\r\n");
	clockIdentity_display(ptpClock->grandmasterIdentity);
	clockQuality_display(&ptpClock->grandmasterClockQuality);
	DBGV("grandmasterpriority1 : %d \r\n", ptpClock->grandmasterPriority1);
	DBGV("grandmasterpriority2 : %d \r\n", ptpClock->grandmasterPriority2);
	DBGV("\r\n");
}

/**\brief Display Global data set of a PtpClock*/
void
displayGlobal(PtpClock * ptpClock)
{
	DBGV("---Ptp Clock Global Time Data Set-- \r\n");
	DBGV("\r\n");

	DBGV("currentUtcOffset : %d \r\n", ptpClock->currentUtcOffset);
	DBGV("currentUtcOffsetValid : %d \r\n", ptpClock->currentUtcOffsetValid);
	DBGV("leap59 : %d \r\n", ptpClock->leap59);
	DBGV("leap61 : %d \r\n", ptpClock->leap61);
	DBGV("timeTraceable : %d \r\n", ptpClock->timeTraceable);
	DBGV("frequencyTraceable : %d \r\n", ptpClock->frequencyTraceable);
	DBGV("ptpTimescale : %d \r\n", ptpClock->ptpTimescale);
	DBGV("timeSource : %d \r\n", ptpClock->timeSource);
	DBGV("\r\n");
}

/**\brief Display Port data set of a PtpClock*/
void
displayPort(PtpClock * ptpClock)
{
	DBGV("---Ptp Clock Port Data Set-- \r\n");
	DBGV("\r\n");

	portIdentity_display(&ptpClock->portIdentity);
	DBGV("port state : %d \r\n", ptpClock->portState);
	DBGV("logMinDelayReqInterval : %d \r\n", ptpClock->logMinDelayReqInterval);
	DBGV("peerMeanPathDelay : \r\n");
	timeInternal_display(&ptpClock->peerMeanPathDelay);
	DBGV("logAnnounceInterval : %d \r\n", ptpClock->logAnnounceInterval);
	DBGV("announceReceiptTimeout : %d \r\n", ptpClock->announceReceiptTimeout);
	DBGV("logSyncInterval : %d \r\n", ptpClock->logSyncInterval);
	DBGV("delayMechanism : %d \r\n", ptpClock->delayMechanism);
	DBGV("logMinPdelayReqInterval : %d \r\n", ptpClock->logMinPdelayReqInterval);
	DBGV("versionNumber : %d \r\n", ptpClock->versionNumber);
	DBGV("\r\n");
}

/**\brief Display ForeignMaster data set of a PtpClock*/
void
displayForeignMaster(PtpClock * ptpClock)
{

	ForeignMasterRecord *foreign;
	int i;

	if (ptpClock->number_foreign_records > 0) {

		DBGV("---Ptp Clock Foreign Data Set-- \r\n");
		DBGV("\r\n");
		DBGV("There is %d Foreign master Recorded \r\n", ptpClock->number_foreign_records);
		foreign = ptpClock->foreign;

		for (i = 0; i < ptpClock->number_foreign_records; i++) {

			portIdentity_display(&foreign->foreignMasterPortIdentity);
			DBGV("number of Announce message received : %d \r\n", foreign->foreignMasterAnnounceMessages);
			msgHeader_display(&foreign->header);
			msgAnnounce_display(&foreign->announce);

			foreign++;
		}

	} else {
		DBGV("No Foreign masters recorded \r\n");
	}

	DBGV("\r\n");


}

/**\brief Display other data set of a PtpClock*/

void
displayOthers(PtpClock * ptpClock)
{

	int i;

	/* Usefull to display name of timers */
#ifdef PTPD_DBGV
	    char timer[][26] = {
		"PDELAYREQ_INTERVAL_TIMER",
		"SYNC_INTERVAL_TIMER",
		"ANNOUNCE_RECEIPT_TIMER",
		"ANNOUNCE_INTERVAL_TIMER"
	};
#endif
	DBGV("---Ptp Others Data Set-- \r\n");
	DBGV("\r\n");

	/*DBGV("master_to_slave_delay : \r\n");
	timeInternal_display(&ptpClock->master_to_slave_delay);
	DBGV("\r\n");
	DBGV("slave_to_master_delay : \r\n");
	timeInternal_display(&ptpClock->slave_to_master_delay);
	*/
	
	DBGV("\r\n");
	DBGV("delay_req_receive_time : \r\n");
	timeInternal_display(&ptpClock->pdelay_req_receive_time);
	DBGV("\r\n");
	DBGV("delay_req_send_time : \r\n");
	timeInternal_display(&ptpClock->pdelay_req_send_time);
	DBGV("\r\n");
	DBGV("delay_resp_receive_time : \r\n");
	timeInternal_display(&ptpClock->pdelay_resp_receive_time);
	DBGV("\r\n");
	DBGV("delay_resp_send_time : \r\n");
	timeInternal_display(&ptpClock->pdelay_resp_send_time);
	DBGV("\r\n");
	DBGV("sync_receive_time : \r\n");
	timeInternal_display(&ptpClock->sync_receive_time);
	DBGV("\r\n");
	//DBGV("R : %f \r\n", ptpClock->R);
	DBGV("sentPdelayReq : %d \r\n", ptpClock->sentPDelayReq);
	DBGV("sentPDelayReqSequenceId : %d \r\n", ptpClock->sentPDelayReqSequenceId);
	DBGV("waitingForFollow : %d \r\n", ptpClock->waitingForFollow);
	DBGV("\r\n");
	DBGV("Offset from master filter : \r\n");
	DBGV("nsec_prev : %d \r\n", ptpClock->ofm_filt.nsec_prev);
	DBGV("y : %d \r\n", ptpClock->ofm_filt.y);
	DBGV("\r\n");
	DBGV("One way delay filter : \r\n");
	DBGV("nsec_prev : %d \r\n", ptpClock->owd_filt.nsec_prev);
	DBGV("y : %d \r\n", ptpClock->owd_filt.y);
	DBGV("s_exp : %d \r\n", ptpClock->owd_filt.s_exp);
	DBGV("\r\n");
	DBGV("observed_drift : %d \r\n", ptpClock->observed_drift);
	DBGV("message activity %d \r\n", ptpClock->message_activity);
	DBGV("\r\n");

	for (i = 0; i < TIMER_ARRAY_SIZE; i++) {
		DBGV("%s : \r\n", timer[i]);
		intervalTimer_display(&ptpClock->itimer[i]);
		DBGV("\r\n");
	}

	netPath_display(&ptpClock->netPath);
	DBGV("mCommunication technology %d \r\n", ptpClock->port_communication_technology);
	clockUUID_display(ptpClock->port_uuid_field);
	DBGV("\r\n");
}


/**\brief Display Buffer in & out of a PtpClock*/
void
displayBuffer(PtpClock * ptpClock)
{

	int i;
	int j;

	j = 0;

	DBGV("PtpClock Buffer Out  \r\n");
	DBGV("\r\n");

	for (i = 0; i < PACKET_SIZE; i++) {
		DBGV(":%02hhx", ptpClock->msgObuf[i]);
		j++;

		if (j == 8) {
			DBGV(" ");

		}
		if (j == 16) {
			DBGV("\r\n");
			j = 0;
		}
	}
	DBGV("\r\n");
	j = 0;
	DBGV("\r\n");

	DBGV("PtpClock Buffer In  \r\n");
	DBGV("\r\n");
	for (i = 0; i < PACKET_SIZE; i++) {
		DBGV(":%02hhx", ptpClock->msgIbuf[i]);
		j++;

		if (j == 8) {
			DBGV(" ");

		}
		if (j == 16) {
			DBGV("\r\n");
			j = 0;
		}
	}
	DBGV("\r\n");
	DBGV("\r\n");
}




/**\brief Display All data set of a PtpClock*/
void
displayPtpClock(PtpClock * ptpClock)
{

	displayDefault(ptpClock);
	displayCurrent(ptpClock);
	displayParent(ptpClock);
	displayGlobal(ptpClock);
	displayPort(ptpClock);
	displayForeignMaster(ptpClock);
	displayBuffer(ptpClock);
	displayOthers(ptpClock);

}
