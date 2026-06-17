#include "MapManager.h"

#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cmath>

// cpp 파일 내부에서만 몰래 쓰이는 공간(익명 네임스페이스)을 만들어서,
// 중복되던 보일러 플레이트 파싱 로직을 이쪽으로 몰아 넣었음.
namespace {
	void LoadVector3Data(const std::string& filePath, std::vector<Vector3>& outList)
	{
		std::ifstream in{ filePath };
		if (!in.is_open()) {

			return; // 파일 없으면 죽기
		}

		std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

		while (!words.empty())
		{
			auto next = std::find(words.begin(), words.end(), "end");
			if (next == words.end()) break;
			next++;

			// 포인터 연산 에러를 막기 위해 토큰 개수가 충분한지 미리 검사
			if (std::distance(words.begin(), next) >= 5)
			{
				auto reader = words.begin();
				std::advance(reader, 2); // start 등 헤더 2개 건너뛰기

				float x = std::stof(*reader++) * -100.0f;
				float y = std::stof(*reader++) * 100.0f;
				float z = std::stof(*reader++) * -100.0f;

				outList.emplace_back(x, y, z);
			}
			words.erase(words.begin(), next);
		}
	}
}

void MapManager::LoadSpawnArea() { LoadVector3Data(".\\PoliceSpawn.txt", PoliceSpawnCBox); }
void MapManager::LoadItemSpawnPoint() { LoadVector3Data(".\\ItemBoxData.txt", ItemPos); }
void MapManager::LoadEscapePoint() { LoadVector3Data(".\\ESCAPE_AREA.txt", EscapePos); }
void MapManager::LoadSpecialEscapePoint() { LoadVector3Data(".\\SPECIAL_ESCAPE_AREA.txt", SpecialEscapePos); }
void MapManager::LoadPlayerSpawnArea() { LoadVector3Data("PLAYER_SPAWN_AREA.txt", PlayerSpawnPos); }


void MapManager::LoadMap()
{
	std::ifstream in{ ".\\ColliderData.txt" };
	if (!in.is_open()) return;

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		if (next == words.end()) break;
		next++;

		if (std::distance(words.begin(), next) >= 14)
		{
			auto reader = words.begin();
			std::advance(reader, 2);

			float centerCBox[3]{};
			float extentCBox[3]{};
			float axisCBox[3][3]{};

			// center		
			centerCBox[0] = std::stof(*reader++) * -100.0f;
			centerCBox[1] = std::stof(*reader++) * 100.0f;
			centerCBox[2] = std::stof(*reader++) * -100.0f;

			// extent
			reader++;
			extentCBox[0] = std::stof(*reader++) * 100.0f;
			extentCBox[1] = std::stof(*reader++) * 100.0f;
			extentCBox[2] = std::stof(*reader++) * 100.0f;

			// up
			reader++;
			axisCBox[1][0] = std::stof(*reader++);
			axisCBox[1][1] = std::stof(*reader++);
			axisCBox[1][2] = std::stof(*reader++);

			// right
			reader++;
			axisCBox[0][0] = std::stof(*reader++);
			axisCBox[0][1] = std::stof(*reader++);
			axisCBox[0][2] = std::stof(*reader++);

			// look
			reader++;
			axisCBox[2][0] = std::stof(*reader++);
			axisCBox[2][1] = std::stof(*reader++);
			axisCBox[2][2] = std::stof(*reader++);

			float translation[3] = { 0.0f, 0.0f, 0.0f };
			MapCBox.push_back(std::make_shared<CBox>(centerCBox, extentCBox, axisCBox, translation));
		}
		words.erase(words.begin(), next);
	}
}

void MapManager::LoadPoliceDir()
{
	std::ifstream in{ ".\\PoliceDirPos.txt" };
	if (!in.is_open()) return;

	std::vector<std::string> words{ std::istream_iterator<std::string>{in}, {} };

	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		if (next == words.end()) break;
		next++;

		if (std::distance(words.begin(), next) >= 14)
		{
			auto reader = words.begin();
			std::advance(reader, 2);

			float c1[3]{}, c2[3]{}, c3[3]{}, c4[3]{};

			c1[0] = std::stof(*reader++) * -100.0f; c1[1] = std::stof(*reader++) * 100.0f; c1[2] = std::stof(*reader++) * -100.0f;
			std::advance(reader, 2);
			c2[0] = std::stof(*reader++) * -100.0f; c2[1] = std::stof(*reader++) * 100.0f; c2[2] = std::stof(*reader++) * -100.0f;
			std::advance(reader, 2);
			c3[0] = std::stof(*reader++) * -100.0f; c3[1] = std::stof(*reader++) * 100.0f; c3[2] = std::stof(*reader++) * -100.0f;
			std::advance(reader, 2);
			c4[0] = std::stof(*reader++) * -100.0f; c4[1] = std::stof(*reader++) * 100.0f; c4[2] = std::stof(*reader++) * -100.0f;

			PoliceDirCBox.push_back(std::make_shared<CBox>(c1, c2, c3, c4));
		}
		words.erase(words.begin(), next);
	}
}

Vector3 MapManager::checkCollision(CBox& playerBox, Vector3& playerOldPos)
{
	playerOldPos.y += 75.f;

	Vector3 currentPlayerPos(playerBox.center[0], playerBox.center[1], playerBox.center[2]);
	bool collideRet = false;
	int collisionDirection = 0;

	for (auto& obj : MapCBox)
	{
		if ((obj->Intersection2(playerBox, collisionDirection)))
		{
			collideRet = true;
			break; // 이미 충돌했으면 더 계산할 필요 없음.
		}
	}

	if (collideRet)
	{
		playerOldPos.y -= 75.f;
		currentPlayerPos.y -= 75.f;
		if (collisionDirection == 0)
		{
			currentPlayerPos.x = playerOldPos.x;
		}
		else if (collisionDirection == 1)
		{
			currentPlayerPos.z = playerOldPos.z;
		}
		return currentPlayerPos;
	}

	currentPlayerPos.y -= 75.f;
	playerOldPos.y -= 75.f;
	return currentPlayerPos;
}

bool MapManager::checkCollisionRay(CBox& ray_temp) const
{
	int collisionDirection = 0;
	for (auto& obj : MapCBox)
	{
		if ((obj->Intersection2(ray_temp, collisionDirection)))
		{
			return true;
		}
	}
	return false;
}

bool MapManager::checkCollisionSafe(CBox& ray_temp) const
{
	// 하드코딩된 금고 좌표들을 가독성 좋게 하나로 묶기.
	const float safe_center[3] = { 0.03788304f * -100.0f, 1.903008f * 100.0f, -0.3053722f * -100.0f };
	const float safe_extent[3] = { 1.981820f * 100.0f, 2.106236f * 100.0f, 1.976723f * 100.0f };

	float dx = std::fabs(safe_center[0] - ray_temp.center[0]);
	float dy = std::fabs(safe_center[1] - ray_temp.center[1]);
	float dz = std::fabs(safe_center[2] - ray_temp.center[2]);

	float sumX = safe_extent[0] + ray_temp.extent[0];
	float sumY = safe_extent[1] + ray_temp.extent[1];
	float sumZ = safe_extent[2] + ray_temp.extent[2];

	// AABB 충돌 확인 공식
	return (dx <= sumX) && (dy <= sumY) && (dz <= sumZ);
}