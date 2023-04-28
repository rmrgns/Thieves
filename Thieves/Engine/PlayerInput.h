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
	

private:

	float		_speed = 200.f;
	float		_mouseRotateSpeed = 0.5f;
	float		_jumpCount = 0.f;
	bool		_jump = false;
	float		_jumpSpeed = 305.f;
	char		direction = 0;

	bool		_checkCameraRotation = true;
	
	Vec3		recv_pos;

};