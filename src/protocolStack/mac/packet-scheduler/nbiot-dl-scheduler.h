/*
 * NbIotDlScheduler.h
 *
 *  Created on: 08/12/2016
 *      Author: great
 */

#ifndef SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_DL_SCHEDULER_H_
#define SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_DL_SCHEDULER_H_

#include "downlink-packet-scheduler.h"

class NbIotDlScheduler: public DownlinkPacketScheduler {
private:
	int lastScheduleDuration;
public:
	NbIotDlScheduler();
	virtual ~NbIotDlScheduler();

	virtual void RBsAllocation();
	virtual double ComputeSchedulingMetric(RadioBearer *bearer,
			double spectralEfficiency, int subChannel);
	int GetDurationOfLastSchedule();
};

#endif /* SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_DL_SCHEDULER_H_ */
