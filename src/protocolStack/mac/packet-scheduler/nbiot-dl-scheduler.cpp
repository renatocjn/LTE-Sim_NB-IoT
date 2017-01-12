/*
 * NbIotDlScheduler.cpp
 *
 *  Created on: 08/12/2016
 *      Author: great
 */

#include "nbiot-dl-scheduler.h"

#include "packet-scheduler.h"
#include "../../../core/spectrum/nbiot-bandwidth-manager.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../AMCModule.h"
#include "../dl-nbiot-AMCModule.h"
#include "../../../flows/radio-bearer.h"
#include "../../../phy/lte-phy.h"
#include <limits>

NbIotDlScheduler::NbIotDlScheduler() {
	setNodeTypeToSchedule(NetworkNode::TYPE_NBIOT_UE);

}

NbIotDlScheduler::~NbIotDlScheduler() {
	Destroy();
}

void NbIotDlScheduler::RBsAllocation() {
	PacketScheduler::FlowsToSchedule *flows = GetFlowsToSchedule();
	DlNbIotAMCModule amcModule;
	NbIotBandwidthManager *bwManager = (NbIotBandwidthManager*) flows->at(0)->GetBearer()->GetDestination()->GetPhy()->GetBandwidthManager();

	double dlRb = bwManager->GetDlSubChannels().at(0);
	double maxMetric = std::numeric_limits<double>::min();
	FlowToSchedule *bestFlow;

	for (int i = 0; i < flows->size(); i++) {
		FlowToSchedule *f = flows->at(i);
		double currMetric = ComputeSchedulingMetric(f->GetBearer(), f->GetSpectralEfficiency().at(0), 0);
		if (currMetric > maxMetric) {
			maxMetric = currMetric;
			bestFlow = f;
		}
	}

	int mcs = amcModule.GetMCSFromCQI(bestFlow->m_cqiFeedbacks.at(0));
	int transportBlockSize = amcModule.GetTBSizeFromMCS(mcs, 1);

//	bestFlow->m_transmittedData = transportBlockSize;
	bestFlow->UpdateAllocatedBits(transportBlockSize);
	bestFlow->m_listOfAllocatedRBs.push_back(dlRb);
//	bestFlow->m_listOfSelectedMCS.push_back(mcs);

	PdcchMapIdealControlMessage *pdcchMsg = new PdcchMapIdealControlMessage();
	pdcchMsg->AddNewRecord(PdcchMapIdealControlMessage::DOWNLINK, dlRb, bestFlow->GetBearer()->GetDestination(), mcs);
	if (pdcchMsg->GetMessage()->size() > 0) {
		GetMacEntity()->GetDevice()->GetPhy()->SendIdealControlMessage(pdcchMsg);
	}
	delete pdcchMsg;
}

double NbIotDlScheduler::ComputeSchedulingMetric(RadioBearer* bearer, double spectralEfficiency, int subChannel) {
	return spectralEfficiency / bearer->GetAverageTransmissionRate();
}
