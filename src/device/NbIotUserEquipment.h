/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef NBIOTUSEREQUIPMENT_H_
#define NBIOTUSEREQUIPMENT_H_

#include "NetworkNode.h"
#include "UserEquipment.h"

class ENodeB;
class Gateway;
class CqiManager;


class NbIotUserEquipment : public UserEquipment {
public:
	NbIotUserEquipment ();
	NbIotUserEquipment (int idElement,
				double posx, double posy,
				Cell *cell,
				NetworkNode* target,
				bool handover);
};

#endif /* NBIOTUSEREQUIPMENT_H_ */
