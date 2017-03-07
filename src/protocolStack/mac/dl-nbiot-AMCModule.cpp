/*
 * NbIotAMCModule.cpp
 *
 *  Created on: 28/11/2016
 *      Author: great
 */

#include "dl-nbiot-AMCModule.h"

// Represents table 16.4.1.5.1-1 from 3GPP TS 36.213
const unsigned short tbsTable[13][8] = { // [mcs][nrb]
		{  16,   32,   56,   88,  120,  152,  208,  256 },
		{  24,   56,   88,  144,  176,  208,  256,  344 },
		{  32,   72,  144,  176,  208,  256,  328,  424 },
		{  40,  104,  176,  208,  256,  328,  440,  568 },
		{  56,  120,  208,  256,  328,  408,  552,  680 },
		{  72,  144,  224,  328,  424,  504,  680       },
		{  88,  176,  256,  392,  504,  600             },
		{ 104,  224,  328,  472,  584,  680             },
		{ 120,  256,  392,  536,  680                   },
		{ 136,  296,  456,  616                         },
		{ 144,  328,  504,  680                         },
		{ 176,  376,  584                               },
		{ 208,  440,  680                               },
};

const unsigned short tbsTableSizes[13] = // size of vectors in tbsTable
		{10, 10, 10, 10, 10, 8, 6, 6, 5, 4, 4, 3, 3};

DlNbIotAMCModule::DlNbIotAMCModule() {}

DlNbIotAMCModule::~DlNbIotAMCModule() {}

int
DlNbIotAMCModule::GetTBSizeFromMCS(int mcs, int nbSFs) {
	if (nbSFs==7 || nbSFs==9) return 0;

	if (mcs > 12) {
		mcs = 12;
	}

	// Represents table 16.4.1.3-1 from 3GPP TS 36.213
	int iSF;
	if (nbSFs < 7) iSF = nbSFs-1;
	else if (nbSFs == 8) iSF = 6;
	else if (nbSFs == 10) iSF = 7;

	return tbsTable[mcs][iSF];
}

int
DlNbIotAMCModule::GetMaxNumberOfSfForMCS(int mcs) {
	return tbsTableSizes[mcs]+1;
}
