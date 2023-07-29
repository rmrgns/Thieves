#include "pch.h"
#include <fstream>
#include <string>
#include <algorithm>
#include "MapManager.h"
#include "CBox.h"

void MapManager::LoadMap()
{
	std::ifstream in{ ".\\ColliderData.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start �� end
		reader++;
		reader++;
		
		float centerCBox[3] {};
		float extentCBox[3]{};
		float axisCBox[3][3]{};

		

		//center		
		centerCBox[0] = std::stof((*reader)) * -100.0f; reader++;
		centerCBox[1] = std::stof((*reader)) * 100.0f; reader++;
		centerCBox[2] = std::stof((*reader)) * -100.0f; reader++;
		
		//extent
		reader++;	
		
		extentCBox[0] = std::stof((*reader)) * 100.0f; reader++;
		extentCBox[1] = std::stof((*reader)) * 100.0f; reader++;
		extentCBox[2] = std::stof((*reader)) * 100.0f; reader++;
		//up
		reader++;

		axisCBox[1][0] = std::stof((*reader)); reader++;
		axisCBox[1][1] = std::stof((*reader)); reader++;
		axisCBox[1][2] = std::stof((*reader)); reader++;
		//right
		reader++;


//		float rightCBox[3];
		axisCBox[0][0] = std::stof((*reader)); reader++;
		axisCBox[0][1] = std::stof((*reader)); reader++;
		axisCBox[0][2] = std::stof((*reader)); reader++;

		//look
		reader++;

		axisCBox[2][0] = std::stof((*reader)); reader++;
		axisCBox[2][1] = std::stof((*reader)); reader++;
		axisCBox[2][2] = std::stof((*reader)); reader++;

		float translation[3] = { 0.0f, 0.0f, 0.0f };
		MapCBox.push_back(std::make_shared<CBox>(centerCBox, extentCBox,axisCBox , translation));
		words.erase(words.begin(), next);
	}

}

void MapManager::LoadSpawnArea()
{
	std::ifstream in{ ".\\PoliceSpawn.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start �� end
		reader++;
		reader++;


		float centerCBox[3]{};

		//center		
		float x = std::stof((*reader)) * -100.0f; reader++;
		float y = std::stof((*reader)) * 100.0f; reader++;
		float z = std::stof((*reader)) * -100.0f; reader++;
	
		PoliceSpawnCBox.emplace_back(x,y,z);
		words.erase(words.begin(), next);
	}

}

void MapManager::LoadPoliceDir()
{
	std::ifstream in{ ".\\PoliceDirPos.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start �� end
		reader++;
		reader++;

		float centerCBox1[3]{};
		float centerCBox2[3]{};
		float centerCBox3[3]{};
		float centerCBox4[3]{};

		//center		
		centerCBox1[0] = std::stof((*reader)) * -100.0f; reader++;
		centerCBox1[1] = std::stof((*reader)) * 100.0f; reader++;
		centerCBox1[2] = std::stof((*reader)) * -100.0f; reader++;
		reader++;
		reader++;

		//center		
		centerCBox2[0] = std::stof((*reader)) * -100.0f; reader++;
		centerCBox2[1] = std::stof((*reader)) * 100.0f; reader++;
		centerCBox2[2] = std::stof((*reader)) * -100.0f; reader++;
		reader++;
		reader++;

		//center		
		centerCBox3[0] = std::stof((*reader)) * -100.0f; reader++;
		centerCBox3[1] = std::stof((*reader)) * 100.0f; reader++;
		centerCBox3[2] = std::stof((*reader)) * -100.0f; reader++;
		reader++;
		reader++;

		//center		
		centerCBox4[0] = std::stof((*reader)) * -100.0f; reader++;
		centerCBox4[1] = std::stof((*reader)) * 100.0f; reader++;
		centerCBox4[2] = std::stof((*reader)) * -100.0f; reader++;
		

		PoliceDirCBox.push_back(std::make_shared<CBox>(centerCBox1, centerCBox2, centerCBox3, centerCBox4));
		words.erase(words.begin(), next);
	}

}

