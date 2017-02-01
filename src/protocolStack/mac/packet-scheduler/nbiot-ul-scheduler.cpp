/*
 * NBIotUlScheduler.cpp
 *
 *  Created on: 08/12/2016
 *      Author: great
 */

#include "nbiot-ul-scheduler.h"
#include "../ul-nbiot-AMCModule.h"
#include "../../../core/spectrum/nbiot-bandwidth-manager.h"
#include "../../../phy/lte-phy.h"

NbIotUlScheduler::NbIotUlScheduler(int scGroupSize) {
	SetMacEntity(0);
	setNodeTypeToSchedule(NetworkNode::TYPE_NBIOT_UE);
	CreateUsersToSchedule();
	currUser = 0;
	this->scGroupSize = scGroupSize;
}

NbIotUlScheduler::~NbIotUlScheduler() {
	Destroy();
	DeleteUsersToSchedule();
}

void NbIotUlScheduler::RBsAllocation() {
	//RoundRobin

	UlNbIotAMCModule amcModule;

	UsersToSchedule *users = this->GetUsersToSchedule();
	UsersToSchedule::iterator it;

	NbIotBandwidthManager *bwManager = (NbIotBandwidthManager*) users->at(0)->m_userToSchedule->GetPhy()->GetBandwidthManager();
	int scToSchedule = bwManager->GetUlBandwidth();
	int nUsersToSchedule = scToSchedule/scGroupSize;
	int sc = 0;
	for(int i=0; i<nUsersToSchedule; i++) {
		if (currUser >= users->size())
			currUser = 0;

		UserToSchedule *selectedUser = users->at(currUser);

		double sinr = selectedUser->m_channelContition.at(0);
		int mcs = amcModule.GetMCSFromCQI(amcModule.GetCQIFromSinr(sinr));
		int tbs = ((amcModule.GetTBSizeFromMCS (mcs, scGroupSize)) / 8);

		for (int j=0; j<scGroupSize; j++)
			selectedUser->m_listOfAllocatedRBs.push_back(sc+j);
		selectedUser->m_transmittedData = tbs;
		selectedUser->m_selectedMCS = mcs;

		currUser++;
		sc += scGroupSize;
	}
}

double NbIotUlScheduler::ComputeSchedulingMetric(RadioBearer* bearer, double spectralEfficiency, int subChannel) {
	return 0.0;
}

double NbIotUlScheduler::ComputeSchedulingMetric(UserToSchedule* user, int subchannel) {
	return 0.0;
}
