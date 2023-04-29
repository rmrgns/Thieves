// Client
#include "pch.h"
#include "PlayerInput.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Engine.h"
#include "Animator.h"

// Server
#include "server/main/network.h"
#include "server/ptr.h"
#include "server/main/packet_manager.h"

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
	// ĳ���� WASD�̵�
	if (INPUT->GetButton(KEY_TYPE::W) ||
		INPUT->GetButton(KEY_TYPE::S) ||
		INPUT->GetButton(KEY_TYPE::A) ||
		INPUT->GetButton(KEY_TYPE::D))
	{
		_move = true;
		if (_attack == 0 && _index != 0)
		{
			_index = 0;
			GetAnimator()->Play(_index);
		}
		if (INPUT->GetButton(KEY_TYPE::W))
		{
			direction = 1;
			Network::GetInst()->SendMovePacket(direction, pos,
				GetTransform()->GetLook(), DELTA_TIME);
			pos += GetTransform()->GetLook() * _speed * DELTA_TIME;
		}
		if (INPUT->GetButton(KEY_TYPE::S))
		{
			direction = 2;

			Network::GetInst()->SendMovePacket(direction, pos,
				GetTransform()->GetLook(), DELTA_TIME);
			pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
		}
		if (INPUT->GetButton(KEY_TYPE::A))
		{
			direction = 3;

			Network::GetInst()->SendMovePacket(direction, pos,
				GetTransform()->GetLook(), DELTA_TIME);
			pos += GetTransform()->GetRight() * _speed * DELTA_TIME;
		}
		if (INPUT->GetButton(KEY_TYPE::D))
		{
			direction = 4;

			Network::GetInst()->SendMovePacket(direction, pos,
				GetTransform()->GetLook(), DELTA_TIME);
			pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
		}
	}
	 if (INPUT->GetButtonUp(KEY_TYPE::W) ||
	 	INPUT->GetButtonUp(KEY_TYPE::S) ||
	 	INPUT->GetButtonUp(KEY_TYPE::A) ||
	 	INPUT->GetButtonUp(KEY_TYPE::D))
	 {
		 _move = false;
	 	if (INPUT->GetButtonUp(KEY_TYPE::W))
	 	{
	 		GetTransform()->ResetAccelerateLook();
	 	}
	 	if (INPUT->GetButtonUp(KEY_TYPE::S))
	 	{
	 		GetTransform()->ResetAccelerateLook();
	 	}
	 	if (INPUT->GetButtonUp(KEY_TYPE::A))
	 	{
	 		GetTransform()->ResetAccelerateRight();
	 	}
	 	if (INPUT->GetButtonUp(KEY_TYPE::D))
	 	{
	 		GetTransform()->ResetAccelerateRight();
	 	}
	 }
	 if (_attack == 0 && _move == false)
	 {
		 _index = 2;
		 GetAnimator()->Play(_index);
	 }

	if (INPUT->GetButtonDown(KEY_TYPE::Q))
	{
		GET_SINGLE(SceneManager)->SetBuildPlayer(true);
	}

	// Attack
	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		if(_attack == 0)
			_attack = 1;
	}
	GET_SINGLE(SceneManager)->SetPlayerPosition(pos);
	PlayerAttack();
	PlayerMove();
	GetTransform()->SetLocalPosition(pos);

	
}

void PlayerInput::PlayerMove() {

	//Vec3 pos = GEngine->GetThievesPacketManager()->GetVec();
	
	Vec3 pos;
	pos.x = GET_SINGLE(SceneManager)->GetPlayerPositionX();
	pos.y = recv_pos.y;
	pos.z = GET_SINGLE(SceneManager)->GetPlayerPositionZ();
	

	//	GET_SINGLE(SceneManager)->SetPlayerPosition(pos);

	// ĳ���� ����
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{

		_jump = true;
		// 점프 시작 패킷 전송
	}

	// ĳ���� ����
	Jump(pos);


	if (_checkCameraRotation == true)
		PlayerRotation();


	// ī�޶� ���콺���� on/off (temp code)
	if (INPUT->GetButtonDown(KEY_TYPE::L))
	{
		if (_checkCameraRotation == true)
			_checkCameraRotation = false;
		else
			_checkCameraRotation = true;
	}
	
	// ī�޶� ĳ���� position ��ġ -> 1��Ī
	GET_SINGLE(SceneManager)->SetPlayerPosition(pos);

	Vec3 rotation;
	rotation.y = GET_SINGLE(SceneManager)->GetPlayerRotation().y;
	 
	GetTransform()->SetLocalRotation(rotation);
	//GetTransform()->SetLocalPosition(pos);
	recv_pos = GetTransform()->GetLocalPosition();

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
		if (pos.y < 0)
		{
			pos.y = 0;
			_jumpCount = 0;
			_jump = false;
			return;
		}
		_jumpCount += DELTA_TIME;
		if (_jumpCount < 1.f)
		{
			
			_jumpSpeed -= 300.f * DELTA_TIME;
			pos += GetTransform()->GetUp() * _jumpSpeed * DELTA_TIME;
		}
		else if(_jumpCount < 2.f)
		{
			_jumpSpeed += 300.f * DELTA_TIME;
			pos -= GetTransform()->GetUp() * _jumpSpeed * DELTA_TIME;
		}
		else
		{
			// 점프 완료 패킷

			_jumpCount = 0;
			_jump = false;
			pos.y = 0;
		}
	}
}

void PlayerInput::PlayerAttack()
{
	// Attack
	if (_attack == 1)
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		_index = 1;
		GetAnimator()->Play(_index);
		_attack = 2;
	}
	else if (_attack == 2)
	{
		_attackCount += DELTA_TIME;
		if (_attackCount > 0.7f)
		{
			_index = 2;
			GetAnimator()->Play(_index);
			_attackCount = 0.f;
			_attack = 0;
		}
	}
	else
		return;

}
