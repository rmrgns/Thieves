#pragma once
#include "server/network//network_object.h"

enum class NW_OBJ_TYPE
{
	OT_NONE,
	OT_MY_PLAYER,
	
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
	NetworkMoveObj(int id,  NW_OBJ_TYPE type, float x, float y, float z )
	{
		m_id = id;
		m_type = type;
		m_position = Vec3(x, y, z);
	}
	NetworkMoveObj(const NetworkMoveObj& other)
	{
		m_id = other.GetID();
		m_type = other.GetType();
		m_position = other.GetPosition();
	}
	~NetworkMoveObj();

	void SetType(const NW_OBJ_TYPE val) { m_type = val; }
	const NW_OBJ_TYPE GetType()const { return m_type; }

	bool m_is_active = true;
private:
	NW_OBJ_TYPE m_type;
};