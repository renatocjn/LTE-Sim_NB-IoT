/*
 * M2MTimeDriven.h
 *
 *  Created on: 08/11/2016
 *      Author: great
 */

#ifndef M2MTIMEDRIVEN_H_
#define M2MTIMEDRIVEN_H_

#include "CBR.h"
#include <vector>

class M2MTimeDriven: public CBR {
public:
	M2MTimeDriven();
	virtual ~M2MTimeDriven();
private:
	static const int PKGSIZE = 125;
	static const double INTERVAL_MIN = 0.05;
	static const double INTERVAL_MAX = 5.0;
};

#endif /* M2MTIMEDRIVEN_H_ */
