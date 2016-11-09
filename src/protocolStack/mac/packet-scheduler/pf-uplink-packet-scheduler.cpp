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

#include "pf-uplink-packet-scheduler.h"
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
#include "../../../utility/split-users-to-schedule.h"

ProportionallyFairUplinkPacketScheduler::ProportionallyFairUplinkPacketScheduler()
{
	SetMacEntity (0);
	CreateUsersToSchedule ();
}

ProportionallyFairUplinkPacketScheduler::~ProportionallyFairUplinkPacketScheduler()
{
// 	delete userActiveTime;
// 	delete userTransmittedData;
	Destroy ();
}

double
ProportionallyFairUplinkPacketScheduler::ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel)
{
	double metric;
	return metric;
}

double
ProportionallyFairUplinkPacketScheduler::ComputeSchedulingMetric (UserToSchedule* user, int subchannel)
{
	int channelCondition = user->m_channelContition.at (subchannel);
	AMCModule* amc = user->m_userToSchedule->GetProtocolStack()->GetMacEntity ()->GetAmcModule ();

	int mcs = amc->GetMCSFromCQI (channelCondition);
	int tbs = amc->GetTBSizeFromMCS (mcs);

	int userId = user->m_userToSchedule->GetIDNetworkNode();
	long txData;
	double activeTime;
	if (userActiveTime.find(userId) == userActiveTime.end()) {
		userActiveTime[userId] = 0.0;
		userTransmittedData[userId] = 0;
	}

	userActiveTime[userId] += 1;
	int metric = tbs/(userTransmittedData[userId]/userActiveTime[userId]);
	return metric;
}

