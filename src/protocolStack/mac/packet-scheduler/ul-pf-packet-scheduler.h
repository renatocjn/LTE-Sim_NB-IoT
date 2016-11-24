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


#ifndef PF_UPLINK_PACKET_SCHEDULER_H_
#define PF_UPLINK_PACKET_SCHEDULER_H_

#include "enhanced-uplink-packet-scheduler.h"
#include <map>

class ProportionallyFairUplinkPacketScheduler : public EnhancedUplinkPacketScheduler {

public:
	ProportionallyFairUplinkPacketScheduler ();
	virtual ~ProportionallyFairUplinkPacketScheduler();

//	void RBsAllocation ();

	virtual double ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel);
    virtual double ComputeSchedulingMetric (UserToSchedule* user, int subchannel);

//private:
//	std::map<int, double> userActiveTime;
//	std::map<int, long> userTransmittedData;
};

#endif /* PF_UPLINK_PACKET_SCHEDULER_H_ */
