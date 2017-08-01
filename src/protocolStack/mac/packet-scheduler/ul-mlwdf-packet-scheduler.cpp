/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010,2011,2012,2013 TELEMATICS LAB, Politecnico di Bari
 *
 * This file is part of LTE-Sim
 *
 * LTE-Sim is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation;
 *
 * LTE-Sim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LTE-Sim; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Giuseppe Piro <g.piro@poliba.it>
 */

#include "ul-mlwdf-packet-scheduler.h"

#include "../../../utility/eesm-effective-sinr.h"
#include "../mac-entity.h"
#include "../../packet/Packet.h"
#include "../../packet/packet-burst.h"
#include "../../../device/NetworkNode.h"
#include "../../../flows/radio-bearer.h"
#include "../../../protocolStack/rrc/rrc-entity.h"
#include "../../../flows/application/Application.h"
#include "../../../device/ENodeB.h"
#include "../../../protocolStack/mac/AMCModule.h"
#include "../../../phy/lte-phy.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../core/idealMessages/ideal-control-messages.h"
#include "../../../flows/QoS/QoSParameters.h"
#include "../../../flows/MacQueue.h"
#include <cmath>

#include "../../../utility/split-users-to-schedule.h"

MlwdfUplinkPacketScheduler::MlwdfUplinkPacketScheduler() {
	SetMacEntity(0);
	CreateUsersToSchedule();
}

MlwdfUplinkPacketScheduler::~MlwdfUplinkPacketScheduler() {
// 	delete userActiveTime;
// 	delete userTransmittedData;
	Destroy();
}

double MlwdfUplinkPacketScheduler::ComputeSchedulingMetric(RadioBearer *bearer, double spectralEfficiency, int subChannel) {
	double metric;
	return metric;
}

double MlwdfUplinkPacketScheduler::ComputeSchedulingMetric(UserToSchedule* user, int subchannel) {
	AMCModule *amc = GetMacEntity()->GetAmcModule();

	double cqi = amc->GetCQIFromSinr(user->m_channelContition.at(subchannel));
	double spectralEfficiency = amc->GetEfficiencyFromCQI (cqi);
	const double dropProbability = 0.1; //P <= 10%

	//source: Aiyetoro et al. Performance Analysis of M-LWDF and EXP-PF Schedulers for Real Time Traffic in Satellite LTE Networks.
	double metric = -std::log10(dropProbability) * //this equals to 1 if dropProbability is 10%
			(spectralEfficiency*180000.0 / user->m_averageTransmissionRate) *
			(user->m_headOfLineDelay / user->m_qosParameters->GetMaxDelay());

	return metric;
}
