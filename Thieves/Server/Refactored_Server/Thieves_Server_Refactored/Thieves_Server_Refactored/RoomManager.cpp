#include "RoomManager.h"
#include <mutex>


void RoomManager::InitRoom()
{
	for (int i = 0; i < MAX_ROOM_SIZE; ++i)
	{
		m_rooms[i] = std::make_shared<Room>(i);

		DEBUG_LOG(m_rooms[i]->GetRoomID());
	}
}

void RoomManager::DestroyRoom()
{
	for (auto& r : m_rooms)
	{
		if (r) r.reset();
	}
}

std::optional<int> RoomManager::GetEmptyRoom()
{
	for (auto& r : m_rooms)
	{
		std::lock_guard<std::recursive_mutex> lock(r->m_state_lock);

		if (ROOM_STATE::RT_FREE == r->GetState())
		{
			r->SetState(ROOM_STATE::RT_USED);
			return r->GetRoomID();
		}
	}

	std::cout << "모든 방이 꽉 찼습니다!" << std::endl;
	return std::nullopt;
}