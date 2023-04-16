#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : public MonoBehaviour
{
public:
	TestCameraScript();
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

	float GetMouseTranslationSpeed() { return _mouseTranslationSpeed; }
	void AccelerateLook();
	void AccelerateRight();

	void CameraRotation();

private:
	float		_speed = 200.f;
	float		_mouseTranslationSpeed = 600.f;
	float		_mouseRotateSpeed = 0.2f;

	bool		_checkCameraRotation = true;

	// 가속도 설정
	Vec3		_vel;
	Vec3		_forceDirection;

};

