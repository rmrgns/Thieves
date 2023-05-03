#include "pch.h"
#include "map_manager.h"

#include "collision_checker.h"
#include<fstream>
#include<sstream>
#include<vector>
#include<bitset>

using namespace std;

// 맵정보 받아오기
void MapManager::LoadMap(const std::string& path)
{
	ifstream map_file(path, ifstream::binary);
	if (!map_file)
	{
		cout << "FAIL: map load" << endl;
		return;
	}

	vector<MapInfo>actor_info_data;
}