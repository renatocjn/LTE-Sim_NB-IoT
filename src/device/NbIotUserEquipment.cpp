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


#include "NbIotUserEquipment.h"
#include "NetworkNode.h"
#include "ENodeB.h"
#include "HeNodeB.h"
#include "Gateway.h"
#include "../phy/ue-lte-phy.h"
#include "CqiManager/cqi-manager.h"
#include "../core/eventScheduler/simulator.h"
#include "../componentManagers/NetworkManager.h"
#include "../protocolStack/rrc/ho/handover-entity.h"
#include "../protocolStack/rrc/ho/ho-manager.h"


NbIotUserEquipment::NbIotUserEquipment ()
{}

NbIotUserEquipment::NbIotUserEquipment (int idElement,
							  double posx,
							  double posy,
							  Cell *cell,
							  NetworkNode* target,
							  bool handover)
{
	SetIDNetworkNode (idElement);
	SetNodeType(NetworkNode::TYPE_NBIOT_UE);
	SetCell(cell);

	m_targetNode = target;

	ProtocolStack *stack = new ProtocolStack (this);
	SetProtocolStack (stack);

	Classifier *classifier = new Classifier ();
	classifier->SetDevice (this);
	SetClassifier (classifier);
	SetNodeState(STATE_IDLE);

// 	//Setup Mobility Model
 	Mobility *m;
// 	if (model == Mobility::RANDOM_DIRECTION)
// 	{
// 		m = new RandomDirection ();
// 	}
// 	else if (model == Mobility::RANDOM_WALK)
// 	{
// 		m = new RandomWalk ();
// 	}
// 	else if (model == Mobility::RANDOM_WAYPOINT)
// 	{
// 		m = new RandomWaypoint ();
// 	}
// 	else if (model == Mobility::CONSTANT_POSITION)
// 	{
// 		m = new ConstantPosition ();
// 	}
// 	else if (model == Mobility::MANHATTAN)
// 	{
// 		m = new Manhattan ();
// 	}
// 	else
// 	{
// 		std::cout << "ERROR: incorrect Mobility Model"<< std::endl;
// 		m = new RandomDirection ();
// 	}

	m = new ConstantPosition ();
	CartesianCoordinates *position = new CartesianCoordinates (posx, posy);
	m->SetHandover (handover);
	m->SetAbsolutePosition (position);
	m->SetNodeID (idElement);
	SetMobilityModel (m);

	m_timePositionUpdate = 0.001;
// 	Simulator::Init()->Schedule(m_timePositionUpdate,
// 								&UserEquipment::UpdateUserPosition,
// 							 this,
// 							 Simulator::Init ()->Now());

	delete position;

	UeLtePhy *phy = new UeLtePhy ();
	phy->SetDevice(this);
	phy->SetBandwidthManager (target->GetPhy ()->GetNbIotBandwidthManager ());
	SetPhy(phy);

	m_cqiManager = NULL;
	m_isIndoor = false;
}
