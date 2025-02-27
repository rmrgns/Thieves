#include "room.h"
#include "pch.h"
#include "object/object.h"
using namespace std;

Room::Room(int room_id)
{
	m_start_time = chrono::system_clock::now();
	m_room_state = ROOM_STATE::RT_FREE;
	m_obj_list.reserve(100);
}

Room::~Room()
{
}

void Room::Init(int user_num)
{
	max_user = user_num;
	max_npc = max_user-user_num;
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

	m_start_time = std::chrono::system_clock::now();
	m_obj_list.clear();
}

void Room::SetRoundStartTime()
{
	m_start_time = std::chrono::system_clock::now();
}

float Room::GetRoundTime() {
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(m_start_time - std::chrono::system_clock::now());
	round_time = (float)time.count();
	round_time /= 1000.0f;
	return round_time;
}