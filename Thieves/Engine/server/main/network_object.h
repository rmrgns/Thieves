#pragma once
#include"server/define.h"
//#include"client/math/vec3.h"
#include <string>
#include <chrono>

class NetworkObj {
public:
	NetworkObj() = default;
	NetworkObj(int id, char* name, float x, float y, float z)
		:m_id(id)
	{
		m_move_time = std::chrono::system_clock::now();
		m_name = std::string(name);
		m_position = Vec3(x, y, z);
		m_rotation = Vec3(0.0f, 0.0f, 0.0f);
		//m_rotation = client_fw::Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	}
	~NetworkObj() = default;


	const Vec3& GetPosition()const { return m_position; }
	const Vec3& GetRotation()const { return m_rotation; }
	const std::string& GetName()const { return m_name; }

	const int GetID()const { return m_id; }
	//const client_fw::Quaternion& GetRotation() const { return m_rotation; }
	//void SetRotation(const client_fw::Quaternion& rot) { m_rotation = rot; }

	void SetID(int val) { m_id = val; }
	void SetPosition(const Vec3& val) { m_position = val; }
	void SetPositionX(const float& val) { m_position.x = val; }
	void SetPositionY(const float& val) { m_position.y = val; }
	void SetPositionZ(const float& val) { m_position.z = val; }
	void SetRotation(const Vec3& val) { m_rotation = val; }
	void SetRotationX(const float& val) { m_rotation.x = val; }
	void SetRotationY(const float& val) { m_rotation.y = val; }
	void SetRotationZ(const float& val) { m_rotation.z = val; }

	void SetName(const std::string& val) { m_name = val; }

	void SetActionType(const int);

	std::chrono::system_clock::time_point m_move_time;
protected:
	std::string m_name;
	Vec3 m_position;
	Vec3 m_rotation;
	int m_actionType;
	int m_id;
	//client_fw::Quaternion m_rotation;
};