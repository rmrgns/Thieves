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
		
		extentCBox[0] = std::stof((*reader)) * 1000.0f; reader++;
		extentCBox[1] = std::stof((*reader)) * 1000.0f; reader++;
		extentCBox[2] = std::stof((*reader)) * 1000.0f; reader++;
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


// true�̸� �浹 
Vector3 MapManager::checkCollision(CBox& playerBox, Vector3& playerOldPos)
{
	playerOldPos.y += 75.f;

	Vector3 currentPlayerPos(playerBox.center[0], playerBox.center[1], playerBox.center[2]);
	Vector3 boxVelocity(currentPlayerPos - playerOldPos);


	// �浹�ϴ� ������ �� ������
	for (auto& obj : MapCBox)
	{
		if (obj->BoxBoxIntersection(playerBox))
		{
			// �浹�߾�.
			playerOldPos.y -= 75.f;
			return (playerOldPos + obj->CalculateSliding(playerBox, boxVelocity));
		}
	}

	currentPlayerPos.y -= 75.f;
	playerOldPos.y -= 75.f;
	// �浹���߾�
	return currentPlayerPos;
}