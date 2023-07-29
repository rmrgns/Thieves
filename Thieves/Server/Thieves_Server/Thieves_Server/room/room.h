#pragma once
#include<vector>
#include<unordered_set>
#include<mutex>
#include<atomic>
#include<chrono>
#include<array>
#include "define.h"
#include "state.h"
#include "protocol.h"
class MoveObjManager;
class Item;

class Room
{
public:
	Room(int room_id);
	Room() = default;
	~Room();



	void Init(int user_num);
	void EnterRoom(int c_id);
	void ResetRoom();

	void LeaveRoom(int c_id);

	int GetRoomID() { return m_room_id; }
	int GetMaxUser() { return max_user; }
	int GetMaxNPC() { return max_npc; }
	int GetRound() { return curr_round; }
	
	std::chrono::system_clock::time_point GetStartRoundTime() { return m_start_time; }
	ROOM_STATE GetState() { return m_room_state; }

	void SetState(ROOM_STATE val) { m_room_state = val; }
	void SetRoundStartTime();
	float GetRoundTime();
	std::chrono::system_clock::time_point GetRoundStartTime();
	void SetRound(int val) { curr_round = val; }

	bool IsPlayerReady(int c_id) { return m_ready_player_list.contains(c_id); }

	void PlayerReady(int c_id);
	void PlayerCancleReady(int c_id);

	int GetRoomMasterId() { return m_room_master_id; }

	int IsGameStarted() { return isGameStarted; }

	int GetNumberOfPlayer();
	int GetNumberOfReadyPlayer();

	std::unordered_set<int>& GetObjList()
	{
		return m_obj_list;
	}

	std::array<Item*, MAX_ITEM>& GetItemList()
	{
		return m_item_list;
	}

	Item* GetItem(int index)
	{
		if (index < 0 || index > MAX_ITEM) return nullptr;

		return m_item_list[index];
	}

	void SetGameStart() { isGameStarted = true; }
	void SetGameEnd() { isGameStarted = false; }

	void SetIsEscapeActive(bool val) { m_isEscapeActive = val; }
	bool GetIsEscapeActive() { return m_isEscapeActive; }

	void SetEscapePos(int index, float& x, float& y, float& z) {
		m_escape_pos[index].x = x;
		m_escape_pos[index].y = y;
		m_escape_pos[index].z = z;
	}

	void SetEscapePos(int index, Vector3& vec) {
		m_escape_pos[index].x = vec.x;
		m_escape_pos[index].y = vec.y;
		m_escape_pos[index].z = vec.z;
	}

	const Vector3& GetEscapePos(int index) {
		return m_escape_pos[index];
	}

	void SetIsSpecialEscapeActive(bool val) { m_isSpecialEscapeActive = val; }
	bool GetIsSpecialEscapeActive() { return m_isSpecialEscapeActive; }

	void SetSpecialEscapePos(float& x, float& y, float& z) {
		m_special_escape_pos.x = x;
		m_special_escape_pos.y = y;
		m_special_escape_pos.z = z;
	}

	void SetSpecialEscapePos(Vector3& vec) {
		m_special_escape_pos.x = vec.x;
		m_special_escape_pos.y = vec.y;
		m_special_escape_pos.z = vec.z;
	}

	const Vector3& GetSpecialEscapePos() {
		return m_special_escape_pos;
	}

	std::mutex m_state_lock;


private:

	bool isGameStarted = false;

	int m_room_id;
	int max_user;
	ROOM_STATE m_room_state;
	int max_npc;
	int curr_round;
	int m_room_master_id;
	std::unordered_set<int> m_obj_list;
	std::array<Item*, MAX_ITEM> m_item_list;
	std::unordered_set<int> m_ready_player_list;
	std::chrono::system_clock::time_point m_start_time;
	float round_time;

	std::array<Vector3, 3> m_escape_pos;
	std::atomic_bool m_isEscapeActive;

	Vector3 m_special_escape_pos;
	std::atomic_bool m_isSpecialEscapeActive;
};
