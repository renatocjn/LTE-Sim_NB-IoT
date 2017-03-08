/*
 * NbIotUlUlScheduler.cpp
 *
 *  Created on: 09/02/2017
 *      Author: great
 */

#include "nbiot-ul-mt-scheduler-v2.h"
#include "../../../utility/eesm-effective-sinr.h"
#include <limits>
#include "../../../protocolStack/mac/ul-nbiot-AMCModule.h"
#include <vector>
#include "../../../phy/lte-phy.h"
#include "../../../core/spectrum/nbiot-bandwidth-manager.h"

NbIotUlMtSchedulerV2::NbIotUlMtSchedulerV2(int scSpacing, int scGroupSize) :
		NbIotUlPfSchedulerV2(scSpacing, scGroupSize) {
}

NbIotUlMtSchedulerV2::~NbIotUlMtSchedulerV2() {
}

double NbIotUlMtSchedulerV2::ComputeSchedulingMetric(UserToSchedule* user, int ruI, double effectiveSinr) {
	return effectiveSinr * 3000.0;
}
