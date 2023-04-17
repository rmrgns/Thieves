#pragma once
#include "MonoBehaviour.h"

class PlayerInput : public MonoBehaviour
{
public:
	PlayerInput();
	virtual ~PlayerInput();

	virtual void LateUpdate() override;

	void PlayerRotation();
	void Jump(Vec3&);

private:
	float		_speed = 200.f;
	float		_mouseRotateSpeed = 0.5f;
	int			_jumpCount = 0;
	bool		_jump = false;
	float		_jumpSpeed = 300.f;
	char		direction = 0;
};