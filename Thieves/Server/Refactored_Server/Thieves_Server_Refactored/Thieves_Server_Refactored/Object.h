#pragma once
#include "vec3.h"
#include "State.hpp"

class Object
{
public:
	Object() = default; // 멤버 변수 선언부에서 초기화하므로 default
	virtual ~Object() = default;

	void Init(int id, OBJ_TYPE type, float x, float y, float z)
	{
		m_id = id;
		m_type = type;
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;

		m_rot.x = 0.0f;
		m_rot.y = 0.0f;
		m_rot.z = 0.0f;
	};

	// 반환값만 사용하는 경우니까, 반환값을 사용 안하면 굳이 쓸 일이 없다.
	// 그러니까 nodiscard 속성을 부여해서, 반환값 안쓰면 에러 뜨도록 해두자.

	[[nodiscard]] float GetPosX() const { return m_pos.x; }
	[[nodiscard]] float GetPosY() const { return m_pos.y; }
	[[nodiscard]] float GetPosZ() const { return m_pos.z; }
	[[nodiscard]] Vector3& GetPos() { return m_pos; }

	[[nodiscard]] float GetRotX() const { return m_rot.x; }
	[[nodiscard]] float GetRotY() const { return m_rot.y; }
	[[nodiscard]] float GetRotZ() const { return m_rot.z; }
	[[nodiscard]] Vector3& GetRot() { return m_rot; }

	[[nodiscard]] int GetID() const { return m_id; }
	[[nodiscard]] OBJ_TYPE GetType() const { return m_type; }

	void SetID(int val) { m_id = val; }
	void SetType(OBJ_TYPE val) { m_type = val; }

	void SetPosX(float val) { m_pos.x = val; }
	void SetPosY(float val) { m_pos.y = val; }
	void SetPosZ(float val) { m_pos.z = val; }
	void SetPos(const Vector3& val) { m_pos = val; }

	void SetRotX(float val) { m_rot.x = val; }
	void SetRotY(float val) { m_rot.y = val; }
	void SetRotZ(float val) { m_rot.z = val; }
	void SetRot(const Vector3& val) { m_rot = val; }

protected:
	int m_id{ 0 };
	OBJ_TYPE m_type{ OBJ_TYPE::OT_MAPOBJ };
	Vector3 m_pos{ 0.0f, 0.0f, 0.0f };
	// 회전 방향 데이터
	Vector3 m_rot{ 0.0f, 0.0f, 0.0f };
};