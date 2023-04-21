#pragma once
#include "server/network//network_object.h"

enum class NW_OBJ_TYPE
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
//���� ��ü�� �Ӽ��� �����ϰ� �����ϴ� ��� ����

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