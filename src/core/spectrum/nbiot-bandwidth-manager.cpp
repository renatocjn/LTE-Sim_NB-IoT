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


#include "nbiot-bandwidth-manager.h"
#include <stdio.h>
#include <iostream>
#include <vector>

#define TONES_FOR_3_75KHz 48
#define TONES_FOR_15KHz 12

NbIotBandwidthManager::NbIotBandwidthManager()
{}


NbIotBandwidthManager::NbIotBandwidthManager(double ulRbChannel, double dlRbChannel, double ulTxBwConf, double dlTxBwConf)
{
	// txBwConf = Transmission bandwidth configuration (3.75 / 15)
	std::vector<double> dlSubChannels;
	std::vector<double> ulSubChannels;

	if (ulTxBwConf == 3.75) {
		SetUlBandwidth(TONES_FOR_3_75KHz);
	} else if (ulTxBwConf == 15) {
		SetUlBandwidth(TONES_FOR_15KHz);
	} else {
		std::cout << "NbIotBandwidthManager: Invalid ulTxBwConf" << std::endl;
		SetUlBandwidth(TONES_FOR_15KHz);
		ulTxBwConf = 15;
	}

	if (dlTxBwConf == 3.75) {
		SetDlBandwidth(TONES_FOR_3_75KHz);
	} else if (dlTxBwConf == 15){
		SetDlBandwidth(TONES_FOR_15KHz);
	} else {
		std::cout << "NbIotBandwidthManager: Invalid dlTxBwConf" << std::endl;
		SetDlBandwidth(TONES_FOR_15KHz);
		dlTxBwConf = 15;
	}

	ulTxBwConf *= 0.001;
	dlTxBwConf *= 0.001;
	double i;
	for(i=ulRbChannel; i<ulRbChannel+.18; i+=ulTxBwConf) {
		ulSubChannels.push_back(i);
	}
	SetUlSubChannels(ulSubChannels);

	for(i=dlRbChannel; i<dlRbChannel+.18; i+=dlTxBwConf) {
		dlSubChannels.push_back(i);
	}
	SetDlSubChannels(dlSubChannels);
}

NbIotBandwidthManager::~NbIotBandwidthManager() {

}

BandwidthManager*
NbIotBandwidthManager::Copy ()
{
	NbIotBandwidthManager *s = new NbIotBandwidthManager ();
	s->SetDlBandwidth (GetDlBandwidth ());
	s->SetUlBandwidth (GetUlBandwidth ());
	//s->SetDlOffsetBw (GetDlOffsetBw ());
	//s->SetUlOffsetBw (GetUlOffsetBw ());
	s->SetDlSubChannels (GetDlSubChannels ());
	s->SetUlSubChannels (GetUlSubChannels ());
	s->SetOperativeSubBand (GetOperativeSubBand ());

	return s;
}

void
NbIotBandwidthManager::Print (void)
{
	std::cout << "NbIotBandwidthManager: " << std::endl;

	std::cout << "\t operative sub band: " << GetOperativeSubBand() <<
	"\n\t m_dlBandwidth " << GetDlBandwidth() <<
	"\n\t m_ulBandwidth " << GetUlBandwidth() << std::endl;

	std::vector<double> subchannels = GetDlSubChannels();
	std::cout << "\t DL channels: ";
	for (unsigned i = 0; i < subchannels.size (); i++)
	{
		std::cout << subchannels.at (i) << " ";
	}
	std::cout <<  std::endl;

	subchannels = GetUlSubChannels();
	std::cout << "\t UL channels: ";
	for (unsigned i = 0; i < subchannels.size (); i++)
	{
		std::cout << subchannels.at (i) << " ";
	}
	std::cout <<  std::endl;
}


