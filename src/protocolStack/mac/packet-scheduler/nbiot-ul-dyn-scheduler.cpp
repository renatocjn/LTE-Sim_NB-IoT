#include "nbiot-ul-dyn-scheduler.h"

#include <cstdio>
#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "../../../core/eventScheduler/simulator.h"
#include "../../../flows/QoS/QoSParameters.h"
#include "../../../utility/eesm-effective-sinr.h"
#include "uplink-packet-scheduler.h"

#define INF 9999999.0

const double NbIotUlDynScheduler::RU_DURATIONS[4] = { 0.008, 0.004, 0.002, 0.001 };
const int NbIotUlDynScheduler::SUBCARRIERS_FOR_CONFIGURATION[4] = { 1, 3, 6, 12 };

NbIotUlDynScheduler::NbIotUlDynScheduler(double rho) :
		NbIotUlScheduler(15, 1) {
	for (int i = 0; i < NUMBER_OF_SUBCARRIERS; i++) {
		allocationTimes[i] = -1;
	}

	for (int i = 0; i < NUMBER_OF_CONFIGURATIONS; i++) {
		int nsc = NUMBER_OF_SUBCARRIERS / SUBCARRIERS_FOR_CONFIGURATION[i];
		allocationLimits[i] = new double[nsc];
		for (int j = 0; j < nsc; j++) {
			allocationLimits[i][j] = -1;
		}
	}

	this->rho = rho;
}

NbIotUlDynScheduler::~NbIotUlDynScheduler() {
}

void NbIotUlDynScheduler::RBsAllocation() {
	updateTimesVectors();

	//Count number of free Sub Carriers
	int freeSCs = NUMBER_OF_SUBCARRIERS;
	for (int i = 0; i < NUMBER_OF_SUBCARRIERS; i++) {
		if (allocationTimes[i] > 0)
			freeSCs--;
	}

	UsersToSchedule* users = GetUsersToSchedule();
	double *metrics = getUserMetrics();

	set<UserToSchedule*> alreadyScheduledUsers;
	UserToSchedule* chosenUser;

//#ifdef NBIOT_DEBUG
//	std::cout << "[NBIOT_DEBUG] Users to be scheduled: " << users->size() << std::endl;
//#endif

	while (freeSCs > 0 && alreadyScheduledUsers.size() < users->size()) {

#ifdef NBIOT_DEBUG
		std::cout << "[NBIOT_DEBUG] ****** Free SCs: " << freeSCs << " ****** " << Simulator::Init()->Now() << std::endl;
#endif

		//Pick next user to be scheduled
		double maxMetric = -INF;
		for (int i = 0; i < users->size(); i++) {
			bool userNotAlreadyScheduled = alreadyScheduledUsers.find(users->at(i)) == alreadyScheduledUsers.end();
			if (metrics[i] >= maxMetric && userNotAlreadyScheduled)
				chosenUser = users->at(i);
		}
		alreadyScheduledUsers.insert(chosenUser);

#ifdef NBIOT_DEBUG
		std::cout << "[NBIOT_DEBUG] * Chosen user: " << chosenUser->m_userToSchedule->GetIDNetworkNode() << " *" << std::endl;
#endif

		//Get values for required number of resource units and
		// required configuration
		int Nru = getNruRequired(chosenUser);
		int C = getRequiredConfigurationForUser(chosenUser, Nru);
		if (C < 0)
			continue; //could not find a fast enough configuration for this userri

		//allocate or reserve resources
		C=3;
		int rui = isConfigurationAvailable(C);
		if (rui < 0) {
			reserveConfiguration(C, chosenUser);
		} else {
//			allocateUser(chosenUser, C, Nru, rui);
			allocateUser(chosenUser, C, 1, rui);
			freeSCs -= SUBCARRIERS_FOR_CONFIGURATION[C];
		}
	}

	//Get the interval until the next schedule time
	double smallerAllocation = INF;
	bool updated = false;
	for (int i = 0; i < NUMBER_OF_SUBCARRIERS; i++) {
		if (allocationTimes[i] > 0 && allocationTimes[i] < smallerAllocation) {
			smallerAllocation = allocationTimes[i];
			updated = true;
		}
	}

	if (updated)
		UpdateNextScheduleTime(0.001);
}

double* NbIotUlDynScheduler::getUserMetrics() {
	UsersToSchedule *users = GetUsersToSchedule();
	double *metrics = new double[users->size()];
	const double reservedConfigurationPriority[4] = {700000.0, 800000.0, 900000.0, 1000000.0};

	for (int i = 0; i < users->size(); i++) {
		UserToSchedule* u = users->at(i);
		if (reservedUsers.find(u) != reservedUsers.end()) {
			int nru = getNruRequired(u);
			int c = getRequiredConfigurationForUser(u, nru);
			if (c==-1)
				metrics[i] = -INF;
			else
				metrics[i] = reservedConfigurationPriority[c];
		} else {
			double effectiveSinr = GetEesmEffectiveSinr(users->at(i)->m_channelContition);
			metrics[i] = ComputeSchedulingMetric(users->at(i), effectiveSinr);
		}
	}

#ifdef NBIOT_DEBUG
	std::cout << "[NBIOT_DEBUG] User metrics: ";
	for (int i = 0; i < users->size(); i++) {
		std::printf("%.5f ", metrics[i]);
	}
	std::cout << std::endl;
#endif

	return metrics;
}

