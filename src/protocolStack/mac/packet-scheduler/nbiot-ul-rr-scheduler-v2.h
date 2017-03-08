/*
 * NBIotUlScheduler.h
 *
 *  Created on: 08/12/2016
 *      Author: great
 */

#ifndef SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_RR_SCHEDULER_V2_H_
#define SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_RR_SCHEDULER_v2_H_

#include "nbiot-ul-scheduler.h"

class NbIotUlRrSchedulerV2: public NbIotUlScheduler {
private:
	int currUser;
	int *ruOccupied;
public:
	NbIotUlRrSchedulerV2(int scSpacing, int scGroupSize);
	virtual ~NbIotUlRrSchedulerV2();

	virtual void RBsAllocation();

	virtual double ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel);
	virtual double ComputeSchedulingMetric (UserToSchedule* user, int subchannel);
};

#endif /* SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_SCHEDULER_H_ */
