/*
 * NbIotUlUlScheduler.cpp
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#include "nbiot-ul-mlwdf-scheduler.h"
#include "../ul-nbiot-AMCModule.h"
#include <stdlib.h>
#include <cmath>
#include "../../../core/eventScheduler/simulator.h"

NbIotUlMLWDFScheduler::NbIotUlMLWDFScheduler(int scSpacing, int scGroupSize) :
	NbIotUlPfScheduler(scSpacing, scGroupSize) {
}

NbIotUlMLWDFScheduler::~NbIotUlMLWDFScheduler() {
}

double NbIotUlMLWDFScheduler::ComputeSchedulingMetric(UserToSchedule* user, int ruI, double effectiveSinr) {
	UlNbIotAMCModule amcModule;
	int cqi = amcModule.GetCQIFromSinr(effectiveSinr);
	int mcs = amcModule.GetMCSFromCQI(cqi);

	//assuming maximum txrate possible (12-tone transmission)
	double instTxRate = amcModule.GetTBSizeFromMCS(mcs, amcModule.GetMaxNumberOfRuForMCS(mcs))/amcModule.GetMaxNumberOfRuForMCS(mcs);
	const double dropProbability = 0.1; //P <= 10%

//#ifdef NBIOT_DEBUG
//	//std::cout << "testercjn " << ((float)rand())/RAND_MAX << std::endl;
//	if (((float)rand())/RAND_MAX < 0.001 && Simulator::Init()->Now() > 2.0) {
//		std::cout << "[NBIOT_DEBUG_TMP] curr time:           " << Simulator::Init()->Now() << std::endl;
//		std::cout << "[NBIOT_DEBUG_TMP] first term:          " << -std::log10(dropProbability) << std::endl;
//		std::cout << "[NBIOT_DEBUG_TMP] Inst Tx Rate:        " << instTxRate << std::endl;
//		std::cout << "[NBIOT_DEBUG_TMP] Avg Tx Rate:         " << user->m_averageTransmissionRate << std::endl;
//		std::cout << "[NBIOT_DEBUG_TMP] head Of Line Delay:  " << user->m_headOfLineDelay << std::endl;
//		std::cout << "[NBIOT_DEBUG_TMP] max delay:           " << user->m_qosParameters->GetMaxDelay() << std::endl;
//		std::cout << "[NBIOT_DEBUG_TMP]" << std::endl;
//	}
//#endif

	//source: Aiyetoro et al. Performance Analysis of M-LWDF and EXP-PF Schedulers for Real Time Traffic in Satellite LTE Networks.
	double metric = -std::log10(dropProbability) * //this equals to 1 if dropProbability is 10%
			(instTxRate/user->m_averageTransmissionRate) * // this is kinda hard for NB-IoT because of multi-tone and single-tone transmissions
			(user->m_headOfLineDelay/user->m_qosParameters->GetMaxDelay());
	return metric;
}
