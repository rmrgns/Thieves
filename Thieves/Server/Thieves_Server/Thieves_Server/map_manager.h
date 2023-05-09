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
		// map ÃÊ±âÈ­
		for (int i = 0; i < MAP_WIDTH; ++i) {
			for (int j = 0; j < MAP_WIDTH; ++j) {
				m_tile_map[i][j].x = j * 1 + 1 - 900;
				m_tile_map[i][j].z = i * 1 + 1 - 900;
				m_tile_map[i][j].type = MAP_OBJ_TYPE::NONE;
			}
		}
		m_map_objects.reserve(1000); // ¸Ê °´Ã¼ º¤ÅÍÀÇ ¿ë·®À» ¹Ì¸® ¿¹¾à
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

	// ¸Ê °´Ã¼ º¤ÅÍ ¹ÝÈ¯
	const std::vector<MapObj>& GetMapObjVec() const {
		return m_map_objects;
	}
	// Æ¯Á¤ Á¾·ùÀÇ ¸Ê °´Ã¼ ¹ÝÈ¯
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
	// ÇÃ·¹ÀÌ¾î ½ºÆù À§Ä¡
	//Vector3 PLAYER_SPAWN_POINT[3]{
	//	{2350.0f,300.0f,3150.0f},
	//	{2450.0f,300.0f,3150.0f},
	//	{2400.0f,300.0f,3150.0f}
	//};

private:
	std::vector<MapObj> m_map_objects; // ¸Ê °´Ã¼ º¤ÅÍ
	MapTile m_tile_map[MAP_WIDTH][MAP_WIDTH]; // ¸Ê Å¸ÀÏ ¸Ê
};

