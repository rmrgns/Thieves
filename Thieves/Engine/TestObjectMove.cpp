#include "pch.h"
#include "TestObjectMove.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

TestObjectMove::TestObjectMove()
{
}

TestObjectMove::~TestObjectMove()
{
}

void TestObjectMove::LateUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::UP))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::DOWN))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::LEFT))
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::RIGHT))
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	GetTransform()->SetLocalPosition(pos);
}