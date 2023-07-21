#pragma once
#include <vector>
#include "CBox.h"

class MapManager
{
public:
	MapManager() = default;
	~MapManager() {};

	void LoadMap();
	void LoadSpawnArea();
	void LoadPoliceDir();
	Vector3 checkCollision(CBox& playerBox, Vector3& playerOldPos);
	bool checkCollisionRay(CBox&);
private:
	std::vector<std::shared_ptr<CBox>> MapCBox;
	std::vector<std::shared_ptr<CBox>> PoliceSpawnCBox;
	std::vector<std::shared_ptr<CBox>> PoliceDirCBox;
};

