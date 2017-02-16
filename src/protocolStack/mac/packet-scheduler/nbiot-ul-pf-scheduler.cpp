/*
 * NbIotUlUlScheduler.cpp
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#include "nbiot-ul-pf-scheduler.h"
#include "nbiot-ul-scheduler.h"
#include "../../../utility/eesm-effective-sinr.h"
#include <limits>
#include "../../../protocolStack/mac/ul-nbiot-AMCModule.h"
#include <vector>
#include "../../../phy/lte-phy.h"
#include "../../../core/spectrum/nbiot-bandwidth-manager.h"

NbIotUlPfScheduler::NbIotUlPfScheduler(int scGroupSize) :
		NbIotUlScheduler(scGroupSize) {}

NbIotUlPfScheduler::~NbIotUlPfScheduler() {}

void
NbIotUlPfScheduler::RBsAllocation() {
	//proportionally fair

	UlNbIotAMCModule amcModule;
	UsersToSchedule *users = this->GetUsersToSchedule();
	UsersToSchedule::iterator it;

	NbIotBandwidthManager *bwManager = (NbIotBandwidthManager *) users->at(0)->m_userToSchedule->GetPhy()->GetBandwidthManager();
	int scToSchedule = bwManager->GetUlBandwidth();
	int nRU = scToSchedule / scGroupSize;
	int sc = 0;

	vector<double> tempSinrVector;
	double metrics[users->size()][nRU];
	double effSinr[users->size()][nRU];

	for (int i = 0; i < users->size(); i++) {
		for (int j = 0; j < nRU; j++) {
			int ruBegin = j * scGroupSize;
			int ruEnd = j * scGroupSize + scGroupSize;
			for (int k = ruBegin; k < ruEnd; k++) {
				tempSinrVector.push_back(users->at(i)->m_channelContition.at(k));
			}
			effSinr[i][j] = GetEesmEffectiveSinr(tempSinrVector);
			metrics[i][j] = ComputeSchedulingMetric(users->at(i), j, effSinr[i][j]);
		}
	}

	bool userAllocated[users->size()];
	for (int i = 0; i < users->size(); i++)
		userAllocated[i] = false;

	for (int i = 0; i < nRU; i++) {
		double maxMetric = -std::numeric_limits<double>::max();
		int selectedUserI;
		for (int j = 0; j < users->size(); j++) {
			if (metrics[j][i] > maxMetric && !userAllocated[j]) {
				userAllocated[j] = true;
				selectedUserI = j;
				maxMetric = metrics[j][i];
			}
		}
		UserToSchedule *selectedUser = users->at(selectedUserI);

		int mcs = amcModule.GetMCSFromCQI(
				amcModule.GetCQIFromSinr(
						effSinr[selectedUserI][i]
											   )
				);
		int tbs = ((amcModule.GetTBSizeFromMCS(mcs, scGroupSize)) / 8);

		for (int j = 0; j < scGroupSize; j++) {
			selectedUser->m_listOfAllocatedRBs.push_back(i * scGroupSize + j);
		}
		selectedUser->m_transmittedData = tbs;
		selectedUser->m_selectedMCS = mcs;

		sc += scGroupSize;
	}
}

double NbIotUlPfScheduler::ComputeSchedulingMetric(UserToSchedule* user, int ruI, double effectiveSinr) {
	return effectiveSinr * 3000.0 / user->m_averageTransmissionRate;
}
