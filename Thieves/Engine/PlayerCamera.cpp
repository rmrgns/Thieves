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
	//Vec3 pos = GetTransform()->GetLocalPosition();
	Vec3 pos = GET_SINGLE(SceneManager)->GetPlayerPosition();
	pos.y += 125.f;
	// ī�޶� ���콺����
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME && _checkCameraRotation == true)
	{
		CameraRotation();
	}

	// ī�޶� ���콺���� on/off (temp code)
	if (INPUT->GetButtonDown(KEY_TYPE::L))
	{
		if (_checkCameraRotation == true)
			_checkCameraRotation = false;
		else
			_checkCameraRotation = true;
	}

	

	GetTransform()->SetLocalPosition(pos);
}

void PlayerCamera::CameraRotation()
{
	const POINT& point = INPUT->GetMousePos();	// ���� ���콺��ǥ�� point������ ����
	POINT WindowCenter = GEngine->GetWindowCenter();
	POINT ClientCenter = GEngine->GetClientCenter();

	// �߾ӿ��� ���콺�� �̵��� �Ÿ��� ���� float������ ����
	float dx = static_cast<float>(point.x - ClientCenter.x);	
	float dy = static_cast<float>(point.y - ClientCenter.y);

	// Ŀ���� ȭ�� �߾����� �̵�
	SetCursorPos(WindowCenter.x, WindowCenter.y);

	// rotation
	Vec3 rotation = GetTransform()->GetLocalRotation();
	rotation.x += DELTA_TIME * dy * _mouseRotateSpeed;
	rotation.y += DELTA_TIME * dx * _mouseRotateSpeed;

	// ���� ȸ�� ����
	if (rotation.x < -LIMIT_ROTATION)
	{
		rotation.x = -LIMIT_ROTATION;
	}
	if (rotation.x > LIMIT_ROTATION)
	{
		rotation.x = LIMIT_ROTATION;
	}

	// ī�޶� ĳ���� rotation ��ġ -> 1��Ī
	GET_SINGLE(SceneManager)->SetPlayerRotation(rotation);
	
	GetTransform()->SetLocalRotation(rotation);
}
