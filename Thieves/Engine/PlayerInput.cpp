#include "pch.h"
#include "PlayerInput.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Engine.h"
#include "server/network/network.h"

PlayerInput::PlayerInput()
{
	
}

PlayerInput::~PlayerInput()
{
}

void PlayerInput::LateUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	direction = 0;
	// 캐릭터 WASD이동
	if (INPUT->GetButton(KEY_TYPE::W))
	{
		direction = 1;
		//pos += GetTransform()->GetLook() * _speed * DELTA_TIME;
		//pos.x += GetTransform()->GetLook().x * _speed * DELTA_TIME;
		//pos.z += GetTransform()->GetLook().z * _speed * DELTA_TIME;
		
	}
	if (INPUT->GetButton(KEY_TYPE::S))
	{
		direction = 2;
		//pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
		//pos.x -= GetTransform()->GetLook().x * _speed * DELTA_TIME;
		//pos.z -= GetTransform()->GetLook().z * _speed * DELTA_TIME;
	}
	if (INPUT->GetButton(KEY_TYPE::A))
	{
		direction = 3;
		//pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
		//pos.x -= GetTransform()->GetRight().x * _speed * DELTA_TIME;
		//pos.z -= GetTransform()->GetRight().z * _speed * DELTA_TIME;
	}
	if (INPUT->GetButton(KEY_TYPE::D))
	{
		direction = 4;
		//pos += GetTransform()->GetRight() * _speed * DELTA_TIME;
		//pos.x += GetTransform()->GetRight().x * _speed * DELTA_TIME;
		//pos.z += GetTransform()->GetRight().z * _speed * DELTA_TIME;
	}

	Network::GetInst()->SendMovePacket(direction, pos,
		GetTransform()->GetLook(),
		DELTA_TIME);



	/*if (thieves_packet_manager->GetDirection() == 1 || thieves_packet_manager->GetDirection() == 2)
	{
		pos = thieves_packet_manager->GetForwardVec();
	}
	if (thieves_packet_manager->GetDirection() == 3 || thieves_packet_manager->GetDirection() == 4)
	{
		pos = thieves_packet_manager->GetRightVec();
	}*/


	if (thieves_packet_manager)
	{
		
		pos += thieves_packet_manager->GetVec();		
	}



	// 캐릭터 점프
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		_jump = true;
	}

	// 캐릭터 점프
	Jump(pos);
	
	// 캐릭터 회전
	PlayerRotation();
	
	// 카메라 캐릭터 position 일치 -> 1인칭
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
			pos.y = 0;
		}
	}
}
