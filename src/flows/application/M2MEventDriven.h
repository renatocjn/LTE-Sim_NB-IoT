/*
 * M2MEventDriven.h
 *
 *  Created on: 08/11/2016
 *      Author: great
 */

#ifndef SRC_FLOWS_APPLICATION_M2MEVENTDRIVEN_H_
#define SRC_FLOWS_APPLICATION_M2MEVENTDRIVEN_H_

#include "Application.h"
#include <vector>

class M2MEventDriven: public Application {
public:
	M2MEventDriven();
	virtual ~M2MEventDriven();

	virtual void DoStart (void);
	virtual void DoStop (void);

	void ScheduleTransmit (double time);
	void Send (void);
private:
	static std::vector<double> makeVectorP();
	static std::vector<double> P = makeVectorP();
	static const int PKGSIZE = 125;
};

#endif /* SRC_FLOWS_APPLICATION_M2MEVENTDRIVEN_H_ */
