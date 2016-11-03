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


#ifndef NbIoTBandwidthManager_H_
#define NbIoTBandwidthManager_H_

#include "bandwidth-manager.h"
#include <vector>

/*
 * This class models the bandwidth used for the transmission
 * in a particular cell.
 * See information on TR 36.101 - Table 5.5-1
 */
class NbIotBandwidthManager : public BandwidthManager {
public:
	NbIotBandwidthManager() {}
	NbIotBandwidthManager(double ulRbChannel, double dlRbChannel, double ulTxBwConf, double dlTxBwConf);
	virtual ~NbIotBandwidthManager();


	BandwidthManager* Copy ();

	void Print (void);
};

#endif /* NbIoTBandwidthManager_H_ */
