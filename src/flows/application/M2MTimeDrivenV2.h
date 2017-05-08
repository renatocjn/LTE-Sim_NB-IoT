/*
 * M2MTimeDriven.h
 *
 *  Created on: 08/11/2016
 *      Author: great
 */

#ifndef M2MTIMEDRIVENV2_H_
#define M2MTIMEDRIVENV2_H_

#include "CBR.h"
#include <vector>

class M2MTimeDrivenV2: public CBR {
public:
	M2MTimeDrivenV2();
	virtual ~M2MTimeDrivenV2();
private:
	static const int PKGSIZE = 125;

	// average of ~ 6.25 kbps of traffic generated per user
	static const double INTERVAL_MIN = 0.05;
	static const double INTERVAL_MAX = 0.32;
};

#endif /* M2MTIMEDRIVEN_H_ */
