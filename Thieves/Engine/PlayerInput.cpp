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


	// ĳ���� WASD�̵�
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

	// ĳ���� ����
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		_jump = true;
	}

	// ĳ���� ����
	Jump(pos);
	
	// ĳ���� ȸ��
	PlayerRotation();
	
	// ī�޶� ĳ���� position ��ġ -> 1��Ī
	GET_SINGLE(SceneManager)->SetPlayerPosition(pos);
	 
	GetTransform()->SetLocalPosition(pos);
}

void PlayerInput::PlayerRotation()
{
	const POINT& point = INPUT->GetMousePos();	// ���� ���콺��ǥ�� point������ ����
	float mouseX = static_cast<float>(point.x);	// point������ ���� float������ ����
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

	GetTransform()->SetLocalRotation(rotation);
}

void PlayerInput::Jump(Vec3& pos)
{
	if (_jump == true)
	{
		if (_jumpCount < 60)
		{
			_jumpCount++;
			_jumpSpeed -= 5.f;
			pos += GetTransform()->GetUp() * _jumpSpeed * DELTA_TIME;
		}
		else if(_jumpCount < 120)
		{
			_jumpCount++;
			_jumpSpeed += 5.f;
			pos -= GetTransform()->GetUp() * _jumpSpeed * DELTA_TIME;
		}
		else
		{
			_jumpCount = 0;
			_jump = false;
		}
	}
}
