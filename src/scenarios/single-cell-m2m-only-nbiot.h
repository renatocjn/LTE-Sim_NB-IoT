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

#include "../protocolStack/mac/packet-scheduler/nbiot-dl-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/nbiot-ul-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/nbiot-ul-pf-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/nbiot-ul-mt-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/nbiot-ul-mt-scheduler-v2.h"
#include "../protocolStack/mac/packet-scheduler/nbiot-ul-pf-scheduler-v2.h"
#include "../protocolStack/mac/packet-scheduler/nbiot-ul-rr-scheduler-v2.h"
#include "../channel/LteChannel.h"
#include "../phy/enb-lte-phy.h"
#include "../phy/ue-lte-phy.h"
#include "../core/spectrum/bandwidth-manager.h"
#include "../core/spectrum/nbiot-bandwidth-manager.h"
#include "../networkTopology/Cell.h"
#include "../protocolStack/packet/packet-burst.h"
#include "../protocolStack/packet/Packet.h"
#include "../core/eventScheduler/simulator.h"
#include "../flows/application/InfiniteBuffer.h"
#include "../flows/application/VoIP.h"
#include "../flows/application/CBR.h"
#include "../flows/application/TraceBased.h"
#include "../flows/application/M2MEventDriven.h"
#include "../flows/application/M2MTimeDriven.h"
#include "../device/IPClassifier/ClassifierParameters.h"
#include "../flows/QoS/QoSParameters.h"
#include "../flows/QoS/QoSForEXP.h"
#include "../flows/QoS/QoSForFLS.h"
#include "../flows/QoS/QoSForM_LWDF.h"
#include "../componentManagers/FrameManager.h"
#include "../utility/seed.h"
#include "../utility/RandomVariable.h"
#include "../utility/nbiot-bw-manager-helper.h"
#include "../phy/wideband-cqi-eesm-error-model.h"
#include "../phy/simple-error-model.h"
#include "../channel/propagation-model/macrocell-urban-area-channel-realization.h"
#include "../load-parameters.h"
#include <queue>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <math.h>
#include <cstring>

#define _15KHz 15

