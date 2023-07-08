#pragma once
#include "Scene.h"
#include <unordered_map>

struct RoomDataInLobby {
	int id;
	int nPlayer;
	bool isInGame;
};

class LobbyScene : public Scene
{
public:
	void InsertRoomData(int id, int nPlayer, bool isInGame)
	{
		RoomDataInLobby data;
		data.id = id;
		data.nPlayer = nPlayer;
		data.isInGame = isInGame;

		m_rData.insert(std::make_pair(id, data));
	}

	void ClearRoomData() {
		m_rData.clear();
	}

	void GetRoomsDataFromNetwork();

	void SetRecvDataEnd(bool val) { isRecvDataEnd = val; }
	bool GetRecvDataEnd() { return isRecvDataEnd; }

	std::unordered_map<int, RoomDataInLobby>& GetRoomsData() {
		return m_rData;
	}

private:
	// 로비에서 룸 데이터를 저장하는 곳
	std::unordered_map<int, RoomDataInLobby> m_rData;
	bool isRecvDataEnd = true;
};

