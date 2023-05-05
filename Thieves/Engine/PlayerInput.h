#pragma once
#include "MonoBehaviour.h"
#include "server/main/packet_manager.h"
#include "server/thieves_server/thieves_packet_manager.h"
#include "server/main/network_object.h"
class PlayerInput : public MonoBehaviour
{
public:
	PlayerInput();
	virtual ~PlayerInput();

	virtual void LateUpdate() override;

	void PlayerRecvPos(bool);
	void PlayerMove();
	void PlayerRotation();
	void Jump(Vec3&);
	void PlayerAttack();

	bool	GetMoveState() { return _moveState; }
	int32	GetJumpState() { return _jumpState; }
	int32	GetAttackState() { return _attackState; }
	void	SetMoveState(bool val) {  _moveState = val; }
	void	SetJumpState(int32 val) {  _jumpState = val; }
	void	SetAttackState(int32 val) {  _attackState = val; }
	
private:
	// Player Move
	float		_speed = 200.f;
	bool		_moveState = false;

	// Player Rotate Speed
	float		_mouseRotateSpeed = 0.5f;

	// Player Jump
	float		_jumpCount = 0.f;
	int32		_jumpState = 0;
	float		_jumpSpeed = 100.f;


	// Player Attack
	float		_attackCount = 0.f;
	int32		_attackState = 0;	// 0=���ݾ���, 1=���ݹ�ư ����, 2=���� ��

	// Send Data to Server
	char		direction = 0;

	//temp
	bool		_checkCameraRotation = true;
	
	Vec3		recv_pos;

	// Animation Index
	int32		_index = 2;

	// player action
	char _action_type;
};