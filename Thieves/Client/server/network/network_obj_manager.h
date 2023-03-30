#pragma once
#include"server/define.h"
#include<unordered_map>
#include<iterator>
//#include<client/math/quaternion.h>
//#include"client/math/vec3.h"
class NetworkObj;
enum class NW_OBJ_TYPE;
class NetworkObjManager
{

public:
	NetworkObjManager() {};
	~NetworkObjManager() {};

	void AddObj(int id, NetworkObj*);
	NetworkObj* GetObj(int id) {
		auto p = m_obj_map.find(id);
		if (p == m_obj_map.end())return nullptr;
		return p->second;

	};
	std::unordered_map<int, NetworkObj*>::const_iterator GetCBegin()
	{
		return m_obj_map.cbegin();
	}
	std::unordered_map<int, NetworkObj*>::const_iterator GetCEnd()
	{
		return m_obj_map.cend();
	}
private:
	std::unordered_map<int, NetworkObj*>m_obj_map;

};
