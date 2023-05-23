#pragma once
#include<array>
#include <iostream>
#include"define.h"
class Room;
class MapManager* m_map_manager;

class RoomManager
{
public:
	RoomManager();
	~RoomManager()
	{
		DestroyRoom();
	}

	void InitRoom();
	void DestroyRoom();
	int GetEmptyRoom();
	Room* GetRoom(int r_id) { return m_rooms[r_id]; }

	std::array<Room*, MAX_ROOM_SIZE>& GetRooms() {
		return m_rooms;
	}

private:
	std::array<Room*, MAX_ROOM_SIZE> m_rooms;

};
