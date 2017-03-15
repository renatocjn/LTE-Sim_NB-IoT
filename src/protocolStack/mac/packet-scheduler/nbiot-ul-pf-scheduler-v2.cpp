/*
 * NbIotUlUlScheduler.cpp
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#include "nbiot-ul-pf-scheduler-v2.h"
#include "../../../utility/eesm-effective-sinr.h"
#include <limits>
#include "../../../protocolStack/mac/ul-nbiot-AMCModule.h"
#include <vector>
#include "../../../phy/lte-phy.h"
#include "../../../core/spectrum/nbiot-bandwidth-manager.h"

NbIotUlPfSchedulerV2::NbIotUlPfSchedulerV2(int scSpacing, int scGroupSize) :
		NbIotUlRrSchedulerV2(scSpacing, scGroupSize) {
}

NbIotUlPfSchedulerV2::~NbIotUlPfSchedulerV2() {
}

void NbIotUlPfSchedulerV2::RBsAllocation() {
	UlNbIotAMCModule amcModule;
	UsersToSchedule* users = this->GetUsersToSchedule();
	UsersToSchedule::iterator it;
	NbIotBandwidthManager* bwManager =
			(NbIotBandwidthManager*) (users->at(0)->m_userToSchedule->GetPhy()->GetBandwidthManager());
	int scToSchedule = bwManager->GetUlBandwidth();
	int nRuToAllocate = scToSchedule / scGroupSize;
	int sc = 0;

	CreateRuUsageVectorIfNecessary(nRuToAllocate);
	UpdateRuUsage();
#ifdef NBIOT_DEBUG
	std::cout << "[NBIOT_DEBUG] scToSchedule " << scToSchedule << " nUsers "
			<< users->size() << " nRU " << nRuToAllocate << std::endl;
#endif
	vector<double> tempSinrVector;
	double metrics[users->size()][nRuToAllocate];
	double effSinr[users->size()][nRuToAllocate];
	for (int i = 0; i < users->size(); i++) {
		for (int j = 0; j < nRuToAllocate; j++) {
			int ruBegin = j * scGroupSize;
			int ruEnd = j * scGroupSize + scGroupSize;
			for (int k = ruBegin; k < ruEnd; k++) {
				tempSinrVector.push_back(
						users->at(i)->m_channelContition.at(k));
			}
			effSinr[i][j] = GetEesmEffectiveSinr(tempSinrVector);
			metrics[i][j] = ComputeSchedulingMetric(users->at(i), j,
					effSinr[i][j]);
		}
	}
	std::cout << "[NBIOT_DEBUG] Metrics" << std::endl;
	for (int i = 0; i < users->size(); i++) {
		std::cout << "[NBIOT_DEBUG] user "
				<< users->at(i)->m_userToSchedule->GetIDNetworkNode() << " | ";
		for (int j = 0; j < nRuToAllocate; j++) {
			std::cout << metrics[i][j] << " ";
		}
		std::cout << std::endl;
	}
	bool userAllocated[users->size()];
	for (int i = 0; i < users->size(); i++)
		userAllocated[i] = false;
	int minDuration = 1000; //this is enough
	for (int i = 0; i < nRuToAllocate; i++) {
		if (!IsRuFree(i))
			continue;

		double maxMetric = -std::numeric_limits<double>::max();
		int selectedUserI;
		for (int j = 0; j < users->size(); j++) {
			if (metrics[j][i] > maxMetric && !userAllocated[j]) {
				selectedUserI = j;
				maxMetric = metrics[j][i];
			}
		}
		userAllocated[selectedUserI] = true;
		UserToSchedule* selectedUser = users->at(selectedUserI);
		int mcs = amcModule.GetMCSFromCQI(
				amcModule.GetCQIFromSinr(effSinr[selectedUserI][i]));
		int nRU = 0, tbs;
		do {
			nRU++;
			if (nRU == 7 || nRU == 9)
				nRU++;

			tbs = ((amcModule.GetTBSizeFromMCS(mcs, nRU)) / 8);
		} while (tbs < selectedUser->m_dataToTransmit
				&& nRU < amcModule.GetMaxNumberOfRuForMCS(mcs));
		OccupyRu(i, nRU);
		if (minDuration > nRU)
			minDuration = nRU;

		for (int j = 0; j < scGroupSize; j++) {
			selectedUser->m_listOfAllocatedRBs.push_back(sc + j);
		}
		selectedUser->m_transmittedData = tbs;
		selectedUser->m_selectedMCS = mcs;
		sc += scGroupSize;
		std::cout << "[NBIOT_DEBUG] Selected user "
				<< selectedUser->m_userToSchedule->GetIDNetworkNode()
				<< " for RU " << i << " MCS: " << mcs << " nRU: " << nRU
				<< " txBytes: " << tbs << std::endl;
	}
	UpdateNextScheduleTime(GetRuDuration() * minDuration);
}

double NbIotUlPfSchedulerV2::ComputeSchedulingMetric(UserToSchedule* user,
		int ruI, double effectiveSinr) {
	return effectiveSinr * 3000.0 / user->m_averageTransmissionRate;
}
