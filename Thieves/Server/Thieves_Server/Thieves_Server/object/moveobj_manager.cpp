#include "pch.h"
#include "moveobj_manager.h"
//#include "lua\function\lua_function.h"
MoveObjManager* MoveObjManager::m_pInst = nullptr;

using namespace std;

//bool MoveObjManager::IsNear(int a, int b)
//{
//	//Vector3& obj_a = m_moveobj_arr[a]->GetPos();
//	//Vector3& obj_b = m_moveobj_arr[b]->GetPos();
//	//return (FOV_RANGE > sqrt(pow(abs(obj_a.x - obj_b.x), 2) + pow(abs(obj_a.z - obj_b.z), 2)));
//
//
//}

//float MoveObjManager::ObjDistance(int a, int b)
//{
//	//Vector3& obj_a = m_moveobj_arr[a]->GetPos();
//	//Vector3& obj_b = m_moveobj_arr[b]->GetPos();
//	//return sqrt(pow(abs(obj_a.x - obj_b.x), 2) + pow(abs(obj_a.z - obj_b.z), 2));
//	return;
//}

//void MoveObjManager::InitLua(const char* script_name, int obj_id, const Vector3& base_pos)
//{
//	Enemy* en = GetEnemy(obj_id);
//	lua_State* L = en->GetLua();
//
//	luaL_openlibs(L);
//
//	int error = luaL_loadfile(L, script_name) ||
//		lua_pcall(L, 0, 0, 0);
//	if (error) { cout << "Error : " << lua_tostring(L, -1); lua_pop(L, 1); }
//
// 	lua_getglobal(L, "initializEnemy");
//	lua_pushnumber(L, en->GetID());
//	lua_pushnumber(L, en->GetPosX());
//	lua_pushnumber(L, en->GetPosY());
//	lua_pushnumber(L, en->GetPosZ());
//	lua_pushnumber(L, en->GetHP());
//	lua_pushnumber(L, en->GetDamge());
//	lua_pushnumber(L, base_pos.x);
//	lua_pushnumber(L, base_pos.y);
//	lua_pushnumber(L, base_pos.z);
//	lua_pushnumber(L, BASE_ID);
//	error = lua_pcall(L, 10, 0, 0);
//	if (error)
//		MoveObjManager::LuaErrorDisplay(L, error);
//	RegisterAPI(L);
//
//}

//void MoveObjManager::RegisterAPI(lua_State* L)
//{
//	lua_register(L, "API_get_x", API_get_x);
//	lua_register(L, "API_get_y", API_get_y);
//	lua_register(L, "API_get_z", API_get_z);
//	lua_register(L, "API_move", API_move);
//	lua_register(L, "API_attack", API_attack);
//
//	lua_register(L, "API_test_lua", API_test_lua);
//
//}





void MoveObjManager::LuaErrorDisplay(lua_State* L, int err_num)
{

	cout << "Error : " << lua_tostring(L, -1) << endl;
	lua_pop(L, 1);

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
	cout << "Get new ID" << endl;
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

//void MoveObjManager::InitNPC()
//{
//	for (int i = NPC_ID_START; i <= NPC_ID_END; ++i)
//		m_moveobj_arr[i] = new Enemy(i);
//}

void MoveObjManager::DestroyObject()
{
	//for (int i = 0; i < MAX_USER; ++i)
	//{
	//	if (STATE::ST_INGAME == m_moveobj_arr[i]->GetState())
	//	{
	//		Disconnect(m_moveobj_arr[i]->GetID());
	//		delete m_moveobj_arr[i];
	//	}
	//}
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
