#pragma once

#include "MonoBehaviour.h"

class FlashLightScript : public MonoBehaviour
{
public:
	FlashLightScript();
	~FlashLightScript();

	virtual void LateUpdate() override;

private:
	// Player Rotate Speed
	float		_mouseRotateSpeed = 0.5f;

};


