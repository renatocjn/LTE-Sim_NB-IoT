#ifndef SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_DYN_SCHEDULER_H_
#define SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_NBIOT_UL_DYN_SCHEDULER_H_

#include "nbiot-ul-scheduler.h"
#include "../ul-nbiot-AMCModule.h"

class NbIotUlDynScheduler: public NbIotUlScheduler {
public:
	NbIotUlDynScheduler(double rho);
	virtual ~NbIotUlDynScheduler();

	virtual void RBsAllocation();

	virtual double ComputeSchedulingMetric(UserToSchedule* user, double effectiveSinr);

private:
	static const double RU_DURATIONS[4];
	static const int SUBCARRIERS_FOR_CONFIGURATION[4];
	static const int NUMBER_OF_CONFIGURATIONS = 4;
	static const int NUMBER_OF_SUBCARRIERS = 12;

	UlNbIotAMCModule amc;
	double allocationTimes[NUMBER_OF_SUBCARRIERS];
	double* allocationLimits[NUMBER_OF_CONFIGURATIONS];
	double lastUpdate = 0;

	set<UserToSchedule*> reservedUsers;

	double rho;

	void updateTimesVectors();
	double* getUserMetrics();

	int getNruRequired(UserToSchedule* u);
	int getRequiredConfigurationForUser(UserToSchedule* u, int nru);

	int isConfigurationAvailable(int conf);
	void allocateUser(UserToSchedule*, int conf, int nru, int ru);
	void reserveConfiguration(int conf, UserToSchedule* user);

	bool isRuBlocked(int conf, int i);
	double getRuBlockedTime(int conf, int i);
};
#endif
