/*
 * NbIotUlUlScheduler.cpp
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#include "nbiot-ul-mt-scheduler.h"
#include "../../../utility/eesm-effective-sinr.h"
#include <limits>
#include "../../../protocolStack/mac/ul-nbiot-AMCModule.h"
#include <vector>
#include "../../../phy/lte-phy.h"
#include "../../../core/spectrum/nbiot-bandwidth-manager.h"

NbIotUlMtScheduler::NbIotUlMtScheduler(int scGroupSize) :
		NbIotUlPfScheduler(scGroupSize) {
}

NbIotUlMtScheduler::~NbIotUlMtScheduler() {
}

double NbIotUlMtScheduler::ComputeSchedulingMetric(UserToSchedule* user, int ruI, double effectiveSinr) {
	return effectiveSinr * 3000.0;
}
