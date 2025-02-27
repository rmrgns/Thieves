#pragma once
#include "network_object.h"

enum class NW_OBJ_TYPE
{
	OT_NONE,
	OT_MY_PLAYER,
	OT_NPC,
	OT_PLAYER
};

//DBError : DB에러
//UserFull : 사용자 가득참
//AlreadyLogin : 현재 접속중인 계정
//InvalidPW : 비번이 틀림
//InvalidID :  없는 아이디
//ExistID : 이미 있는 아이디
enum class eLoginFailType
{
	kDBError, kUserFull,
	kAlreadyLogin, kInvalidPW,
	kInvalidID, kExistID,
};
//게임 객체의 속성을 저장하고 설정하는 멤버 변수

class NetworkMoveObj : public NetworkObj
{
public:
	NetworkMoveObj();
	NetworkMoveObj(int id,  NW_OBJ_TYPE obj_type, float x, float y, float z)
	{
		m_id = id;
		m_obj_type = obj_type;
		m_position = Vec3(x, y, z);
		//m_action_type = action_type;
	
	}
	NetworkMoveObj(const NetworkMoveObj& other)
	{
		m_id = other.GetID();
		m_obj_type = other.GetType();
		m_position = other.GetPosition();
		m_action_type = other.GetActionType();

	}
	~NetworkMoveObj();

	void SetType(const NW_OBJ_TYPE val) { m_obj_type = val; }
	const NW_OBJ_TYPE GetType()const { return m_obj_type; }
	
	void SetActionType(const int& val) { m_action_type = val; }
	const int GetActionType()const { return m_action_type; }

	bool GetIsActive() { return m_is_active; }
	void SetIsActive(bool val) { m_is_active = val; }

	bool GetIsInvincible() { return m_is_invincible; }
	void SetIsInvincible(bool val) { m_is_invincible = val; }

	bool GetIsStun() { return m_is_stun; }
	void SetIsStun(bool val) { m_is_stun = val; }

private:
	NW_OBJ_TYPE m_obj_type;
	int m_action_type;
	bool m_is_active = true;
	bool m_is_invincible = false;
	bool m_is_stun = false;
};