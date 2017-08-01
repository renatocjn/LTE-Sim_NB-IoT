#ifndef SPLITUSERSTOSCHEDULE_H_
#define SPLITUSERSTOSCHEDULE_H_

#include "../protocolStack/mac/packet-scheduler/uplink-packet-scheduler.h"
#include "../device/NetworkNode.h"

//int splitUsersToSchedule(UplinkPacketScheduler::UsersToSchedule* users,
//		UplinkPacketScheduler::UsersToSchedule* nbiotUsers,
//		UplinkPacketScheduler::UsersToSchedule* regularUsers) {
//	for (UplinkPacketScheduler::UsersToSchedule::iterator it = users->begin(); it != users->end(); it++) {
//		NetworkNode* node = (*it)->m_userToSchedule;
//		if (node->GetNodeType() == NetworkNode::TYPE_UE) {
//			regularUsers->push_back(*it);
//		} else if (node->GetNodeType() == NetworkNode::TYPE_NBIOT_UE) {
//			nbiotUsers->push_back(*it);
//		}
//	}
//	return 0;
//}

#endif /* SPLITUSERSTOSCHEDULE_H_ */
