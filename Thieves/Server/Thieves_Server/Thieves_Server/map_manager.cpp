#include "pch.h"
#include "map_manager.h"

#include "collision_checker.h"
#include<fstream>
#include<sstream>
#include <string>
#include <algorithm>
#include<vector>
#include<bitset>

using namespace std;

// 맵정보 받아오기
void MapManager::LoadMap(const std::string& path)
{
	ifstream map_file(path, ifstream::binary);
	if (!map_file)
	{
		cout << "Fail : map load" << endl;
		return;
	}


	vector<MapInfo>actor_info_data;
	actor_info_data.reserve(1000);


	UINT temp_col_count = 0;

//	stringstream ss;
//	string line;
//	string prefix;
//	Vector3 temp_pos;


//	std::vector<Vector3> scales;
//	std::vector<Vector3> collision_centers;
//	std::vector<Vector3> collision_size;

	std::vector<std::string>words{ std::istream_iterator<std::string>{map_file}, {} };

	
	while (!words.empty())
	{
		auto next = std::find(words.begin(), words.end(), "end");
		next++;

		auto reader = words.begin();

		//start end
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

//		MapObjs.push_back(std::make_shared<OBB>(center, extent, up, right, look));
		m_map_objects.emplace_back(center, extent, up, right, look);

		temp_col_count++;
		words.erase(words.begin(), next);
	}

	// 맵 파일에서 한 줄 씩 읽기
	//while (getline(map_file, line))
	//{
	//	ss.clear();
	//	prefix.clear();
	//	ss.str(line);
	//	ss >> prefix;
	//	switch (HashCode(prefix.c_str()))
	//	{
	//	case HashCode("center"):
	//	{
	//		temp_col_count++;
	//		ss >> temp_pos.x >> temp_pos.y >> temp_pos.z;
	//		temp_pos.x = temp_pos.x * 10;
	//		temp_pos.y = temp_pos.y * 10;
	//		temp_pos.z = temp_pos.z * 10;
	//		
	//		collision_centers.emplace_back(std::move(temp_pos));
	//		
	//		break;
	//	}
	//	case HashCode("size"): {
	//		ss >> temp_pos.x >> temp_pos.y >> temp_pos.z;
	//		temp_pos.x = temp_pos.x * 5;
	//		temp_pos.y = temp_pos.y * 5;
	//		temp_pos.z = temp_pos.z * 5;
	//		scales.emplace_back(std::move(temp_pos));
	//		break;
	//	}
	//	}
	//}	
	//// 수정 필요 if 이 좌표안에 있으면 true 아니면 false 출력
	//for (int i = 0 ; i < temp_col_count; ++i)
	//{
	//		m_map_objects.emplace_back(i, collision_centers[i], scales[i], true, OBJ_TYPE::OT_MAPOBJ);	
	//}
	//	BlockTileMap();
}



bool MapManager::CheckCollision(BoxCollision& obj_collision)
{
	for (auto& map_obj : m_map_objects)
	{
		if (map_obj.GetIsBlocked() == false)continue;
//		if (CollisionChecker::CheckCollisions(obj_collision, BoxCollision(map_obj.GetPos(), map_obj.GetExtent())))
//			return true;

	}
	return false;
}


bool MapManager::CheckInRange(BoxCollision& collision)
{
	bitset<4>check_set;
	check_set.reset();

	for (auto& map_obj : m_map_objects)
	{
		if (OBJ_TYPE::OT_ACTIViTY_AREA != map_obj.GetType())continue;
		if (CollisionChecker::CheckInRange(collision.GetMinPos().x, collision.GetMinPos().z,
			map_obj.GetMinPos(), map_obj.GetMaxPos())) {
			check_set.set(0);
		}
		if (CollisionChecker::CheckInRange(collision.GetMinPos().x, collision.GetMaxPos().z,
			map_obj.GetMinPos(), map_obj.GetMaxPos())) {
			check_set.set(1);
		}
		if (CollisionChecker::CheckInRange(collision.GetMaxPos().x, collision.GetMinPos().z,
			map_obj.GetMinPos(), map_obj.GetMaxPos())) {
			check_set.set(2);
		}
		if (CollisionChecker::CheckInRange(collision.GetMaxPos().x, collision.GetMaxPos().z,
			map_obj.GetMinPos(), map_obj.GetMaxPos())) {
			check_set.set(3);
		}
	}

	return check_set.all();
}

bool MapManager::CheckInRange(const Vector3& pos, OBJ_TYPE map_type)
{
	for (auto& map_obj : m_map_objects)
	{
		if (map_type != map_obj.GetType())continue;
		if (CollisionChecker::CheckInRange(pos.x, pos.z, map_obj.GetMinPos(), map_obj.GetMaxPos())) {
			return true;
		}

	}
	return false;
}


