/*
 * NBIotUlScheduler.cpp
 *
 *  Created on: 08/12/2016
 *      Author: great
 */

#include "nbiot-ul-scheduler.h"

NbIotUlScheduler::NbIotUlScheduler() {
	SetMacEntity (0);
	setNodeTypeToSchedule(NetworkNode::TYPE_NBIOT_UE);
	CreateUsersToSchedule ();
}

NbIotUlScheduler::~NbIotUlScheduler() {
	Destroy();
	DeleteUsersToSchedule();
}

void
NbIotUlScheduler::RBsAllocation() {
	//TODO implementar alocação UL do NBIoT
}

double NbIotUlScheduler::ComputeSchedulingMetric(RadioBearer* bearer, double spectralEfficiency, int subChannel) {
}

double NbIotUlScheduler::ComputeSchedulingMetric(UserToSchedule* user, int subchannel) {
	//TODO implementar métrica de alocação UL do NB-IoT
}
