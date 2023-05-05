#pragma once
#include "vec/vec3.h"
#include <string>

enum class MAP_OBJ_TYPE
{
	NONE,
	BLOCK,
	UNBLOCK,
	BASE,
	SPAWN_AREA
};

class MapInfo
{
public:
	MapInfo(std::string name, Vector3 pos)
		:name(name), position(pos)
	{

	}
	MapInfo() = default;
	~MapInfo() = default;

	std::string name;
	Vector3 position;

};

struct MapTile {
	float x, z;
	MAP_OBJ_TYPE type;
};