void MapManager::LoadItemSpawnPoint()
{
	std::ifstream in{ ".\\ItemBoxData.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start �� end
		reader++;
		reader++;

		float x = std::stof((*reader)) * -100.0f; reader++;
		float y = std::stof((*reader)) * 100.0f; reader++;
		float z = std::stof((*reader)) * -100.0f; reader++;
		ItemPos.emplace_back(x, y, z);

		words.erase(words.begin(), next);
	}
}

void MapManager::LoadEscapePoint()
{
	std::ifstream in{ ".\\ESCAPE_AREA.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start �� end
		reader++;
		reader++;


		float x = std::stof((*reader)) * -100.0f; reader++;
		float y = std::stof((*reader)) * 100.0f; reader++;
		float z = std::stof((*reader)) * -100.0f; reader++;
		EscapePos.emplace_back(x, y, z);

		words.erase(words.begin(), next);
	}
}

void MapManager::LoadSpecialEscapePoint()
{
	std::ifstream in{ ".\\SPECIAL_ESCAPE_AREA.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start �� end
		reader++;
		reader++;

		float x = std::stof((*reader)) * -100.0f; reader++;
		float y = std::stof((*reader)) * 100.0f; reader++;
		float z = std::stof((*reader)) * -100.0f; reader++;
		SpecialEscapePos.emplace_back(x, y, z);

		words.erase(words.begin(), next);
	}
}

void MapManager::LoadPlayerSpawnArea()
{
	std::ifstream in{ ".\\PLAYER_SPAWN_AREA.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start �� end
		reader++;
		reader++;

		float x = std::stof((*reader)) * -100.0f; reader++;
		float y = std::stof((*reader)) * 100.0f; reader++;
		float z = std::stof((*reader)) * -100.0f; reader++;
		PlayerSpawnPos.emplace_back(x, y, z);

		words.erase(words.begin(), next);
	}
}

// true�̸� �浹 
Vector3 MapManager::checkCollision(CBox& playerBox, Vector3& playerOldPos)
{
	playerOldPos.y += 75.f;

	Vector3 currentPlayerPos(playerBox.center[0], playerBox.center[1], playerBox.center[2]);
	Vector3 boxVelocity(currentPlayerPos - playerOldPos);
	bool collideRet = FALSE;
	int collisionDirection = 0;
	// �浹�ϴ� ������ �� ������
	for (auto& obj : MapCBox)
	{
		
		Vector3 collisionSlidingVector = { 0,0,0 };
		if ((obj->Intersection2(playerBox, collisionDirection)))
		{
			collideRet = TRUE;
		}
	}
	if (collideRet)
	{
		//if (obj->BoxBoxIntersection2(playerBox)) {
		playerOldPos.y -= 75.f;
		currentPlayerPos.y -= 75.f;
		if (collisionDirection == 0)
		{
			currentPlayerPos.x = playerOldPos.x;
			currentPlayerPos.y;
			currentPlayerPos.z;
		}
		else if (collisionDirection == 1)
		{
			currentPlayerPos.x;
			currentPlayerPos.y;
			currentPlayerPos.z = playerOldPos.z;
		}
		collisionDirection = 0;
		collideRet = FALSE;
		return currentPlayerPos;

		//}
	}


	currentPlayerPos.y -= 75.f;
	playerOldPos.y -= 75.f;
	collisionDirection = 0;
	collideRet = FALSE;
	return currentPlayerPos;
}

bool MapManager::checkCollisionRay(CBox& ray_temp)
{
	Vector3 currentPlayerPos(ray_temp.center[0], ray_temp.center[1], ray_temp.center[2]);
	
	bool collideRet = FALSE;
	int collisionDirection = 0;
	// �浹�ϴ� ������ �� ������
	for (auto& obj : MapCBox)
	{

		Vector3 collisionSlidingVector = { 0,0,0 };
		if ((obj->Intersection2(ray_temp, collisionDirection)))
		{
			return TRUE;
		}
	}

	return TRUE;
}

