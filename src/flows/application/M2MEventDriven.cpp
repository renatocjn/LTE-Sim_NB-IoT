/*
 * M2MEventDriven.cpp
 *
 *  Created on: 08/11/2016
 *      Author: great
 */

#include "M2MEventDriven.h"
#include "../../core/eventScheduler/simulator.h"
#include "../../utility/RandomVariable.h"
#include "../radio-bearer.h"
#include <math.h>
#include <stdlib.h>
#define MAX_TTI 350

M2MEventDriven::M2MEventDriven() {
	SetApplicationType(Application::APPLICATION_TYPE_M2M_ED);
}

std::vector<double> M2MEventDriven::makeVectorP(){
	std::vector<double> localP;
	localP.push_back(0);
	for (int i=1; i<MAX_TTI; i++) {
		localP.push_back(1.0 - exp(-0.02*i));
	}
	return localP;
}

M2MEventDriven::~M2MEventDriven() {
	Destroy();
}

void
M2MEventDriven::DoStart (void)
{
	Simulator::Init()->Schedule(0.0, &M2MEventDriven::Send, this);
}

void
M2MEventDriven::DoStop (void)
{}

void
M2MEventDriven::ScheduleTransmit (double time)
{
	if ( (Simulator::Init()->Now () + time) < GetStopTime () )
	{
		Simulator::Init()->Schedule(time, &M2MEventDriven::Send, this);
	}
}


void
M2MEventDriven::Send (void)
{
	//CREATE A NEW PACKET (ADDING UDP, IP and PDCP HEADERS)
	Packet *packet = new Packet ();
	int uid = Simulator::Init()->GetUID ();

	packet->SetID(uid);
	packet->SetTimeStamp (Simulator::Init()->Now ());
	packet->SetSize (PKGSIZE);

	PacketTAGs *tags = new PacketTAGs ();
	tags->SetApplicationType(PacketTAGs::APPLICATION_TYPE_M2M_ED);
	tags->SetApplicationSize (packet->GetSize ());
	packet->SetPacketTags(tags);


	UDPHeader *udp = new UDPHeader (GetClassifierParameters ()->GetSourcePort (),
			GetClassifierParameters ()->GetDestinationPort ());
	packet->AddUDPHeader (udp);

	IPHeader *ip = new IPHeader (GetClassifierParameters ()->GetSourceID (),
			GetClassifierParameters ()->GetDestinationID ());
	packet->AddIPHeader (ip);

	PDCPHeader *pdcp = new PDCPHeader ();
	packet->AddPDCPHeader (pdcp);

	Trace (packet);

	GetRadioBearer()->Enqueue (packet);

	int t = 1;
	double r = rand()/RAND_MAX;
	while(r > P[t] && t<MAX_TTI) t++;
	ScheduleTransmit (t);
}
