/*
 * NbIotUlUlScheduler.cpp
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#include "nbiot-ul-mlwdf-scheduler.h"
#include "../ul-nbiot-AMCModule.h"
#include <cmath>

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

	//source: Aiyetoro et al. Performance Analysis of M-LWDF and EXP-PF Schedulers for Real Time Traffic in Satellite LTE Networks.
	const double dropProbability = 0.1; //P <= 10%
	double metric = -std::log(dropProbability) * //this equals to 1 if dropProbability is 10%
			(instTxRate/user->m_averageTransmissionRate) * // this is kinda hard for NB-IoT because of multi-tone and single-tone transmissions
			(user->m_headOfLineDelay/user->m_qosParameters->GetMaxDelay());
	return metric;
}
