#pragma once
#include<array>
#include<vector>
#include"move_object/player.h"
#include<unordered_set>
#include"move_object/enemy.h"
class MoveObjManager
{
private:
	static MoveObjManager* m_pInst;

public:
	static MoveObjManager* GetInst()
	{
		if (!m_pInst)
			m_pInst = new MoveObjManager;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst)
		{
			delete m_pInst;
			m_pInst = NULL;
		}
	}
public:
	MoveObjManager() {  };
	~MoveObjManager() = default;

	Player* GetPlayer(int id) {

		Player* p = ((Player*)(m_moveobj_arr[id]));
		return p;

	}
	Enemy* GetEnemy(int id) {
		//Enemy* e = ;
		return ((Enemy*)(m_moveobj_arr[id]));
	}
	MoveObj* GetMoveObj(int id) { return m_moveobj_arr[id]; }
	bool IsPlayer(int id) { return (id >= 0) && (id < MAX_USER); }
	bool IsNear(int a, int b);
	bool IsNPC(int id);
	float ObjDistance(int a, int b);
	void InitLua(const char* script_name, int obj_id, const Vector3& base_pos);
	void RegisterAPI(lua_State* L);

	static void LuaErrorDisplay(lua_State* L, int err_num);
	int GetNewID();
	void Disconnect(int);
	void InitPlayer();
	void InitNPC();
	void DestroyObject();

private:
	std::array <MoveObj*, MAX_USER + MAX_NPC>m_moveobj_arr;

	int m_id;
};
