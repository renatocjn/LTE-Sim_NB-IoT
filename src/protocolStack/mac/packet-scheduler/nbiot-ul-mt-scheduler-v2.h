/*
 * NbIotUlUlScheduler.h
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#ifndef SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_MT_SCHEDULER_V2_H_
#define SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_MT_SCHEDULER_V2_H_

#include "nbiot-ul-pf-scheduler-v2.h"

class NbIotUlMtSchedulerV2: public NbIotUlPfSchedulerV2 {
public:
	NbIotUlMtSchedulerV2(int scSpacing, int scGroupSize);
	virtual ~NbIotUlMtSchedulerV2();

	virtual double ComputeSchedulingMetric (UserToSchedule* user, int ruIdx, double effectiveSinr);
};

#endif /* SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_MT_SCHEDULER_V2_H_ */
