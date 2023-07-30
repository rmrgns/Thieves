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
	int32		_attackState = 0;	// 0=공격안함, 1=공격버튼 눌림, 2=공격 중
	float		_attackCoolTime = 1.8f; // 공격 쿨타임
	char		_mode = 1;

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