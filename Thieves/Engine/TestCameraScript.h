#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : public MonoBehaviour
{
public:
	TestCameraScript();
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

	float GetMouseTranslationSpeed() { return _mouseTranslationSpeed; }

private:
	float		_speed = 200.f;
	float		_mouseTranslationSpeed = 600.f;
	float		_mouseRotateSpeed = 1.f;

};

