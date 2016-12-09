/*
 * NbIotAMCModule.cpp
 *
 *  Created on: 28/11/2016
 *      Author: great
 */

#include "ul-nbiot-AMCModule.h"
//TODO Finish Ul Nb-Iot AMC Module
const unsigned short iRuToNRu[8] = {1, 2, 3, 4, 5, 6, 8, 10};

const unsigned short tbsTable[13][8] = {
		{  16,   32,   56,   88,   120,  152,  208,  256 },
		{  24,   56,   88,  144,   176,  208,  256,  344 },
		{  32,   72,  144,  176,   208,  256,  328,  424 },
		{  40,  104,  176,  208,   256,  328,  440,  568 },
		{  56,  120,  208,  256,   328,  408,  552,  680 },
		{  72,  144,  224,  328,   424,  504,  680,  872 },
		{  88,  176,  256,  392,   504,  600,  808, 1000 },
		{ 104,  224,  328,  472,   584,  712, 1000       },
		{ 120,  256,  392,  536,   680,  808             },
		{ 136,  296,  456,  616,   776,  936             },
		{ 144,  328,  504,  680,   872, 1000             },
		{ 176,  376,  584,  776,  1000                   },
		{ 208,  440,  680, 1000                          },
};

UlNbIotAMCModule::UlNbIotAMCModule() {}

UlNbIotAMCModule::~UlNbIotAMCModule() {}

int UlNbIotAMCModule::GetTBSizeFromMCS(int mcs) {
}

int UlNbIotAMCModule::GetTBSizeFromMCS(int mcs, int nbRBs) {
}
