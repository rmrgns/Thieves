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

	// ĳ���� ����
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		if (_jumpState == 0) {
			_jumpState = 1;

		}
		// 점프 시작 패킷 전송)
	}

	// ĳ���� ����
	Jump(pos);

	// ĳ���� WASD�̵�
	if (INPUT->GetButton(KEY_TYPE::W) ||
		INPUT->GetButton(KEY_TYPE::S) ||
		INPUT->GetButton(KEY_TYPE::A) ||
		INPUT->GetButton(KEY_TYPE::D))
	{
		_moveState = true;
		if (_attackState == 0 && _index != 0  && _jumpState == 0)
		{
			_index = 0;
			GetAnimator()->Play(_index);
		}
		if (INPUT->GetButton(KEY_TYPE::W))
		{
			direction = 1;
//			Network::GetInst()->SendMovePacket(direction, pos,GetTransform()->GetLook(), DELTA_TIME, _jumpState );
		}
		if (INPUT->GetButton(KEY_TYPE::S))
		{
			direction = 2;

//			Network::GetInst()->SendMovePacket(direction, pos,GetTransform()->GetLook(), DELTA_TIME, _jumpState);
		}
		if (INPUT->GetButton(KEY_TYPE::A))
		{
			direction = 3;

//			Network::GetInst()->SendMovePacket(direction, pos,GetTransform()->GetLook(), DELTA_TIME, _jumpState);
		}
		if (INPUT->GetButton(KEY_TYPE::D))
		{
			direction = 4;

//			Network::GetInst()->SendMovePacket(direction, pos,GetTransform()->GetLook(), DELTA_TIME, _jumpState);
		}
	}

	Network::GetInst()->SendMovePacket(direction, pos, GetTransform()->GetLook(), DELTA_TIME, _jumpState);
	 if (INPUT->GetButtonUp(KEY_TYPE::W) ||
	 	INPUT->GetButtonUp(KEY_TYPE::S) ||
	 	INPUT->GetButtonUp(KEY_TYPE::A) ||
	 	INPUT->GetButtonUp(KEY_TYPE::D))
	 {
		 _moveState = false;
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
	 if (_attackState == 0 && _moveState == false && _jumpState == 0)
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
		if(_attackState == 0)
			_attackState = 1;
	}

	PlayerAttack();
	PlayerMove();
}

void PlayerInput::PlayerMove() {

	//Vec3 pos = GEngine->GetThievesPacketManager()->GetVec();
	
	Vec3 pos;
	pos.x = GET_SINGLE(SceneManager)->GetPlayerPositionX();
	pos.y = GET_SINGLE(SceneManager)->GetPlayerPositionY();
	pos.z = GET_SINGLE(SceneManager)->GetPlayerPositionZ();


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

	GetTransform()->SetLocalPosition(pos);
	GetTransform()->SetLocalRotation(rotation);
	
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
	if (_jumpState == 1)
	{
		_index = 3;
		GetAnimator()->Play(_index);
		_jumpState = 2;
	}
	else if (_jumpState == 2)
	{
		if (pos.y < 0)
		{
			pos.y = 0;
			//_jumpCount = 0;
			_jumpState = 0;
			//return;
		}
		//_jumpCount += DELTA_TIME;
		if (_jumpCount < 1.f)
		{
			//_jumpSpeed -= 300.f * DELTA_TIME;
			//pos += GetTransform()->GetUp() * _jumpSpeed * DELTA_TIME;
		}
		else if(_jumpCount < 2.f)
		{
			//_jumpSpeed += 300.f * DELTA_TIME;
			//pos -= GetTransform()->GetUp() * _jumpSpeed * DELTA_TIME;
		}
		else
		{
			// 점프 완료 패킷
			//_jumpCount = 0;
			//_jumpState = 0;
			//pos.y = 0;
		}
	}
}

void PlayerInput::PlayerAttack()
{
	// Attack
	if (_attackState == 1)
	{
		_index = 1;
		GetAnimator()->Play(_index);
		_attackState = 2;
	}
	else if (_attackState == 2)
	{
		_attackCount += DELTA_TIME;
		if (_attackCount > 0.7f)
		{
			_index = 2;
			GetAnimator()->Play(_index);
			_attackCount = 0.f;
			_attackState = 0;
		}
	}
	else
		return;

}
