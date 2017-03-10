/*
 * NBIotUlScheduler.h
 *
 *  Created on: 08/12/2016
 *      Author: great
 */

#ifndef SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_RR_SCHEDULER_v2_H_
#define SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_RR_SCHEDULER_v2_H_

#include "nbiot-ul-scheduler.h"
#include <vector>

class NbIotUlRrSchedulerV2: public NbIotUlScheduler {
private:
	int currUser;
	std::vector<double> ruOccupied;
	double lastUpdate;

public:
	NbIotUlRrSchedulerV2(int scSpacing, int scGroupSize);
	virtual ~NbIotUlRrSchedulerV2();

	const void CreateRuUsageVectorIfNecessary(int nRuToSchedule);
	const void UpdateRuUsage();
	const bool IsRuFree(int i);
	const void OccupyRu(int i, int ms);

	virtual void RBsAllocation();

	virtual double ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel);
	virtual double ComputeSchedulingMetric (UserToSchedule* user, int subchannel);
};

#endif /* SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_SCHEDULER_H_ */
