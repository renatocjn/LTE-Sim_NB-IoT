/*
 * NbIotAMCModule.h
 *
 *  Created on: 28/11/2016
 *      Author: great
 */

#ifndef SRC_PROTOCOLSTACK_MAC_DL_NBIOT_AMCMODULE_H_
#define SRC_PROTOCOLSTACK_MAC_DL_NBIOT_AMCMODULE_H_

#include "AMCModule.h"

class DlNbIotAMCModule: public AMCModule {
public:
	DlNbIotAMCModule();
	virtual ~DlNbIotAMCModule();

	int GetTBSizeFromMCS(int mcs, int nbSFs);

	int GetMaxNumberOfSfForMCS(int mcs);
};

#endif /* SRC_PROTOCOLSTACK_MAC_DL_NBIOT_AMCMODULE_H_ */
