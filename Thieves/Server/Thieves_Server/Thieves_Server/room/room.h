#pragma once
#include<vector>
#include<unordered_set>
#include<mutex>
#include<atomic>
#include<chrono>
#include<array>
#include "state.h"
class Object;

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
	void SetRound(int val) { curr_round = val; }

	bool IsPlayerReady(int c_id) { return m_ready_player_list.contains(c_id); }

	void PlayerReady(int c_id) { m_ready_player_list.insert(c_id); }
	void PlayerCancleReady(int c_id) { m_ready_player_list.erase(c_id); }

	int GetNumberOfPlayer() {
		int num = 0;
		for (int pl : m_obj_list)
		{
			if (true == MoveObjManager::GetInst()->IsPlayer(pl)) num++;
		}

		return num;
	}

	std::unordered_set<int>& GetObjList()
	{
		return m_obj_list;
	}

	std::mutex m_state_lock;
private:
	int m_room_id;
	int max_user;
	ROOM_STATE m_room_state;
	int max_npc;
	int curr_round;
	std::unordered_set<int>m_obj_list;
	std::unordered_set<int>m_ready_player_list;
	std::chrono::system_clock::time_point m_start_time;
	float round_time;
};
