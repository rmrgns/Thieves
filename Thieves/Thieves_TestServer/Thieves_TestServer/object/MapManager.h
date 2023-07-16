#pragma once
#include <vector>
#include "CBox.h"
#include "area/escape_area.h"
#include "area/Item_box.h"
#include "area/spawn_area.h"
#include "area/special_escape_area.h"
class MapManager
{
public:
	MapManager() = default;
	~MapManager() {};

	void LoadMap();
	void LoadItem();
	void LoadEscapeArea();
	void LoadSpecialEscapeArea();
	void LoadSpawnArea();
	
	Vector3 checkCollision(CBox& playerBox, Vector3& playerOldPos);
	Vector3 checkCollisionRay(CBox& playerBox);
private:
	std::vector<std::shared_ptr<CBox>> MapCBox;
	std::vector<std::shared_ptr<Escape_area>> EscpaeArea;
	std::vector<std::shared_ptr<Item_area>> ItemArea;
	std::vector<std::shared_ptr<Spawn_area>> SpawnArea;
	std::vector<std::shared_ptr<SSpawn_area>>SSpawnArea ;

};

