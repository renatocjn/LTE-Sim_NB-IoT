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
#include "../../../utility/eesm-effective-sinr.h"

NbIotUlScheduler::NbIotUlScheduler(int scSpacing, int scGroupSize) {
	SetMacEntity(0);
	setNodeTypeToSchedule(NetworkNode::TYPE_NBIOT_UE);
	CreateUsersToSchedule();
	this->scGroupSize = scGroupSize;

	if (scSpacing == 15) {
		if (scGroupSize == 1)
			ruDuration = 0.008;
		if (scGroupSize == 3)
			ruDuration = 0.004;
		if (scGroupSize == 6)
			ruDuration = 0.002;
		if (scGroupSize == 12)
			ruDuration = 0.001;
	} else {
		ruDuration = 0.032;
	}
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
	int nUsersToSchedule = scToSchedule / scGroupSize;
	int sc = 0;
	for (int i = 0; i < nUsersToSchedule; i++) {
		if (currUser >= users->size())
			currUser = 0;

		UserToSchedule *selectedUser = users->at(currUser);

		vector<double> temp;
		for (int j = i * scGroupSize; j < i * scGroupSize + scGroupSize; j++) {
			temp.push_back(selectedUser->m_channelContition.at(j));
		}

		double sinr = GetEesmEffectiveSinr(temp);
		int mcs = amcModule.GetMCSFromCQI(amcModule.GetCQIFromSinr(sinr));
		int tbs = ((amcModule.GetTBSizeFromMCS(mcs, 1)) / 8);

		for (int j = 0; j < scGroupSize; j++)
			selectedUser->m_listOfAllocatedRBs.push_back(sc + j);
		selectedUser->m_transmittedData = tbs;
		selectedUser->m_selectedMCS = mcs;

		currUser++;
		sc += scGroupSize;
	}
	UpdateNextScheduleTime(ruDuration);
}

double NbIotUlScheduler::ComputeSchedulingMetric(RadioBearer* bearer, double spectralEfficiency, int subChannel) {
	return 0.0;
}

double NbIotUlScheduler::ComputeSchedulingMetric(UserToSchedule* user, int subchannel) {
	return 0.0;
}

const void NbIotUlScheduler::UpdateNextScheduleTime(double deltat) {
	nextScheduleT += deltat;
}

const double NbIotUlScheduler::GetNextScheduleTime() {
	return nextScheduleT;
}

const double NbIotUlScheduler::GetRuDuration() {
	return ruDuration;
}
