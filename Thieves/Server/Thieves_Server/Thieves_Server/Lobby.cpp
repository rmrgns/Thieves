#include "pch.h"
#include "Lobby.h"
#include "room/room_manager.h"
#include "object/moveobj_manager.h"

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
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->state_lock.lock();
	cl->SetState(STATE::ST_LOGIN);
	cl->state_lock.unlock();
	
}

void Lobby::ResetLobby()
{
	player_list.clear();
}

void Lobby::LeaveLobby(int c_id)
{
	player_list.erase(c_id);
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->state_lock.lock();
	cl->SetState(STATE::ST_ACCEPT);
	cl->state_lock.unlock();
}
