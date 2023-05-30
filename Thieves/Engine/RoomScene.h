#pragma once
#include "Scene.h"
#include "server/protocol.h"
#include <vector>
#include <unordered_map>

struct PlayerDataInRoom {
	int id;
	std::wstring name;
	bool isReady;
};

class RoomScene : public Scene
{
public:
	void InsertPlayerData(int id, char* name)
	{
		std::string str(name);

		PlayerDataInRoom newData;
		newData.id = id;
		newData.name = std::wstring().assign(str.begin(), str.end());
		m_pData.insert(make_pair(id, newData));
	}

	void RemovePlayerData(int id)
	{
		m_pData.erase(id);
	}

	void ClearPlayerData()
	{
		m_pData.clear();
	}

	void GetRoomDataFromNetwork();

	void SetRecvDataEnd(bool val) { isRecvDataEnd = val; };
	bool GetRecvDataEnd() { return isRecvDataEnd; };

	void SetPlayerReady(int id, bool val) { m_pData.find(id)->second.isReady = val; }

	std::unordered_map<int, PlayerDataInRoom>& GetRoomData() {
		return m_pData;
	}

private:
	int m_room_id = -1;
	// 룸 내부 플레이어의 데이터를 저장하는 곳
	unordered_map<int, PlayerDataInRoom> m_pData;
	bool isRecvDataEnd = true;
};

