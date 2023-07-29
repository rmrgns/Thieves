#pragma once
#include "server/main/network_object.h"

enum class ITEM_STATE {
	IT_NONE,
	IT_SPAWN,
	IT_OCCUPIED,
	IT_SET
};

class Item : public NetworkObj
{
public:
	Item() = default;

	Item(int id, char* name, float x, float y, float z, int type) : m_item_type(type) {
		m_id = id;
		m_name = std::string(name);
		m_position = Vec3(x, y, z);
		m_rotation = Vec3(0.f, 0.f, 0.f);
		
	}

	~Item() = default;

	void SetItemType(int val) { m_item_type = val; }
	int GetItemType() { return m_item_type; }

protected:
	int m_item_type = -1;
	ITEM_STATE state = ITEM_STATE::IT_NONE;
};

