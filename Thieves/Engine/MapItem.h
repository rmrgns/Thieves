#pragma once
#include "server/main/network_object.h"

enum class ITEM_STATE {
	IT_NONE,
	IT_SPAWN,
	IT_OCCUPIED,
	IT_SET
};

class MapItem : public NetworkObj
{
public:
	MapItem();

	MapItem(int id, const char* name, float x, float y, float z, int type, ITEM_STATE st) {
		m_id = id;
		m_item_type = type;
		m_name = std::string(name);
		m_position = Vec3(x, y, z);
		m_rotation = Vec3(0.f, 0.f, 0.f);
		m_item_type = type;
		state = st;
	}

	MapItem(const MapItem& other)
	{
		m_id = other.GetID();
		m_name = other.GetName();
		m_position = other.GetPosition();
		m_rotation = other.GetRotation();
		m_item_type = other.m_item_type;
		state = other.state;
	}

	~MapItem();

	void SetItemType(int val) { m_item_type = val; }
	int GetItemType() { return m_item_type; }

	void SetState(ITEM_STATE val) { state = val; }
	ITEM_STATE GetState() { return state; }

protected:
	int m_item_type = -1;
	ITEM_STATE state = ITEM_STATE::IT_NONE;
};

