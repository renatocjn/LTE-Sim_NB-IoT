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
	//TODO implementar alocação UL do NBIoT

	UlNbIotAMCModule amcModule;

	UsersToSchedule *users = this->GetUsersToSchedule();
	UsersToSchedule::iterator it;

//	TODO Fazer metrica de escolha dinamica tamanho grupo de sc
//	double sum = 0;
//	for(it=users->begin(); it!=users->end(); it++) {
//		sum += (*it)->m_dataToTransmit;
//	}
//	double meanDataToTransmit = sum/users->size();
//
//	double ScSizeMetric = meanDataToTransmit / users->size();
//	int scGroupSize;
//
//	if (ScSizeMetric > ___ ) {
//		scGroupSize = 12;
//	} else if (ScSizeMetric > ___ ) {
//		scGroupSize = 6;
//	} else if (ScSizeMetric > ___ ) {
//		scGroupSize = 3;
//	} else {
//		scGroupSize = 1;
//	}

	NbIotBandwidthManager *bwManager = (NbIotBandwidthManager*) users->at(0)->m_userToSchedule->GetPhy()->GetBandwidthManager();
	int scToSchedule = bwManager->GetUlBandwidth();
	int nUsersToSchedule = scToSchedule/scGroupSize;
	int sc = 0;
	for(int i=0; i<nUsersToSchedule; i++) {
		if (currUser > users->size())
			currUser = 0;

		UserToSchedule *selectedUser = users->at(currUser);

		double sinr = selectedUser->m_channelContition.at(0);
		int mcs = amcModule.GetMCSFromCQI(amcModule.GetCQIFromSinr(sinr));
		int tbs = ((amcModule.GetTBSizeFromMCS (mcs, scGroupSize)) / 8);

		for (int j=0; j<scGroupSize; j++)
			selectedUser->m_listOfAllocatedRBs.at(sc+j);
		selectedUser->m_transmittedData = tbs;
		selectedUser->m_selectedMCS = mcs;

		currUser++;
		sc += scGroupSize;
	}
}

double NbIotUlScheduler::ComputeSchedulingMetric(RadioBearer* bearer, double spectralEfficiency, int subChannel) {
}

double NbIotUlScheduler::ComputeSchedulingMetric(UserToSchedule* user, int subchannel) {
//TODO implementar métrica de alocação UL do NB-IoT?
}
