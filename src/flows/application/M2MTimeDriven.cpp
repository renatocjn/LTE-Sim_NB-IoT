/*
 * M2MTimeDriven.cpp
 *
 *  Created on: 08/11/2016
 *      Author: great
 */

#include "M2MTimeDriven.h"
#include "Application.h"
#include "../../protocolStack/packet/PacketTAGs.h"
#include "../../utility/RandomVariable.h"

M2MTimeDriven::M2MTimeDriven() {
	SetApplicationType(Application::APPLICATION_TYPE_M2M_TD);
	setTagType(PacketTAGs::APPLICATION_TYPE_M2M_TD);

	SetSize(PKGSIZE);
	SetInterval(INTERVAL_MIN+GetRandomVariable(INTERVAL_MAX-INTERVAL_MIN));
}

M2MTimeDriven::~M2MTimeDriven() {
	Destroy();
}

