/*
 * NbIotUlUlScheduler.cpp
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#include "nbiot-ul-exp-delay-scheduler.h"
#include "cmath";

NbIotUlExpDelayScheduler::NbIotUlExpDelayScheduler(int scSpacing, int scGroupSize) :
	NbIotUlPfScheduler(scSpacing, scGroupSize) {
}

NbIotUlExpDelayScheduler::~NbIotUlExpDelayScheduler() {
}

double NbIotUlExpDelayScheduler::ComputeSchedulingMetric(UserToSchedule* user, int ruI, double effectiveSinr) {
	return exp(user->m_headOfLineDelay/user->m_qosParameters->GetMaxDelay());
}
