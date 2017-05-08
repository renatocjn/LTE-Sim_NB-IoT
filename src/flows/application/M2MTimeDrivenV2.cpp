/*
 * M2MTimeDriven.cpp
 *
 *  Created on: 08/11/2016
 *      Author: great
 */

#include "M2MTimeDrivenV2.h"
#include "Application.h"
#include "../../protocolStack/packet/PacketTAGs.h"
#include "../../utility/RandomVariable.h"

M2MTimeDrivenV2::M2MTimeDrivenV2() {
	SetApplicationType(Application::APPLICATION_TYPE_M2M_TD);
	setTagType(PacketTAGs::APPLICATION_TYPE_M2M_TD);

	SetSize(PKGSIZE);
	SetInterval(INTERVAL_MIN+GetRandomVariable(INTERVAL_MAX-INTERVAL_MIN));
}

M2MTimeDrivenV2::~M2MTimeDrivenV2() {
	Destroy();
}

