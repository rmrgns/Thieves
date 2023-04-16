#pragma once
#include "MonoBehaviour.h"

class PlayerInput : public MonoBehaviour
{
public:
	PlayerInput();
	virtual ~PlayerInput();

	virtual void LateUpdate() override;

	void PlayerRotation();

private:
	float		_speed = 200.f;
	float		_mouseRotateSpeed = 0.5f;
};