// void
// ProportionallyFairUplinkPacketScheduler::RBsAllocation ()
// {
// #ifdef SCHEDULER_DEBUG
// 	std::cout << " ---- PF UL RBs Allocation";
// #endif
//
// 	UsersToSchedule *users = GetUsersToSchedule ();
// 	int nbOfRBs = GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetUlSubChannels ().size ();
//
// 	//create a matrix of flow metrics
// 	double metrics[nbOfRBs][users->size ()];
// 	for (int i = 0; i < nbOfRBs; i++)
// 		{
// 		for (int j = 0; j < users->size (); j++)
// 			{
// 			metrics[i][j] = ComputeSchedulingMetric (users->at (j), i);
// 			}
// 		}
//
// #ifdef SCHEDULER_DEBUG
// 	std::cout << ", available RBs " << nbOfRBs << ", users " << users->size () << std::endl;
// 	for (int ii = 0; ii < users->size (); ii++)
// 	{
// 		std::cout << "\t metrics for user " << users->at (ii)->m_userToSchedule->GetIDNetworkNode ();
// 		for (int jj = 0; jj < nbOfRBs; jj++)
// 		{
// 			std::cout << " " << metrics[jj][ii];
// 		}
// 		std::cout << std::endl;
// 	}
// #endif
//
// 	//RBs allocation
// 	int s = 0;
// 	while (s < nbOfRBs)
// 	{
// // 		std::cout << "loop?" << std::endl;
// 		double targetMetric = metrics[0][0];
// 		bool RBIsAllocated = false;
// 		UserToSchedule* scheduledUser;
//
// 		//select user for this RB
// 		for (int k = 0; k < users->size (); k++)
// 		{
// 			if (metrics[s][k] >= targetMetric)
// 			{
// 				targetMetric = metrics[s][k];
// 				RBIsAllocated = true;
// 				scheduledUser = users->at (k);
// 			}
// 		}
//
// 		if (RBIsAllocated)
// 		{
// // 			std::cout << "rb is allocated" << std::endl;
// 			int dataToTransmit = scheduledUser->m_dataToTransmit;
// 			int availableRBs = nbOfRBs - s;
// 			std::vector<double> sinrs;
// 			for (std::vector<int>::iterator c = scheduledUser->m_channelContition.begin ();
// 				 c != scheduledUser->m_channelContition.end (); c++)
// 			{
// 				sinrs.push_back (GetMacEntity ()->GetAmcModule ()->GetSinrFromCQI (*c));
// 			}
//
// 			double effectiveSinr =  GetEesmEffectiveSinr (sinrs);
// 			int mcs = GetMacEntity ()->GetAmcModule ()->GetMCSFromCQI (GetMacEntity ()->GetAmcModule ()->GetCQIFromSinr (effectiveSinr));
// 			int tbs = (GetMacEntity ()->GetAmcModule ()->GetTBSizeFromMCS (mcs, availableRBs)) / 8;
//
// 			if (tbs <= dataToTransmit)
// 			{
// // 				std::cout << "not enough tbs" << std::endl;
//
// 				scheduledUser->m_transmittedData = tbs;
//
// 				for (int ss = s; ss < nbOfRBs; ss++)
// 				{
// 					scheduledUser->m_listOfAllocatedRBs.push_back (ss); // XXX it doesn't describe correctly assigned PRBs, edit: maybe it does now?
// 				}
// 				s = nbOfRBs;
// 				scheduledUser->m_selectedMCS = mcs;
// 			}
// 			else {
// // 				std::cout << "enough tbs" << std::endl;
// 				//int rbsNeeded = (floor) (scheduledUser->m_dataToTransmit /
// 				//		(GetMacEntity ()->GetAmcModule ()->GetTBSizeFromMCS (mcs, 1) / 8));
//
// 				int rbsNeeded = 0;
// 				int availableData;
// 				do {
// 					rbsNeeded++;
// 					availableData = GetMacEntity ()->GetAmcModule ()->GetTBSizeFromMCS (mcs, rbsNeeded) / 8;
// 				} while (scheduledUser->m_dataToTransmit <= availableData);
//
// 				scheduledUser->m_transmittedData = availableData;
// 				s += rbsNeeded;
//
// 				for (int ss = s; ss < s+rbsNeeded; ss++) {
// 					scheduledUser->m_listOfAllocatedRBs.push_back (ss); // XXX it doesn't describe correctly assigned PRBs, edit: maybe it does now?
// 				}
// 				scheduledUser->m_selectedMCS = mcs;
// 			}
// 			userTransmittedData[scheduledUser->m_userToSchedule->GetIDNetworkNode()] += scheduledUser->m_transmittedData;
// 		}
// 	}
// }

