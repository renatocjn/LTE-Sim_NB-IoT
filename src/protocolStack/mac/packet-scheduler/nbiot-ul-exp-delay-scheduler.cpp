/*
 * NbIotUlUlScheduler.cpp
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#include "nbiot-ul-exp-delay-scheduler.h"
#include "cmath"
#include "stdlib.h"
#include "../../../core/eventScheduler/simulator.h"

NbIotUlExpDelayScheduler::NbIotUlExpDelayScheduler(int scSpacing, int scGroupSize) :
		NbIotUlPfScheduler(scSpacing, scGroupSize) {
}

NbIotUlExpDelayScheduler::~NbIotUlExpDelayScheduler() {
}

double NbIotUlExpDelayScheduler::ComputeSchedulingMetric(UserToSchedule* user, double effectiveSinr) {
//	#ifdef NBIOT_DEBUG
//		if (((float)rand())/RAND_MAX < 0.001 && Simulator::Init()->Now() > 2.0) {
//			std::cout << "[NBIOT_DEBUG_TMP] curr time:           " << Simulator::Init()->Now() << std::endl;
//			std::cout << "[NBIOT_DEBUG_TMP] head Of Line Delay:  " << user->m_headOfLineDelay << std::endl;
//			std::cout << "[NBIOT_DEBUG_TMP] max delay:           " << user->m_qosParameters->GetMaxDelay() << std::endl;
//			std::cout << "[NBIOT_DEBUG_TMP] metric:              " << user->m_headOfLineDelay/user->m_qosParameters->GetMaxDelay() << std::endl;
//			std::cout << "[NBIOT_DEBUG_TMP]" << std::endl;
//		}
//	#endif
	return exp(user->m_headOfLineDelay / user->m_qosParameters->GetMaxDelay());
}
