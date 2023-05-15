#pragma once
#include<vector>
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

	int GetRoomID() { return room_id; }
	int GetMaxUser() { return max_user; }
	int GetMaxNPC() { return max_npc; }
	int GetRound() { return curr_round; }
	
	std::chrono::system_clock::time_point GetStartRoundTime() { return m_start_time; }
	ROOM_STATE GetState() { return m_room_state; }

	void SetState(ROOM_STATE val) { m_room_state = val; }
	void SetRoundStartTime();
	void SetRoundTime();
	float GetRoundTime() { return round_time; }
	void SetRound(int val) { curr_round = val; }

	std::vector<int>& GetObjList()
	{
		return m_obj_list;
	}

	std::mutex m_state_lock;
private:
	int room_id;
	int max_user;
	ROOM_STATE m_room_state;
	int max_npc;
	int curr_round;
	std::vector<int>m_obj_list;
	std::chrono::system_clock::time_point m_start_time;
	float round_time;
};
