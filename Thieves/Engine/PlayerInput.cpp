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
		_moveState = true;
		
		if (_attackState == 0 && _index != 0  && _jumpState == 0)
		{
			_index = 0;
			GetAnimator()->Play(_index);
		}
	}
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

	// ĳ���� ����
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		if (_jumpState == 0) {
			_action_type = (char)PL_ACTION_TYPE::JUMP;
			_jumpState = 1;
		}
	
		// 점프 시작 패킷 전송
	}

	// ĳ���� ����
	Jump(pos);

	if (INPUT->GetButton(KEY_TYPE::KEY_1)) {
		_mode = 1;
	}
	if (INPUT->GetButton(KEY_TYPE::KEY_2))
	{
		_mode = 2;
	}

	// Attack
	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		if (_mode == 1)
		{
			if (_attackState == 0)
			{
				_action_type = (char)PL_ACTION_TYPE::ATTACK;
				_attackState = 1;
			}
		}
		else if (_mode == 2) {

		}
	}

	//GET_SINGLE(SceneManager)->SetPlayerPosition(pos);
	PlayerAttack();
	PlayerMove();
	//GetTransform()->SetLocalPosition(pos);

	
}

void PlayerInput::PlayerMove() {

	//Vec3 pos = GEngine->GetThievesPacketManager()->GetVec();


	if (_checkCameraRotation == true)
		PlayerRotation();


	// ĳ���� ����
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		if (_jumpState == 0)
			_jumpState = 1;
		// 점프 시작 패킷 전송
	}

	// ĳ���� ����
	//Jump(pos);

	// ī�޶� ���콺���� on/off (temp code)
	if (INPUT->GetButtonDown(KEY_TYPE::L))
	{
		if (_checkCameraRotation == true)
			_checkCameraRotation = false;
		else
			_checkCameraRotation = true;
	}
	
	// ī�޶� ĳ���� position ��ġ -> 1��Ī
	//GET_SINGLE(SceneManager)->SetPlayerPosition(pos);

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
		_jumpSpeed -= 100.f * DELTA_TIME;
		pos += GetTransform()->GetUp() * _jumpSpeed * DELTA_TIME;

		if (pos.y < 0)
		{
			pos.y = 0;
			_jumpCount = 0;
			_jumpState = 0;
			_jumpSpeed = 100.f;
			return;
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
			Network::GetInst()->SendAttackPacket();
			_index = 2;
			GetAnimator()->Play(_index);
			_attackCount = 0.f;
			_attackState = 3;
		}
	}
	else if (_attackState == 3)		// attack cooltime
	{
		_attackCount += DELTA_TIME;
		if (_attackCount > _attackCoolTime)
		{
			_attackCount = 0.f;
			_attackState = 0;
		}
	}
	else
		return;

}
