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

#define NBIOT_DEBUG

static void SingleCellNbIot(double radius, int nbUE, char* trafficType,
		char* scheduler, int seed) {

	// define simulation times
	double duration = 6;
	double flow_duration = 5;
	double nbiotTxBwConfiguration = 15;

	//int cluster = 4;
	double bandwidth = 1.4;

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

	//std::vector <BandwidthManager*> spectrums = RunFrequencyReuseTechniques (1, cluster, bandwidth);
	BandwidthManager* spectrum = new BandwidthManager(bandwidth, bandwidth, 0,
			0);
	//NbIotBandwidthManager* nbiotSpectrum = createNbIotBwManager (spectrum, nbiotTxBwConfiguration);

	//Create a set of a couple of channels
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
	if (strcmp(scheduler, "roundrobin") == 0 || strcmp(scheduler, "rr") == 0) {
		enb->SetULScheduler(ENodeB::ULScheduler_TYPE_ROUNDROBIN);
	} else if (strcmp(scheduler, "maximumthroughput") == 0
			|| strcmp(scheduler, "mt") == 0) {
		enb->SetULScheduler(ENodeB::ULScheduler_TYPE_MAXIMUM_THROUGHPUT);
	} else if (strcmp(scheduler, "proportionallyfair") == 0
			|| strcmp(scheduler, "pf") == 0) {
		enb->SetULScheduler(ENodeB::ULScheduler_TYPE_PF);
	} else {
		std::cout << "\tThe Scheduler \"" << scheduler
				<< "\" is not yet implemented!\n\tOptions are:\n\troundrobin(rr)\n\tmaximumthroughput(mt)\n\tproportionallyfair(pf)\n"
				<< std::endl;
		return;
	}

	enb->GetPhy()->SetBandwidthManager(spectrum);

	std::cout << "Created enb, id " << enb->GetIDNetworkNode() << ", cell id "
			<< enb->GetCell()->GetIdCell() << ", position: "
			<< enb->GetMobilityModel()->GetAbsolutePosition()->GetCoordinateX()
			<< " "
			<< enb->GetMobilityModel()->GetAbsolutePosition()->GetCoordinateY()
			<< ", channels id " << enb->GetPhy()->GetDlChannel()->GetChannelId()
			<< enb->GetPhy()->GetUlChannel()->GetChannelId() << std::endl;

	spectrum->Print();
	//nbiotSpectrum->Print();

	ulChannels->at(0)->AddDevice((NetworkNode*) enb);

	nm->GetENodeBContainer()->push_back(enb);
	eNBs->push_back(enb);

	//Create GW
	Gateway *gw = new Gateway();
	nm->GetGatewayContainer()->push_back(gw);

	int destinationPort = 101;
	int applicationID = 0;

	std::vector<VoIP*> *voipContainner = new std::vector<VoIP*>;
	std::vector<CBR*> *cbrContainner = new std::vector<CBR*>;
	std::vector<TraceBased*> *videoContainner = new std::vector<TraceBased*>;
	std::vector<M2MTimeDriven*> *tdContainner = new std::vector<M2MTimeDriven*>;
	std::vector<M2MEventDriven*> *edContainner =
			new std::vector<M2MEventDriven*>;

	//Create UEs
	int idUE = 1;

	//NbIoT traffic
	int nbED = ceil(0.3 * nbUE);
	int nbTD = nbUE - nbED;

	//Mixed traffic;
	int nbCBR = ceil(nbUE * 0.35);
	int nbVideo = ceil(nbUE * 0.45);
	int nbVoIP = nbUE - nbCBR - nbVideo;

	for (int i = 0; i < nbUE; i++) {
		//ue's random position
		double posX = (double) rand() / RAND_MAX;
		posX = 0.95 * (((2 * radius * 1000) * posX) - (radius * 1000));
		double posY = (double) rand() / RAND_MAX;
		posY = 0.95 * (((2 * radius * 1000) * posY) - (radius * 1000));

		double speedDirection = GetRandomVariable(360.) * ((2. * 3.14) / 360.);

		//UserEquipment* ue = new UserEquipment(idUE, posX, posY, c, enb, NetworkNode::TYPE_NBIOT_UE, 0); //handover false!
		UserEquipment* ue = new UserEquipment(idUE, posX, posY, c, enb, 0,
				Mobility::CONSTANT_POSITION);

		std::cout << "Created UE - id " << idUE << " position " << posX << " "
				<< posY << " direction " << speedDirection << std::endl;

		//ue->GetPhy()->SetBandwidthManager(nbiotSpectrum);
		ue->GetPhy()->SetBandwidthManager(spectrum);

		ue->GetMobilityModel()->GetAbsolutePosition()->Print();
		ue->GetPhy()->SetDlChannel(enb->GetPhy()->GetDlChannel());
		ue->GetPhy()->SetUlChannel(enb->GetPhy()->GetUlChannel());

		FullbandCqiManager *cqiManager = new FullbandCqiManager();
		cqiManager->SetCqiReportingMode(CqiManager::PERIODIC);
		cqiManager->SetReportingInterval(1);
		cqiManager->SetDevice(ue);
		ue->SetCqiManager(cqiManager);

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

		// CREATE UPLINK APPLICATION FOR THIS UE
		double start_time = 0.1 + GetRandomVariable(0.4);
		double duration_time = start_time + flow_duration;

		if (strcmp(trafficType, "nbiot") == 0) {

			if (nbED > 0) {
				M2MEventDriven* edApplication = new M2MEventDriven();
				edApplication->SetSource(ue);
				edApplication->SetDestination(gw);
				edApplication->SetApplicationID(applicationID);
				edApplication->SetStartTime(start_time);
				edApplication->SetStopTime(duration_time);

				// create qos parameters
				QoSParameters *qosParameters = new QoSParameters();
				qosParameters->SetMaxDelay(0.05); //50ms
				edApplication->SetQoSParameters(qosParameters);

				//create classifier parameters
				ClassifierParameters *cp = new ClassifierParameters(
						ue->GetIDNetworkNode(), gw->GetIDNetworkNode(), 0,
						destinationPort,
						TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
				edApplication->SetClassifierParameters(cp);

				edContainner->push_back(edApplication);
				std::cout << "CREATED M2M_ED APPLICATION, ID " << applicationID
						<< std::endl;
				nbED--;
			} else {
				M2MTimeDriven* tdApplication = new M2MTimeDriven();
				tdApplication->SetSource(ue);
				tdApplication->SetDestination(gw);
				tdApplication->SetApplicationID(applicationID);
				tdApplication->SetStartTime(start_time);
				tdApplication->SetStopTime(duration_time);

				// create qos parameters
				QoSParameters *qosParameters = new QoSParameters();
				qosParameters->SetMaxDelay(tdApplication->GetInterval());
				tdApplication->SetQoSParameters(qosParameters);

				//create classifier parameters
				ClassifierParameters *cp = new ClassifierParameters(
						ue->GetIDNetworkNode(), gw->GetIDNetworkNode(), 0,
						destinationPort,
						TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
				tdApplication->SetClassifierParameters(cp);

				tdContainner->push_back(tdApplication);
				std::cout << "CREATED M2M_TD APPLICATION, ID " << applicationID
						<< ", Interval: " << tdApplication->GetInterval()
						<< std::endl;
			}

		} else if (strcmp(trafficType, "voip") == 0) {
			VoIP* voIPApplication = new VoIP();
			voIPApplication->SetSource(ue);
			voIPApplication->SetDestination(gw);
			voIPApplication->SetApplicationID(applicationID);
			voIPApplication->SetStartTime(start_time);
			voIPApplication->SetStopTime(duration_time);

			QoSParameters *qos = new QoSParameters();
			qos->SetMaxDelay(0.1); //100ms
			voIPApplication->SetQoSParameters(qos);

			//create classifier parameters
			ClassifierParameters *cp = new ClassifierParameters(
					ue->GetIDNetworkNode(), gw->GetIDNetworkNode(), 0,
					destinationPort,
					TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
			voIPApplication->SetClassifierParameters(cp);

			std::cout << "CREATED VOIP APPLICATION, ID " << applicationID
					<< std::endl;

			voipContainner->push_back(voIPApplication);

		} else if (strcmp(trafficType, "video") == 0) {
			TraceBased* videoApplication = new TraceBased();
			videoApplication->SetSource(ue);
			videoApplication->SetDestination(gw);
			videoApplication->SetApplicationID(applicationID);
			videoApplication->SetStartTime(start_time);
			videoApplication->SetStopTime(duration_time);

			string video_trace("foreman_H264_");
			//string video_trace ("highway_H264_");
			//string video_trace ("mobile_H264_");

			string _file(
					path + "src/flows/application/Trace/" + video_trace
							+ "128k.dat");
			videoApplication->SetTraceFile(_file);
			std::cout << "		selected video @ 128k " << _file << std::endl;

			QoSParameters *qos = new QoSParameters();
			qos->SetMaxDelay(0.150); //150ms
			videoApplication->SetQoSParameters(qos);

			//create classifier parameters
			ClassifierParameters *cp = new ClassifierParameters(
					ue->GetIDNetworkNode(), gw->GetIDNetworkNode(), 0,
					destinationPort,
					TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
			videoApplication->SetClassifierParameters(cp);

			std::cout << "CREATED VIDEO APPLICATION, ID " << applicationID
					<< std::endl;

			videoContainner->push_back(videoApplication);

		} else if (strcmp(trafficType, "cbr") == 0) {
			CBR* cbrApplication = new CBR();
			cbrApplication->SetSource(ue);
			cbrApplication->SetDestination(gw);
			cbrApplication->SetApplicationID(applicationID);
			cbrApplication->SetStartTime(start_time);
			cbrApplication->SetStopTime(duration_time);
			cbrApplication->SetInterval(0.016);
			cbrApplication->SetSize(256);

			// create qos parameters
			QoSParameters *qosParameters = new QoSParameters();
			qosParameters->SetMaxDelay(0.300); // 300ms

			cbrApplication->SetQoSParameters(qosParameters);

			//create classifier parameters
			ClassifierParameters *cp = new ClassifierParameters(
					ue->GetIDNetworkNode(), gw->GetIDNetworkNode(), 0,
					destinationPort,
					TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
			cbrApplication->SetClassifierParameters(cp);

			std::cout << "CREATED CBR APPLICATION, ID " << applicationID
					<< std::endl;

			cbrContainner->push_back(cbrApplication);

		} else if (strcmp(trafficType, "mixed") == 0) {

			if (nbCBR > 0) {
				CBR* cbrApplication = new CBR();
				cbrApplication->SetSource(ue);
				cbrApplication->SetDestination(gw);
				cbrApplication->SetApplicationID(applicationID);
				cbrApplication->SetStartTime(start_time);
				cbrApplication->SetStopTime(duration_time);
				cbrApplication->SetInterval(0.016);
				cbrApplication->SetSize(256);

				// create qos parameters
				QoSParameters *qosParameters = new QoSParameters();
				qosParameters->SetMaxDelay(0.300); // 300ms

				cbrApplication->SetQoSParameters(qosParameters);

				//create classifier parameters
				ClassifierParameters *cp = new ClassifierParameters(
						ue->GetIDNetworkNode(), gw->GetIDNetworkNode(), 0,
						destinationPort,
						TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
				cbrApplication->SetClassifierParameters(cp);

				std::cout << "CREATED CBR APPLICATION, ID " << applicationID
						<< ", Interval: " << cbrApplication->GetInterval()
						<< std::endl;

				cbrContainner->push_back(cbrApplication);
				nbCBR--;
			} else if (nbVideo > 0) {
				TraceBased* videoApplication = new TraceBased();
				videoApplication->SetSource(ue);
				videoApplication->SetDestination(gw);
				videoApplication->SetApplicationID(applicationID);
				videoApplication->SetStartTime(start_time);
				videoApplication->SetStopTime(duration_time);

				string video_trace("foreman_H264_");
				//string video_trace ("highway_H264_");
				//string video_trace ("mobile_H264_");

				string _file(
						path + "src/flows/application/Trace/" + video_trace
								+ "128k.dat");
				videoApplication->SetTraceFile(_file);
				std::cout << "		selected video @ 128k " << _file << std::endl;

				QoSParameters *qos = new QoSParameters();
				qos->SetMaxDelay(0.150); //150ms
				videoApplication->SetQoSParameters(qos);

				//create classifier parameters
				ClassifierParameters *cp = new ClassifierParameters(
						ue->GetIDNetworkNode(), gw->GetIDNetworkNode(), 0,
						destinationPort,
						TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
				videoApplication->SetClassifierParameters(cp);

				std::cout << "CREATED VIDEO APPLICATION, ID " << applicationID
						<< std::endl;

				videoContainner->push_back(videoApplication);
				nbVideo--;
			} else {
				VoIP* voIPApplication = new VoIP();
				voIPApplication->SetSource(ue);
				voIPApplication->SetDestination(gw);
				voIPApplication->SetApplicationID(applicationID);
				voIPApplication->SetStartTime(start_time);
				voIPApplication->SetStopTime(duration_time);

				QoSParameters *qos = new QoSParameters();
				qos->SetMaxDelay(0.1); //100ms
				voIPApplication->SetQoSParameters(qos);

				//create classifier parameters
				ClassifierParameters *cp = new ClassifierParameters(
						ue->GetIDNetworkNode(), gw->GetIDNetworkNode(), 0,
						destinationPort,
						TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
				voIPApplication->SetClassifierParameters(cp);

				std::cout << "CREATED VOIP APPLICATION, ID " << applicationID
						<< std::endl;

				voipContainner->push_back(voIPApplication);
			}
		} else {
			std::cout << "\tThe traffic \"" << trafficType
					<< "\" is not implemented!\n\t options are:\n \tnbiot\n\tvoip\n\tvideo\n\tcbr\n\tmixed\n"
					<< std::endl;
			return;
		}

		destinationPort++;
		applicationID++;
		idUE++;

	}

	simulator->SetStop(duration);
	simulator->Run();

	//Delete created objects
	cbrContainner->clear();
	delete cbrContainner;
	voipContainner->clear();
	delete voipContainner;
	tdContainner->clear();
	delete tdContainner;
	edContainner->clear();
	delete edContainner;
	videoContainner->clear();
	delete videoContainner;
	eNBs->clear();
	delete eNBs;
	delete frameManager;
	//delete nm;
	delete simulator;

}
