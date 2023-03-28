#pragma once
#include "server/network//network_object.h"

enum class NEWORK_OBJ_TYPE
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


class NetworkMoveObj : public NetworkObj
{
public:
	NetworkMoveObj();
	~NetworkMoveObj();
};