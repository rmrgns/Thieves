#pragma once
#include"pch.h"
class Object
{
public:
	Object() {
		m_type = OBJ_TYPE::OT_MAPOBJ;
		m_pos = Vector3(0.0f, 0.0f, 0.0f);
	};

	virtual ~Object() = default;


	void Init(int id, OBJ_TYPE type, float x, float y, float z)
	{
		m_id = id;
		m_type = type;
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;

//		m_type = type;
//		m_pos.x = f_x;
//		m_pos.y = f_y;
//		m_pos.z = f_z;
	};
	float GetPosX()const { return posX; };
	float GetPosY()const { return posY; };
	float GetPosZ()const { return posZ; };
	Vector3& GetPos() { return m_pos; };

	int GetID()const { return m_id; };
	OBJ_TYPE GetType()const { return m_type; };

	void SetID(int val) { m_id = val; };
	void SetType(OBJ_TYPE val) { m_type = val; };

	void SetPosX(float val) { posX = val; };
	void SetPosY(float val) { posY = val; };
	void SetPosZ(float val) { posZ = val; };
	void SetPos(const Vector3& val) { m_pos = val; };



	//char GetDirection()const { return direction; };

	//float GetPosForwardX()const { return f_x; };
	//float GetPosForwardY()const { return f_y; };
	//float GetPosForwardZ()const { return f_z; };\

	//float GetPosRightX()const { return r_x; };
	//float GetPosRightY()const { return r_y; };
	//float GetPosRightZ()const { return r_z; };

	//float GetPosSX()const { return s_x; };
	//float GetPosSY()const { return s_y; };
	//float GetPosSZ()const { return s_z; };
	


	//Vector3& GetPos() { return m_pos; }




	
//	OBJ_TYPE GetType()const { return m_type; };


//	void SetType(OBJ_TYPE val) { m_type = val; }

//	void SetPosX(float val) { m_pos.x = val; };
//	void SetPosY(float val) { m_pos.y = val; };
//	void SetPosZ(float val) { m_pos.z = val; };
	
protected:
	int m_id;
	OBJ_TYPE m_type;
	Vector3 m_pos;
	

//	OBJ_TYPE m_type;
	float	posX, posY, posZ;

	//char direction;


};