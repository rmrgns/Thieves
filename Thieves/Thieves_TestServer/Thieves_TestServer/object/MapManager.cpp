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

		//start 와 end
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

	std::ifstream in{ ".\\ESCAPE_AREA.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start 와 end
		reader++;
		reader++;

		float centerCBox[3]{};
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
		EscpaeArea.push_back(std::make_shared<Escape_area>(centerCBox, extentCBox, axisCBox, translation));
		words.erase(words.begin(), next);
	}

	std::ifstream in{ ".\\ItemBoxData.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start 와 end
		reader++;
		reader++;

		float centerCBox[3]{};
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
		ItemArea.push_back(std::make_shared<Item_area>(centerCBox, extentCBox, axisCBox, translation));
		words.erase(words.begin(), next);
	}

	std::ifstream in{ ".\\SPAWN_AREA.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start 와 end
		reader++;
		reader++;

		float centerCBox[3]{};
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
		SpawnArea.push_back(std::make_shared<Spawn_area>(centerCBox, extentCBox, axisCBox, translation));
		words.erase(words.begin(), next);
	}

	std::ifstream in{ ".\\SPECIAL_ESCAPE_AREA.txt" };

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start 와 end
		reader++;
		reader++;

		float centerCBox[3]{};
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
		SSpawnArea.push_back(std::make_shared<SSpawn_area>(centerCBox, extentCBox, axisCBox, translation));
		words.erase(words.begin(), next);
	}
}


// true이면 충돌 
Vector3 MapManager::checkCollision(CBox& playerBox, Vector3& playerOldPos)
{
	playerOldPos.y += 75.f;

	Vector3 currentPlayerPos(playerBox.center[0], playerBox.center[1], playerBox.center[2]);
	Vector3 boxVelocity(currentPlayerPos - playerOldPos);
	bool collideRet = FALSE;
	int collisionDirection = 0;
	// 충돌하는 순간의 맵 데이터
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