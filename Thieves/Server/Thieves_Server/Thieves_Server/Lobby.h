#pragma once
#include <vector>

class RoomManager;

class Lobby
{
public:
	Lobby() = default;
	~Lobby();

	void Init();
	void EnterLobby(int c_id);
	void ResetLobby();

	void LeaveLobby(int c_id);

private:
	std::vector<int> player_list;


};

