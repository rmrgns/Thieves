#include "pch.h"
#include "PlayerInput.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Engine.h"
#include "server/network/network.h"
#include "ptr.h"
#include "server/packet/packet_manager.h"

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
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;
		Network::GetInst()->SendMovePacket(direction, recv_pos,
			GetTransform()->GetLook(), DELTA_TIME);
	}
	if (INPUT->GetButton(KEY_TYPE::S))
	{
		direction = 2;
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
		Network::GetInst()->SendMovePacket(direction, recv_pos,
			GetTransform()->GetLook(), DELTA_TIME);
	}
	if (INPUT->GetButton(KEY_TYPE::A))
	{
		direction = 3;
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
		Network::GetInst()->SendMovePacket(direction, recv_pos,
			GetTransform()->GetLook(), DELTA_TIME);
	}
	if (INPUT->GetButton(KEY_TYPE::D))
	{
		direction = 4;
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;
		Network::GetInst()->SendMovePacket(direction, recv_pos,
			GetTransform()->GetLook(), DELTA_TIME);
	}

	bool bRecv = GEngine->GetThievesPacketManager()->GetRecv();
	//PlayerRecvPos(bRecv);

}

void PlayerInput::PlayerRecvPos( bool bRecv) {

	if (bRecv == true) {
		bRecv = false;
		PlayerInput::PlayerMove();
	}
}


void PlayerInput::PlayerMove() {

	Vec3 pos = GEngine->GetThievesPacketManager()->GetVec();

	GET_SINGLE(SceneManager)->SetPlayerPosition(pos);

	// 캐릭터 점프
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		_jump = true;
	}

	// 캐릭터 점프
	Jump(pos);

	// 캐릭터 회전
	if (_checkCameraRotation == true)
		PlayerRotation();


	// 카메라 마우스제어 on/off (temp code)
	if (INPUT->GetButtonDown(KEY_TYPE::L))
	{
		if (_checkCameraRotation == true)
			_checkCameraRotation = false;
		else
			_checkCameraRotation = true;
	}

	// 카메라 캐릭터 position 일치 -> 1인칭
	GET_SINGLE(SceneManager)->SetPlayerPosition(GEngine->GetThievesPacketManager()->GetVec());

	GetTransform()->SetLocalPosition(GEngine->GetThievesPacketManager()->GetVec());
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
		}
	}
}
