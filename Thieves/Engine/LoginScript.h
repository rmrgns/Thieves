#pragma once
#include "MonoBehaviour.h"

class LoginScript : public MonoBehaviour
{
public:
	LoginScript();
	virtual ~LoginScript();

	virtual void LateUpdate() override;

private:
	float		_speed = 20.f;

};

