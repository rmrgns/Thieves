#pragma once
#include <vector>
#include "CBox.h"
#include "define.h"

class MapManager
{
public:
	MapManager() = default;
	~MapManager() {};

	void LoadMap();
	void LoadMapFromBinary();
	void LoadSpawnArea();
	void LoadPoliceDir();
	void LoadItemSpawnPoint();
	void LoadEscapePoint();
	void LoadSpecialEscapePoint();
	void LoadPlayerSpawnArea();
	std::vector<Vector3>& GetItemPos() { return ItemPos; }
	std::vector<Vector3>& GetEscapePos() { return ItemPos; }
	std::vector<Vector3>& GetSpecialEscapePos() { return ItemPos; }
	std::vector<Vector3>& GetPlayerSpawnPos() { return ItemPos; }
	Vector3 checkCollision(CBox& playerBox, Vector3& playerOldPos);
	bool checkCollisionRay(CBox&);
private:
	std::vector<std::shared_ptr<CBox>> MapCBox;
	std::vector<std::shared_ptr<CBox>> PoliceSpawnCBox;
	std::vector<std::shared_ptr<CBox>> PoliceDirCBox;
	std::vector<Vector3> ItemPos;
	std::vector<Vector3> EscapePos;
	std::vector<Vector3> SpecialEscapePos;
	std::vector<Vector3> PlayerSpawnPos;
};

