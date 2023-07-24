	#include "pch.h"
#include "moveobj_manager.h"
//#include "lua\function\lua_function.h"
MoveObjManager* MoveObjManager::m_pInst = nullptr;

using namespace std;


bool MoveObjManager::IsNear(int a, int b)
{
	return false;
}

bool MoveObjManager::IsNPC(int id)
{
	return false;
}

float MoveObjManager::ObjDistance(int a, int b)
{
	return 0.0f;
}

int MoveObjManager::GetNewID()
{
	Player* cl = NULL;
	for (int i = 0; i < MAX_USER; ++i)
	{

		cl = GetPlayer(i);

		cl->state_lock.lock();
		if (STATE::ST_FREE == cl->GetState())
		{
			cl->SetState(STATE::ST_ACCEPT);
			cl->state_lock.unlock();
			return i;
		}
		cl->state_lock.unlock();
	}
	cout << "USER FULL!" << endl;
	return -1;
}

void MoveObjManager::Disconnect(int c_id)
{
	Player* cl = GetPlayer(c_id);
	cl->state_lock.lock();
	closesocket(cl->GetSock());
	cl->ResetPlayer();
	cl->state_lock.unlock();
}

void MoveObjManager::InitPlayer()
{
	for (int i = 0; i < MAX_USER; ++i)
		m_moveobj_arr[i] = new Player;


}

void MoveObjManager::InitNPC()
{
}


void MoveObjManager::DestroyObject()
{
	for (auto obj : m_moveobj_arr)
	{
		if (true == IsPlayer(obj->GetID()))
		{
			if (GetPlayer(obj->GetID())->GetState() != STATE::ST_FREE)
				Disconnect(obj->GetID());
		}
		if (obj)
		{
			delete obj;
			obj = NULL;
		}
	}
}
