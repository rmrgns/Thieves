#include "room.h"
#include <chrono>

Room::Room(int room_id)
{
}

Room::~Room()
 
	curr_round = 0;
}

void Room::EnterRoom(int c_id)
{
	m_obj_list.push_back(c_id);
}

void Room::ResetRoom()
{
	max_user = 0;
	max_npc = 0;
	curr_round = 0;

	m_round_time = std::chrono::system_clock::now();
	m_obj_list.clear();
}

void Room::SetRoundTime(int seconds)
{
	m_round_time = std::chrono::system_clock::now() + (std::chrono::milliseconds(1) * seconds);
}