#pragma once
#include "object/object.h"
#include "collisioner.h"
class MapObj : public Object
{
public:
	MapObj(int id, const Vector3& pos, const Vector3& extent, bool is_blocking, OBJ_TYPE type)
		:m_extent(extent), m_is_blocking(is_blocking)
	{
		m_id = id;
		m_pos = pos;
		m_type = type;
		m_ground_pos = Vector3(pos.x, pos.y - extent.y, pos.z);
		SetMinPos();
		SetMaxPos();
		//std::cout << m_min_pos << std::endl;
		//std::cout << m_max_pos << std::endl;

	};
	MapObj() = default;
	~MapObj() = default;
	bool GetIsBlocked() { return m_is_blocking; }
	const Vector3& GetMaxPos()const { return m_max_pos; }
	const Vector3& GetMinPos()const { return m_min_pos; }
	const Vector3& GetGroundPos()const { return m_ground_pos; }
	const Vector3& GetExtent()const { return m_extent; }
private:

	void SetMinPos();
	void SetMaxPos();
	bool m_is_blocking = false;
	Vector3 m_min_pos;
	Vector3 m_max_pos;
	Vector3 m_extent;
	Vector3 m_ground_pos;


};
