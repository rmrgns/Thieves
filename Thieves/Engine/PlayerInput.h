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
	bool		_move = false;

	// Player Rotate Speed
	float		_mouseRotateSpeed = 0.5f;

	// Player Jump
	float		_jumpCount = 0.f;
	bool		_jump = false;
	float		_jumpSpeed = 305.f;

	// Player Attack
	float		_attackCount = 0.f;
	int32		_attack = 0;	// 0=none, 1=attack on, 2=attack

	// Send Data to Server
	char		direction = 0;

	//temp
	bool		_checkCameraRotation = true;
	
	Vec3		recv_pos;

	// Animation Index
	int32		_index = 2;
};