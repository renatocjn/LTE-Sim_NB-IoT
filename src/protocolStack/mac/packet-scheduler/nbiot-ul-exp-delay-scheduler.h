/*
 * NbIotUlUlScheduler.h
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#ifndef SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_EXP_DELAY_SCHEDULER_H_
#define SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_EXP_DELAY_SCHEDULER_H_

#include "nbiot-ul-pf-scheduler.h"

class NbIotUlExpDelayScheduler: public NbIotUlPfScheduler {
public:
	NbIotUlExpDelayScheduler(int scSpacing, int scGroupSize);
	virtual ~NbIotUlExpDelayScheduler();

	virtual double ComputeSchedulingMetric (UserToSchedule* user, int ruIdx, double effectiveSinr);
};

#endif /* SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_PF_SCHEDULER_H_ */
