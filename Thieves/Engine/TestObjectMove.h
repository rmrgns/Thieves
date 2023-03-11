#pragma once
#include "MonoBehaviour.h"

class TestObjectMove : public MonoBehaviour
{
public:
	TestObjectMove();
	virtual ~TestObjectMove();

	virtual void LateUpdate() override;

private:
	float		_speed = 10.f;
	float		_objectTranslationSpeed = 10.f;
};
