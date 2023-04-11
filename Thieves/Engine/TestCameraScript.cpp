#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
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

	//// mouse translation �ڵ�
	//{
	//	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	//	{
	//		const POINT& point = INPUT->GetMousePos();
	//		INPUT->SetPrevMousePos(point);
	//	}
	//	else if (INPUT->GetButton(KEY_TYPE::LBUTTON))
	//	{
	//		const POINT& point = INPUT->GetMousePos();	// ���� ���콺��ǥ�� point������ ����
	//		// translation ����
	//		{
	//			if (point.x > INPUT->GetPrevMousePos().x)
	//			{
	//				pos -= GetTransform()->GetRight() * (point.x - INPUT->GetPrevMousePos().x) * _mouseTranslationSpeed * DELTA_TIME;
	//			}
	//			else if (point.x < INPUT->GetPrevMousePos().x)
	//			{
	//				//pos += GetTransform()->GetRight() * (point.x - INPUT->GetPrevMousePos().x) * _mouseTranslationSpeed * DELTA_TIME;
	//				pos += GetTransform()->GetRight() * (INPUT->GetPrevMousePos().x - point.x) * _mouseTranslationSpeed * DELTA_TIME;
	//			}
	//			if (point.y > INPUT->GetPrevMousePos().y)
	//			{
	//				pos += GetTransform()->GetUp() * (point.y - INPUT->GetPrevMousePos().y) * _mouseTranslationSpeed * DELTA_TIME;
	//			}
	//			else if (point.y < INPUT->GetPrevMousePos().y)
	//			{
	//				pos -= GetTransform()->GetUp() * (INPUT->GetPrevMousePos().y - point.y) * _mouseTranslationSpeed * DELTA_TIME;
	//			}
	//		}
	//		INPUT->SetPrevMousePos(point);	// Input class�� _prevMousePos�� point�� ��ǥ�� ����
	//	}
	//}
	
	//if (INPUT->GetButton(KEY_TYPE::RBUTTON))
	//{
	//	const POINT& point = INPUT->GetMousePos();	// ���� ���콺��ǥ�� point������ ����

	//	// rotation ����
	//	{
	//		if (point.x > 0 && point.x < GEngine->GetWindow().width - 15)
	//		{
	//			if (point.x > INPUT->GetPrevMousePos().x)
	//			{
	//				Vec3 rotation = GetTransform()->GetLocalRotation();
	//				rotation.y += DELTA_TIME * (point.x - INPUT->GetPrevMousePos().x) * _mouseRotateSpeed;
	//				GetTransform()->SetLocalRotation(rotation);
	//			}
	//			else if (point.x < INPUT->GetPrevMousePos().x)
	//			{
	//				Vec3 rotation = GetTransform()->GetLocalRotation();
	//				rotation.y -= DELTA_TIME * (INPUT->GetPrevMousePos().x - point.x) * _mouseRotateSpeed;
	//				GetTransform()->SetLocalRotation(rotation);
	//			}
	//		}

	//		if (point.y > 0 && point.y < GEngine->GetWindow().height - 40)
	//		{
	//			if (point.y > INPUT->GetPrevMousePos().y)
	//			{
	//				Vec3 rotation = GetTransform()->GetLocalRotation();
	//				rotation.x += DELTA_TIME * (point.y - INPUT->GetPrevMousePos().y) * _mouseRotateSpeed;
	//				GetTransform()->SetLocalRotation(rotation);
	//			}
	//			else if (point.y < INPUT->GetPrevMousePos().y)
	//			{
	//				Vec3 rotation = GetTransform()->GetLocalRotation();
	//				rotation.x -= DELTA_TIME * (INPUT->GetPrevMousePos().y - point.y) * _mouseRotateSpeed;
	//				GetTransform()->SetLocalRotation(rotation);
	//			}
	//		}}
	//	INPUT->SetPrevMousePos(point);	// Input class�� _prevMousePos�� point�� ��ǥ�� ����
	//}

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