#include "room.h"
#include "pch.h"
#include "object/object.h"
#include "object/moveobj_manager.h"
#include "Item.h"
using namespace std;

Room::Room(int room_id)
{
	m_room_id = room_id;
	m_room_master_id = -1;
	m_start_time = chrono::system_clock::now();
	m_room_state = ROOM_STATE::RT_FREE;
	m_obj_list.reserve(max_user + max_npc);
	for (int i = 0; i < m_item_list.size(); ++i) {
		m_item_list[i] = new Item();
	}
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
	if (m_room_master_id == -1) {
		m_room_master_id = c_id;
	}
	cl->state_lock.unlock();
}

void Room::ResetRoom()
{
	max_user = 0;
	max_npc = 0;
	curr_round = 0;
	m_room_master_id = -1;
	m_start_time = std::chrono::system_clock::now();
	m_obj_list.clear();
	for (int i = 0; i < m_item_list.size(); ++i) {
		m_item_list[i]->SetItemCode(-1);
		m_item_list[i]->SetState(ITEM_STATE::ST_NOTUSED);
	}
}

void Room::LeaveRoom(int c_id)
{

	
	if (m_obj_list.contains(c_id))
	{
		Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
		if (cl->GetState() == STATE::ST_INROOMREDDY || cl->GetState() == STATE::ST_INGAME) {
			PlayerCancleReady(c_id);
		}
		m_obj_list.erase(c_id);	
	}


	
	if (m_obj_list.empty())
	{
		m_state_lock.lock();
		m_room_state = ROOM_STATE::RT_FREE;
		m_room_master_id = -1;
		isGameStarted = false;
		m_state_lock.unlock();

		for (int i = 0; i < m_item_list.size(); ++i) {
			m_item_list[i]->SetItemCode(-1);
			m_item_list[i]->SetState(ITEM_STATE::ST_NOTUSED);
		}
	}
	else {
		if (m_room_master_id == c_id) {
			m_room_master_id = *(m_obj_list.begin());
		}
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
}

void Room::PlayerCancleReady(int c_id)
{
	if (m_ready_player_list.contains(c_id)) {
		m_ready_player_list.erase(c_id);
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

int Room::GetNumberOfReadyPlayer()
{
	int num = 0;
	for (int pl : m_ready_player_list)
	{
		if (true == MoveObjManager::GetInst()->IsPlayer(pl)) num++;
	}

	return num;
}
