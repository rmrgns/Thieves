#pragma once
#include <vector>
#include "map_object.h"
#include "map_info.h"
#include "define.h"
#include "collisioner.h"

class MapManager
{
public:
	MapManager() {
		// map �ʱ�ȭ
		for (int i = 0; i < 180; ++i) {
			for (int j = 0; j < 180; ++j) {
				m_tile_map[i][j].x = j * 10 + 10 - 900;
				m_tile_map[i][j].z = i * 10 + 10 - 900;
				m_tile_map[i][j].type = MAP_OBJ_TYPE::NONE;
			}
		}
		m_map_objects.reserve(1000); // �� ��ü ������ �뷮�� �̸� ����
	};
	~MapManager() = default;

	// map Load
	void LoadMap(const std::string& path);

	// map Tile block
	void BlockTileMap();

	// Collision Check
	bool CheckCollision(BoxCollision& obj_collision);

	// Range
	bool CheckInRange(BoxCollision& collision);
	bool CheckInRange(const Vector3& pos, OBJ_TYPE map_type);

	// �� ��ü ���� ��ȯ
	const std::vector<MapObj>& GetMapObjVec() const {
		return m_map_objects;
	}
	// Ư�� ������ �� ��ü ��ȯ
	MapObj GetMapObjectByType(OBJ_TYPE type)
	{
		for (auto& obj : m_map_objects)
		{
			if (obj.GetType() == type)
			{
				return obj;
			}
		}
		return MapObj();
	}
	// �÷��̾� ���� ��ġ
	//Vector3 PLAYER_SPAWN_POINT[3]{
	//	{2350.0f,300.0f,3150.0f},
	//	{2450.0f,300.0f,3150.0f},
	//	{2400.0f,300.0f,3150.0f}
	//};

private:
	std::vector<MapObj> m_map_objects; // �� ��ü ����
	MapTile m_tile_map[1800][1800]; // �� Ÿ�� ��
};

