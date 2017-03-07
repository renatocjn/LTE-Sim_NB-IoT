/*
 * NbIotUlUlScheduler.h
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#ifndef SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_PF_SCHEDULER_V2_H_
#define SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_PF_SCHEDULER_V2_H_

#include "nbiot-ul-scheduler.h"

class NbIotUlPfSchedulerV2: public NbIotUlScheduler {
public:
	NbIotUlPfSchedulerV2(int scSpacing, int scGroupSize);
	virtual ~NbIotUlPfSchedulerV2();

	virtual void RBsAllocation();

	virtual double ComputeSchedulingMetric (UserToSchedule* user, int ruIdx, double effectiveSinr);
};

#endif /* SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_PF_SCHEDULER_H_ */
