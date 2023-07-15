#pragma once
#include "object/object.h"
//#include "util/collision/collisioner.h"
class MapObj : public Object
{
public:
	MapObj(int id, const Vector3& pos, const Vector3& extent, bool is_blocking, OBJ_TYPE type)
	{
		m_id = id;
		m_pos = pos;
		m_type = type;

	};
	MapObj() = default;
	~MapObj() = default;

private:

};
