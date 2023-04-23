#pragma once
#include "MonoBehaviour.h"

class PlayerCamera : public MonoBehaviour
{
public:
	PlayerCamera();
	virtual ~PlayerCamera();
	virtual void LateUpdate() override;

	void CameraRotation();
private:
	float		_speed = 200.f;
	float		_mouseTranslationSpeed = 600.f;
	float		_mouseRotateSpeed = 0.5f;

	bool		_checkCameraRotation = true;

};