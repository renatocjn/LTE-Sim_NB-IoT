/*
 * NbIotUlUlScheduler.h
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#ifndef SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_MLWDF_SCHEDULER_H_
#define SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_MLWDF_SCHEDULER_H_

#include "nbiot-ul-pf-scheduler.h"

class NbIotUlMLWDFScheduler: public NbIotUlPfScheduler {
public:
	NbIotUlMLWDFScheduler(int scSpacing, int scGroupSize);
	virtual ~NbIotUlMLWDFScheduler();

	virtual double ComputeSchedulingMetric (UserToSchedule* user, int ruIdx, double effectiveSinr);
};

#endif /* SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_PF_SCHEDULER_H_ */
