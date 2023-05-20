#include "pch.h"
#include "FlashLightScript.h"
#include "Transform.h"
#include "Input.h"
#include "Engine.h"
#include "Timer.h"
#include "Light.h"
#include "GameObject.h"

// Temp
#include "SceneManager.h"
#include "Scene.h"
#include "Text.h"

#include "server/main/network.h"
#include "server/thieves_server/thieves_packet_manager.h"
#include "server/main/network_move_object.h"
#include "server/thieves_server/game_info.h"

FlashLightScript::FlashLightScript()
{
}

FlashLightScript::~FlashLightScript()
{
}

void FlashLightScript::LateUpdate()
{
	Vec3 pos(0.0f, 0.0f, 0.0f);
	Vec3 rotation(0.0f, 0.0f, 0.0f);
	int myID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();
	
	if (myID != -1) pos = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetPosition();
	if (myID != -1) rotation = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetRotation();
	pos.y = 0.f;

	FlashLightRotation();

	float yRotation = _rotation.y;

	float look_x = _x * cos(yRotation) + _z * sin(yRotation);
	float look_y = _y;
	float look_z = -_x * sin(yRotation) + _z * cos(yRotation);
	_x = look_x;
	_z = look_z;
	Vec3 lookVec = { look_x,look_y, look_z };
	if (INPUT->GetButtonDown(KEY_TYPE::Q))
	{
		Vec3 look = GET_SINGLE(SceneManager)->GetLookVec();
		this->GetGameObject()->GetLight()->SetLightDirection(look);

	}
	Vec3 look = GET_SINGLE(SceneManager)->GetLookVec();
	this->GetGameObject()->GetLight()->SetLightDirection(look);
	//GetTransform()->GetGameObject()->GetLight()->SetLightDirection(lookVec);
	WCHAR text[100] = L"";
	//::wsprintf(text, L"x:%d, y:%d, z:%d", int(_x), int(_y),int(_z));
	::wsprintf(text, L"x:%d, y:%d, z:%d", int(_rotation.x * 1000), int(_rotation.y * 1000),int(_rotation.z * 1000));
	::SetWindowText(GEngine->GetWindow().hwnd, text);
	GetTransform()->SetLocalPosition(pos);
	//GetTransform()->SetLocalRotation(rotation);
}

void FlashLightScript::FlashLightRotation()
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
	_rotation = GetTransform()->GetLocalRotation();

	//rotation.x += DELTA_TIME * (mouseY - viewMouseY) * _mouseRotateSpeed;
	_rotation.y += DELTA_TIME * (mouseX - viewMouseX) * _mouseRotateSpeed;

	GetTransform()->SetLocalRotation(_rotation);
}