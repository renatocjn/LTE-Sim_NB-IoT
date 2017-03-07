/*
 * NBIotUlScheduler.cpp
 *
 *  Created on: 08/12/2016
 *      Author: great
 */

#include "nbiot-ul-rr-scheduler-v2.h"
#include "../ul-nbiot-AMCModule.h"
#include "../../../core/spectrum/nbiot-bandwidth-manager.h"
#include "../../../phy/lte-phy.h"

NbIotUlRrSchedulerV2::NbIotUlRrSchedulerV2(int scSpacing, int scGroupSize):
NbIotUlScheduler(scSpacing, scGroupSize) {
	currUser = 0;
	ruOccupied = 0;
}

NbIotUlRrSchedulerV2::~NbIotUlRrSchedulerV2() {
	Destroy();
	DeleteUsersToSchedule();
}

void NbIotUlRrSchedulerV2::RBsAllocation() {
	UlNbIotAMCModule amcModule;

	UsersToSchedule *users = this->GetUsersToSchedule();
	UsersToSchedule::iterator it;

	NbIotBandwidthManager *bwManager = (NbIotBandwidthManager*) users->at(0)->m_userToSchedule->GetPhy()->GetBandwidthManager();
	int scToSchedule = bwManager->GetUlBandwidth();
	int nRuToSchedule = scToSchedule/scGroupSize;
	int sc = 0;

	if (ruOccupied == 0) {
		ruOccupied = new bool[nRuToSchedule];
		for (int i=0; i<nRuToSchedule; i++) ruOccupied[i] = false;
	}

	for(int i=0; i<nRuToSchedule; i++) {
		if (currUser >= users->size())
			currUser = 0;

		UserToSchedule *selectedUser = users->at(currUser);

		double sinr = selectedUser->m_channelContition.at(0);
		int mcs = amcModule.GetMCSFromCQI(amcModule.GetCQIFromSinr(sinr));
		int tbs = ((amcModule.GetTBSizeFromMCS (mcs, 1)) / 8);

		for (int j=0; j<scGroupSize; j++)
			selectedUser->m_listOfAllocatedRBs.push_back(sc+j);
		selectedUser->m_transmittedData = tbs;
		selectedUser->m_selectedMCS = mcs;

		currUser++;
		sc += scGroupSize;
	}
}

double NbIotUlRrSchedulerV2::ComputeSchedulingMetric(RadioBearer* bearer, double spectralEfficiency, int subChannel) {
	return 0.0;
}

double NbIotUlRrSchedulerV2::ComputeSchedulingMetric(UserToSchedule* user, int subchannel) {
	return 0.0;
}
