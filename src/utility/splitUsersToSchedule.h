

void
splitUsersToSchedule(UsersToSchedule* users,
					 UsersToSchedule* nbiotUsers,
					 UsersToSchedule* regularUsers)
{
	for(UsersToSchedule::iterator it = users->begin();
		it != users->end(); it++)
	{
		NetworkNode* node = it->m_userToSchedule;
		if (node->GetNodeType() == NetworkNode::TYPE_UE)
		{

		}
		else if (node->GetNodeType() == NetworkNode::TYPE_NBIOT_UE)
		{

		}
	}
}
