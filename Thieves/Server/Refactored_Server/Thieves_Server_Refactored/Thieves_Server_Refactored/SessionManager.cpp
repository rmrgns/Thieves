#include "SessionManager.hpp"

void SessionManager::InitializeSessions()
{
	InitializeSListHead(&freeSessionList);

	for (int i = 0; i < MAX_USER; ++i)
	{
		sessionNodes[i].SessionId = i;

		sessions[i].SetStateCallback([this](int id) {
			this->ReturnSessionId(id);
			});

		InterlockedPushEntrySList(&freeSessionList, &(sessionNodes[i].ItemEntry));
	}

	std::cout << "Session Initialized. \n";
}

std::optional<int> SessionManager::GetEmptySessionId()
{

	PSLIST_ENTRY entry = InterlockedPopEntrySList(&freeSessionList);

	if (entry == nullptr) {
		return std::nullopt; // 꽉 찼음
	}

	SessionNode* node = reinterpret_cast<SessionNode*>(entry);

	std::cout << "Pop [" << node->SessionId << "]. \n";

	return node->SessionId;
}

void SessionManager::ReturnSessionId(int id)
{
	sessions[id].SetState(static_cast<int>(S_STATE::ST_FREE));

	InterlockedPushEntrySList(&freeSessionList, &(sessionNodes[id].ItemEntry));

	std::cout << "Push [" << id << "] \n";
}