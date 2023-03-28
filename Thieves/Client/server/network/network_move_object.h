#pragma once
#include "server/network//network_object.h"

enum class NEWORK_OBJ_TYPE
{
	OT_NONE,
	OT_MY_PLAYER,
	
	OT_PLAYER
};

//DBError : DB����
//UserFull : ����� ������
//AlreadyLogin : ���� �������� ����
//InvalidPW : ����� Ʋ��
//InvalidID :  ���� ���̵�
//ExistID : �̹� �ִ� ���̵�
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