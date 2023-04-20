#include "room.h"
#include "pch.h"
#include "object/object.h"
using namespace std;

Room::Room(int room_id)
{
	m_round_time = chrono::system_clock::now();
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

	m_round_time = std::chrono::system_clock::now();
	m_obj_list.clear();
}

void Room::SetRoundTime(int seconds)
{
	m_round_time = std::chrono::system_clock::now() + (std::chrono::milliseconds(1) * seconds);
}