static void SingleCellM2mOnlyNbIot(double radius, int nbUE, char* ulScheduler,
		int nbIotClusterSize, int seed) {

	// define simulation times
	double duration = 11;
	double flow_duration = 5;

	//int cluster = 4;
	double bandwidth = 3;

	// CREATE COMPONENT MANAGER
	Simulator *simulator = Simulator::Init();
	FrameManager *frameManager = FrameManager::Init();
	NetworkManager* nm = NetworkManager::Init();

	// CONFIGURE SEED
	if (seed >= 0) {
		//int commonSeed = GetCommonSeed (seed);
		srand(seed);
	} else {
		srand(time(NULL));
	}
	std::cout << "Simulation with SEED = " << seed << std::endl;

	frameManager->SetFrameStructure(FrameManager::FRAME_STRUCTURE_FDD);

	CartesianCoordinates center = GetCartesianCoordinatesForCell(0,
			radius * 1000.);

	std::vector<Cell*> *cells = new std::vector<Cell*>;
	Cell *c = new Cell(0, radius, 0.035, center.GetCoordinateX(),
			center.GetCoordinateY());
	cells->push_back(c);
	nm->GetCellContainer()->push_back(c);

	std::cout << "Created Cell, id " << c->GetIdCell() << ", position: "
			<< c->GetCellCenterPosition()->GetCoordinateX() << " "
			<< c->GetCellCenterPosition()->GetCoordinateY() << std::endl;

	BandwidthManager* h2hspectrum = new BandwidthManager(bandwidth, bandwidth,
			0, 0);
	NbIotBandwidthManager* nbiotSpectrum = createNbIotBwManager(h2hspectrum,
	_15KHz);

	std::vector<LteChannel*> *dlChannels = new std::vector<LteChannel*>;
	std::vector<LteChannel*> *ulChannels = new std::vector<LteChannel*>;

	LteChannel *dlCh = new LteChannel();
	dlCh->SetChannelId(0);
	dlChannels->push_back(dlCh);

	LteChannel *ulCh = new LteChannel();
	ulCh->SetChannelId(0);
	ulChannels->push_back(ulCh);

	//create eNBs
	std::vector<ENodeB*> *eNBs = new std::vector<ENodeB*>;

	ENodeB* enb = new ENodeB(0, c);
	enb->GetPhy()->SetDlChannel(dlChannels->at(0));
	enb->GetPhy()->SetUlChannel(ulChannels->at(0));

	enb->SetDLScheduler(ENodeB::DLScheduler_TYPE_PROPORTIONAL_FAIR);
	enb->SetULScheduler(ENodeB::ULScheduler_TYPE_PF);

	enb->SetNbIotDLScheduler(new NbIotDlScheduler());
	if (strcmp(ulScheduler, "roundrobin") == 0
			|| strcmp(ulScheduler, "rr") == 0)
		enb->SetNbIotULScheduler(
				new NbIotUlScheduler(_15KHz, nbIotClusterSize));

	else if (strcmp(ulScheduler, "maximumthroughput") == 0
			|| strcmp(ulScheduler, "mt") == 0)
		enb->SetNbIotULScheduler(
				new NbIotUlMtScheduler(_15KHz, nbIotClusterSize));

	else if (strcmp(ulScheduler, "proportionallyfair") == 0
			|| strcmp(ulScheduler, "pf") == 0)
		enb->SetNbIotULScheduler(
				new NbIotUlPfScheduler(_15KHz, nbIotClusterSize));

	else if (strcmp(ulScheduler, "roundrobinv2") == 0
			|| strcmp(ulScheduler, "rrv2") == 0)
		enb->SetNbIotULScheduler(
				new NbIotUlRrSchedulerV2(_15KHz, nbIotClusterSize));

	else if (strcmp(ulScheduler, "maximumthroughputv2") == 0
			|| strcmp(ulScheduler, "mtv2") == 0)
		enb->SetNbIotULScheduler(
				new NbIotUlMtSchedulerV2(_15KHz, nbIotClusterSize));

	else if (strcmp(ulScheduler, "proportionallyfairv2") == 0
			|| strcmp(ulScheduler, "pfv2") == 0)
		enb->SetNbIotULScheduler(
				new NbIotUlPfSchedulerV2(_15KHz, nbIotClusterSize));

	else {
		std::cout << "\tThe Scheduler \"" << ulScheduler
				<< "\" is not yet implemented!\n\tOptions are:\n\troundrobin(rr)\n\tmaximumthroughput(mt)\n\tproportionallyfair(pf)\n"
				<< std::endl;
		return;
	}

	EnbLtePhy *p = (EnbLtePhy*) enb->GetPhy();
	p->SetBandwidthManager(h2hspectrum, nbiotSpectrum);

	std::cout << "Created enb, id " << enb->GetIDNetworkNode() << ", cell id "
			<< enb->GetCell()->GetIdCell() << ", position: "
			<< enb->GetMobilityModel()->GetAbsolutePosition()->GetCoordinateX()
			<< " "
			<< enb->GetMobilityModel()->GetAbsolutePosition()->GetCoordinateY()
			<< ", channels id " << enb->GetPhy()->GetDlChannel()->GetChannelId()
			<< enb->GetPhy()->GetUlChannel()->GetChannelId() << std::endl;

	h2hspectrum->Print();
	nbiotSpectrum->Print();

	ulChannels->at(0)->AddDevice((NetworkNode*) enb);

	nm->GetENodeBContainer()->push_back(enb);
	eNBs->push_back(enb);

	//Create GW
	Gateway *gw = new Gateway();
	nm->GetGatewayContainer()->push_back(gw);

	int destinationPort = 101;
	int applicationID = 0;

	std::vector<M2MTimeDriven*> *tdContainner = new std::vector<M2MTimeDriven*>;
	std::vector<M2MEventDriven*> *edContainner =
			new std::vector<M2MEventDriven*>;

	//Create UEs
	int idUE = 1;

	//Mixed traffic;
	// nbUE  = 30% H2H + 70% M2M              (Ericsson Mobility Report, 2016)
	// nbM2M = 30% ED  + 70% TD               (Maia et al, 2016)
	// nbH2H = 35% CBR + 45% Video + 20% VoIP (Self-Defined)

	int nbM2M = nbUE;

	int nbED = floor(0.3 * nbM2M);
	int nbTD = nbM2M - nbED;
	//int nbED = nbM2M;
	//int nbTD = 0;

//	std::cout << "Nb of H2H   Devices: " << nbH2H << std::endl;
//	std::cout << "Nb of CBR   Devices: " << nbCBR << std::endl;
//	std::cout << "Nb of Video Devices: " << nbVideo << std::endl;
//	std::cout << "Nb of VoIP  Devices: " << nbVoIP << std::endl;
//	std::cout << "Nb of M2M   Devices: " << nbM2M << std::endl;
//	std::cout << "Nb of ED    Devices: " << nbED << std::endl;
//	std::cout << "Nb of TD    Devices: " << nbTD << std::endl;

	for (int i = 0; i < nbUE; i++) {
		//ue's random position
		double posX = (double) rand() / RAND_MAX;
		posX = 0.95 * (((2 * radius * 1000) * posX) - (radius * 1000));
		double posY = (double) rand() / RAND_MAX;
		posY = 0.95 * (((2 * radius * 1000) * posY) - (radius * 1000));

		UserEquipment* ue = new UserEquipment(idUE, posX, posY, c, enb, 0,
				Mobility::CONSTANT_POSITION);

		std::cout << "Created UE - id " << idUE << " position " << posX << " "
				<< posY << std::endl;
		//ue->GetMobilityModel()->GetAbsolutePosition()->Print();
		ue->GetPhy()->SetDlChannel(enb->GetPhy()->GetDlChannel());
		ue->GetPhy()->SetUlChannel(enb->GetPhy()->GetUlChannel());

//		FullbandCqiManager *cqiManager = new FullbandCqiManager();
//		cqiManager->SetCqiReportingMode(CqiManager::PERIODIC);
//		cqiManager->SetReportingInterval(1);
//		cqiManager->SetDevice(ue);
//		ue->SetCqiManager(cqiManager);

		WidebandCqiEesmErrorModel *errorModel = new WidebandCqiEesmErrorModel();
		ue->GetPhy()->SetErrorModel(errorModel);

		nm->GetUserEquipmentContainer()->push_back(ue);

		// register ue to the enb
		enb->RegisterUserEquipment(ue);

		// define the channel realization
		MacroCellUrbanAreaChannelRealization* c_dl =
				new MacroCellUrbanAreaChannelRealization(enb, ue);
		enb->GetPhy()->GetDlChannel()->GetPropagationLossModel()->AddChannelRealization(
				c_dl);
		MacroCellUrbanAreaChannelRealization* c_ul =
				new MacroCellUrbanAreaChannelRealization(ue, enb);
		enb->GetPhy()->GetUlChannel()->GetPropagationLossModel()->AddChannelRealization(
				c_ul);

		ue->GetPhy()->GetDlChannel()->AddDevice(ue);

		double start_time = 0.1 + GetRandomVariable(0.4);
		double duration_time = start_time + flow_duration;

		nbM2M--;
		ue->GetPhy()->SetBandwidthManager(nbiotSpectrum);
		ue->SetNodeType(NetworkNode::TYPE_NBIOT_UE);
		if (nbED > 0) {
			nbED--;
			M2MEventDriven* edApplication = new M2MEventDriven();
			edApplication->SetSource(ue);
			edApplication->SetDestination(enb);
			edApplication->SetApplicationID(applicationID);
			edApplication->SetStartTime(start_time);
			edApplication->SetStopTime(duration_time);

			//edApplication->SetSize(80);

			// create qos parameters
			QoSParameters *qosParameters = new QoSParameters();
			qosParameters->SetMaxDelay(0.050); //50ms
//			qosParameters->SetMaxDelay(999999); // inf
//			qosParameters->SetMaxDelay(10); //10s
			edApplication->SetQoSParameters(qosParameters);

			//create classifier parameters
			ClassifierParameters *cp = new ClassifierParameters(
					ue->GetIDNetworkNode(), enb->GetIDNetworkNode(), 0,
					destinationPort,
					TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
			edApplication->SetClassifierParameters(cp);

			edContainner->push_back(edApplication);
			std::cout << "CREATED M2M_ED APPLICATION, ID " << applicationID
					<< std::endl;
		} else {
			nbTD--;
			M2MTimeDriven* tdApplication = new M2MTimeDriven();
			tdApplication->SetSource(ue);
			tdApplication->SetDestination(enb);
			tdApplication->SetApplicationID(applicationID);
			tdApplication->SetStartTime(start_time);
			tdApplication->SetStopTime(duration_time);

			//tdApplication->SetSize(80);

			// create qos parameters
			QoSParameters *qosParameters = new QoSParameters();
			qosParameters->SetMaxDelay(tdApplication->GetInterval());
//			qosParameters->SetMaxDelay(10);
			tdApplication->SetQoSParameters(qosParameters);

			//create classifier parameters
			ClassifierParameters *cp = new ClassifierParameters(
					ue->GetIDNetworkNode(), enb->GetIDNetworkNode(), 0,
					destinationPort,
					TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
			tdApplication->SetClassifierParameters(cp);

			tdContainner->push_back(tdApplication);
			std::cout << "CREATED M2M_TD APPLICATION, ID " << applicationID
					<< ", Interval: " << tdApplication->GetInterval()
					<< std::endl;
		}
		destinationPort++;
		applicationID++;
		idUE++;
	}

	simulator->SetStop(duration);
	simulator->Run();

//Delete created objects
	tdContainner->clear();
	delete tdContainner;
	edContainner->clear();
	delete edContainner;
	delete enb->GetNbIotUlScheduler();
	eNBs->clear();
	delete eNBs;
	delete frameManager;
//delete nm;
	delete simulator;

}