#include "pch.h"
#include "PlayerCamera.h"
#include "SceneManager.h"
#include "Engine.h"
#include "Input.h"
#include "Timer.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"

#include "server/main/network.h"
#include "server/thieves_server/thieves_packet_manager.h"
#include "server/main/network_move_object.h"
#include "server/thieves_server/game_info.h"

PlayerCamera::PlayerCamera()
{
}

PlayerCamera::~PlayerCamera()
{
}

void PlayerCamera::LateUpdate()
{
	//Vec3 pos = GetTransform()->GetLocalPosition();
	
	int myID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();
	Vec3 pos(0.0f, 0.0f, 0.0f);
	if(myID != -1) pos = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetPosition();
	pos.y += 125.f;
	//pos.x += 100.f;
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
	int myID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();
	if(myID != -1) Network::GetInst()->GetNetworkObjMap().find(myID)->second->SetRotation(rotation);
	GET_SINGLE(SceneManager)->SetLookVec(GetTransform()->GetLook());
	GetTransform()->SetLocalRotation(rotation);
}
