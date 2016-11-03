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


#ifndef NBIOT_BW_MANAGER_HELPER_H_
#define NBIOT_BW_MANAGER_HELPER_H_

#include <stdint.h>
#include "stdlib.h"
#include <math.h>
#include <vector>
#include "../core/spectrum/bandwidth-manager.h"
#include "../componentManagers/FrameManager.h"


// Extract a RB from a regular bandwidth manager
// to create a nbiot bandwidth manager

NbIotBandwidthManager* createNbIotBwManager(BandwidthManager* bwManager, double txBwConf) {
	vector<double> ulSubChannels = bwManager->GetUlSubChannels();
	vector<double> dlSubChannels = bwManager->GetDlSubChannels();

	double ulRb = ulSubChannels.back();
	ulSubChannels.pop_back();
	bwManager->SetUlSubChannels(ulSubChannels);
	bwManager->SetUlBandwidth(ulSubChannels.size());

	double dlRb = dlSubChannels.back();
	dlSubChannels.pop_back();
	bwManager->SetDlSubChannels(dlSubChannels);
	bwManager->SetDlBandwidth(dlSubChannels.size());

	return new NbIotBandwidthManager(ulRb, dlRb, txBwConf, txBwConf);
}




#endif /* NBIOT_BW_MANAGER_HELPER_H_ */
