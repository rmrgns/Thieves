#pragma once
#include"pch.h"
class Object
{
public:
	Object() {
		m_type = OBJ_TYPE::OT_MAPOBJ;
		m_pos = Vector3(0.0f, 300.0f, 0.0f);
	};

	virtual ~Object() = default;


	void Init(int id, OBJ_TYPE type,
		float f_x, float f_y, float f_z,
		float r_x, float r_y, float r_z,
		float s_x, float s_y, float s_z,
		char direction
	)
	{
		m_id = id;
		m_type = type;
		m_pos.x = f_x;
		m_pos.y = f_y;
		m_pos.z = f_z;
	};

	char GetDirection()const { return direction; };

	float GetPosForwardX()const { return f_x; };
	float GetPosForwardY()const { return f_y; };
	float GetPosForwardZ()const { return f_z; };\

	float GetPosRightX()const { return r_x; };
	float GetPosRightY()const { return r_y; };
	float GetPosRightZ()const { return r_z; };

	float GetPosSX()const { return s_x; };
	float GetPosSY()const { return s_y; };
	float GetPosSZ()const { return s_z; };
	//Vector3& GetPos() { return m_pos; }


	int GetID()const { return m_id; };
	OBJ_TYPE GetType()const { return m_type; };


	void SetID(int val) { m_id = val; };
	void SetType(OBJ_TYPE val) { m_type = val; }

	void SetPosX(float val) { m_pos.x = val; };
	void SetPosY(float val) { m_pos.y = val; };
	void SetPosZ(float val) { m_pos.z = val; };
	//void SetPos(const Vector3& val) { m_pos = val; }
protected:
	int m_id;
	OBJ_TYPE m_type;
	Vector3 m_pos;
	float	s_x, s_y, s_z;
	float	f_x, f_y, f_z;
	float	r_x, r_y, r_z;
	char direction;


};