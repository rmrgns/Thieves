#pragma once
#include <array>
#include <iostream>
#include <memory>
#include <optional>
#include "define.h"
#include "Room.h"

class RoomManager
{
public:
	static RoomManager& GetInst()
	{
		static RoomManager instance;
		return instance;
	}

	RoomManager() = default;
	~RoomManager() = default;

	RoomManager(const RoomManager&) = delete;
	RoomManager& operator=(const RoomManager&) = delete;

	void InitRoom();
	void DestroyRoom();

	// 방이 꽉 찼을 경우 -1 대신 std::nullopt를 반환해서 안전성을 높임.
	[[nodiscard]] std::optional<int> GetEmptyRoom();

	[[nodiscard]] Room* GetRoom(int r_id) { return m_rooms[r_id].get(); }

	[[nodiscard]] std::array<std::shared_ptr<Room>, MAX_ROOM_SIZE>& GetRooms() { return m_rooms; }

private:
	// 스마트 포인터 사용으로, 안전하게 관리.
	std::array<std::shared_ptr<Room>, MAX_ROOM_SIZE> m_rooms{};
};