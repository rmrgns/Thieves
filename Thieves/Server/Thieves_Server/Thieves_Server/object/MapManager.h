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
	void LoadPoliceDir1();
	void LoadPoliceDir2();
	void LoadPoliceDir3();
	void LoadPoliceDir4();
	void LoadPoliceDir5();
	void LoadPoliceDir6();
	void LoadPoliceDir7();
	void LoadPoliceDir8();

	std::vector<Vector3>& GetItemPos() { return ItemPos; }
	std::vector<Vector3>& GetEscapePos() { return EscapePos; }
	std::vector<Vector3>& GetSpecialEscapePos() { return SpecialEscapePos; }
	std::vector<Vector3>& GetPlayerSpawnPos() { return PlayerSpawnPos; }
	std::vector<Vector3>& GetNPCSpawnPos() { return PoliceSpawnCBox; }

	std::vector<Vector3>& GetNPCDirPos1() { return PoliceDirCBox1; }
	std::vector<Vector3>& GetNPCDirPos2() { return PoliceDirCBox2; }
	std::vector<Vector3>& GetNPCDirPos3() { return PoliceDirCBox3; }
	std::vector<Vector3>& GetNPCDirPos4() { return PoliceDirCBox4; }
	std::vector<Vector3>& GetNPCDirPos5() { return PoliceDirCBox5; }
	std::vector<Vector3>& GetNPCDirPos6() { return PoliceDirCBox6; }
	std::vector<Vector3>& GetNPCDirPos7() { return PoliceDirCBox7; }
	std::vector<Vector3>& GetNPCDirPos8() { return PoliceDirCBox8; }

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

	std::vector<Vector3> PoliceDirCBox1;
	std::vector<Vector3> PoliceDirCBox2;
	std::vector<Vector3> PoliceDirCBox3;
	std::vector<Vector3> PoliceDirCBox4;
	std::vector<Vector3> PoliceDirCBox5;
	std::vector<Vector3> PoliceDirCBox6;
	std::vector<Vector3> PoliceDirCBox7;
	std::vector<Vector3> PoliceDirCBox8;

	std::vector<std::shared_ptr<CBox>> PoliceDirCBox;
	std::vector<Vector3> ItemPos;
	std::vector<Vector3> EscapePos;
	std::vector<Vector3> SpecialEscapePos;
	std::vector<Vector3> PlayerSpawnPos;

	std::vector<Vector3> PlayerPos;

	CBox m_box_manager;


};

