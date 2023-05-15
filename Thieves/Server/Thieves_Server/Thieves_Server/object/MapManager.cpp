#include "pch.h"
#include <fstream>
#include <string>
#include <algorithm>
#include "MapManager.h"

void MapManager::LoadMap()
{
	std::ifstream in{ ".\\ColliderData.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start ¿Í end
		reader++; 
		reader++;

		//center
		Vector3 center = Vector3();
		center.x = std::stof((*reader)) * -100.0f; reader++;
		center.y = std::stof((*reader)) * 100.0f; reader++;
		center.z = std::stof((*reader)) * -100.0f; reader++;

		//extent
		reader++;

		Vector3 extent = Vector3();
		extent.x = std::stof((*reader)) * 100.0f; reader++;
		extent.y = std::stof((*reader)) * 100.0f; reader++;
		extent.z = std::stof((*reader)) * 100.0f; reader++;

		//up
		reader++;

		Vector3 up = Vector3();
		up.x = std::stof((*reader)); reader++;
		up.y = std::stof((*reader)); reader++;
		up.z = std::stof((*reader)); reader++;

		//right
		reader++;

		Vector3 right = Vector3();
		right.x = std::stof((*reader)); reader++;
		right.y = std::stof((*reader)); reader++;
		right.z = std::stof((*reader)); reader++;

		//right
		reader++;

		Vector3 look = Vector3();
		look.x = std::stof((*reader)); reader++;
		look.y = std::stof((*reader)); reader++;
		look.z = std::stof((*reader)); reader++;

		MapObjs.push_back(std::make_shared<OBB>(center, extent, up, right, look));

		words.erase(words.begin(), next);
	}

}

bool MapManager::isCollision(OBB& playerObb)
{
	for (auto& obj : MapObjs)
	{
		if (obj->ObbIntersectWithBox(playerObb)) return true;
	}
	
	return false;
}
