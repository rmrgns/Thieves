#pragma once
#include <vector>

#include "OBB.h"
#include "CBox.h"
#include "collision_detection.h"

#//include "c_box.h"


class MapManager
{
public:
	MapManager() = default;
	~MapManager() {};

	void LoadMap();
	
	
	bool checkCollision(CBox& playerBox);

private:
	std::vector<std::shared_ptr<CBox>> MapCBox;
};

