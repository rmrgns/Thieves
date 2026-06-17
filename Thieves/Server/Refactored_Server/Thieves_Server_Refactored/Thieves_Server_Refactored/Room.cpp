#include "Room.h"
#include "MoveObjManager.h"
//#include "Item.h"

using namespace std;

Room::Room(int room_id)
{
	m_room_id = room_id;
	m_room_master_id = -1;
	m_start_time = std::chrono::system_clock::now();
	m_room_state = ROOM_STATE::RT_FREE;

	for (int i = 0; i < m_item_list.size(); ++i) {
		m_item_list[i] = std::make_shared<Item>();
	}
}

void Room::Init(int user_num)
{
	max_user = user_num;
	max_npc = max_user + 8;
	curr_round = 0;
}

void Room::EnterRoom(int c_id)
{
	std::lock_guard<std::recursive_mutex> lock(m_state_lock);

	m_obj_list.insert(c_id);

	if (auto cl = MoveObjManager::GetInst().GetPlayer(c_id))
	{
		cl->SetState(P_STATE::ST_INROOM);
		// 방장이 아무도 없으면 내가 방장!
		if (m_room_master_id == -1) {
			m_room_master_id = c_id;
		}
	}
}

void Room::EnterEnemyRoom(int c_id)
{
	std::lock_guard<std::recursive_mutex> lock(m_state_lock);
	m_obj_list.insert(c_id);
}

void Room::ResetRoom()
{
	std::lock_guard<std::recursive_mutex> lock(m_state_lock);

	max_user = 0;
	max_npc = 0;
	curr_round = 0;
	m_room_master_id = -1;
	m_start_time = std::chrono::system_clock::now();
	m_obj_list.clear();
	m_ready_player_list.clear();

	m_isEscapeActive = false;
	m_isSpecialEscapeActive = false;

	for (auto& item : m_item_list) {
		if (item) {
			item->SetItemCode(-1);
			item->SetState(ITEM_STATE::ST_NOTUSED);
		}
	}
}

void Room::LeaveRoom(int c_id)
{
	std::lock_guard<std::recursive_mutex> lock(m_state_lock);

	if (m_obj_list.contains(c_id))
	{
		if (MoveObjManager::GetInst().IsPlayer(c_id))
		{
			auto cl = MoveObjManager::GetInst().GetPlayer(c_id);
			if (cl && (cl->GetState() == P_STATE::ST_INROOMREDDY || cl->GetState() == P_STATE::ST_INGAME)) {
				PlayerCancleReady(c_id);
			}
		}
		m_obj_list.erase(c_id);
	}

	bool isPlayerEmpty = (GetNumberOfPlayer() == 0);

	if (isPlayerEmpty)
	{
		m_room_state = ROOM_STATE::RT_FREE;
		m_room_master_id = -1;
		isGameStarted = false;
		isGameEnd = false;
		m_isEscapeActive = false;
		m_isSpecialEscapeActive = false;

		for (auto& item : m_item_list) {
			item->SetItemCode(-1);
			item->SetState(ITEM_STATE::ST_NOTUSED);
		}
	}
	else
	{
		if (m_room_master_id == c_id)
		{
			m_room_master_id = -1; // 임시 초기화
			for (int obj : m_obj_list)
			{
				if (MoveObjManager::GetInst().IsPlayer(obj))
				{
					m_room_master_id = obj;
					break; // 플레이어를 찾았으니 방장 위임 완료!
				}
			}
		}
	}
}

void Room::SetRoundStartTime()
{
	m_start_time = std::chrono::system_clock::now();
}

float Room::GetRoundTime() const
{
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now() - m_start_time);
	return static_cast<float>(time.count()) / 1000.0f;
}

void Room::PlayerReady(int c_id)
{
	m_ready_player_list.insert(c_id);
}

void Room::PlayerCancleReady(int c_id)
{
	m_ready_player_list.erase(c_id);
}

int Room::GetNumberOfPlayer() const
{
	int num = 0;
	for (int pl : m_obj_list)
	{
		if (MoveObjManager::GetInst().IsPlayer(pl)) num++;
	}
	return num;
}

int Room::GetNumberOfReadyPlayer() const
{
	int num = 0;
	for (int pl : m_ready_player_list)
	{
		if (MoveObjManager::GetInst().IsPlayer(pl)) num++;
	}
	return num;
}