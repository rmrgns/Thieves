#pragma once
#include <vector>
#include <unordered_set>
#include <mutex>
#include <chrono>
#include <array>
#include <memory>
#include "Item.h"
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
	~Room() = default;

	void Init(int user_num);
	void EnterRoom(int c_id);
	void ResetRoom();
	void LeaveRoom(int c_id);
	void EnterEnemyRoom(int c_id);

	[[nodiscard]] int GetRoomID() const { return m_room_id; }
	[[nodiscard]] int GetMaxUser() const { return max_user; }
	[[nodiscard]] int GetMaxNPC() const { return max_npc; }
	[[nodiscard]] int GetRound() const { return curr_round; }

	[[nodiscard]] ROOM_STATE GetState() const { return m_room_state; }
	void SetState(ROOM_STATE val) { m_room_state = val; }

	void SetRoundStartTime();
	[[nodiscard]] float GetRoundTime() const;
	[[nodiscard]] std::chrono::system_clock::time_point GetRoundStartTime() const { return m_start_time; }
	void SetRound(int val) { curr_round = val; }

	[[nodiscard]] bool IsPlayerReady(int c_id) const { return m_ready_player_list.contains(c_id); }
	void PlayerReady(int c_id);
	void PlayerCancleReady(int c_id);

	[[nodiscard]] int GetRoomMasterId() const { return m_room_master_id; }

	void SetGameStart() { isGameStarted = true; }
	void SetGameEnd() { isGameStarted = false; }
	[[nodiscard]] bool IsGameStarted() const { return isGameStarted; }

	void SetGameEnd(bool value) { isGameEnd = value; }
	[[nodiscard]] bool IsGameEnd() const { return isGameEnd; }

	[[nodiscard]] int GetNumberOfPlayer() const;
	[[nodiscard]] int GetNumberOfReadyPlayer() const;

	[[nodiscard]] std::unordered_set<int>& GetObjList() { return m_obj_list; }
	void SetObjList(int obj) { m_obj_list.insert(obj); }

	// 스마트 포인터 배열로 변경
	[[nodiscard]] std::array<std::shared_ptr<Item>, MAX_ITEM>& GetItemList() { return m_item_list; }
	[[nodiscard]] Item* GetItem(int index)
	{
		if (index < 0 || index >= MAX_ITEM) return nullptr;
		return m_item_list[index].get();
	}

	void SetIsEscapeActive(bool val) { m_isEscapeActive = val; }
	[[nodiscard]] bool GetIsEscapeActive() const { return m_isEscapeActive; }

	void SetEscapePos(int index, float x, float y, float z) { m_escape_pos[index] = { x, 0.0f, z }; }
	void SetEscapePos(int index, const Vector3& vec) { m_escape_pos[index] = { vec.x, 0.0f, vec.z }; }
	[[nodiscard]] const Vector3& GetEscapePos(int index) const { return m_escape_pos[index]; }

	void SetIsSpecialEscapeActive(bool val) { m_isSpecialEscapeActive = val; }
	[[nodiscard]] bool GetIsSpecialEscapeActive() const { return m_isSpecialEscapeActive; }

	void SetSpecialEscapePos(float x, float y, float z) { m_special_escape_pos = { x, y, z }; }
	void SetSpecialEscapePos(const Vector3& vec) { m_special_escape_pos = vec; }
	[[nodiscard]] const Vector3& GetSpecialEscapePos() const { return m_special_escape_pos; }

	// Room 단위 동기화 용 lock
	// 재귀 락 std::recursive_mutex을 쓰면 PacketManager에서 락을 걸고 들어와도 데드락 안 발생함.
	std::recursive_mutex m_state_lock;

private:
	int m_room_id{ -1 };
	int max_user{ 0 };
	int max_npc{ 0 };
	int curr_round{ 0 };
	int m_room_master_id{ -1 };

	ROOM_STATE m_room_state{ ROOM_STATE::RT_FREE };
	std::chrono::system_clock::time_point m_start_time;

	bool isGameStarted{ false };
	bool isGameEnd{ false };

	std::unordered_set<int> m_obj_list;
	std::unordered_set<int> m_ready_player_list;
	std::array<std::shared_ptr<Item>, MAX_ITEM> m_item_list{};

	std::array<Vector3, 3> m_escape_pos{};
	Vector3 m_special_escape_pos{};
	Vector3 m_player_spawn{};

	// room 안에서 동기화 되는 거니까, 아토믹 설정 안해도 됨.
	bool m_isEscapeActive{ false };
	bool m_isSpecialEscapeActive{ false };
};