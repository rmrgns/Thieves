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
	void LoadSpawnArea();
	void LoadPoliceDir();
	void LoadItemSpawnPoint();
	void LoadEscapePoint();
	void LoadSpecialEscapePoint();
	void LoadPlayerSpawnArea();


	std::vector<Vector3>& GetItemPos() { return ItemPos; }
	std::vector<Vector3>& GetEscapePos() { return EscapePos; }
	std::vector<Vector3>& GetSpecialEscapePos() { return SpecialEscapePos; }
	std::vector<Vector3>& GetPlayerSpawnPos() { return PlayerSpawnPos; }
	std::vector<Vector3>& GetNPCSpawnPos() { return PoliceSpawnCBox; }
	Vector3 checkCollision(CBox& playerBox, Vector3& playerOldPos);
	bool checkCollisionRay(CBox&);
	bool checkCollisionSafe(CBox& ray_temp);
	

	bool CheckInRange2(float x, float z, const Vector3& min_pos, const Vector3& max_pos)
	{
		return (min_pos.x <= x && x <= max_pos.x && min_pos.z <= z && z <= max_pos.z);
	}
	//std::vector<Vector3> PoliceSpawnCBox;
private:
	std::vector<std::shared_ptr<CBox>> MapCBox;
	std::vector<Vector3> PoliceSpawnCBox;
	std::vector<std::shared_ptr<CBox>> PoliceDirCBox;
	std::vector<Vector3> ItemPos;
	std::vector<Vector3> EscapePos;
	std::vector<Vector3> SpecialEscapePos;
	std::vector<Vector3> PlayerSpawnPos;

	std::vector<Vector3> PlayerPos;

	CBox m_box_manager;


};

