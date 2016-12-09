/*
 * NbIotAMCModule.h
 *
 *  Created on: 28/11/2016
 *      Author: great
 */

#ifndef SRC_PROTOCOLSTACK_MAC_UL_NBIOT_AMCMODULE_H_
#define SRC_PROTOCOLSTACK_MAC_UL_NBIOT_AMCMODULE_H_

#include "AMCModule.h"

class UlNbIotAMCModule: public AMCModule {
public:
	UlNbIotAMCModule();
	virtual ~UlNbIotAMCModule();

	int
	GetTBSizeFromMCS (int mcs);
	int
	GetTBSizeFromMCS (int mcs, int nbRBs);
};

#endif /* SRC_PROTOCOLSTACK_MAC_UL_NBIOT_AMCMODULE_H_ */
