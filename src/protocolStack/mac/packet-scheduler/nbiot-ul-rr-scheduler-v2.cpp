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
#include "../../../core/eventScheduler/simulator.h"
#include "../../../utility/eesm-effective-sinr.h"

NbIotUlRrSchedulerV2::NbIotUlRrSchedulerV2(int scSpacing, int scGroupSize) :
		NbIotUlScheduler(scSpacing, scGroupSize) {
	currUser = 0;

	ruOccupied = std::vector<double>();
	lastUpdate = Simulator::Init()->Now();
}

NbIotUlRrSchedulerV2::~NbIotUlRrSchedulerV2() {
	Destroy();
	ruOccupied.clear();
	DeleteUsersToSchedule();
}

void NbIotUlRrSchedulerV2::RBsAllocation() {
	UlNbIotAMCModule amcModule;

	UsersToSchedule *users = this->GetUsersToSchedule();
	UsersToSchedule::iterator it;

	NbIotBandwidthManager *bwManager = (NbIotBandwidthManager*) users->at(0)->m_userToSchedule->GetPhy()->GetBandwidthManager();
	int scToSchedule = bwManager->GetUlBandwidth();
	int nRuToSchedule = scToSchedule / scGroupSize;
	int sc = 0;

#ifdef NBIOT_DEBUG
	std::cout << "[NBIOT_DEBUG] scToSchedule " << scToSchedule << " nUsers "
			<< users->size() << " nRU " << nRuToSchedule << std::endl;
#endif

	CreateRuUsageVectorIfNecessary(nRuToSchedule);
	UpdateRuUsage();

	int minDuration = 1000; //this is enough
	for (int i = 0; i < nRuToSchedule; i++) {
		if (!IsRuFree(i))
			continue;

		if (currUser >= users->size())
			currUser = 0;

		UserToSchedule *selectedUser = users->at(currUser);

		vector<double> temp;
		for (int j = 0; j < scGroupSize; j++) {
			temp.push_back(selectedUser->m_channelContition.at(sc + j));
		}
		double sinr = GetEesmEffectiveSinr(temp);
		int mcs = amcModule.GetMCSFromCQI(amcModule.GetCQIFromSinr(sinr));

		int transmittedData, nRU = 0;
		do {
			nRU++;
			if (nRU == 7 || nRU == 9)
				nRU++;
			transmittedData = amcModule.GetTBSizeFromMCS(mcs, nRU) / 8;
		} while (transmittedData < selectedUser->m_dataToTransmit && nRU < amcModule.GetMaxNumberOfRuForMCS(mcs));

		for (int j = 0; j < scGroupSize; j++)
			selectedUser->m_listOfAllocatedRBs.push_back(sc + j);
		selectedUser->m_transmittedData = transmittedData;
		selectedUser->m_selectedMCS = mcs;

		currUser++;
		sc += scGroupSize;

#ifdef NBIOT_DEBUG
		std::cout << "[NBIOT_DEBUG] Selected user "
				<< selectedUser->m_userToSchedule->GetIDNetworkNode()
				<< " for RU " << i << " MCS: " << mcs << " txBytes: " << transmittedData
				<< std::endl;
#endif

		servedUsers.insert(selectedUser->m_userToSchedule->GetIDNetworkNode());
		OccupyRu(i, nRU);
		if (minDuration > nRU)
			minDuration = nRU;
	}
	UpdateNextScheduleTime(minDuration*GetRuDuration());
}

double NbIotUlRrSchedulerV2::ComputeSchedulingMetric(RadioBearer* bearer, double spectralEfficiency, int subChannel) {
	return 0.0;
}

double NbIotUlRrSchedulerV2::ComputeSchedulingMetric(UserToSchedule* user, int subchannel) {
	return 0.0;
}

const void NbIotUlRrSchedulerV2::CreateRuUsageVectorIfNecessary(int nRuToSchedule) {
	if (ruOccupied.empty()) {
		for (int i = 0; i < nRuToSchedule; i++)
			ruOccupied.push_back(0.0);
	}
}

const void NbIotUlRrSchedulerV2::UpdateRuUsage() {
	double deltat = Simulator::Init()->Now() - lastUpdate;
	lastUpdate = Simulator::Init()->Now();

	for (int i = 0; i < ruOccupied.size(); i++) {
		ruOccupied[i] -= deltat;
	}
}

const bool NbIotUlRrSchedulerV2::IsRuFree(int i) {
	if (ruOccupied[i] <= 0.0)
		return true;
	else
		return false;
}

const void NbIotUlRrSchedulerV2::OccupyRu(int i, int ms) {
	ruOccupied[i] = ((double) ms) * GetRuDuration();
}
