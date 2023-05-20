#pragma once

#include "MonoBehaviour.h"

class FlashLightScript : public MonoBehaviour
{
public:
	FlashLightScript();
	~FlashLightScript();

	virtual void LateUpdate() override;

public:
	void FlashLightRotation();
	Vec3 GetVector() { return _lookVec; }
private:
	// Player Rotate Speed
	float		_mouseRotateSpeed = 0.5f;
	Vec3		_rotation = { 0.f,0.f,0.f };
	Vec3		_lookVec = { 1.f,0.f,0.f };
	float _x;
	float _y;
	float _z;
};


