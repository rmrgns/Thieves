#pragma once
#include "object/object.h"

//플레이어와 npc의 부모
class MoveObj :public Object
{
public:
	MoveObj() {
		//m_rotation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	};

	virtual ~MoveObj() = default;

	
	Vector3 GetOriginX()const { return m_origin_pos; }

	virtual void Reset()
	{
		m_is_active = false;
		ZeroMemory(m_name, MAX_NAME_SIZE + 1);
	}

	//Vector4 GetRotation() { return m_rotation; }

	int GetRoomID() const { return m_room_id; }
	char* GetName() { return m_name; }

	bool GetIsActive()const { return m_is_active.load(std::memory_order_acquire); }
	void SetIsActive(bool val) { m_is_active.store(val, std::memory_order_release); }

	//void SetRotaion(const Vector4& val) { m_rotation = val; }
	void SetRoomID(int val) { m_room_id = val; }

	void SetOriginPos(const Vector3& val) { m_origin_pos = val; }

	void SetAnimationNumber(const int& val) { animNum = val; };
	int GetAnimationNumber() { return animNum; };

	int		m_last_move_time = 0;
protected:
	std::atomic_bool m_is_active = false;
	int		m_room_id;

	char m_name[MAX_NAME_SIZE + 1];
	//Vector4 m_rotation;
	Vector3 m_origin_pos;
	int animNum = -1;
};