void NbIotUlDynScheduler::updateTimesVectors() {
	double deltat = Simulator::Init()->Now() - lastUpdate;

	for (int i = 0; i < NUMBER_OF_CONFIGURATIONS; i++) {
		for (int j = 0; j < NUMBER_OF_SUBCARRIERS / SUBCARRIERS_FOR_CONFIGURATION[i]; j++)
			allocationLimits[i][j] -= deltat;
	}

	for (int i = 0; i < NUMBER_OF_SUBCARRIERS; i++) {
		allocationTimes[i] -= deltat;
	}

	lastUpdate = Simulator::Init()->Now();

#ifdef NBIOT_DEBUG
	std::cout << "[NBIOT_DEBUG] ******** SC allocation vector ********" << std::endl;
	std::cout << "[NBIOT_DEBUG] ";
	for (int i = 0; i < NUMBER_OF_SUBCARRIERS; i++)
	std::printf("%.3f ", allocationTimes[i]);
	std::cout << std::endl;

	std::cout << "[NBIOT_DEBUG] ******** SC allocation limit ********" << std::endl;
	for (int i = 0; i < NUMBER_OF_CONFIGURATIONS; i++) {
		int nsc = NUMBER_OF_SUBCARRIERS / SUBCARRIERS_FOR_CONFIGURATION[i];
		std::cout << "[NBIOT_DEBUG] Configuration " << i << " | ";
		for (int j = 0; j < nsc; j++) {
			std::printf("%.3f ", allocationLimits[i][j]);
		}
		std::cout << std::endl;
	}
#endif
}

int NbIotUlDynScheduler::getNruRequired(UserToSchedule* user) {
	double effectiveSinr = GetEesmEffectiveSinr(user->m_channelContition);
	int CQI = amc.GetCQIFromSinr(effectiveSinr);
	int MCS = amc.GetMCSFromCQI(CQI);

	double holReportRemainingData = user->m_headOfLinePkgSize * 8.0;
//	double holReportRemainingData = 125.0 * 8.0;

//	std::cout << "[getNruRequired] holReportRemainingData " << holReportRemainingData/8.0 << std::endl;
//	std::cout << "[getNruRequired] MCS " << MCS << std::endl;

	int Nru = amc.GetMaxNumberOfRuForMCS(MCS);
//	std::cout << "[getNruRequired] MaxNumberOfRuForMCS " << Nru << std::endl;
	for (int i = 1; i <= amc.GetMaxNumberOfRuForMCS(MCS); i++) {
		if (i == 7 || i == 9)
			continue;

		if (holReportRemainingData <= amc.GetTBSizeFromMCS(MCS, i)) {
			Nru = i;
			break;
		}
	}

	return Nru;
}

int NbIotUlDynScheduler::getRequiredConfigurationForUser(UserToSchedule* u, int nru) {
	double maxDelay = u->m_qosParameters->GetMaxDelay();
	double holDelay = u->m_headOfLineDelay;
	int conf = -1;

//
//	std::cout << "[getRequiredConfigurationForUser] maxDelay " << maxDelay << std::endl;
//	std::cout << "[getRequiredConfigurationForUser] holDelay " << holDelay << std::endl;

	for (int i = 0; i < NUMBER_OF_CONFIGURATIONS; i++) {
		if ((1-rho) * maxDelay >= (RU_DURATIONS[i] * nru) + holDelay) {
			conf = i;
			break;
		}
	}
	return conf;
}

int NbIotUlDynScheduler::isConfigurationAvailable(int conf) {
	int nRuForC = NUMBER_OF_SUBCARRIERS / SUBCARRIERS_FOR_CONFIGURATION[conf];
	int nsc = SUBCARRIERS_FOR_CONFIGURATION[conf];

	for (int i = 0; i < nRuForC; i++) {
		bool isBlocked = (isRuBlocked(conf, i) && getRuBlockedTime(conf, i) < RU_DURATIONS[conf]);
		if (isBlocked)
			continue;

		bool free = true;
		for (int j = 0; j < nsc; j++) {
			if (allocationTimes[i * nsc + j] > 0) {
				free = false;
				break;
			}
		}

		if (free) {
#ifdef NBIOT_DEBUG
			std::cout << "[NBIOT_DEBUG] RU " << i << " for configuration " << conf << " available" << std::endl;
#endif
			return i;
		}
	}
#ifdef NBIOT_DEBUG
	std::cout << "[NBIOT_DEBUG] No RU available for configuration " << conf << std::endl;
#endif
	return -1;
}

