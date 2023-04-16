#include "pch.h"
#include "PlayerInput.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Engine.h"

PlayerInput::PlayerInput()
{
}

PlayerInput::~PlayerInput()
{
}

void PlayerInput::LateUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::W))
	{
		//pos += GetTransform()->GetLook() * _speed * DELTA_TIME;
		pos.x += GetTransform()->GetLook().x * _speed * DELTA_TIME;
		pos.z += GetTransform()->GetLook().z * _speed * DELTA_TIME;
	}
	if (INPUT->GetButton(KEY_TYPE::S))
	{
		//pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
		pos.x -= GetTransform()->GetLook().x * _speed * DELTA_TIME;
		pos.z -= GetTransform()->GetLook().z * _speed * DELTA_TIME;
	}
	if (INPUT->GetButton(KEY_TYPE::A))
	{
		//pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
		pos.x -= GetTransform()->GetRight().x * _speed * DELTA_TIME;
		pos.z -= GetTransform()->GetRight().z * _speed * DELTA_TIME;
	}
	if (INPUT->GetButton(KEY_TYPE::D))
	{
		//pos += GetTransform()->GetRight() * _speed * DELTA_TIME;
		pos.x += GetTransform()->GetRight().x * _speed * DELTA_TIME;
		pos.z += GetTransform()->GetRight().z * _speed * DELTA_TIME;
	}

	// 카메라 마우스제어
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		PlayerRotation();
	}

	GET_SINGLE(SceneManager)->SetPlayerPosition(pos);

	GetTransform()->SetLocalPosition(pos);
}

void PlayerInput::PlayerRotation()
{
	const POINT& point = INPUT->GetMousePos();	// 현재 마우스좌표를 point변수에 저장
	float mouseX = static_cast<float>(point.x);	// point변수의 값을 float값으로 변경
	//float mouseY = static_cast<float>(point.y);

	POINT WindowCenter = GEngine->GetWindowCenter();
	SetCursorPos(WindowCenter.x, WindowCenter.y);

	POINT ClientCenter = GEngine->GetClientCenter();

	float viewMouseX = static_cast<float>(ClientCenter.x);
	//float viewMouseY = static_cast<float>(ClientCenter.y);

	// rotation
	Vec3 rotation = GetTransform()->GetLocalRotation();

	//rotation.x += DELTA_TIME * (mouseY - viewMouseY) * _mouseRotateSpeed;
	rotation.y += DELTA_TIME * (mouseX - viewMouseX) * _mouseRotateSpeed;
	if (rotation.x < -LIMIT_ROTATION)
	{
		rotation.x = -LIMIT_ROTATION;
	}
	if (rotation.x > LIMIT_ROTATION)
	{
		rotation.x = LIMIT_ROTATION;
	}
	GetTransform()->SetLocalRotation(rotation);
}