void
ProportionallyFairUplinkPacketScheduler::RBsAllocation ()
{
	#ifdef SCHEDULER_DEBUG
	std::cout << " ---- UL RBs Allocation";
	#endif

	UsersToSchedule *users = GetUsersToSchedule ();
	UsersToSchedule *regularUsers = new UsersToSchedule();
	UsersToSchedule *nbiotUsers = new UsersToSchedule();

	splitUsersToSchedule(users, regularUsers, nbiotUsers);

	int nbOfRBs = GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetUlSubChannels ().size ();

	bool* allocatedUsers = new bool[regularUsers->size()];
	for (unsigned i=0; i<regularUsers->size(); i++)
		allocatedUsers[i] = false;
	int nbAllocatedUsers = 0;
	std::vector<double> * AllocatedUsersSINR = new std::vector<double>[regularUsers->size ()];

	//create a matrix of flow metrics
	double metrics[nbOfRBs][regularUsers->size ()];
	for (int i = 0; i < nbOfRBs; i++)
	{
		for (unsigned j = 0; j < regularUsers->size (); j++)
		{
			metrics[i][j] = ComputeSchedulingMetric (regularUsers->at (j), i);
		}
	}

	#ifdef SCHEDULER_DEBUG
	std::cout << ", available RBs " << nbOfRBs << ", Users " << regularUsers->size () << std::endl;
	for (int ii = 0; ii < regularUsers->size (); ii++)
	{
		std::cout << "\t metrics for user "
		<< regularUsers->at (ii)->m_userToSchedule->GetIDNetworkNode ();
		for (int jj = 0; jj < nbOfRBs; jj++)
		{
			std::cout << " " << metrics[jj][ii];
		}
		std::cout << std::endl;
	}
	#endif

	//RBs allocation
	int s = 0;
	//while (s < nbOfRBs)
	for (int s=0; s<nbOfRBs; s++)
	{
		double targetMetric = metrics[s][0];
		bool RBIsAllocated = false;
		UserToSchedule* scheduledUser;
		int scheduledUseri;

		//select user for this RB
		for (unsigned k = 0; k < regularUsers->size (); k++)
		{
			if (metrics[s][k] >= targetMetric && !allocatedUsers[k])
			{
				targetMetric = metrics[s][k];
				RBIsAllocated = true;
				scheduledUser = regularUsers->at (k);
				scheduledUseri = k;
			}
		}

		if (RBIsAllocated)
		{
			int dataToTransmit = scheduledUser->m_dataToTransmit;
			scheduledUser->m_listOfAllocatedRBs.push_back (s);

			double sinr = GetMacEntity ()->GetAmcModule ()->GetSinrFromCQI (scheduledUser->m_channelContition.at(s));
			AllocatedUsersSINR[scheduledUseri].push_back(sinr);

			double effectiveSinr =  GetEesmEffectiveSinr (AllocatedUsersSINR[scheduledUseri]);
			int mcs = GetMacEntity ()->GetAmcModule ()->GetMCSFromCQI (
				GetMacEntity ()->GetAmcModule ()->GetCQIFromSinr (effectiveSinr));
			int tbs = (GetMacEntity ()->GetAmcModule ()->GetTBSizeFromMCS (mcs, scheduledUser->m_listOfAllocatedRBs.size())) / 8;
			scheduledUser->m_selectedMCS = mcs;

			if (tbs >= dataToTransmit)
			{
				scheduledUser->m_transmittedData = dataToTransmit;
				allocatedUsers[scheduledUseri] = true;
				nbAllocatedUsers++;
			}
			else
			{
				scheduledUser->m_transmittedData = tbs;
			}

			// 			if (tbs <= dataToTransmit)
			// 			{
			// 				s = nbOfRBs;
			// 				scheduledUser->m_transmittedData = tbs;
			//
			// 				for (int ss = s; ss < nbOfRBs; ss++)
			// 				{
			// 					scheduledUser->m_listOfAllocatedRBs.push_back (ss); // XXX it doesn't describe correctly assigned PRBs, edit: maybe it does now?
			// 				}
			// 				scheduledUser->m_selectedMCS = mcs;
			// 			}
			// 			else
			// 			{
			// 				//int rbsNeeded = (floor) (scheduledUser->m_dataToTransmit /
			// 				//		(GetMacEntity ()->GetAmcModule ()->GetTBSizeFromMCS (mcs, 1) / 8));
			//
			// 				int rbsNeeded = 0;
			// 				int availableData;
			// 				do {
			// 					rbsNeeded++;
			// 					availableData = GetMacEntity ()->GetAmcModule ()->GetTBSizeFromMCS (mcs, rbsNeeded) / 8;
			// 				} while (scheduledUser->m_dataToTransmit <= availableData);
			//
			// 				scheduledUser->m_transmittedData = availableData;
			// 				s += rbsNeeded;
			//
			// 				for (int ss = s; ss < s+rbsNeeded; ss++) {
			// 					scheduledUser->m_listOfAllocatedRBs.push_back (ss); // XXX it doesn't describe correctly assigned PRBs, edit: maybe it does now?
			// 				}
			// 				scheduledUser->m_selectedMCS = mcs;
			// 			}
		}
	}

	for (unsigned i=0; i < users->size(); i++) {
		if (allocatedUsers[i]) {
			int uid = users->at(i)->m_userToSchedule->GetIDNetworkNode();
			userTransmittedData[uid] += users->at(i)->m_transmittedData;
		}
	}
}
