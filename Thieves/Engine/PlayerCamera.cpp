#include "pch.h"
#include "PlayerCamera.h"
#include "SceneManager.h"
#include "Engine.h"
#include "Input.h"
#include "Timer.h"
#include "Transform.h"
#include "Camera.h"
PlayerCamera::PlayerCamera()
{
}

PlayerCamera::~PlayerCamera()
{
}

void PlayerCamera::LateUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
	pos = GET_SINGLE(SceneManager)->GetPlayerPosition();
	pos.y += 125.f;
	// ī�޶� ���콺����
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		CameraRotation();
	}
	
	GetTransform()->SetLocalPosition(pos);
}

void PlayerCamera::CameraRotation()
{
	const POINT& point = INPUT->GetMousePos();	// ���� ���콺��ǥ�� point������ ����
	float mouseX = static_cast<float>(point.x);	// point������ ���� float������ ����
	float mouseY = static_cast<float>(point.y);

	POINT WindowCenter = GEngine->GetWindowCenter();
	SetCursorPos(WindowCenter.x, WindowCenter.y);

	POINT ClientCenter = GEngine->GetClientCenter();

	float viewMouseX = static_cast<float>(ClientCenter.x);
	float viewMouseY = static_cast<float>(ClientCenter.y);

	// rotation
	Vec3 rotation = GetTransform()->GetLocalRotation();

	rotation.x += DELTA_TIME * (mouseY - viewMouseY) * _mouseRotateSpeed;
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