void NbIotUlDynScheduler::allocateUser(UserToSchedule* user, int conf, int nru, int rui) {
	double effectiveSinr = GetEesmEffectiveSinr(user->m_channelContition);
	int CQI = amc.GetCQIFromSinr(effectiveSinr);
	int MCS = amc.GetMCSFromCQI(CQI);

	//Limit the number of RUs to non reserved space
	if (isRuBlocked(conf, rui)) {
		nru = std::min(nru, (int) std::floor(getRuBlockedTime(conf, rui) / RU_DURATIONS[conf]));
		if (nru == 7 || nru == 9)
			nru--;
	}
	int TBS = ((amc.GetTBSizeFromMCS(MCS, nru)) / 8);

	//Allocate resources
	const int nSC = SUBCARRIERS_FOR_CONFIGURATION[conf];
	for (int i = 0; i < nSC; i++)
		user->m_listOfAllocatedRBs.push_back(rui*nSC + i);
	user->m_transmittedData = TBS;
	user->m_selectedMCS = MCS;

	//Update time vector
	for (int i = 0; i < nSC; i++)
		allocationTimes[rui * nSC + i] = nru * RU_DURATIONS[conf];

	if (reservedUsers.find(user) != reservedUsers.end())
		reservedUsers.erase(user);

#ifdef NBIOT_DEBUG
	std::cout << "[NBIOT_DEBUG] Allocated RU " << rui << " Nru " << nru << " C " << conf << " deltat " << nru * RU_DURATIONS[conf] << " txBytes " << TBS << " T " << Simulator::Init()->Now() << std::endl;
#endif
}

void NbIotUlDynScheduler::reserveConfiguration(int conf, UserToSchedule* user) {
	int nruForC = NUMBER_OF_SUBCARRIERS / SUBCARRIERS_FOR_CONFIGURATION[conf];
	int nsc = SUBCARRIERS_FOR_CONFIGURATION[conf];

	const double max = INF;
	double chosenRuMax = max;
	int chosenRuIdx = -1;

	for (int i = 0; i < nruForC; i++) {
		double ruiMax = -max; // RU longest SC allocation

		if (isRuBlocked(conf, i)) { //Dont reserve RU again
			continue;
		}

		for (int j = 0; j < nsc; j++) {
			if (allocationTimes[i * nsc + j] > ruiMax)
				ruiMax = allocationTimes[i * nsc + j];
		}

		//Choose RU that will be free first
		if (ruiMax < chosenRuMax) {
			chosenRuMax = ruiMax;
			chosenRuIdx = i;
		}
	}

	//If there is any non-reserved RU and the user can wait
	if (chosenRuIdx != -1 && user->m_qosParameters->GetMaxDelay() > user->m_headOfLineDelay + chosenRuMax) {
		allocationLimits[conf][chosenRuIdx] = chosenRuMax;
		reservedUsers.insert(user);
#ifdef NBIOT_DEBUG
		std::cout << "[NBIOT_DEBUG] Reserved RU " << chosenRuIdx << " in configuration " << conf << std::endl;
#endif

	}
#ifdef NBIOT_DEBUG
	else {
		std::cout << "[NBIOT_DEBUG] There isnt any unreserved RU with configuration " << conf << std::endl;
	}
#endif
}

double NbIotUlDynScheduler::ComputeSchedulingMetric(UserToSchedule* user, double effectiveSinr) {
//	return exp(user->m_headOfLineDelay / user->m_qosParameters->GetMaxDelay()); //ExpDelay metric

	int cqi = amc.GetCQIFromSinr(effectiveSinr);
	int mcs = amc.GetMCSFromCQI(cqi);

//	//Assuming maximum txrate possible (12-tone transmission)
	double instTxRate = amc.GetTBSizeFromMCS(mcs, amc.GetMaxNumberOfRuForMCS(mcs)) / amc.GetMaxNumberOfRuForMCS(mcs);

	return instTxRate / user->m_averageTransmissionRate; //PF metric

//	MLWDF metric
//	source: Aiyetoro et al. Performance Analysis of M-LWDF and EXP-PF Schedulers for Real Time Traffic in Satellite LTE Networks.
//	const double dropProbability = 0.1; //P <= 10%
//	double metric = -std::log10(dropProbability) * //this equals to 1 if dropProbability is 10%
//			(instTxRate / user->m_averageTransmissionRate) * // this is kinda hard for NB-IoT because of multi-tone and single-tone transmissions
//			(user->m_headOfLineDelay / user->m_qosParameters->GetMaxDelay());
//	return metric;
}

bool NbIotUlDynScheduler::isRuBlocked(int conf, int i) {
	if (getRuBlockedTime(conf, i) > 0)
		return true;
	else
		return false;
}

double NbIotUlDynScheduler::getRuBlockedTime(int conf, int i) {
	if (conf == 3) {
		if (allocationLimits[conf][0] > 0.0)
			return allocationLimits[conf][0];
		else
			return 0;

	} else {
		const int tmp[4] = { 3, 2, 2, 2 };
		double t = getRuBlockedTime(conf + 1, i / tmp[conf]);

		if (t > 0.0)
			return t;
		else
			return allocationLimits[conf][i];

	}
}
