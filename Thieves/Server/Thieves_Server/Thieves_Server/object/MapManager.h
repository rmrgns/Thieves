#pragma once
#include <vector>

#include "OBB.h"
#include "map_object.h"
#include "c_box.h"


class MapManager
{
public:
	MapManager() = default;
	~MapManager() {};

	void LoadMap();
	
	bool isCollision(OBB& playerObb);

private:
	std::vector<std::shared_ptr<OBB>> MapObjs;
	std::vector<std::shared_ptr<ColliderBox>>c_box;
};

