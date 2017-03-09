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
	UsersToSchedule *users = this->GetUsersToSchedule();
	UsersToSchedule::iterator it;

	NbIotBandwidthManager *bwManager = (NbIotBandwidthManager *) users->at(0)->m_userToSchedule->GetPhy()->GetBandwidthManager();
	int scToSchedule = bwManager->GetUlBandwidth();
	int nRU = scToSchedule / scGroupSize;
	int sc = 0;

	CreateRuUsageVectorIfNecessary(nRU);
	UpdateRuUsage();

#ifdef NBIOT_DEBUG
	std::cout << "[NBIOT_DEBUG] scToSchedule " << scToSchedule << std::endl;
	std::cout << "[NBIOT_DEBUG] nRU " << nRU << std::endl;
#endif
	vector<double> tempSinrVector;
	double metrics[users->size()][nRU];
	double effSinr[users->size()][nRU];

//	std::cout << "[NBIOT_DEBUG] Channel Conditions" << std::endl;
	for (int i = 0; i < users->size(); i++) {
//		std::cout << "[NBIOT_DEBUG] User " << users->at(i)->m_userToSchedule->GetIDNetworkNode() << " | ";
		for (int j = 0; j < nRU; j++) {
			int ruBegin = j * scGroupSize;
			int ruEnd = j * scGroupSize + scGroupSize;
			for (int k = ruBegin; k < ruEnd; k++) {
				tempSinrVector.push_back(users->at(i)->m_channelContition.at(k));
//				std::cout << users->at(i)->m_channelContition.at(k) << " ";
			}
			effSinr[i][j] = GetEesmEffectiveSinr(tempSinrVector);
			metrics[i][j] = ComputeSchedulingMetric(users->at(i), j, effSinr[i][j]);
		}
//		std::cout << std::endl;
	}

#ifdef NBIOT_DEBUG
	std::cout << "[NBIOT_DEBUG] Metrics" << std::endl;
	for (int i = 0; i < users->size(); i++) {
		std::cout << "[NBIOT_DEBUG] user " << users->at(i)->m_userToSchedule->GetIDNetworkNode() << " | ";
		for (int j = 0; j < nRU; j++) {
			std::cout << metrics[i][j] << " ";
		}
		std::cout << std::endl;
	}
#endif

	bool userAllocated[users->size()];
	for (int i = 0; i < users->size(); i++)
		userAllocated[i] = false;

	int minDuration = 1000; //this is enough
	for (int i = 0; i < nRU; i++) {
		if (!IsRuFree(i)) continue;

		double maxMetric = -std::numeric_limits<double>::max();
		int selectedUserI;
		for (int j = 0; j < users->size(); j++) {
			if (metrics[j][i] > maxMetric && !userAllocated[j]) {
				selectedUserI = j;
				maxMetric = metrics[j][i];
			}
		}
		userAllocated[selectedUserI] = true;
		UserToSchedule *selectedUser = users->at(selectedUserI);

		int mcs = amcModule.GetMCSFromCQI(amcModule.GetCQIFromSinr(effSinr[selectedUserI][i]));
		int nSF = 0, tbs;
		do {
			nSF++;
			if (nSF == 7 || nSF == 9) nSF++;
			tbs = ((amcModule.GetTBSizeFromMCS(mcs, nSF)) / 8);
		} while (tbs < selectedUser->m_dataToTransmit && nSF < amcModule.GetMaxNumberOfRuForMCS(mcs));

		OccupyRu(i, nSF);
		if (minDuration < nSF) minDuration = nSF;

		for (int j = 0; j < scGroupSize; j++) {
			selectedUser->m_listOfAllocatedRBs.push_back(i * scGroupSize + j);
		}
		selectedUser->m_transmittedData = tbs;
		selectedUser->m_selectedMCS = mcs;

		sc += scGroupSize;

#ifdef NBIOT_DEBUG
		std::cout << "[NBIOT_DEBUG] Selected user " << selectedUser->m_userToSchedule->GetIDNetworkNode()
		<< " for RU " << i << " MCS: " << mcs << " TBS: " << tbs << std::endl;
#endif
		UpdateNextScheduleTime(minDuration);
	}
}

double NbIotUlPfSchedulerV2::ComputeSchedulingMetric(UserToSchedule* user, int ruI, double effectiveSinr) {
	return effectiveSinr * 3000.0 / user->m_averageTransmissionRate;
}
