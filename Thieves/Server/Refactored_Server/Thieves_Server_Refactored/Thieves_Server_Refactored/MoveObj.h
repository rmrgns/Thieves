#pragma once
#include "vec3.h"
#include "Object.h"
#include "protocol.hpp"
#include <Windows.h>
#include <atomic>


// 플레이어와 npc의 부모
class MoveObj : public Object
{
public:
	MoveObj() = default;
	virtual ~MoveObj() = default;

	[[nodiscard]] Vector3 GetOriginX() const { return m_origin_pos; }

	virtual void Reset()
	{
		m_is_active.store(false, std::memory_order_release);
		ZeroMemory(m_name, MAX_NAME_SIZE + 1);
	}

	[[nodiscard]] int GetRoomID() const { return m_room_id; }
	[[nodiscard]] char* GetName() { return m_name; }

	[[nodiscard]] bool GetIsActive() const { return m_is_active.load(std::memory_order_acquire); }
	void SetIsActive(bool val) { m_is_active.store(val, std::memory_order_release); }

	void SetRoomID(int val) { m_room_id = val; }

	void SetOriginPos(const Vector3& val) { m_origin_pos = val; }

	void SetAnimationNumber(const int& val) { animNum = val; }
	[[nodiscard]] int GetAnimationNumber() const { return animNum; }

	void SetJump(const bool& val) { isJump = val; }
	[[nodiscard]] bool GetJump() const { return isJump; }

	void SetUpVelocity(const float& val) { upVelocity = val; }
	[[nodiscard]] float GetUpVelocity() const { return upVelocity; }

	int m_last_move_time{ 0 };

protected:
	std::atomic<bool> m_is_active{ false };
	int m_room_id{ 0 };

	char m_name[MAX_NAME_SIZE + 1]{};
	Vector3 m_origin_pos{};
	int animNum{ -1 };
	bool isJump{ false }; // 현재 점프 중인지 아닌지를 나타내는 변수
	float upVelocity{ 0.0f }; // 위쪽 방향 속도를 나타내는 변수
};

