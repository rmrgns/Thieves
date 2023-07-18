#pragma once
#include <unordered_set>

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

	std::unordered_set<int>& GetPlayerList() { return player_list; };

private:
	std::unordered_set<int> player_list;


};

