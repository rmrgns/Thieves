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
	float		_jumpSpeed = 305.f;

	// Player Attack
	float		_attackCount = 0.f;
	int32		_attackState = 0;	// 0=공격안함, 1=공격버튼 눌림, 2=공격 중

	// Send Data to Server
	char		direction = 0;

	//temp
	bool		_checkCameraRotation = true;
	
	Vec3		recv_pos;

	// Animation Index
	int32		_index = 2;
};