#pragma once
#include "object/object.h"

//플레이어와 npc의 부모
class MoveObj :public Object
{
public:
	MoveObj() {
		m_rotation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	};

	virtual ~MoveObj() = default;

	Vector3 GetOriginX()const { return m_origin_pos; }

	virtual void Reset()
	{
	
	}

	Vector4 GetRotation() { return m_rotation; }

	void SetRotaion(const Vector4& val) { m_rotation = val; }


	void SetOriginPos(const Vector3& val) { m_origin_pos = val; }
	std::mutex m_hp_lock;
	int		m_last_move_time = 0;
protected:
	Vector4 m_rotation;
	Vector3 m_origin_pos;
};