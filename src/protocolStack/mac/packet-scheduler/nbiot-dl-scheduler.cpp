/*
 * NbIotDlScheduler.cpp
 *
 *  Created on: 08/12/2016
 *      Author: great
 */

#include "nbiot-dl-scheduler.h"

#include "packet-scheduler.h"
#include "../../../core/spectrum/nbiot-bandwidth-manager.h"
#include "../../../protocolStack/mac/AMCModule.h"
#include <limits>

NbIotDlScheduler::NbIotDlScheduler() {
	setNodeTypeToSchedule(NetworkNode::TYPE_NBIOT_UE);

}

NbIotDlScheduler::~NbIotDlScheduler() {
	Destroy();
}

void NbIotDlScheduler::RBsAllocation() {
	PacketScheduler::FlowsToSchedule *flows = GetFlowsToSchedule();
	AMCModule amcModule;
	NbIotBandwidthManager *bwManager = (NbIotBandwidthManager*) flows->at(0)->GetBearer()->GetDestination()->GetPhy()->GetBandwidthManager();

	double dlRb = bwManager->GetDlSubChannels().at(0);
	double maxMetric = std::numeric_limits<double>::min();
	int bestUser;

	for (int i = 0; i < flows->size(); i++) {
		FlowToSchedule *f = flows->at(i);
		double currMetric = ComputeSchedulingMetric(f->GetBearer(), f->GetSpectralEfficiency().at(0), 0);
		if (currMetric > maxMetric) {
			maxMetric = currMetric;
			bestUser = i;
		}
	}
}

double NbIotDlScheduler::ComputeSchedulingMetric(RadioBearer* bearer, double spectralEfficiency, int subChannel) {
	return spectralEfficiency / bearer->GetAverageTransmissionRate();
}
