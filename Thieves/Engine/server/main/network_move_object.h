#pragma once
#include "network_object.h"

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
	void SetIsActive(bool val) { m_is_active; }
private:
	NW_OBJ_TYPE m_obj_type;
	int m_action_type;
	bool m_is_active = true;
};