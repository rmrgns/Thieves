#include "room.h"
#include "pch.h"
#include "object/object.h"
#include "object/moveobj_manager.h"
using namespace std;

Room::Room(int room_id)
{
	m_room_id = room_id;
	m_start_time = chrono::system_clock::now();
	m_room_state = ROOM_STATE::RT_FREE;
	m_obj_list.reserve(max_user + max_npc);
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
	m_obj_list.insert(c_id);
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->state_lock.lock();
	cl->SetState(STATE::ST_INROOM);
	cl->state_lock.unlock();
}

void Room::ResetRoom()
{
	max_user = 0;
	max_npc = 0;
	curr_round = 0;

	m_start_time = std::chrono::system_clock::now();
	m_obj_list.clear();
}

void Room::LeaveRoom(int c_id)
{
	if (m_obj_list.contains(c_id))
	{
		m_obj_list.erase(c_id);
		Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
		cl->state_lock.lock();
		cl->SetState(STATE::ST_LOGIN);
		cl->state_lock.unlock();
	}


	
	if (m_obj_list.empty())
	{
		m_state_lock.lock();
		m_room_state = ROOM_STATE::RT_FREE;
		m_state_lock.unlock();
	}
	
}

void Room::SetRoundStartTime()
{
	m_start_time = std::chrono::system_clock::now();
}

float Room::GetRoundTime() {
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(m_start_time
		- std::chrono::system_clock::now());
	round_time = (float)time.count();
	round_time /= 1000.0f;
	return round_time;
}

void Room::PlayerReady(int c_id)
{
	m_ready_player_list.insert(c_id);
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->state_lock.lock();
	cl->SetState(STATE::ST_INROOMREDDY);
	cl->state_lock.unlock();
}

void Room::PlayerCancleReady(int c_id)
{
	if (m_ready_player_list.contains(c_id)) {
		m_ready_player_list.erase(c_id);
		Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
		cl->state_lock.lock();
		cl->SetState(STATE::ST_INROOM);
		cl->state_lock.unlock();
	}
}

int Room::GetNumberOfPlayer()
{
	int num = 0;
	for (int pl : m_obj_list)
	{
		if (true == MoveObjManager::GetInst()->IsPlayer(pl)) num++;
	}

	return num;

}
