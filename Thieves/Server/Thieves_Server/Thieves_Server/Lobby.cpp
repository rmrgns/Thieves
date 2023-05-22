#include "pch.h"
#include "Lobby.h"
#include "room/room_manager.h"

Lobby::~Lobby()
{
	player_list.clear();
}

void Lobby::Init()
{

}

void Lobby::EnterLobby(int c_id)
{
	player_list.insert(c_id);
}

void Lobby::ResetLobby()
{
	player_list.clear();
}

void Lobby::LeaveLobby(int c_id)
{
	player_list.erase(c_id);
}
