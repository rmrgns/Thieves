#include "MoveObjManager.h"
#include "State.hpp"
#include <iostream>

using namespace std;

std::shared_ptr<Player> MoveObjManager::GetPlayer(int id)
{
	if (!IsPlayer(id) || !m_moveobj_arr[id]) return nullptr;
	return std::static_pointer_cast<Player>(m_moveobj_arr[id]);
}

std::shared_ptr<Enemy> MoveObjManager::GetEnemy(int id)
{
	if (!IsNPC(id) || !m_moveobj_arr[id]) return nullptr;
	return std::static_pointer_cast<Enemy>(m_moveobj_arr[id]);
}

std::shared_ptr<MoveObj> MoveObjManager::GetMoveObj(int id)
{
	if (id < 0 || id >= static_cast<int>(m_moveobj_arr.size())) return nullptr;
	return m_moveobj_arr[id];
}

void MoveObjManager::ActivePlayer(int sessionId)
{
	if (auto cl = GetPlayer(sessionId))
	{
		std::lock_guard<std::mutex> lock(cl->state_lock);
		cl->SetState(STATE::ST_ACCEPT);
	}
}

void MoveObjManager::RemovePlayer(int sessionId)
{
	if (auto cl = GetPlayer(sessionId))
	{
		std::lock_guard<std::mutex> lock(cl->state_lock);
		cl->ResetPlayer();
		cl->SetState(STATE::ST_FREE);
	}
}

void MoveObjManager::InitPlayer()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		m_moveobj_arr[i] = std::make_shared<Player>();
	}
}

void MoveObjManager::InitNPC()
{
	for (int i = NPC_ID_START; i <= NPC_ID_END; ++i)
	{
		m_moveobj_arr[i] = std::make_shared<Enemy>(i);
	}
}

void MoveObjManager::ClearObjects()
{
	for (auto& obj : m_moveobj_arr)
	{
		if (obj) obj.reset();
	}
}