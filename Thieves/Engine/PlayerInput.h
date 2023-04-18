#pragma once
#include "MonoBehaviour.h"
#include "server/packet/packet_manager.h"
#include "thieves_server/thieves_packet/thieves_packet_manager.h"
class PlayerInput : public MonoBehaviour
{
public:
	PlayerInput();
	virtual ~PlayerInput();

	virtual void LateUpdate() override;

	void PlayerRotation();
	void Jump(Vec3&);

	shared_ptr<ThievesPacketManager> thieves_packet_manager;

private:
	float		_speed = 200.f;
	float		_mouseRotateSpeed = 0.5f;
	int			_jumpCount = 0;
	bool		_jump = false;
	float		_jumpSpeed = 300.f;
	char		direction = 0;

	
};