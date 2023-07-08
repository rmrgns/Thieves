#pragma once
#include <vector>
#include "CBox.h"

class MapManager
{
public:
	MapManager() = default;
	~MapManager() {};

	void LoadMap();
	
	
	Vector3 checkCollision(CBox& playerBox, Vector3& playerOldPos);
private:
	std::vector<std::shared_ptr<CBox>> MapCBox;
};

