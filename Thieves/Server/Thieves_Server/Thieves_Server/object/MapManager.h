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
	Vector3 CalculateSlidingVector(const Vector3& previousPositionA, const Vector3& currentPositionA,
		const Vector3& previousPositionB, const Vector3& currentPositionB);
private:
	std::vector<std::shared_ptr<CBox>> MapCBox;
};

