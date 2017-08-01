/*
 * NbIotUlUlScheduler.h
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#ifndef SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_PF_SCHEDULER_H_
#define SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_PF_SCHEDULER_H_

#include "nbiot-ul-scheduler.h"

class NbIotUlPfScheduler: public NbIotUlScheduler {
public:
	NbIotUlPfScheduler(int scSpacing, int scGroupSize);
	virtual ~NbIotUlPfScheduler();

	virtual void RBsAllocation();

	virtual double ComputeSchedulingMetric(UserToSchedule* user, double effectiveSinr);
};

#endif /* SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_PF_SCHEDULER_H_ */
