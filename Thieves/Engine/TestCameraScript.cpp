#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"

#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Engine.h"

TestCameraScript::TestCameraScript()
{
}

TestCameraScript::~TestCameraScript()
{
}

void TestCameraScript::LateUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::E))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::Z))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y += DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::C))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y -= DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	// picking �ڵ�
	/*if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();
		GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);
	}*/

	// ī�޶� ���콺���� on/off (temp code)
	if (INPUT->GetButtonDown(KEY_TYPE::L))
	{
		if (_checkCameraRotation == true)
			_checkCameraRotation = false;
		else
			_checkCameraRotation = true;
	}

	// ī�޶� ���콺����
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME && _checkCameraRotation == true)
	{
		CameraRotation();
	}

	GetTransform()->SetLocalPosition(pos);
}


void TestCameraScript::CameraRotation()
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
	if (rotation.x < -1.575f)
	{
		rotation.x = -1.575f;
	}
	if (rotation.x > 1.575f)
	{
		rotation.x = 1.575f;
	}
	GetTransform()->SetLocalRotation(rotation);
}